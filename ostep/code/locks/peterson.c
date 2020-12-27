/***
 * 
 * Peterson's Algorithm
 * p.321
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int flag[2];
int turn; 

void init(void)
{
    flag[0] = flag[1] = 0; 
    turn = 0; 
}

void lock()
{
    flag[self] = 1; 
    turn = 1 - self; 
    while ((flag[1 - self] == 1) && (turn == 1 - self)); /* Spin-wait*/
}

void unlock()
{
    flag[self] = 0; 
}