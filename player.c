#include "ztrap.h"

int player_message_handler(game_object_t *obj, message_t mes)
{
    if(mes.type == message_type_hash("sdl-event"))
    {
        SDL_Event event = *(SDL_Event *)mes.data;
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_a:
                LOG("pressed a\n");
                return 1;
            case SDLK_UP:
            {
                player_object_t *data = obj->data;
                data->y -=10;
                return 1;
            }
            case SDLK_DOWN:
            {
                player_object_t *data = obj->data;
                data->y +=10;
                return 1;
            }
            case SDLK_LEFT:
            {
                player_object_t *data = obj->data;
                data->x -=10;
                return 1;
            }
            case SDLK_RIGHT:
            {
                player_object_t *data = obj->data;
                data->x +=10;
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
    lsdl_draw_image(engine, image_render_set_get_image("test_set", data->anim_frame), data->x, data->y, 32, 32);
}

void player_update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
    player_object_t *data = obj->data;
    data->anim_frame = ticks % data->num_frames;
}
