.global vec_len_sve

#define index_reg x10
#define length x2
#define struct_length x2
#define offset x22
#define index_vec z31.s

vec_len_sve:
    cmp x2, 0
    ble .return

    cntw offset

    mov struct_length, 12
    mul offset, offset, struct_length

    b .loop_check
.loop:
    ld3w {z1.s- z3.s}, p0/z, [x1]

    fmul z1.s, p0/m, z1.s, z1.s
    fmla z1.s, p0/m, z2.s, z2.s
    fmla z1.s, p0/m, z3.s, z3.s
    fsqrt z1.s, p0/m, z1.s

    st1w z1.s, p0, [x0, index_reg, lsl#2]

    incw index_reg
    add x1, x1, offset
.loop_check:
    whilelt p0.s, index_reg, length
    b.first .loop
.return:
    ret
