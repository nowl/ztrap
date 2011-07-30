#include "ztrap.h"

static int
message_handler(game_object_t *obj, message_t *mes)
{
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
    zc->game_object = game_object_create("zombie-controller", zc);
    game_object_set_recv_callback_c_func(zc->game_object, message_handler);
    game_object_set_render_callback_c_func(zc->game_object, render);
    game_object_set_update_callback_c_func(zc->game_object, update);
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  zc->game_object,
                                  "map-move");

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
