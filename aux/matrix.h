#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int out;
    int in;
    int keys; // keys <= out * in
    char *buf;
} Matrix;

void initMatrix();
void cleanupMatrix();
void poll(Matrix *);
void printmat(Matrix *);

#endif
