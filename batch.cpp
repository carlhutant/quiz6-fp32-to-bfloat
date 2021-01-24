#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <ctime>

void fp32tobf16(float *fp32, float *ans, int size) {
    for (int i = 0; i < size; i++) {
        float y = fp32[i];
        unsigned int* py = (unsigned int*)&y;
        unsigned int exp, man;
        exp = *py & 0x7F800000u;
        man = *py & 0x007FFFFFu;
        if (!exp && !man) { /* zero */
            ans[i] = fp32[i];
            continue;
        }
        if (exp == 0x7F800000u) {  /* infinity or NaN */
            *py |= ((man != 0) << 16);
            *py &= 0xFFFF0000;
            ans[i] = y;
            continue;
        }
        /* Normalized and Denormalized number.*/
        float r = fp32[i];
        unsigned int* pr = (unsigned int*)&r;
        *pr &= 0xff800000;
        r /= 256;
        y = fp32[i] + r;
        *pr &= 0x80000000;
        *pr |= ((exp == 0) << 15);
        y += r;
        *py &= 0xffff0000;
        ans[i] = y;
    }
}

void fp32tobf16_noif(float* fp32, float* ans, int size) {
    for (int i = 0; i < size; i++) {
        float y = fp32[i];
        unsigned int* py = (unsigned int*)&y;
        float r = fp32[i];
        unsigned int* pr = (unsigned int*)&r;
        unsigned int exp, man;
        exp = *py & 0x7F800000u;
        man = *py & 0x007FFFFFu;
        //NaN
        *py |= ((exp == 0x7F800000u && man != 0) << 16);
        //Normals rounding
        *pr &= 0xff800000;
        r /= 256;
        y = fp32[i] + r;
        //Denormals rounding
        *pr &= 0x80000000;
        *pr |= ((exp == 0 && man != 0) << 15);
        y += r;
        *py &= 0xffff0000;
        ans[i] = y;
    }
}

void fp32tobf16_batch(float* fp32, float* ans, int size) {
    for (int i = 0; i < size; i += 4) {
        unsigned int sign[4] = { 0x80000000u, 0x80000000u , 0x80000000u , 0x80000000u };
        unsigned int exp[4] = { 0x7f800000u, 0x7f800000u , 0x7f800000u , 0x7f800000u };
        unsigned int man[4] = { 0x007FFFFFu, 0x007FFFFFu , 0x007FFFFFu , 0x007FFFFFu };
        unsigned int zero[4] = { 0x0u, 0x0u , 0x0u , 0x0u };
        float normal_round_div[4] = { 256, 256, 256, 256 };
        unsigned int denormal_round[4] = { 0x00008000u, 0x00008000u, 0x00008000u, 0x00008000u };
        unsigned int bit16[4] = { 0xffff0000u, 0xffff0000u, 0xffff0000u, 0xffff0000u };
        float out[4];

        __m128 y = _mm_load_ps(&fp32[i]);
        __m128 r = _mm_load_ps(&fp32[i]);
        __m128 cons_sign = _mm_load_ps((float*)sign);
        __m128 cons_exp = _mm_load_ps((float*)exp);
        __m128 cons_man = _mm_load_ps((float*)man);
        __m128 cons_zero = _mm_load_ps((float*)zero);
        
        __m128 m_exp = _mm_and_ps(y, cons_exp);
        __m128 m_man = _mm_and_ps(y, cons_man);

        /*_mm_store_ps(out, y);
        printf("y %08x\n", *(unsigned int*)&out[0]);
        printf("y %08x\n", *(unsigned int*)&out[1]);
        printf("y %08x\n", *(unsigned int*)&out[2]);
        printf("y %08x\n", *(unsigned int*)&out[3]);*/

        //NaN
        __m128 exp_all_1 = _mm_cmpeq_ps(m_exp, cons_exp);
        __m128 man_not_0 = _mm_cmpneq_ps(m_man, cons_zero);
        /*_mm_store_ps(out, exp_all_1);
        printf("exp_all_1 %08x\n", *(unsigned int*)&out[0]);
        printf("exp_all_1 %08x\n", *(unsigned int*)&out[1]);
        printf("exp_all_1 %08x\n", *(unsigned int*)&out[2]);
        printf("exp_all_1 %08x\n", *(unsigned int*)&out[3]);
        _mm_store_ps(out, man_not_0);
        printf("man_not_0 %08x\n", *(unsigned int*)&out[0]);
        printf("man_not_0 %08x\n", *(unsigned int*)&out[1]);
        printf("man_not_0 %08x\n", *(unsigned int*)&out[2]);
        printf("man_not_0 %08x\n", *(unsigned int*)&out[3]);*/
        __m128 nan_fix = _mm_and_ps(exp_all_1, man_not_0);
        nan_fix = _mm_and_ps(nan_fix, cons_man);
        /*_mm_store_ps(out, nan_fix);
        printf("nan_fix %08x\n", *(unsigned int*)&out[0]);
        printf("nan_fix %08x\n", *(unsigned int*)&out[1]);
        printf("nan_fix %08x\n", *(unsigned int*)&out[2]);
        printf("nan_fix %08x\n\n", *(unsigned int*)&out[3]);*/
        y = _mm_or_ps(y, nan_fix);
        //Normals rounding
        __m128 nrd = _mm_load_ps(normal_round_div);
        r = _mm_or_ps(_mm_and_ps(r, cons_sign), _mm_and_ps(r, cons_exp));
        r = _mm_div_ps(r, nrd);
        y = _mm_add_ps(y, r);
        //Denormals rounding
        __m128 dr = _mm_load_ps((float*)denormal_round);
        r = _mm_and_ps(r, cons_sign);
        r = _mm_or_ps(r, _mm_and_ps(_mm_and_ps(_mm_cmpeq_ps(m_exp, cons_zero), man_not_0), dr));//*pr |= ((exp == 0 && man != 0) << 15);
        _mm_store_ps(out, r);
        y = _mm_add_ps(y, r);
        __m128 mbit16 = _mm_load_ps((float*)bit16);
        y = _mm_and_ps(y, mbit16);
        _mm_store_ps(&ans[i], y);

        /*_mm_store_ps(out, m_exp);
        printf("%08x  ", *(unsigned int *)&fp32[i]);
        printf("%08x\n", *(unsigned int*)&out[0]);
        printf("%08x  ", *(unsigned int*)&fp32[i]);
        printf("%08x\n", *(unsigned int*)&out[1]);
        printf("%08x  ", *(unsigned int*)&fp32[i]);
        printf("%08x\n", *(unsigned int*)&out[2]);
        printf("%08x  ", *(unsigned int*)&fp32[i]);
        printf("%08x\n", *(unsigned int*)&out[3]);*/
    }
}

