/* Defines multiple auxiliary functions, which many of the libraries will use
*/


// sets values of given pair
void set_pair(Pair *pair, long x, long y)
{
    pair->x = x;
    pair->y = y;
}


// returns the maximum between two numbers
long max(long a, long b)
{
    return a > b? a: b;
}


// returns the minimum between two numbers
long min(long a, long b)
{
    return a < b? a: b;
}

