#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

/* Defines a Variable Bar, which is a bar that starts full and decreases based on present value
   In this project, there are two types: health and stamina bars
 */

#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "auxiliary.h"

struct var_bar {
    Pair coords;
    Pair size;
    bool inverted;  // true for right player
    short max_value;
    short value;
    ALLEGRO_COLOR color;
};
typedef struct var_bar Var_Bar;

// creates variable bar object
Var_Bar *create_var_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value, ALLEGRO_COLOR color);

// updates given variable bar with given value
void update_var_bar(Var_Bar *bar, short value);

// draws variable bar
void draw_var_bar(Var_Bar *bar);

// destroys variable bar object
void destroy_var_bar(Var_Bar *bar);

// creates a health bar
Var_Bar *create_health_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value);

// creates a stamina bar
Var_Bar *create_stamina_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value);

#endif