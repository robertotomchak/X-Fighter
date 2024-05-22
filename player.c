/* Defines the Player of the games */
#include "player.h"

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
        p->coords.y = max(min_y + p->size.y, p->coords.y + p->speed.y);
    p->speed.y += gravity;
}

// returns true if the players collide horizontally
bool collision_x(Player *p1, Player *p2)
{
    return (p1->coords.x >= p2->coords.x && p1->coords.x < p2->coords.x + p2->size.x) || (p2->coords.x >= p1->coords.x && p2->coords.x < p1->coords.x + p1->size.x);
}

// returns true if the players collide vertically
bool collision_y(Player *p1, Player *p2)
{
    long h1, h2;
    h1 = p1->size.y;
    h2 = p2->size.y;
    if (p1->status == CROUCH)
        h1 /=  2;
    if (p2->status == CROUCH)
        h2 /= 2;

    return (p1->coords.y <= p2->coords.y && p1->coords.y > p2->coords.y - h2) || (p2->coords.y <= p1->coords.y && p2->coords.y > p1->coords.y - h1);
}

// updates player based on event type and key 
// min_screen and max_screen define screen limits
// deals with collision with p_other
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity, Player *p_other)
{
    long h, h_other;
    h = p->size.y;
    h_other = p_other->size.y;
    if (p->status == CROUCH)
        h /= 2;
    if (p_other->status == CROUCH)
        h_other /= 2;
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

    if (p->joystick.left.active) {
        // other player at left
        if (p_other->coords.x + p_other->size.x <= p->coords.x && collision_y(p, p_other))
            min_screen.x = p_other->coords.x + p_other->size.x;
        p->coords.x = max(min_screen.x, p->coords.x - p->speed.x);

    }
    if (p->joystick.right.active) {
        // other player at right
        if (p->coords.x + p->size.x <= p_other->coords.x && collision_y(p, p_other))
            max_screen.x = p_other->coords.x;
        p->coords.x = min(max_screen.x - p->size.x, p->coords.x + p->speed.x);
    }
    if (p->joystick.up.active) {
        p->status = AIR;
        p->speed.y = -p->jump_speed;
        p->joystick.up.active = 0;
    }
    // vertical movement (only if player is on the air and not on the top of the other)
    if (p->status == AIR && !(collision_x(p, p_other) && p->coords.y == p_other->coords.y - h_other)) {
        move_player_y(p, min_screen.y, max_screen.y, gravity);
        if (p->coords.y >= max_screen.y)
            p->status = p->joystick.down.active? CROUCH: STANDING;
        // collision with other player
        else if (collision_x(p, p_other) && collision_y(p, p_other)) {
            // other player is above
            if (p->speed.y < 0)
                p->coords.y = p_other->coords.y + h;
            // other player is below
            else
                p->coords.y = p_other->coords.y - h_other;
            p->speed.y = 0;               
        }
    }
    else if (p->status != AIR) {
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
        al_draw_filled_rectangle(p->coords.x, p->coords.y - p->size.y / 2, p->coords.x + p->size.x, p->coords.y, p->color);
    else
        al_draw_filled_rectangle(p->coords.x, p->coords.y - p->size.y, p->coords.x + p->size.x, p->coords.y, p->color);
}

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p)
{
    free(p);
    p = NULL;
}



