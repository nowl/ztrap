#include "ztrap.h"

static int
message_handler(game_object_t *obj, message_t *mes)
{
    return 0;
}                    

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
    zombie_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    lsdl_draw_image(engine, image_loader_get("zombie1"),
                    map_view_pos_to_screen_x(mv, data->x),
                    map_view_pos_to_screen_y(mv, data->y),
                    32,
                    32,
                    1.0);
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
}

zombie_t *
zombie_create(char *name)
{
    zombie_t *z = malloc(sizeof(*z));
    z->game_object = game_object_create(name, z);
    game_object_set_recv_callback_c_func(z->game_object, message_handler);
    game_object_set_render_callback_c_func(z->game_object, render);
    game_object_set_update_callback_c_func(z->game_object, update);
    
    /*
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  z->game_object,
                                  "map-move");
    */
    return z;
}

void
zombie_destroy(zombie_t *z)
{
    free(z);
}
