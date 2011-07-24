#include "ztrap.h"

int global_message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("sdl-event"))
    {
        SDL_Event event = *(SDL_Event *)mes->data;
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                engine_quit(lapis_get_engine());
                return 1;
            default:
                break;
            }
        }
    }

    return 0;
}                    

int main(int argc, char *argv[])
{
    lapis_init();

    engine_t *engine = lapis_get_engine();
    set_ticks_per_second(15);

    game_state_t *state = game_state_create(0);

    engine_switch_state(engine, state);

    /* initialize video */
    
    lsdl_set_video_mode(engine->sdl_driver,
                        1024, 768, 0);

    SDL_EnableKeyRepeat(500, 50);

    /* load graphics */
    image_loader_load("player", "data.png", 0, 0, 64, 64);
    image_loader_load("wall", "data.png", 0, 64, 64, 64);

    /* create object */

    game_object_t * global_object = game_object_create("global", NULL);
    game_state_append_object(state, global_object);
    game_object_set_recv_callback_c_func(global_object, global_message_handler);
    game_state_append_bcast_recvr(state,
                                  global_object,
                                  "sdl-event");
    
    player_object_t player_data;
    player_data.x = 0;
    player_data.y = 0;

    game_object_t * player = game_object_create("player", &player_data);
    game_object_set_recv_callback_c_func(player, player_message_handler);
    game_state_append_bcast_recvr(state, player, "sdl-event");
    game_object_set_render_callback_c_func(player, player_render);
    game_object_set_update_callback_c_func(player, player_update);

    /* build map */

    map_t *map = map_create(1024, 1024);
    map_build_bsp(map, 0.75, 1.0);
    map_random_replace(map, 0, 1, 0.1);
    map_random_replace(map, 1, 0, 0.1);
    //debug_map_display(map);
    debug_map_display_file(map);
    
    map_view_t *mv = map_view_create(16, 16, 31, 22);
    mv->map = map;

    
    game_state_append_object(state, mv->game_object);
    game_state_append_object(state, player);

    player_data.mv = mv;
            
    lapis_mainloop();

    map_view_destroy(mv);
    map_destroy(map);

    /* this will be cleaned up by the OS */
    //game_state_destroy(state);
    //lapis_deinit();

    return 0;
}
