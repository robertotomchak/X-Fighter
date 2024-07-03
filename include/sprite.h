/*
    Defines a sprite object, which can draw images based on a sprite file
    Sprite must be one row and <num> columns
*/

#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "auxiliary.h"

struct sprite {
    ALLEGRO_BITMAP *bitmap;
    Pair source_size;  // size of each sprite in original file
    Pair dest_size;  // size of each sprite in application
    Pair coords;  // coords of top left
    int num;  // number of total sprites
    int index;  // sprite now (0..num - 1)
};
typedef struct sprite Sprite;

// creates a sprite object
Sprite *create_sprite(const char *filename, long source_size_x, long source_size_y, long dest_size_x, long dest_size_y, int num);

// defines wanted sprite index
void set_sprite_index(Sprite *img, int index);

// sets coords of sprite
// coords of middle-bottom (more convenient for my application)
void set_sprite_coords(Sprite *img, long x, long y);

// draws sprite on given coords
void draw_sprite(Sprite *img, bool reverse);

// frees memory
void destroy_sprite (Sprite *img);