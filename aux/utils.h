#ifndef UTILS_H
#define UTILS_H

#define timeit(expr, n) do{unsigned long a=gettime(); for(int i = 0; i < n; i++) {expr;} unsigned long b=gettime(); printf("took %lu ns to run " #expr " %d times, i.e. %f each, %f a second\n", b-a, n, ((double)b-a)/n, 1000000000/(((double)b-a)/n));} while(0)

unsigned long gettime();
void elapsed(unsigned long, unsigned long);

#endif
