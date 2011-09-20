#include "ztrap.h"

int global_message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("sdl-event"))
    {
        SDL_Event *event = mes->data->data;
        if(event->type == SDL_KEYDOWN)
        {
            switch(event->key.keysym.sym)
            {
            case SDLK_ESCAPE:
                engine_quit(lapis_get_engine());
                return 1;
            default:
                break;
            }
        }
        else if(event->type == SDL_QUIT)
        {
            engine_quit(lapis_get_engine());
            return 1;
        }
        else if(event->type == SDL_VIDEORESIZE)
        {
            player_movement_t *loc = malloc(sizeof(*loc));
            loc->x = event->resize.w;
            loc->y = event->resize.h;
            ref_t *ref = ref_create(loc);
            lsdl_resize_internal(event->resize.w, event->resize.h);
            message_create_and_send(NULL, NULL, "window-resize", ref, ASYNC);
            ref_dec(ref);
        }
    }

    return 0;
}                    

int main(int argc, char *argv[])
{
    lapis_init();
    
    engine_t *engine = lapis_get_engine();
    set_ticks_per_second(30);

    game_state_t *state = game_state_create(0);
    state->num_render_levels = RL_NUM_LEVELS;

    engine_switch_state(engine, state);

    /* initialize video */
    
    lsdl_set_video_mode(engine->sdl_driver,
                        1024, 768, 0, 1);

    SDL_EnableKeyRepeat(500, 50);
    SDL_WM_SetCaption( "Zombie Trap v. 0.1", NULL );

    /* test */
    /* load graphics */
    lapis_lua_t *lua = lua_scripting_init();
    if( lua_scripting_run_file(lua, "main.lua") != 0 )
        return 0;
    /* end test */

    /* create objects */

    game_object_t * global_object = game_object_create("global", NULL);
    game_state_append_object(state, global_object);
    game_object_set_recv_callback_c_func(global_object, global_message_handler);
    game_state_append_bcast_recvr(state,
                                  global_object,
                                  "sdl-event");
    
    player_object_t player_data;
    player_data.x = 1;
    player_data.y = 1;
    player_data.rounds = 5;
    player_data.light = 500;
    player_data.change_light_timer = player_data.change_light_timer_max = 15;

    game_object_t * player = game_object_create("player", &player_data);
    game_object_set_recv_callback_c_func(player, player_message_handler);
    game_state_append_bcast_recvr(state, player, "sdl-event");
    game_object_set_render_callback_c_func(player, player_render);
    game_object_set_update_callback_c_func(player, player_update);
    player->render_level = RL_PLAYER;

    zombie_controller_t *zc = zombie_controller_create();

    /* build map */

    map_t *map = map_create(64, 64);
    map_build_bsp(map, 0.75, 1.0);
    map_random_replace(map, 0, 1, 0.1);
    map_random_replace(map, 1, 0, 0.1);
    map_random_replace(map, 0, 'r', 1.0/50/50);
    map_random_replace(map, 0, 'L', 1.0/30/30);
    map_place_once(map, 0, 'P');
    //debug_map_display(map);
    debug_map_display_file(map);
       
    map_view_t *mv = map_view_create(16, 16, 31, 22);
    mv->map = map;

    /* build hud */
    hud_t *hud = hud_create();

    game_state_append_object(state, mv->game_object);
    game_state_append_object(state, zc->game_object);
    game_state_append_object(state, player);
    game_state_append_object(state, hud->game_object);
    

    lapis_mainloop();

    map_view_destroy(mv);
    map_destroy(map);

    /* this will be cleaned up by the OS */
    //game_state_destroy(state);
    //lapis_deinit();

    return 0;
}
