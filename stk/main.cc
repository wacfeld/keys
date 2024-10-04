#include <ADSR.h>
#include <Generator.h>

using namespace stk;

void printtick(Generator *g, int n) {
    StkFrames fr(1, 1);
    for(int i = 0; i < n; i++) {
        g->tick(fr);
        printf("%f\n", fr[0]);
    }
}

int main() {
    ADSR adsr;
    adsr.keyOn();
    printtick(&adsr, 1000);
    adsr.keyOff();
    puts("off");
    printtick(&adsr, 5);
    adsr.setAttackTarget(0.5);
    adsr.keyOn();
    printtick(&adsr, 100);
}