void fp64tofp32(double *fp64,  double *ans, int size) {
    for (int i = 0; i < size; i++) {
        printf("fp64[i] = %016llx\n", *(unsigned long long*) & fp64[i]);
        double y = fp64[i];
        unsigned long long* py = (unsigned long long*)&y;
        unsigned long long exp62_60, exp, man;

        exp62_60 = *py & 0x7000000000000000u;
        exp = *py & 0x7FF0000000000000u;
        man = *py & 0x000FFFFFFFFFFFFFu;
        /* zero */
        if (!exp && !man) {
            ans[i] = fp64[i];
            continue;
        }
        /* infinity or NaN */
        if (exp == 0x7FF0000000000000u) {
            *py &= 0xFF80000000000000;
            *py |= (unsigned long long)(man != 0) << 32;
            ans[i] = y;
            continue;
        }
        /* Normalized and Denormalized number.*/
        double r = fp64[i];
        unsigned long long* pr = (unsigned long long*) & r;
        if (exp62_60) {
            *py &= 0x8000000000000000;
            *py |= 0x7F80000000000000;
            ans[i] = y;
            continue;
        }
        //Normals rounding
        *pr &= 0xFFF0000000000000;
        r /= 2097152;
        printf("fp64[i] = %016llx\n", *(unsigned long long*) & fp64[i]);
        printf("r = %016llx\n", *pr);
        y = fp64[i] + r;
        printf("y = %016llx\n", *py);
        //Denormals rounding
        *pr &= 0x8000000000000000;
        unsigned long long tmp = (exp == 0);
        *pr |= (tmp << 28);
        y += r;
        *py = *py << 3;
        *py |= *pr;
        *py &= 0xFFFFFFFF00000000;
        ans[i] = y;
    }
}

