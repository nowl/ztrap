#include "ztrap.h"
#if 0
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
#endif

int main(int argc, char *argv[])
{
#if 0
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
    player_data.level = 1;
    player_data.score = 0;
    player_data.rounds = 5;
    player_data.light = 650;
    player_data.dead = 0;
    player_data.change_light_timer = player_data.change_light_timer_max = 15;

    game_object_t * player = game_object_create("player", &player_data);
    game_object_set_recv_callback_c_func(player, player_message_handler);
    game_state_append_bcast_recvr(state, player, "sdl-event");
    game_object_set_render_callback_c_func(player, player_render);
    game_object_set_update_callback_c_func(player, player_update);
    player->render_level = RL_PLAYER;

    zombie_controller_t *zc = zombie_controller_create();

    /* build initial map */

    map_t *map = map_build(64, 64, 1);
       
    map_view_t *mv = map_view_create(16, 16, 31, 22);
    mv->map = map;

    map_set_good_player_pos(map, &player_data);

    /* build hud */
    hud_t *hud = hud_create();

    game_state_append_object(state, mv->game_object);
    game_state_append_object(state, zc->game_object);
    game_state_append_object(state, player);
    game_state_append_object(state, hud->game_object);

    lapis_mainloop();

    map_view_destroy(mv);
    //map_destroy(mv->map);

    /* this will be cleaned up by the OS */
    //game_state_destroy(state);
    //lapis_deinit();

#endif

    struct entity *e = create_entity("testing");
    entity_set_meta(e, "render:final", 5, NULL);
    entity_set_meta(e, "render:final", 6, NULL);
    struct entity_meta *meta = entity_get_meta(e, "render:final");
    printf("%d\n", meta->type);

    struct component *c = create_component("renderable");
    component_add_responder(c, 0);
    c = create_component("foo component");
    component_add_responder(c, 0);

    struct component_list *cl = component_responder_lookup(0);
    for(; cl; cl = cl->next)
    {
        printf("responder component name = %s\n", cl->component.name);
    }

    destroy_component(c);

    printf("destroyed component\n");

    cl = component_responder_lookup(0);
    for(; cl; cl = cl->next)
    {
        printf("responder component name = %s\n", cl->component.name);
    }

    destroy_entity(e);

    cleanup_mempools();

    return 0;
}
