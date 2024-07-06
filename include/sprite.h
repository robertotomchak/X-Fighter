#ifndef SPRITE_H
#define SPRITE_H
/*
    Defines a sprite object, which can draw images based on a sprite file
    Sprite must be one row and <num> columns
*/

// defines of sprites of this project
#define SPRITE_WIDTH 200
#define SPRITE_HEIGHT 120
#define NUM_SPRITES 8
#define CORRECTION_RATIO 1.25  // make sprite have the correct height

// name of each sprite
#define NORMAL1_SPRITE 0
#define WALK1_SPRITE 1
#define NORMAL2_SPRITE 2
#define WALK2_SPRITE 3
#define KICK_SPRITE 4
#define PREP_SPRITE 5
#define PUNCH_SPRITE 6
#define CROUCH_SPRITE 7

// number of frames that stays in sprite
#define WALK_FRAMES 10
#define PREP_FRAMES 30
#define PUNCH_FRAMES 15
#define KICK_FRAMES 15
#define TOTAL_WALK_SPRITES 4

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
Sprite *create_sprite(const char *filename, long source_size_x, long source_size_y, int num);

// resizes based on given height
void resize_sprite_by_height(Sprite *img, long height);

// defines wanted sprite index
void set_sprite_index(Sprite *img, int index);

// sets coords of sprite
// coords of middle-bottom (more convenient for my application)
void set_sprite_coords(Sprite *img, long x, long y);

// draws sprite on given coords
void draw_sprite(Sprite *img, bool reverse);

// frees memory
void destroy_sprite (Sprite *img);

#endif
