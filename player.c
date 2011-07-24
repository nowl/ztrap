#include "ztrap.h"

static void
send_attempt_move_to_map_view(player_object_t *player)
{
    map_loc_t *loc = malloc(sizeof(*loc));
    loc->x = player->nx;
    loc->y = player->ny;
    message_t *message = message_create(NULL, 
                                        player->mv->game_object,
                                        "player-attempt-move",
                                        loc,
                                        1);
    message_deliver(message, SYNC);
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
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP2:
            {
                player_object_t *data = obj->data;
                data->nx = data->x;
                data->ny = data->y + 1;
                send_attempt_move_to_map_view(data);                
                return 1;
            }
            case SDLK_KP4:
            {
                player_object_t *data = obj->data;
                data->nx = data->x - 1;
                data->ny = data->y;
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP6:
            {
                player_object_t *data = obj->data;
                data->nx = data->x + 1;
                data->ny = data->y;
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP7:
            {
                player_object_t *data = obj->data;
                data->nx = data->x - 1;
                data->ny = data->y - 1;
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP9:
            {
                player_object_t *data = obj->data;
                data->nx = data->x + 1;
                data->ny = data->y - 1;
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP1:
            {
                player_object_t *data = obj->data;
                data->nx = data->x - 1;
                data->ny = data->y + 1;
                send_attempt_move_to_map_view(data);
                return 1;
            }
            case SDLK_KP3:
            {
                player_object_t *data = obj->data;
                data->nx = data->x + 1;
                data->ny = data->y + 1;
                send_attempt_move_to_map_view(data);
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
    }

    return 0;
}                    

void player_render(engine_t *engine, game_object_t *obj, float interpolation)
{
    player_object_t *data = obj->data;
    lsdl_draw_image(engine, image_loader_get("player"),
                    map_view_pos_to_screen_x(data->mv, data->x),
                    map_view_pos_to_screen_y(data->mv, data->y),
                    32,
                    32,
                    1.0);
}

void player_update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{

    /* move the map based on where the player is */

    player_object_t *data = obj->data;
    if( map_view_pos_to_screen_y(data->mv, data->y) > (768-150) )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = DOWN;
        message_t *message = message_create(NULL, 
                                            NULL,
                                            "map-move",
                                            dir,
                                            1);
        message_deliver(message, SYNC);
        
    }
    if( map_view_pos_to_screen_y(data->mv, data->y) < 150 )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = UP;
        message_t *message = message_create(NULL, 
                                            NULL,
                                            "map-move",
                                            dir,
                                            1);
        message_deliver(message, SYNC);
        
    }
    if( map_view_pos_to_screen_x(data->mv, data->x) < 150 )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = LEFT;
        message_t *message = message_create(NULL, 
                                            NULL,
                                            "map-move",
                                            dir,
                                            1);
        message_deliver(message, SYNC);
        
    }
    if( map_view_pos_to_screen_x(data->mv, data->x) > (1024-150) )
    {
        enum direction4 *dir = malloc(sizeof(*dir));
        *dir = RIGHT;
        message_t *message = message_create(NULL, 
                                            NULL,
                                            "map-move",
                                            dir,
                                            1);
        message_deliver(message, SYNC);
    }        
}
