#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

/* Defines the Health Bar of the game */

#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "auxiliary.h"

struct health_bar {
    Pair coords;
    Pair size;
    bool inverted;  // true for right player
    short max_health;
    short health;
};
typedef struct health_bar Health_Bar;

// creates health bar object
Health_Bar *create_health_bar(short x, short y, short size_x, short size_y, bool inverted, short max_health);

// updates given health bar with player's health
void update_health_bar(Health_Bar *hb, short p_health);

// draws health bar
void draw_health_bar(Health_Bar *hb);

// destroys health bar object
void destroy_health_bar(Health_Bar *hb);

#endif