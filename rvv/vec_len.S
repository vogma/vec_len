.global vec_len_rvv_as
vec_len_rvv_as:

    li          t2, 12

.loop:
    vsetvli     t0, a2, e32, m1, ta, ma

    vlseg3e32.v v0, 0(a1)# load

    vfmul.vv    v3, v0, v0 # compute
    vfmacc.vv   v3, v1, v1
    vfmacc.vv   v3, v2, v2
    vfsqrt.v    v3, v3

    vse32.v     v3, 0(a0)#store

    slli        t1, t0, 2
    add         a0, a0, t1 # bump result pointer
    sub         a2, a2, t0

    slli        t2, t0, 2
    slli        t3, t2, 1
    add         t3, t2, t3
    add         a1, a1, t3

    bnez        a2, .loop

    ret
