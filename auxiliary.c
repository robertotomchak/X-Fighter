/* Defines multiple auxiliary functions, which many of the libraries will use
*/

#include "auxiliary.h"


// sets values of given pair
void set_pair(Pair *pair, int x, int y)
{
    pair->x = x;
    pair->y = y;
}

// returns the absolute value of x
int abs(int x)
{
    return x > 0? x: -x;
}

// returns the maximum between two numbers
int max(int a, int b)
{
    return a > b? a: b;
}


// returns the minimum between two numbers
int min(int a, int b)
{
    return a < b? a: b;
}

