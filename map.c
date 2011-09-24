#include "ztrap.h"

map_t * MAP = NULL;

static int
map_cost(unsigned int x, unsigned int y)
{
    if( map_get_value(MAP, x, y) == 1 )
        return -1;
    else
        return 1;
}

map_t *
map_create(int w, int h)
{
    astar_init(w, h, map_cost);

    map_t *r = malloc(sizeof(*r));
    r->tiles = malloc(sizeof(*r->tiles) * w * h);
    r->discovered = malloc(sizeof(*r->discovered) * w * h);
    r->ambiance = malloc(sizeof(*r->ambiance) * w * h);
    r->visibility = malloc(sizeof(*r->visibility) * w * h);
    r->width = w;
    r->height = h;

    int i;
    for(i=0; i<w*h; i++)
    {
        float a = random_float();
        if(a < 0.25) a = 0.25;
        if(a > 0.9) a = 0.9;
        r->ambiance[i] = a;

        r->discovered[i] = 0;
    }

    MAP = r;

    return r;
}

void
map_destroy(map_t *map)
{
    free(map->visibility);
    free(map->tiles);
    free(map->discovered);
    free(map->ambiance);
    free(map);
}

void
map_set_value(map_t *map, int x, int y, int value)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        map->tiles[map->width * y + x] = value;
}

int
map_get_value(map_t *map, int x, int y)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        return map->tiles[map->width * y + x];

    return -1;
}

void
map_set_ambiance(map_t *map, int x, int y, float value)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        map->ambiance[map->width * y + x] = value;
}

float
map_get_ambiance(map_t *map, int x, int y)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        return map->ambiance[map->width * y + x];

    return 1.0;
}

void
map_set_visibility(map_t *map, int x, int y, float value)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        map->visibility[map->width * y + x] = value;
}

float
map_get_visibility(map_t *map, int x, int y)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        return map->visibility[map->width * y + x];

    return 0.0;
}

void
map_set_discovered(map_t *map, int x, int y, char value)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        map->discovered[map->width * y + x] = value;
}

char
map_get_discovered(map_t *map, int x, int y)
{
    if(x >= 0 && x < map->width && y >= 0 && y < map->height)
        return map->discovered[map->width * y + x];

    return 1;
}

static void map_fill_region(map_t *map, int xs, int ys, int xe, int ye, int value)
{
    int x, y;
    for(x=xs; x<=xe; x++)
        for(y=ys; y<=ye; y++)
            map_set_value(map, x, y, value);
}

static void
display_map(FILE *fout, map_t *map)
{
    fprintf(fout, "tiles\n");
    int x, y;
    for(y=0; y<map->height; y++)
    {
        for(x=0; x<map->width; x++)
            fprintf(fout, "%d", map->tiles[y*map->width + x]);
        fprintf(fout, "\n");
    }        
}

void debug_map_display(map_t *map)
{
    display_map(stdout, map);
}


void debug_map_display_file(map_t *map)
{
    FILE *f = fopen("debug.txt", "w");
    display_map(f, map);
    fclose(f);
}

static void map_build_bsp_rec(map_t *map, int xs, int ys, int xe, int ye, int depth, float rec_prob, float decay)
{
    if(xe-xs < 2 || ye-ys < 2)
    {
        /* if too small to split then just fill the region and exit */
        map_fill_region(map, xs+1, ys+1, xe-1, ye-1, 0);
        return;
    }

    switch(random_int_min_max(0, 1))
    {
    case 0:
    {
        /* split vertically */
        //LOG("splitting vertically\n");

        /* pick a random point on the x axis */
        int splitting_point = random_int_min_max(xs+1, xe-1);
        //LOG("splitting point on x: %d\n", splitting_point);

        if(random_float() < rec_prob)
            map_build_bsp_rec(map, xs, ys, splitting_point, ye, depth + 1, rec_prob * decay, decay);
        else
            map_fill_region(map, xs+1, ys+1, splitting_point-1, ye-1, 0);

        if(random_float() < rec_prob)
            map_build_bsp_rec(map, splitting_point, ys, xe, ye, depth + 1, rec_prob * decay, decay);
        else
            map_fill_region(map, splitting_point+1, ys+1, xe-1, ye-1, 0);
        break;
    }
    case 1:
    {
        /* split horizontally */
        //LOG("splitting horizontally\n");

        /* pick a random point on the y axis */
        int splitting_point = random_int_min_max(ys+1, ye-1);
        //LOG("splitting point on y: %d\n", splitting_point);

        if(random_float() < rec_prob)
            map_build_bsp_rec(map, xs, ys, xe, splitting_point, depth + 1, rec_prob * decay, decay);
        else
            map_fill_region(map, xs+1, ys+1, xe-1, splitting_point-1, 0);

        if(random_float() < rec_prob)
            map_build_bsp_rec(map, xs, splitting_point, xe, ye, depth + 1, rec_prob * decay, decay);
        else
            map_fill_region(map, xs+1, splitting_point+1, xe-1, ye-1, 0);
        break;
    }
    }    
}

static void map_build_bsp(map_t *map, float rec_factor, float decay_factor)
{
    map_fill_region(map, 0, 0, map->width-1, map->height-1, 1);
    map_build_bsp_rec(map, 0, 0, map->width-1, map->height-1, 1, rec_factor, decay_factor);
}

static void map_random_replace(map_t *map, int orig, int new, float prob)
{
    int x,y;
    for(y=0; y<map->height; y++)
        for(x=0; x<map->width; x++)
            if(map_get_value(map, x, y) == orig && random_float() < prob)
                map_set_value(map, x, y, new);
}

static int map_count_radius(map_t *map, int x, int y, int radius, int type)
{
    int i, j, num = 0;
    for(j = y - radius; j < y + radius; j++)
        for(i = x - radius; i < x + radius; i++)
            if(map_get_value(map, i, j) == type)
                num++;

    return num;
}

static void map_place_once(map_t *map, int orig, int value, int radius, int min_num_empty)
{
    int placed = 0;

    while(!placed)
    {
        int x = random_int_min_max(0, map->width-1);
        int y = random_int_min_max(0, map->height-1);
        
        if(map_get_value(map, x, y) == orig)
        {
            /* count */
            if(radius > 0)
            {
                if(map_count_radius(map, x, y, radius, orig) < min_num_empty)
                    continue;
            }
                
            map_set_value(map, x, y, value);
            placed = 1;
        }
    }
}

void map_set_good_player_pos(map_t *map, player_object_t *player)
{
    int placed = 0;
    
    while(!placed)
    {
        int x = random_int_min_max(0, map->width-1);
        int y = random_int_min_max(0, map->height-1);
        
        if(map_get_value(map, x, y) == 0)
        {
            if(map_count_radius(map, x, y, 5, 0) < 75)
                continue;
            
            player->x = x;
            player->y = y;
            placed = 1;
        }
    }
}

map_t *map_build(int width, int height, int level)
{
    map_t *map = map_create(width*level, height*level);
    map_build_bsp(map, 0.75, 1.0);
    map_random_replace(map, 0, 1, 0.1);
    map_random_replace(map, 1, 0, 0.1);
    map_random_replace(map, 0, 'R', 1.0/20/20);
    map_random_replace(map, 0, 'L', 1.0/15/15);
    map_place_once(map, 0, 'P', 5, 75);

    return map;
}
