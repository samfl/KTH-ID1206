#include <math.h>

#define MAX 2000
#define MIN 8

int request(void)
{
    /* k is log(MAX/MIN) */
    double k = log(((double) MAX) / MIN );

    /* r is [0 ..k] */
    double r = ((double)(rand() % (int)(k*10000))) / 10000;

    /* size if [0 .. MAX] */
    int size = (int) ((double)MAX / exp(r));

    return size;
}
