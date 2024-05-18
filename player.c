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
Player *create_player(short up, short left, short down, short right, short x, short y, short size_x, short size_y, short speed_x, short jump_speed, short stamina_speed, bool face_right, ALLEGRO_COLOR color)
{
    Player *p = malloc(sizeof(Player));
    if (!p)
        return NULL;
    p->joystick.up = up;
    p->joystick.left = left;
    p->joystick.down = down;
    p->joystick.right = right;

    p->coords.x = x;
    p->coords.y = y;
    p->size.x = size_x;
    p->size.y = size_y;

    p->speed.x = speed_x;
    p->speed.y = 0;  // starts on ground
    p->jump_speed = jump_speed;
    p->status = STANDING;

    p->health = 100; // starts at full health
    p->stamina = 100;  // starts at full stamina;
    p->stamina_speed = stamina_speed;

    p->face_right = p->face_right;
    p->color = color;

    return p;
}

// moves player horizontally
void move_player_x(Player *p, short min_x, short max_x, bool left)
{
    if (left)
        p->coords.x = max(min_x, p->coords.x - p->speed.x);
    else
        p->coords.x = min(max_x, p->coords.x + p->speed.x);
}

// moves player vertically and updates its speed
void move_player_y(Player *p, short min_y, short max_y, short gravity)
{
    if (p->status != AIR)
        return;
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
    // movement
    // horizontal movement
    if (event == ALLEGRO_EVENT_KEY_CHAR || event == ALLEGRO_EVENT_KEY_DOWN) {
        if (key == p->joystick.left)
            move_player_x(p, min_screen.x, max_screen.x, true);
        else if (key == p->joystick.right)
            move_player_x(p, min_screen.x, max_screen.x, false);
    }
    // vertical movement
    move_player_y(p, min_screen.y, max_screen.y, gravity);

    // status update
    switch (p->status) {
        case STANDING:
            // jump
            if (event == ALLEGRO_EVENT_KEY_DOWN && key == p->joystick.up) {
                p->status = AIR;
                p->speed.y = -p->jump_speed;
            }
            // crouching
            else if (event == ALLEGRO_EVENT_KEY_DOWN && key == p->joystick.down)
                p->status = CROUCH;
            break;
        case AIR:
            if (p->coords.y + p->size.y >= max_screen.y)
                p->status = STANDING;
            break;
        case CROUCH:
            if (event == ALLEGRO_EVENT_KEY_UP && key == p->joystick.down)
                p->status = STANDING;
            break;
        default:
            break;
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