int main() {
    /*fp32 to bfloat*/
    double cpu_time_used1 = 0;
    double cpu_time_used2 = 0;

    /*unsigned int sign[] = { 0x00000000, 0x80000000 };
    unsigned int exp[] = { 0x00000000, 0x70000000, 0x00800000, 0x7f800000 };
    unsigned int man[] = { 0x00000000, 0x00400000, 0x00000001, 0x007fffff };
    const int sign_size = sizeof(sign) / sizeof(sign[0]);
    const int exp_size = sizeof(exp) / sizeof(exp[0]);
    const int man_size = sizeof(man) / sizeof(man[0]);
    const int data_size = sign_size * exp_size * man_size;
    unsigned int data[data_size];
    printf("fp32 to bf16\n");
    for (int i = 0; i < sign_size; i++) {
        for (int j = 0; j < exp_size; j++) {
            for (int k = 0; k < man_size; k++) {
                int index = i * exp_size * man_size + j * man_size + k;
                data[index] = 0;
                data[index] |= sign[i];
                data[index] |= exp[j];
                data[index] |= man[k];
                //printf("%2d %08x\n", index, data[index]);
            }
        }
    }
    float* fp32 = (float*)&data;
    float bf_a[data_size];
    float bf_b[data_size];
    
    fp32tobf16_noif(fp32, bf_a, data_size);
    fp32tobf16_batch(fp32, bf_b, data_size);
    
    for (int i = 0; i < data_size; i++) {
        printf("sign is %08x\n", data[i] & 0x80000000);
        printf("exp  is %08x\n", data[i] & 0x7f800000);
        printf("man  is %08x\n", data[i] & 0x007fffff);

        printf("fp32   %f\n", fp32[i]);
        printf("fp32x  %08x\n", data[i]);
        printf("no_if %08x\n", *(unsigned int*)&bf_a[i]);
        printf("batch %08x\n", *(unsigned int*)&bf_b[i]);
        printf("\n");
    }*/
    clock_t start2, end2;
    start2 = clock();
    for (int it = 1; it < 2; it++) {
        for (int loop = 0; loop < 10000*it; loop++) {
            srand(time(NULL));
            const int data_size = 10000 * 4;
            unsigned int data[data_size];
            for (int i = 0; i < data_size; i++) {
                unsigned int r1, r2, r3, r4;
                r1 = ((unsigned int)rand() & 0x00ff);
                r2 = ((unsigned int)rand() & 0x00ff) << 8;
                r3 = ((unsigned int)rand() & 0x00ff) << 16;
                r4 = ((unsigned int)rand() & 0x00ff) << 24;
                data[i] = r1 | r2 | r3 | r4;
            }

            float* fp32 = (float*)&data;
            float bf_a[data_size];

            clock_t start, end; // 儲存時間用的變數
            start = clock(); // 計算開始時間
            fp32tobf16_noif(fp32, bf_a, data_size);
            end = clock(); // 計算結束時間
            cpu_time_used1 += ((double)end - (double)start) / CLOCKS_PER_SEC; // 計算實際花費時間

            start = clock(); // 計算開始時間
            fp32tobf16_batch(fp32, bf_a, data_size);
            end = clock(); // 計算結束時間
            cpu_time_used2 += ((double)end - (double)start) / CLOCKS_PER_SEC; // 計算實際花費時間
        }
        printf("fp32tobf16_noif_time_used = %llf\n", cpu_time_used1);
        printf("fp32tobf16_batch_time_used = %llf\n", cpu_time_used2);
        cpu_time_used1 = 0;
        cpu_time_used2 = 0;
    }
    end2 = clock();
    printf("time_used = %llf\n", ((double)end2 - (double)start2) / CLOCKS_PER_SEC);
    /*fp64 to fp32*//*
    unsigned long long lsign[] = { 0x0000000000000000, 0x8000000000000000 };
    unsigned long long lexp[] = { 0x0000000000000000, 0x7000000000000000, 0x0010000000000000, 0x7ff0000000000000 };
    unsigned long long lman[] = { 0x0000000000000000, 0x0008000000000000, 0x0000000010000000, 0x0000000000000001, 0x000fffffffffffff };
    const int lsign_size = sizeof(lsign) / sizeof(lsign[0]);
    const int lexp_size = sizeof(lexp) / sizeof(lexp[0]);
    const int lman_size = sizeof(lman) / sizeof(lman[0]);
    const int ldata_size = lsign_size * lexp_size * lman_size;
    unsigned long long int ldata[ldata_size];
    printf("fp64 to fp32\n");
    for (int i = 0; i < lsign_size; i++) {
        for (int j = 0; j < lexp_size; j++) {
            for (int k = 0; k < lman_size; k++) {
                int lindex = i * lexp_size * lman_size + j * lman_size + k;
                ldata[lindex] = 0;
                ldata[lindex] |= lsign[i];
                ldata[lindex] |= lexp[j];
                ldata[lindex] |= lman[k];
                //printf("%2d %016llx\n", lindex, ldata[lindex]);
            }
        }
    }
    double* fp64 = (double*)&ldata;
    double lfp32[ldata_size];

    start = clock(); // 計算開始時間
    fp64tofp32(fp64, lfp32, ldata_size);
    end = clock(); // 計算結束時間
    double cpu_time_used2 = ((double)end - (double)start) / CLOCKS_PER_SEC; // 計算實際花費時間
    */
    /*
    for (int i = 0; i < ldata_size; i++) {
        printf("sign is %016llx\n", ldata[i] & 0x8000000000000000);
        printf("exp  is %016llx\n", ldata[i] & 0x7ff0000000000000);
        printf("man  is %016llx\n", ldata[i] & 0x000fffffffffffff);
        
        unsigned int uint32 = (unsigned int)((*(unsigned long long int*) & lfp32[i]) >> 32);
        float fp32 = *(float*)&uint32;

        printf("fp64 %llf\n", fp64[i]);
        printf("fp32 %f\n", fp32);
        printf("fp64x %016llx\n", ldata[i]);
        printf("fp32x %08x\n", uint32);
        printf("\n");
    }*/
    
    
    return 0;
}