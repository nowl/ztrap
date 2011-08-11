#include "ztrap.h"

static int bullet_counter = 0;

static void
send_attempt_move_to_map_view(player_object_t *player, enum direction8 dir)
{
    player_movement_t *loc = malloc(sizeof(*loc));
    loc->x = player->nx;
    loc->y = player->ny;
    loc->dir = dir;
    message_create_and_send("player", "map-view", "player-attempt-move", loc, 1, SYNC);
}

static int
attempt_move_player(int key, game_object_t *obj)
{
    switch(key)
    {
    case SDLK_KP8:
    {
        player_object_t *data = obj->data;
        data->nx = data->x;
        data->ny = data->y - 1;
        send_attempt_move_to_map_view(data, N);
        return 1;
    }
    case SDLK_KP2:
    {
        player_object_t *data = obj->data;
        data->nx = data->x;
        data->ny = data->y + 1;
        send_attempt_move_to_map_view(data, S);                
        return 1;
    }
    case SDLK_KP4:
    {
        player_object_t *data = obj->data;
        data->nx = data->x - 1;
        data->ny = data->y;
        send_attempt_move_to_map_view(data, W);
        return 1;
    }
    case SDLK_KP6:
    {
        player_object_t *data = obj->data;
        data->nx = data->x + 1;
        data->ny = data->y;
        send_attempt_move_to_map_view(data, E);
        return 1;
    }
    case SDLK_KP7:
    {
        player_object_t *data = obj->data;
        data->nx = data->x - 1;
        data->ny = data->y - 1;
        send_attempt_move_to_map_view(data, NW);
        return 1;
    }
    case SDLK_KP9:
    {
        player_object_t *data = obj->data;
        data->nx = data->x + 1;
        data->ny = data->y - 1;
        send_attempt_move_to_map_view(data, NE);
        return 1;
    }
    case SDLK_KP1:
    {
        player_object_t *data = obj->data;
        data->nx = data->x - 1;
        data->ny = data->y + 1;
        send_attempt_move_to_map_view(data, SW);
        return 1;
    }
    case SDLK_KP3:
    {
        player_object_t *data = obj->data;
        data->nx = data->x + 1;
        data->ny = data->y + 1;
        send_attempt_move_to_map_view(data, SE);
        return 1;
    }
    default:
        break;
    }           

    return 0;
}

static void
fire_bullet(player_object_t *po, int dx, int dy)
{
    char *bullet_name;
    asprintf(&bullet_name, "bullet-%d", bullet_counter++);
    bullet_t *b = bullet_create(bullet_name);
    b->x = po->x + dx;
    b->y = po->y + dy;
    b->dx = dx;
    b->dy = dy;
    game_state_append_object(lapis_get_engine()->state, b->game_object);
    free(bullet_name);
}

static int
attempt_fire(int key, game_object_t *obj)
{
    switch(key)
    {
    case SDLK_KP8:
    {
        fire_bullet(obj->data, 0, -1);
        return 1;
    }
    case SDLK_KP2:
    {
        fire_bullet(obj->data, 0, 1);
        return 1;
    }
    case SDLK_KP4:
    {
        fire_bullet(obj->data, -1, 0);
        return 1;
    }
    case SDLK_KP6:
    {
        fire_bullet(obj->data, 1, 0);
        return 1;
    }
    case SDLK_KP7:
    {
        fire_bullet(obj->data, -1, -1);
        return 1;
    }
    case SDLK_KP9:
    {
        fire_bullet(obj->data, 1, -1);
        return 1;
    }
    case SDLK_KP1:
    {
        fire_bullet(obj->data, -1, 1);
        return 1;
    }
    case SDLK_KP3:
    {
        fire_bullet(obj->data, 1, 1);
        return 1;
    }
    default:
        break;
    }           

    return 0;
}

int player_message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("sdl-event"))
    {
        SDL_Event event = *(SDL_Event *)mes->data;
        if(event.type == SDL_KEYDOWN)
        {
            int shift_down = event.key.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT);

            if(shift_down)
                return attempt_fire(event.key.keysym.sym, obj);
            
            if(!shift_down)
                return attempt_move_player(event.key.keysym.sym, obj);
        }
    }
    else if(mes->type == lapis_hash("move-clear"))
    {
        player_object_t *data = obj->data;
        data->x = data->nx;
        data->y = data->ny;

        message_create_and_send(NULL, NULL, "player-move", NULL, 0, SYNC);
    }

    return 0;
}                    

void player_render(engine_t *engine, game_object_t *obj, float interpolation)
{
    player_object_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    lsdl_draw_image(engine, image_loader_get("player"),
                    map_view_pos_to_screen_x(mv, data->x),
                    map_view_pos_to_screen_y(mv, data->y),
                    32,
                    32,
                    1.0, 1.0, 1.0);
}

static int set_light = 0;

void player_update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
    player_object_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;

    if(!set_light)
    {
        set_light = 1;

        player_movement_t *loc = malloc(sizeof(*loc));
        loc->x = data->x;
        loc->y = data->y;
        message_create_and_send("player", "map-view", "player-announce-position", loc, 1, SYNC);
        
        mes_light_amt_t *light = malloc(sizeof(*light));
        light->lighting = 0.5;
        message_create_and_send("player", "map-view", "player-lighting", light, 1, SYNC);
    }


    /* move the map based on where the player is */

    if( map_view_pos_to_screen_y(mv, data->y) > (mv->screen_h-150) )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = DOWN;
        message_create_and_send("player", "map-view", "map-move", dir, 1, SYNC);        
    }
    if( map_view_pos_to_screen_y(mv, data->y) < 150 )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = UP;
        message_create_and_send("player", "map-view", "map-move", dir, 1, SYNC);        
    }
    if( map_view_pos_to_screen_x(mv, data->x) < 150 )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = LEFT;
        message_create_and_send("player", "map-view", "map-move", dir, 1, SYNC);
    }
    if( map_view_pos_to_screen_x(mv, data->x) > (mv->screen_w-150) )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = RIGHT;
        message_create_and_send("player", "map-view", "map-move", dir, 1, SYNC);
    }
}
