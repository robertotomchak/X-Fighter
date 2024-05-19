/* Defines the Player of the games */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "player.h"
#include "auxiliary.h"

#define STANDING 0
#define CROUCH 1
#define AIR 2


// creates a player
Player *create_player(short up, short left, short down, short right, short x, short y, short size_x, short size_y, short speed_x, short jump_speed, bool face_right, ALLEGRO_COLOR color)
{
    Player *p = malloc(sizeof(Player));
    if (!p)
        return NULL;
    p->joystick.up.keycode = up;
    p->joystick.left.keycode = left;
    p->joystick.down.keycode = down;
    p->joystick.right.keycode = right;
    // starts with no keys presed
    p->joystick.up.active = 0;
    p->joystick.left.active = 0;
    p->joystick.down.active = 0;
    p->joystick.right.active = 0;

    p->coords.x = x;
    p->coords.y = y;
    p->size.x = size_x;
    p->size.y = size_y;

    p->speed.x = speed_x;
    p->speed.y = 0;  // starts on ground
    p->jump_speed = jump_speed;
    p->status = STANDING;

    p->health = 100; // starts at full health

    p->face_right = p->face_right;
    p->color = color;

    return p;
}


// moves player vertically and updates its speed
void move_player_y(Player *p, short min_y, short max_y, short gravity)
{
    if (p->speed.y > 0)
        p->coords.y = min(max_y, p->coords.y + p->speed.y);
    else
        p->coords.y = max(min_y, p->coords.y + p->speed.y);
    p->speed.y += gravity;
}

// updates player based on event type and key 
// min_screen and max_screen define screen limits
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity)
{
    // if key pressed or release, update joystick
    if (event == ALLEGRO_EVENT_KEY_DOWN || event == ALLEGRO_EVENT_KEY_UP) {
        if (key == p->joystick.left.keycode)
            p->joystick.left.active ^= 1;
        else if (key == p->joystick.right.keycode)
            p->joystick.right.active ^= 1;
        else if (key == p->joystick.up.keycode && event == ALLEGRO_EVENT_KEY_DOWN && p->status == STANDING)
            p->joystick.up.active = 1;
        else if (key == p->joystick.down.keycode)
            p->joystick.down.active ^= 1;
            
        return;
    }

    // if update screen, move player and update status if necessary
    if (event != ALLEGRO_EVENT_TIMER)
        return;

    if (p->joystick.left.active)
        p->coords.x = max(min_screen.x, p->coords.x - p->speed.x);
    if (p->joystick.right.active)
        p->coords.x = min(max_screen.x - p->size.x, p->coords.x + p->speed.x);
    if (p->joystick.up.active) {
        p->status = AIR;
        p->speed.y = -p->jump_speed;
        p->joystick.up.active = 0;
    }
    // vertical movement
    if (p->status == AIR) {
        move_player_y(p, min_screen.y, max_screen.y, gravity);
        if (p->coords.y + p->size.y >= max_screen.y)
            p->status = p->joystick.down.active? CROUCH: STANDING;
    }
    else {
        if (p->joystick.down.active)
            p->status = CROUCH;
        else
            p->status = STANDING;
    }
}

// draws player
void draw_player(Player *p)
{
    if (p->status == CROUCH)
        al_draw_filled_rectangle(p->coords.x, p->coords.y + p->size.y / 2, p->coords.x + p->size.x, p->coords.y + p->size.y, p->color);
    else
        al_draw_filled_rectangle(p->coords.x, p->coords.y, p->coords.x + p->size.x, p->coords.y + p->size.y, p->color);
}

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p)
{
    free(p);
    p = NULL;
}



