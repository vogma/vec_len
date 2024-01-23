#include <stdio.h>
#include <math.h>
#include <arm_sve.h>

struct pt
{
    float x;
    float y;
    float z;
};

void vec_len_sve(float *r, struct pt *v, int n);

void vec_len_sve_intrinsics(float *r, struct pt *v, int n)
{
    int vl = svcntw();
    for (int i = 0; i < n; i += vl, r += vl, v += vl)
    {
        svbool_t predicate = svwhilelt_b32(i, n);
        svfloat32x3_t vector_regs = svld3(predicate, &(v->x));

        svfloat32_t vec_x = svget3(vector_regs, 0);
        svfloat32_t vec_y = svget3(vector_regs, 1);
        svfloat32_t vec_z = svget3(vector_regs, 2);

        vec_x = svmul_z(predicate, vec_x, vec_x);
        vec_x = svmla_m(predicate, vec_x, vec_y, vec_y);
        vec_x = svmla_m(predicate, vec_x, vec_z, vec_z);
        vec_x = svsqrt_z(predicate, vec_x);
        
        svst1(predicate, r, vec_x);
    }
}

void vec_len(float *r, struct pt *v, int n)
{
    for (int i = 0; i < n; ++i)
    {
        struct pt p = v[i];
        r[i] = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
    }
}

#define N 6
struct pt v[N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};

int main()
{
    float lens[N], lens_sve[N], lens_sve_intrinsics[N];

    vec_len(lens, v, N);
    vec_len_sve(lens_sve, v, N);
    vec_len_sve_intrinsics(lens_sve_intrinsics, v, N);

    for (int i = 0; i < N; ++i)
    {
        printf("%f %f %f\n", lens[i], lens_sve[i], lens_sve_intrinsics[i]);
    }
    return 0;
}