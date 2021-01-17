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

double fp64tofp32(double x) {
    double y = x;
    long *py = (long *) &y;
    unsigned long exp, man;
    exp = *py & 0x7FF0000000000000u;
    man = *py & 0x000FFFFFFFFFFFFFu;
    if (!exp && !man){ /* zero */           
        return x;
    }
    if (exp == 0x7FF0000000000000u){ /* infinity or NaN */
        *py |= (long)(man != 0) << 32;
        *py &= 0xFFFFFFFF00000000;
        return y;
    }
    /* Normalized number. round to nearest */
    double r = x;
    unsigned long *pr = (long *) &r;
    *pr &= 0xFFF0000000000000;
    r /= 2097152;
    y = x + r;
    *pr &= 0x8000000000000000;
    long tmp = (exp == 0);
    *pr |= (tmp << 31);
    y += r;
    *py &= 0xFFFFFFFF00000000;
    return y;
}

void print_hex(float x) {
    int *p = (int *) &x;
    printf("%f=%x\n", x, *p);
}

void print_long_hex(double x) {
    long *p = (long *) &x;
    printf("%lf=%lx\n", x, *p);
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
    printf("fp64tofp32\n");
    unsigned long lsign[] = {0x0000000000000000, 0x8000000000000000};
    unsigned long lexp[]={0x0000000000000000, 0x7000000000000000, 0x0010000000000000, 0x7ff0000000000000};
    unsigned long lman[]={0x0000000000000000, 0x0008000000000000, 0x0000000080000000, 0x0000000000000001, 0x000fffffffffffff};
    unsigned long ldata=0;
    for (int i = 0; i < sizeof(lsign) / sizeof(lsign[0]); i++) {
        for (int j = 0; j < sizeof(lexp) / sizeof(lexp[0]); j++){
            for (int k = 0; k < sizeof(lman) / sizeof(lman[0]); k++) {
                ldata=0;
                ldata|=lsign[i];
                ldata|=lexp[j];
                ldata|=lman[k];
                double *a=(double *)&ldata;
                printf("sign is %lx\n",lsign[i]);
                printf("exp is %lx\n",lexp[j]);
                printf("man is %lx\n",lman[k]);
                print_long_hex(*a);
                double bf_a = fp64tofp32(*a);
                print_long_hex(bf_a);
                printf("\n");
            }
        }
    }

    return 0;
}