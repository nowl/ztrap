#include "ztrap.h"

static void
send_attempt_move_to_map_view(player_object_t *player, enum direction8 dir)
{
    player_movement_t *loc = malloc(sizeof(*loc));
    loc->x = player->nx;
    loc->y = player->ny;
    loc->dir = dir;
    message_create_and_send("player", "map-view", "player-attempt-move", loc, 1, SYNC);
}

int player_message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("sdl-event"))
    {
        SDL_Event event = *(SDL_Event *)mes->data;
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
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
