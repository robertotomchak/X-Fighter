/*
    Defines a sprite object, which can draw images based on a sprite file
    Sprite must be one row and <num> columns
*/

#include "sprite.h"

// creates a sprite object
Sprite *create_sprite(const char *filename, long source_size_x, long source_size_y, int num)
{
    Sprite *img = malloc(sizeof(Sprite));
    if (!img)
        return NULL;

    img->bitmap = al_load_bitmap(filename);
    if (!img->bitmap) {
        free(img);
        return NULL;
    }
    img->source_size.x = source_size_x;
    img->source_size.y = source_size_y;
    // by default, has same size of original sprite
    img->dest_size.x = source_size_x;
    img->dest_size.y = source_size_y;
    img->num = num;
    // begins at first sprite by default and origin coords
    img->index = 0;
    img->coords.x = 0;
    img->coords.y = 0;

    return img;
}

// resizes based on given height
void resize_sprite_by_height(Sprite *img, long height)
{
    img->dest_size.y = height;
    img->dest_size.x = img->source_size.x * height / img->source_size.y;
}

// defines wanted sprite index
void set_sprite_index(Sprite *img, int index)
{
    img->index = index % img->num;
}

// sets coords of sprite
// coords of middle-bottom (more convenient for my application)
void set_sprite_coords(Sprite *img, long x, long y)
{
    // turn middle-bottom to top-left
    img->coords.x = x - img->dest_size.x / 2;
    img->coords.y = y - img->dest_size.y;
}

// draws sprite on given coords
void draw_sprite(Sprite *img, bool reverse)
{
    int flag = reverse? ALLEGRO_FLIP_HORIZONTAL: 0;
    al_draw_scaled_bitmap(
        img->bitmap, img->index * img->source_size.x, 0,
        img->source_size.x, img->source_size.y,
        img->coords.x, img->coords.y, img->dest_size.x, img->dest_size.y, flag
    );
}

// frees memory
void destroy_sprite (Sprite *img)
{
    al_destroy_bitmap(img->bitmap);
    free(img); img = NULL;
}
