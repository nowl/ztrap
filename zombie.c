#include "ztrap.h"

static int
message_handler(game_object_t *obj, message_t *mes)
{
    zombie_t *z = obj->data;

    if(mes->type == lapis_hash("bullet-pos"))
    {
        player_movement_t *loc = mes->data->data;

        if(loc->x == z->x && loc->y == z->y)
        {
            /* mark for destroy on next update */
            z->destroy = 1;

            /* also destroy the bullet */
            bullet_t * bullet = mes->sender->data;
            bullet->destroy = 1;

            /* increase score */
            player_object_t *player = game_object_get_by_name("player")->data;
            player->score += 500;
        }
    }
    
    return 0;
}                    

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
    zombie_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    if( map_get_visibility(mv->map, data->x, data->y) > 0.5 )
        lsdl_draw_image(engine, image_loader_get("zombie1"),
                        map_view_pos_to_screen_x(mv, data->x),
                        map_view_pos_to_screen_y(mv, data->y),
                        32,
                        32,
                        1.0, 0, 0);
}

static void
zombie_remove_and_destroy(zombie_t *zombie)
{
    game_object_remove(zombie->game_object);
    game_object_destroy(lapis_get_engine(), zombie->game_object);
    zombie_destroy(zombie);
    zombie_controller_t *zc = game_object_get_by_name("zombie-controller")->data;
    zc->active_zombie_counter--;
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
    zombie_t *data = obj->data;

    if(data->destroy)
    {
        zombie_remove_and_destroy(data);
        return;
    }
    
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

        /* check player death */
        if(data->x == player->x && data->y == player->y)
            player->dead = 1;
    }
}

zombie_t *
zombie_create(char *name)
{
    zombie_t *z = malloc(sizeof(*z));
    z->game_object = game_object_create(name, z);
    z->game_object->render_level = RL_ZOMBIES;
    z->next_path_timer_max = 30;
    z->next_path_timer = z->next_path_timer_max;
    z->destroy = 0;
    game_object_set_recv_callback_c_func(z->game_object, message_handler);
    game_object_set_render_callback_c_func(z->game_object, render);
    game_object_set_update_callback_c_func(z->game_object, update);
    
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  z->game_object,
                                  "bullet-pos");
    return z;
}

void
zombie_destroy(zombie_t *z)
{
    free(z);
}
