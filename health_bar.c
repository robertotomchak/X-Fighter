/* Defines the Health Bar of the game */

#include "health_bar.h"

/*
struct health_bar {
    Pair coords;
    Pair size;
    bool end_left;  // which size the health ends
    short max_health;
    short health;
};
typedef struct health_bar Health_Bar;
*/

// creates health bar object
Health_Bar *create_health_bar(short x, short y, short size_x, short size_y, bool inverted, short max_health)
{
    Health_Bar *hb = malloc(sizeof(Health_Bar));
    if (!hb)
        return NULL;
    hb->coords.x = x;
    hb->coords.y = y;
    hb->size.x = size_x;
    hb->size.y = size_y;
    hb->inverted = inverted;
    hb->max_health = max_health;
    hb->health = max_health; // starts at full
}

// updates given health bar with player's health
void update_health_bar(Health_Bar *hb, short p_health)
{
    hb->health = p_health;
}

// draws health bar
void draw_health_bar(Health_Bar *hb)
{
    // calculating size of present health bar
    long size_health = (hb->size.x * hb->health) / hb->max_health;

    if (!hb->inverted) {
        al_draw_filled_rectangle(hb->coords.x, hb->coords.y, hb->coords.x + hb->size.x, hb->coords.y + hb->size.y, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(hb->coords.x, hb->coords.y, hb->coords.x + size_health, hb->coords.y + hb->size.y, al_map_rgb(255, 0, 0));
    }
    else {
        al_draw_filled_rectangle(hb->coords.x - hb->size.x, hb->coords.y, hb->coords.x, hb->coords.y + hb->size.y, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(hb->coords.x - size_health, hb->coords.y, hb->coords.x, hb->coords.y + hb->size.y, al_map_rgb(255, 0, 0));
    }   
}

// destroys health bar object
void destroy_health_bar(Health_Bar *hb)
{
    free(hb); hb = NULL;
}