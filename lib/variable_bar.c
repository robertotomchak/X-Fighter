/* Defines a Variable Bar, which is a bar that starts full and decreases based on present value
   In this project, there are two types: health and stamina bars
 */

#include "variable_bar.h"

// creates variable bar object
Var_Bar *create_var_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value, ALLEGRO_COLOR color)
{
    Var_Bar *bar = malloc(sizeof(Var_Bar));
    if (!bar)
        return NULL;
    bar->coords.x = x;
    bar->coords.y = y;
    bar->size.x = size_x;
    bar->size.y = size_y;
    bar->inverted = inverted;
    bar->max_value = max_value;
    bar->value = max_value; // starts at full
    bar->color = color;

	return bar;
}

// updates given variable bar with given value
void update_var_bar(Var_Bar *bar, short value)
{
    bar->value = value;
}

// draws variable bar
void draw_var_bar(Var_Bar *bar)
{
    // calculating size of present variable bar
    long size = (bar->size.x * bar->value) / bar->max_value;

    if (!bar->inverted) {
        al_draw_filled_rectangle(bar->coords.x, bar->coords.y, bar->coords.x + bar->size.x, bar->coords.y + bar->size.y, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(bar->coords.x, bar->coords.y, bar->coords.x + size, bar->coords.y + bar->size.y, bar->color);
    }
    else {
        al_draw_filled_rectangle(bar->coords.x - bar->size.x, bar->coords.y, bar->coords.x, bar->coords.y + bar->size.y, al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(bar->coords.x - size, bar->coords.y, bar->coords.x, bar->coords.y + bar->size.y, bar->color);
    }   
}

// destroys variable bar object
void destroy_var_bar(Var_Bar *bar)
{
    free(bar); bar = NULL;
}

// creates a health bar
Var_Bar *create_health_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value)
{
    return create_var_bar(x, y, size_x, size_y, inverted, max_value, al_map_rgb(255, 0, 0));
}

// creates a stamina bar
Var_Bar *create_stamina_bar(short x, short y, short size_x, short size_y, bool inverted, short max_value)
{
    return create_var_bar(x, y, size_x, size_y, inverted, max_value, al_map_rgb(0, 0, 255));
}
