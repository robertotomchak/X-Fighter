#include <stdio.h>

/* Defines important constants for keyboard events
 * like event values, keyboard keys values, etc
 * Also defines the Controls structure for players
*/


// creates a Control and returns a pointer to it
Controls *create_control(short left, short right, short up, short down)
{
    Control *control = malloc(sizeof(Control));
    control->left = left;
    control->left = right;
    control->left = up;
    control->left = down;
    return control;
}

