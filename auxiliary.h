#ifndef AUXILIARY_H
#define AUXILIARY_H

/* Defines multiple auxiliary functions, which many of the libraries will use
*/

struct pair {
    long x;
    long y;
};
typedef struct pair Pair;

// sets values of given pair
void set_pair(Pair *pair, long x, long y);


// returns the maximum between two numbers
long max(long a, long b);


// returns the minimum between two numbers
long min(long a, long b);

#endif
