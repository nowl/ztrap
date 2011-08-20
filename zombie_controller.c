#include "ztrap.h"

static void
new_zombie(zombie_controller_t *zc)
{
    player_object_t *player = game_object_get_by_name("player")->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    map_t *map = mv->map;
    int x = player->x + random_int_min_max(10, 20);
    int y = player->y + random_int_min_max(10, 20);
    x *= random_int_min_max(0, 1) == 0 ? 1 : -1;
    y *= random_int_min_max(0, 1) == 0 ? 1 : -1;

    if(x < 0) x = 0;
    if(y < 0) y = 0;
    
    if(x >= map->width) x = map->width - 1;
    if(y >= map->height) y = map->height - 1;

    if(map_get_value(map, x, y) == 1)
    {
        /* obstruction */
        return;
    }

    char *zombie_name;
    asprintf(&zombie_name, "zombie-%d", zc->total_zombie_counter++);
    zc->active_zombie_counter++;

    zombie_t *zom = zombie_create(zombie_name);
    zom->x = x;
    zom->y = y;

    LOG("new zombie \"%s\" at %d, %d\n", zombie_name, x, y);

    game_state_append_object(lapis_get_engine()->state, zom->game_object);

    free(zombie_name);
}

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
    zombie_controller_t *zc = obj->data;
    if(--zc->next_zombie_timer == 0)
    {
        zc->next_zombie_timer = random_int_min_max(30, 60);
        new_zombie(zc);
    }
}

zombie_controller_t *
zombie_controller_create()
{
    zombie_controller_t *zc = malloc(sizeof(*zc));
    zc->total_zombie_counter = zc->active_zombie_counter = 0;
    zc->next_zombie_timer = random_int_min_max(50, 100);
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
    free(zc);
}
