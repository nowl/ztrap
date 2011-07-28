#include "ztrap.h"

static void
send_move_clear()
{
    message_create_and_send("map-view",
                            "player",
                            "move-clear",
                            NULL,
                            0,
                            SYNC);
}

static void
map_move_block(map_t *map, int x, int y, int dx, int dy)
{
    float amb = map_get_ambiance(map, x, y);
    map_set_ambiance(map, x+dx, y+dy, amb);
    map_set_value(map, x+dx, y+dy, 1);
    map_set_value(map, x, y, 0);
}

static int
message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("map-move"))
    {
        enum direction4 dir = *(enum direction4 *)mes->data;
        switch(dir)
        {
        case LEFT:
        {
            map_view_t *mv = obj->data;
            int new = mv->xs - 8;
            if(new < 0) new = mv->xs;
            int move = mv->xs - new;
            mv->xs -= move;
            mv->xe -= move;                
            return 1;
        }
        case RIGHT:
        {
            map_view_t *mv = obj->data;
            int new = mv->xe + 8;
            if(new >= mv->map->width) new = mv->map->width-1;
            int move = mv->xe - new;
            mv->xs -= move;
            mv->xe -= move;                
            return 1;
        }
        case UP:
        {
            map_view_t *mv = obj->data;
            int new = mv->ys - 8;
            if(new < 0) new = mv->ys;
            int move = mv->ys - new;
            mv->ys -= move;
            mv->ye -= move;                
            return 1;
        }
        case DOWN:
        {
            map_view_t *mv = obj->data;
            int new = mv->ye + 8;
            if(new >= mv->map->height) new = mv->map->height-1;
            int move = mv->ye - new;
            mv->ys -= move;
            mv->ye -= move;                
            return 1;
        }
        default:
                break;
        }
    }
    else if(mes->type == lapis_hash("player-announce-position"))
    {
        map_view_t *mv = obj->data;
        player_movement_t *loc = mes->data;

        mv->player_x = loc->x;
        mv->player_y = loc->y;        
    }
    else if(mes->type == lapis_hash("player-attempt-move"))
    {
        map_view_t *mv = obj->data;
        player_movement_t *loc = mes->data;
        int set_new_player_pos = 0;

        /* first check if there is no obstruction, otherwise check if
         * there is a clear space behind the wall */
        if( map_get_value(mv->map, loc->x, loc->y) != 1 )
        {
            send_move_clear();
            set_new_player_pos = 1;
        }
        else
        {
            switch(loc->dir)
            {
            case N:
                if( map_get_value(mv->map, loc->x, loc->y - 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, 0, -1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case S:
                if( map_get_value(mv->map, loc->x, loc->y + 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, 0, 1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case E:
                if( map_get_value(mv->map, loc->x + 1, loc->y) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, 1, 0);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case W:
                if( map_get_value(mv->map, loc->x - 1, loc->y) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, -1, 0);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case NW:
                if( map_get_value(mv->map, loc->x - 1, loc->y - 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, -1, -1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case NE:
                if( map_get_value(mv->map, loc->x + 1, loc->y - 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, 1, -1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case SW:
                if( map_get_value(mv->map, loc->x - 1, loc->y + 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, -1, 1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            case SE:
                if( map_get_value(mv->map, loc->x + 1, loc->y + 1) != 1 )
                {
                    map_move_block(mv->map, loc->x, loc->y, 1, 1);
                    send_move_clear();
                    set_new_player_pos = 1;
                }
                break;
            }            
        }

        if(set_new_player_pos)
        {
            mv->player_x = loc->x;
            mv->player_y = loc->y;
        }
    }
    else if(mes->type == lapis_hash("player-lighting"))
    {
        map_view_t *mv = obj->data;
        mes_light_amt_t *light = mes->data;
        
        mv->lighting = light->lighting;        
    }
    else if(mes->type == lapis_hash("window-resize"))
    {
        map_view_t *mv = obj->data;
        player_movement_t *loc = mes->data;
        
        mv->screen_w = loc->x;
        mv->screen_h = loc->y;

        /* change map-view size */
        int w = mv->screen_w / 32 - 1;
        int h = mv->screen_h / 32 - 2;
        mv->xe = mv->xs + w - 1;
        mv->ye = mv->ys + h - 1;
    }
    
    return 0;
}

static int
dist(int x, int y, int nx, int ny)
{
    return (nx-x)*(nx-x) + (ny-y)*(ny-y);
}

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
    map_view_t *mv = obj->data;
    int x, y;
    float light_noise = (random_float()-0.5)*.5;
    for(y=mv->ys; y<=mv->ye; y++)
        for(x=mv->xs; x<=mv->xe; x++)
            if(map_get_value(mv->map, x, y) == 1)
            {
                int d = dist(mv->player_x, mv->player_y, x, y);
                float darken;
                darken = 25 * (mv->lighting + light_noise) / d;
                darken = darken > 1.0 ? 1.0 : darken;
                                
                lsdl_draw_image(engine, 
                                image_loader_get("wall"),
                                mv->screen_pos_x + (x - mv->xs) * 32,
                                mv->screen_pos_y + (y - mv->ys) * 32,
                                32,
                                32,
                                darken * map_get_ambiance(mv->map, x, y));
            }
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
}

map_view_t *
map_view_create(int screen_pos_x, int screen_pos_y, int width, int height)
{
    map_view_t *r = malloc(sizeof(*r));
    r->map = NULL;
    r->screen_pos_x = screen_pos_x;
    r->screen_pos_y = screen_pos_y;
    r->xs = 0;
    r->xe = width-1;
    r->ys = 0;
    r->ye = height-1;
    r->game_object = game_object_create("map-view", r);
    r->screen_w = 1024;
    r->screen_h = 768;
    game_object_set_recv_callback_c_func(r->game_object, message_handler);
    game_object_set_render_callback_c_func(r->game_object, render);
    game_object_set_update_callback_c_func(r->game_object, update);
    game_object_set_recv_callback_c_func(r->game_object, message_handler);
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  r->game_object,
                                  "map-move");
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  r->game_object,
                                  "window-resize");
    
    return r;
}

void
map_view_destroy(map_view_t *mv)
{
    free(mv);
}

int
map_view_pos_to_screen_x(map_view_t *mv, int x)
{
    return mv->screen_pos_x + (x - mv->xs) * 32;
}

int
map_view_pos_to_screen_y(map_view_t *mv, int y)
{
    return mv->screen_pos_y + (y - mv->ys) * 32;
}
