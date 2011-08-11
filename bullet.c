#include "ztrap.h"

static int
message_handler(game_object_t *obj, message_t *mes)
{
    return 0;
}                    

static void
render(engine_t *engine, game_object_t *obj, float interpolation)
{
    bullet_t *data = obj->data;
    map_view_t *mv = game_object_get_by_name("map-view")->data;
    lsdl_draw_image(engine, image_loader_get("bullet1"),
                    map_view_pos_to_screen_x(mv, data->x),
                    map_view_pos_to_screen_y(mv, data->y),
                    32,
                    32,
                    0, 1.0, 0);
}

static void
bullet_remove_and_destroy(bullet_t *b)
{
    game_object_remove(b->game_object);
    game_object_destroy(lapis_get_engine(), b->game_object);
    bullet_destroy(b);
}

static void
update(engine_t *engine, game_object_t *obj, unsigned int ticks)
{
    bullet_t *data = obj->data;
    
    if(--data->next_move_timer == 0)
    {
        data->next_move_timer = data->next_move_timer_max;
        data->x += data->dx;
        data->y += data->dy;

        /* destroy bullet if it goes off the screen */
        map_view_t *mv = game_object_get_by_name("map-view")->data;
        int screenx = map_view_pos_to_screen_x(mv, data->x);
        int screeny = map_view_pos_to_screen_y(mv, data->y);
        if(screenx < 0 || screenx > mv->screen_w ||
           screeny < 0 || screeny > mv->screen_h)
        {
            bullet_remove_and_destroy(data);
        }
    }
}

bullet_t *
bullet_create(char *name)
{
    bullet_t *b = malloc(sizeof(*b));
    b->game_object = game_object_create(name, b);
    b->next_move_timer_max = 5;
    b->next_move_timer = b->next_move_timer_max;
    game_object_set_recv_callback_c_func(b->game_object, message_handler);
    game_object_set_render_callback_c_func(b->game_object, render);
    game_object_set_update_callback_c_func(b->game_object, update);
    
    /*
    game_state_append_bcast_recvr(lapis_get_engine()->state,
                                  b->game_object,
                                  "map-move");
    */
    return b;
}

void
bullet_destroy(bullet_t *b)
{
    free(b);
}
