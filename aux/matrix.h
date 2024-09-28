typedef struct {
    int out;
    int in;
    int keys; // keys <= out * in
    char *buf;
} Matrix;

void init();
void cleanup();
void poll(Matrix *mat);
void printmat(Matrix *mat);
