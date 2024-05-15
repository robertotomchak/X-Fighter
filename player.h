#ifndef PLAYER_H
#define PLAYER_H

/* Defines the Player of the games */

#include "keyboard_defines.h"
#include "auxiliary.h"


struct player {
    Control controls;
    Pair coords;
    Pair size;
    Pair speed;
    short health;
};
typedef struct player Player;


