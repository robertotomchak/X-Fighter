
#include "bot.h"

/*
    Defines the Bot plays for the game
    OBS: MUST SET SEED FOR RANDOM PLAYS
    Bot's Strategies:
        - If far away from other player, move to it
        - If crounching with less than 50% stamina or far away from other player, stop crouching
        - If in range of kick, make a random move (punch, kick, crouch or jump)
        - If in air or attack state, do nothing
*/

// returns a random value between 0 and 100 (inclusive)
int random_prob()
{
    return rand() % 101;
}

// calculates if bot is far from player
// far = outside of punch range
bool is_far(Player *bot, Player *p_other)
{
    if (bot->face_right)
        return bot->coords.x + bot->size.x + bot->hit_sup->size.x < p_other->coords.x;
    else
        return p_other->coords.x + p_other->size.x < bot->coords.x - bot->hit_sup->size.x; 
}

// makes a random move, based on defined probabilities
void bot_random_move(Player *bot, int *event_type, int *keycode)
{
    int prob = random_prob();
    if (prob <= PROB_CROUCH) {
        *event_type = ALLEGRO_EVENT_KEY_DOWN;
        *keycode = bot->joystick.down.keycode;
        return;
    }
    prob -= PROB_CROUCH;
    if (prob <= PROB_JUMP) {
        *event_type = ALLEGRO_EVENT_KEY_DOWN;
        *keycode = bot->joystick.up.keycode;
        return;
    }
    prob -= PROB_JUMP;
    if (prob <= PROB_KICK) {
        *event_type = ALLEGRO_EVENT_KEY_DOWN;
        *keycode = bot->joystick.hit_inf.keycode;
        return;
    }
    prob -= PROB_KICK;
    if (prob <= PROB_PUNCH) {
        *event_type = ALLEGRO_EVENT_KEY_DOWN;
        *keycode = bot->joystick.hit_sup.keycode;
        return;
    }
    prob -= PROB_PUNCH;
}

// makes a play and stores in pointers
// if does nothing, sets pointers to null
void bot_play(Player *bot, Player *p_other, int *event_type, int *keycode)
{
    // only makes plays in standing or crouch states
    if (bot->state != STANDING && bot->state != CROUCH) {
        *event_type = ALLEGRO_EVENT_TIMER;
        keycode = NULL;
        return;
    }
    // if crouching and is low on stamina or far away, stop crounching
    if (bot->state == CROUCH && (bot->stamina < MAX_STAMINA || is_far(bot, p_other))) {
        *event_type = ALLEGRO_EVENT_KEY_UP;
        *keycode = bot->joystick.down.keycode;
        return;
    }
    // if low in stamina, do nothing
    if (bot->stamina < MAX_STAMINA) {
        *event_type = ALLEGRO_EVENT_TIMER;
        keycode = NULL;
        return;
    }
    // if in range, make random move
    if (!is_far(bot, p_other)) {
        bot_random_move(bot, event_type, keycode);
        return;
    }
    // else, walk to the player
    if (bot->face_right) {
        // already walking to correct direction
        if (bot->joystick.right.active && !bot->joystick.left.active) {
            *event_type = ALLEGRO_EVENT_TIMER;
            keycode = NULL;
        }
        // walking in wrong direction
        else if (bot->joystick.left.active) {
            *event_type = ALLEGRO_EVENT_KEY_UP;
            *keycode = bot->joystick.left.keycode;
        }
        // start walking
        else {
            *event_type = ALLEGRO_EVENT_KEY_DOWN;
            *keycode = bot->joystick.right.keycode;
        }
    }
    else {
        // already walking to correct direction
        if (bot->joystick.left.active && !bot->joystick.right.active) {
            *event_type = ALLEGRO_EVENT_TIMER;
            keycode = NULL;
        }
        // walking in wrong direction
        else if (bot->joystick.right.active) {
            *event_type = ALLEGRO_EVENT_KEY_UP;
            *keycode = bot->joystick.right.keycode;
        }
        // start walking
        else {
            *event_type = ALLEGRO_EVENT_KEY_DOWN;
            *keycode = bot->joystick.left.keycode;
        }
    }
    return;
}