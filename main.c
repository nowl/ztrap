#include "ztrap.h"

int global_message_handler(game_object_t *obj, message_t mes)
{
    if(mes.type == message_type_hash("sdl-event"))
    {
        SDL_Event event = *(SDL_Event *)mes.data;
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
    set_ticks_per_second(60);

    game_state_t *state = game_state_create(0);

    engine_switch_state(engine, state);

    /* initialize video */
    
    lsdl_set_video_mode(engine->sdl_driver,
                        800, 600, 0);

    SDL_EnableKeyRepeat(500, 50);

    /* load graphics */
    image_loader_load("test1", "screenshot1.jpg", 100, 100, 32, 32);
    image_loader_load("test2", "screenshot1.jpg", 110, 110, 32, 32);

    image_render_set_create("test_set");
    image_render_set_add("test_set", "test1", 60);
    image_render_set_add("test_set", "test2", 60);

    /* create object */

    game_object_t * global_object = game_object_create(NULL);
    game_state_append_object(state, global_object);
    game_object_set_recv_callback_c_func(global_object, global_message_handler);
    game_state_append_bcast_recvr(state,
                                  global_object,
                                  message_type_hash("sdl-event"));
    
    player_object_t player_data;
    player_data.x = 400;
    player_data.y = 300;
    player_data.anim_frame = 0;
    player_data.num_frames = 120;

    game_object_t * player = game_object_create(&player_data);
    game_state_append_object(state, player);
    game_object_set_recv_callback_c_func(player, player_message_handler);
    game_state_append_bcast_recvr(state, player, message_type_hash("sdl-event"));
    game_object_set_render_callback_c_func(player, player_render);
    game_object_set_update_callback_c_func(player, player_update);

    /* build map */

    map_t *map = map_create(0, 0, 1024, 1024);
    map_build_bsp(map, 0.75, 1.0);
    map_random_replace(map, 0, 1, 0.1);
    map_random_replace(map, 1, 0, 0.1);
    //debug_map_display(map);
    debug_map_display_file(map);
    map_destroy(map);
        
    lapis_mainloop();

    /* this will be cleaned up by the OS */
    //game_state_destroy(state);
    //lapis_deinit();

    return 0;
}
