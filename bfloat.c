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
	unsigned long exp62_60, exp, man;
	exp62_60 = *py & 0x7000000000000000u;
	exp = *py & 0x7FF0000000000000u;
    man = *py & 0x000FFFFFFFFFFFFFu;
	if (!exp && !man){ /* zero */           
        return x;
    }
	if (exp == 0x7FF0000000000000u){ /* infinity or NaN */
		*py &= 0xFF80000000000000;
        *py |= (long)(man != 0) << 32;
        return y;
    }
	/* Normalized and Denormalized number. round to nearest */
    double r = x;
    unsigned long *pr = (long *) &r;
	if(exp62_60){
		*py &= 0x8000000000000000;
		*py |= 0x7F80000000000000;
		return y;
	}
    *pr &= 0xFFF0000000000000;
    r /= 2097152;
    y = x + r;
    *pr &= 0x8000000000000000;
    long tmp = (exp == 0);
    *pr |= (tmp << 28);
    y += r;
	*py = *py << 3;
	*py |= *pr;
    *py &= 0xFFFFFFFF00000000;
    return y;
}

void print_hex(float x) {
    int *p = (int *) &x;
    printf("%08x", *p);
}

void print_long_hex(double x) {
    long *p = (long *) &x;
    printf("%016lx", *p);
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
                printf("sign is %08x\n",sign[i]);
                printf("exp  is %08x\n",exp[j]);
                printf("man  is %08x\n",man[k]);
				printf("fp32   %f = ", *a);
                print_hex(*a);
				printf("\n");
                float bf_a = fp32tobf16(*a);
				printf("bfloat \t\t");
                print_hex(bf_a);
                printf("\n\n");
            }
        }
    }
    printf("fp64tofp32\n");
    unsigned long lsign[] = {0x0000000000000000, 0x8000000000000000};
    unsigned long lexp[]={0x0000000000000000, 0x7000000000000000, 0x0010000000000000, 0x7ff0000000000000};
    unsigned long lman[]={0x0000000000000000, 0x0008000000000000, 0x0000000010000000, 0x0000000000000001, 0x000fffffffffffff};
    unsigned long ldata=0;
    for (int i = 0; i < sizeof(lsign) / sizeof(lsign[0]); i++) {
        for (int j = 0; j < sizeof(lexp) / sizeof(lexp[0]); j++){
            for (int k = 0; k < sizeof(lman) / sizeof(lman[0]); k++) {
                ldata=0;
                ldata|=lsign[i];
                ldata|=lexp[j];
                ldata|=lman[k];
                double *a=(double *)&ldata;
                printf("sign is %016lx\n",lsign[i]);
                printf("exp  is %016lx\n",lexp[j]);
                printf("man  is %016lx\n",lman[k]);
				printf("fp64 %lf = ", *a);
                print_long_hex(*a);
				printf("\n");
				*a = fp64tofp32(*a);
				ldata = ldata >> 32;
				unsigned int data = (unsigned int)ldata;
                float *fp32 = (float *)&data;
				printf("fp32 %f = 00000000", *fp32);
                print_hex(*fp32);
                printf("\n\n");
            }
        }
    }

    return 0;
}