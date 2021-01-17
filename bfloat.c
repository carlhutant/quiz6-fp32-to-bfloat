#include <stdio.h>

float fp32tobf16(float x) {
    float y = x;
    int *py = (int *) &y;
    unsigned int exp, man;
    exp = *py & 0x7F800000u;
    man = *py & 0x007FFFFFu;
    if (!exp && !man) /* zero */           
        return x;
    if (exp == 0x7F800000u) {  /* infinity or NaN */
    // make sure the mantissa is non-zero if NaN
    *py |= ((man != 0) << 16);
    *py &= 0xFFFF0000;
    return y;
	}

    /* Normalized number. round to nearest */
    float r = x;
    int *pr = (int *) &r;
    *pr &= 0xff800000;
    r /= 256;
    y = x + r;
	*pr &= 0x80000000;
	*pr |= ((exp == 0) << 15);
	y += r;
    *py &= 0xffff0000;
    return y;
}

void print_hex(float x) {
    int *p = (int *) &x;
    printf("%f=%x\n", x, *p);
}

int main() {
    unsigned int sign[] = {0x00000000, 0x80000000};
    unsigned int exp[]={0x00000000, 0x07000000, 0x00800000, 0x7f800000};
    unsigned int man[]={0x00000000, 0x00400000, 0x00000001, 0x007fffff};
    unsigned int data=0;
    printf("fp32tobf16\n");
    for (int i = 0; i < sizeof(sign) / sizeof(sign[0]); i++) {
        for (int j = 0; j < sizeof(exp) / sizeof(exp[0]); j++){
            for (int k = 0; k < sizeof(man) / sizeof(man[0]); k++) {
                data=0;
                data|=sign[i];
                data|=exp[j];
                data|=man[k];
                float *a=(float *)&data;
                printf("sign is %x\n",sign[i]);
                printf("exp is %x\n",exp[j]);
                printf("man is %x\n",man[k]);
                print_hex(*a);
                float bf_a = fp32tobf16(*a);
                print_hex(bf_a);
                printf("\n");
            }
        }
    }
    return 0;
}