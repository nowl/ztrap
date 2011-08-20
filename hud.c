#include "ztrap.h"


static int
message_handler(game_object_t *obj, message_t *mes)
{
    return 0;
}                    

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
    hud_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    player_object_t *player = game_object_get_by_name("player")->data;

    char *text;
    asprintf(&text, "%d", player->rounds);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   "Rounds", 255, 255, 255, 10, mv->screen_h-64+15);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   text, 0, 255, 0, 90, mv->screen_h-64+15);
    free(text);

    asprintf(&text, "%d", player->light);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   "Light", 255, 255, 255, 150, mv->screen_h-64+15);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   text, 0, 255, 0, 210, mv->screen_h-64+15);
    free(text);

    zombie_controller_t *zc = game_object_get_by_name("zombie-controller")->data;
    asprintf(&text, "%d", zc->active_zombie_counter);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   "Zombies", 255, 255, 255, 270, mv->screen_h-64+15);
    lsdl_draw_text(engine, "VeraMono.ttf", 16,
                   text, 0, 255, 0, 360, mv->screen_h-64+15);
    free(text);
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
/*
    zombie_t *data = obj->data;
    
    if(--data->next_path_timer == 0)
    {
        data->next_path_timer = data->next_path_timer_max;

        player_object_t *player = game_object_get_by_name("player")->data;
        struct astar_pos_t b = {data->x, data->y};
        struct astar_pos_t e = {player->x, player->y};

        astar_best_path(b, e, 250);

        astar_pos_vector_t path = astar_retrieve_path();
        int path_len = astar_retrieve_path_length();

        if(!path || path_len > 100)
        {
            zombie_remove_and_destroy(data);
            return;
        }

        if(path_len > 0)
        {
            data->x = path[1].x;
            data->y = path[1].y;
        }
    }
*/
}

hud_t *
hud_create()
{
    hud_t *h = malloc(sizeof(*h));
    h->game_object = game_object_create("hud", h);
    h->game_object->render_level = RL_HUD;
    game_object_set_recv_callback_c_func(h->game_object, message_handler);
    game_object_set_render_callback_c_func(h->game_object, render);
    game_object_set_update_callback_c_func(h->game_object, update);
    
    /*
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  z->game_object,
                                  "map-move");
    */
    return h;
}

void
hud_destroy(hud_t *h)
{
    free(h);
}
