#ifndef KEYBOARD_DEFINES_H
#define KEYBOARD_DEFINES_H

/* Defines important constants for keyboard events
 * like event values, keyboard keys values, etc
 * Also defines the Controls structure for players
*/

#define EVENT_PRESS_KEY 10
#define EVENT_HOLD_KEY 11
#define EVENT_RELEASE_KEY 12

#define LEFT_ARROW_KEY 82
#define RIGHT_ARROW_KEY 83
#define UP_ARROW_KEY 84
#define DOWN_ARROW_KEY 85
#define SPACE_KEY 75
#define A_KEY 1
#define D_KEY 4
#define W_KEY 23
#define S_KEY 19
#define ENTER_KEY 67

// Controls: keeps track of controls of player
struct controls {
    short left;
    short right;
    short up;
    short down;
};
typedef struct controls Controls;

// sets keys of given control
void set_control(Controls *control, short left, short right, short up, short down);

#endif
