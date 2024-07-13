#ifndef BOT_H
#define BOT_H

#include <stdlib.h>
#include <stdbool.h>

#include "player.h"

#define PROB_CROUCH 20
#define PROB_JUMP 10
#define PROB_KICK 30
#define PROB_PUNCH 40

/*
    Defines the Bot plays for the game
    OBS: MUST SET SEED FOR RANDOM PLAYS
    Bot's Strategies:
        - If low in stamina, do nothing
        - If far away from other player, move to it
        - If crounching with low stamina or far away from other player, stop crouching
        - If in range, make a random move (punch, kick, crouch or jump)
        - If in air or attack state, do nothing
*/

// makes a play and stores in pointers
// if does nothing, sets pointers to null
void bot_play(Player *bot, Player *p_other, int *event_type, int *keycode);

#endif