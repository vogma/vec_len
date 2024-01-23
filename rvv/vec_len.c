#include <stdio.h>
#include <math.h>
#include <riscv_vector.h>

// source https://github.com/brucehoult/rvv_example/blob/main/main.c
struct pt
{
    float x;
    float y;
    float z;
};

// void vec_len_rvv_as(float *r, struct pt *v, int n);
// void vec_len_as(float *r, struct pt *v, int n);

void vec_len(float *r, struct pt *v, int n)
{
    for (int i = 0; i < n; ++i)
    {
        struct pt p = v[i];
        r[i] = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
    }
}

void vec_len_rvv_as(float *r, struct pt *v, int n);

void vec_len_rvv_intrinsics(float *r, struct pt *v, int n)
{
    vfloat32m1_t vec_result;

    for (int vl; n > 0; n -= vl, v += vl, r += vl)
    {
        vl = __riscv_vsetvl_e32m1(n);

        // loads interleaved Xs, Ys, Zs into 3 registers
        vfloat32m1x3_t points = __riscv_vlseg3e32_v_f32m1x3(&(v->x), vl);

        // get access to points
        vfloat32m1_t vec_x = __riscv_vget_v_f32m1x3_f32m1(points, 0);
        vfloat32m1_t vec_y = __riscv_vget_v_f32m1x3_f32m1(points, 1);
        vfloat32m1_t vec_z = __riscv_vget_v_f32m1x3_f32m1(points, 2);

        // compute sqrtf(p.x * p.x + p.y * p.y + p.z * p.z)
        vec_result = __riscv_vfmul(vec_x, vec_x, vl);
        vec_result = __riscv_vfmacc(vec_result, vec_y, vec_y, vl);
        vec_result = __riscv_vfmacc(vec_result, vec_z, vec_z, vl);
        vec_result = __riscv_vfsqrt(vec_result, vl);

        // store results to r
        __riscv_vse32(r, vec_result, vl);
    }
}

#define N 6
struct pt v[N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};

int main()
{
    float lens[N], lens_rvv[N], lens_rvv_intrinsics[N];

    vec_len(lens, v, N);
    vec_len_rvv_as(lens_rvv, v, N);
    vec_len_rvv_intrinsics(lens_rvv_intrinsics, v, N);

    for (int i = 0; i < N; ++i)
    {
        printf("%f %f %f\n", lens[i], lens_rvv[i], lens_rvv_intrinsics[i]);
    }
    return 0;
}