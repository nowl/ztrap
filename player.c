#include "ztrap.h"

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
                data->y -=1;
                return 1;
            }
            case SDLK_KP2:
            {
                player_object_t *data = obj->data;
                data->y +=1;
                return 1;
            }
            case SDLK_KP4:
            {
                player_object_t *data = obj->data;
                data->x -=1;
                return 1;
            }
            case SDLK_KP6:
            {
                player_object_t *data = obj->data;
                data->x +=1;
                return 1;
            }
            case SDLK_KP7:
            {
                player_object_t *data = obj->data;
                data->y -=1;
                data->x -=1;
                return 1;
            }
            case SDLK_KP9:
            {
                player_object_t *data = obj->data;
                data->y -=1;
                data->x +=1;
                return 1;
            }
            case SDLK_KP1:
            {
                player_object_t *data = obj->data;
                data->x -=1;
                data->y +=1;
                return 1;
            }
            case SDLK_KP3:
            {
                player_object_t *data = obj->data;
                data->x +=1;
                data->y +=1;
                return 1;
            }
            default:
                break;
            }           
        }
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
