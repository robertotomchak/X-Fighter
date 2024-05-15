#include <stdio.h>

/* Defines important constants for keyboard events
 * like event values, keyboard keys values, etc
 * Also defines the Controls structure for players
*/


// sets keys of given control
void set_control(Controls *control, short left, short right, short up, short down)
{
    control->left = left;
    control->right = right;
    control->up = up;
    control->down = down;
}
