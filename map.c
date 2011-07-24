#include "ztrap.h"

map_t *
map_create(int w, int h)
{
    map_t *r = malloc(sizeof(*r));
    r->tiles = malloc(sizeof(*r->tiles) * w * h);
    r->ambiance = malloc(sizeof(*r->ambiance) * w * h);
    r->width = w;
    r->height = h;

    int i;
    for(i=0; i<w*h; i++)
    {
        float a = random_float();
        if(a < 0.25) a = 0.25;
        if(a > 0.9) a = 0.9;
        r->ambiance[i] = a;
    }

    return r;
}

void
map_destroy(map_t *map)
{
    free(map->tiles);
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
map_fill_region(map_t *map, int xs, int ys, int xe, int ye, int value)
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

void
debug_map_display(map_t *map)
{
    display_map(stdout, map);
}


void
debug_map_display_file(map_t *map)
{
    FILE *f = fopen("debug.txt", "w");
    display_map(f, map);
    fclose(f);
}

static void
map_build_bsp_rec(map_t *map, int xs, int ys, int xe, int ye, int depth, float rec_prob, float decay)
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

void
map_build_bsp(map_t *map, float rec_factor, float decay_factor)
{
    map_fill_region(map, 0, 0, map->width-1, map->height-1, 1);
    map_build_bsp_rec(map, 0, 0, map->width-1, map->height-1, 1, rec_factor, decay_factor);
}

void
map_random_replace(map_t *map, int orig, int new, float prob)
{
    int x,y;
    for(y=0; y<map->height; y++)
        for(x=0; x<map->width; x++)
            if(map_get_value(map, x, y) == orig && random_float() < prob)
                map_set_value(map, x, y, new);
}
