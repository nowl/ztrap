#include "ztrap.h"

static void
new_zombie(zombie_controller_t *zc)
{
    player_object_t *player = game_object_get_by_name("player")->data;
    int x = player->x + random_int_min_max(10, 20);
    int y = player->y + random_int_min_max(10, 20);
    x *= random_int_min_max(0, 1) == 0 ? 1 : -1;
    y *= random_int_min_max(0, 1) == 0 ? 1 : -1;

    LOG("TODO: new zombie at %d, %d\n", x, y);
}

static int
message_handler(game_object_t *obj, message_t *mes)
{
    if(mes->type == lapis_hash("player-move"))
    {
        zombie_controller_t *zc = obj->data;
        if(--zc->next_zombie_timer == 0)
        {
            zc->next_zombie_timer = random_int_min_max(5, 10);
            new_zombie(zc);
        }
    }

    return 0;
}                    

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
}

zombie_controller_t *
zombie_controller_create()
{
    zombie_controller_t *zc = malloc(sizeof(*zc));
    zc->zombies = NULL;
    zc->next_zombie_timer = random_int_min_max(5, 10);
    zc->game_object = game_object_create("zombie-controller", zc);
    game_object_set_recv_callback_c_func(zc->game_object, message_handler);
    game_object_set_render_callback_c_func(zc->game_object, render);
    game_object_set_update_callback_c_func(zc->game_object, update);
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  zc->game_object,
                                  "player-move");

    return zc;
}

void
zombie_controller_destroy(zombie_controller_t *zc)
{
    if(zc->zombies)
    {
        aatree_node_t *n = aatree_first(zc->zombies);
        while(n)
        {
            free(n->data);
            zc->zombies = aatree_delete(zc->zombies, n);
            n = aatree_first(zc->zombies);
        }
    }

    free(zc);
}
