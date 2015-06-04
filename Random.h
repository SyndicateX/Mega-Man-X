/*Random.h
* Exports functionality for generating random integers and real numbers within a specified range. Also exports
* a function for true with some probability.
*/
#ifndef _RANDOM_H
#define _RANDOM_H

/*Function: randomInteger
*Usage: number = randomInteger(low, hight);
*Description: Returns a pseudo-random integer between low and high, inclusive.
*/
int randomInteger(int a, int b);

/*Function: randomReal
*Usage: realNumber = randomReal(low, high)
*Description: Returns a pseudo-random double value in the range [low, high).
*/
double randomReal(double a, double b);

/*Function: randomChance
*Usage:if(randomChance(percent))...
*Description: Returns true with probability percent, and false with probability 1 - percent.
*/
bool randomChance(double percent);


#endif