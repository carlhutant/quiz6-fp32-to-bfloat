#include <stdio.h>

float fp32tobf16(float x) {
    float y = x;
    int *py = (int *) &y;
    unsigned int exp, man;
    exp = *py & 0x7F800000u;
    man = *py & 0x007FFFFFu;
    if (!exp && !man) /* zero */           
        return x;
    if (exp == 0x7F800000u) /* infinity or NaN */
        return x;

    /* Normalized number. round to nearest */
    float r = x;
    int *pr = (int *) &r;
    *pr &= BB1;
    r /= 256;
    y = x + r;

    *py &= BB2;
    return y;
}

int main(){
    return 0;
}