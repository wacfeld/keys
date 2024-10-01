#ifndef MATRIX_H
#define MATRIX_H
#include <pigpio.h>
#include <atomic>

typedef struct {
    int out;
    int in;
    int keys; // keys <= out * in
    std::atomic<char> *buf;
} Matrix;

void initMatrix(int);
void cleanupMatrix();
void poll(Matrix *);
void printmat(Matrix *);

#endif
