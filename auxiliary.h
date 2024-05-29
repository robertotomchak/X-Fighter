#ifndef AUXILIARY_H
#define AUXILIARY_H

/* Defines multiple auxiliary functions, which many of the libraries will use
*/

struct pair {
    int x;
    int y;
};
typedef struct pair Pair;

// sets values of given pair
void set_pair(Pair *pair, int x, int y);

// returns the absolute value of x
int abs(int x);

// returns the maximum between two numbers
int max(int a, int b);


// returns the minimum between two numbers
int min(int a, int b);

#endif
