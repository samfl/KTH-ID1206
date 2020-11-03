#include <stdio.h>

int main(void)
{
    foo: 

    printf("the code: %p\n", &&foo);

    fgetc(stdin);

    return 0;
}

/*
$ ./code& - start process in the background
$ cat /proc/PID/maps - Memory mapping of the process
* The The code segment has protection: r-xp (read, execute private)
$ fg (Brings the suspended process to the foreground)

*/