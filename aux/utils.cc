#include <unistd.h>
#include <ctime>
#include <stdio.h>

// return the current unix time in nanoseconds
unsigned long gettime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec*1000000000 + time.tv_nsec;
}

void elapsed(unsigned long a, unsigned long b) {
    printf("%lu ns have elapsed\n", b-a);
}
