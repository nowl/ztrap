#ifndef __ZTRAP_H__
#define __ZTRAP_H__

#include "lapis.h"

/* data structures */

typedef struct player_object player_object_t;
typedef struct map map_t;
typedef struct map_view map_view_t;
typedef struct player_movement player_movement_t;
typedef struct mes_light_amt mes_light_amt_t;
typedef struct zombie zombie_t;
typedef struct zombie_controller zombie_controller_t;
typedef struct bullet bullet_t;

struct bullet
{
    int x, y;
    int dx, dy;
    int next_move_timer, next_move_timer_max;
    game_object_t *game_object;
};

struct zombie
{
    int x, y;
    int next_path_timer, next_path_timer_max;
    game_object_t *game_object;
};

struct zombie_controller
{
    unsigned int zombie_counter;
    game_object_t *game_object;
    int next_zombie_timer;
};

struct player_object
{
    int x, y, nx, ny;
    unsigned long set_light_timer;
    unsigned long set_light_timer_max;
};

struct map
{
    int width;
    int height;
    int *tiles;
    float *ambiance;
};

enum direction4
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum direction8
{
    N,
    S,
    E,
    W,
    NW,
    NE,
    SE,
    SW
};

struct mes_light_amt
{
    float lighting;
};

struct map_view
{
    map_t *map;
    int screen_pos_x, screen_pos_y;
    int xs, xe, ys, ye;
    int player_x, player_y;
    float lighting;
    game_object_t *game_object;
    int screen_w, screen_h;
};

struct player_movement
{
    int x, y;
    enum direction8 dir;
};

/* player */

int  player_message_handler(game_object_t *obj, message_t *mes);
void player_render(engine_t *engine, game_object_t *obj, float interpolation);
void player_update(engine_t *engine, game_object_t *obj, unsigned int ticks);

/* map */

map_t *map_create(int w, int h);
void   map_destroy(map_t *map);
void   debug_map_display(map_t *map);
void   debug_map_display_file(map_t *map);
void   map_fill_region(map_t *map, int ul_x, int ul_y, int w, int h, int value);
void   map_build_bsp(map_t *map, float rec_factor, float decay_factor);
void   map_random_replace(map_t *map, int orig, int new, float prob);
int    map_get_value(map_t *map, int x, int y);
void   map_set_value(map_t *map, int x, int y, int value);
float  map_get_ambiance(map_t *map, int x, int y);
void   map_set_ambiance(map_t *map, int x, int y, float value);

/* map_view */

map_view_t *map_view_create(int screen_pos_x, int screen_pos_y, int width, int height);
void        map_view_destroy(map_view_t *mv);
int         map_view_pos_to_screen_x(map_view_t *mv, int x);
int         map_view_pos_to_screen_y(map_view_t *mv, int y);

/* zombie_controller */

zombie_controller_t *zombie_controller_create();
void                 zombie_controller_destroy(zombie_controller_t *zc);

/* zombie */

zombie_t *zombie_create(char *name);
void      zombie_destroy(zombie_t *z);

/* bullet */

bullet_t *bullet_create(char *name);
void      bullet_destroy(bullet_t *z);


#endif  /* __ZTRAP_H__ */
