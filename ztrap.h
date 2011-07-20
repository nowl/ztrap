#ifndef __ZTRAP_H__
#define __ZTRAP_H__

#include "lapis.h"

/* data structures */

typedef struct player_object player_object_t;
typedef struct map map_t;

struct player_object
{
    int x, y;
    int anim_frame;
    unsigned int num_frames;
};

struct map
{
    int upper_left_x;
    int upper_left_y;
    int width;
    int height;
    int *tiles;
};

/* player */

int  player_message_handler(game_object_t *obj, message_t mes);
void player_render(engine_t *engine, game_object_t *obj, float interpolation);
void player_update(engine_t *engine, game_object_t *obj, unsigned int ticks);

/* map */

map_t *map_create(int x, int y, int w, int h);
void   map_destroy(map_t *map);
void   debug_map_display(map_t *map);
void   debug_map_display_file(map_t *map);
void   map_fill_region(map_t *map, int ul_x, int ul_y, int w, int h, int value);
void   map_build_bsp(map_t *map, float rec_factor, float decay_factor);
void   map_random_replace(map_t *map, int orig, int new, float prob);

#endif  /* __ZTRAP_H__ */
