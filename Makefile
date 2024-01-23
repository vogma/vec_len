SVE=sve
RVV=rvv
BUILD=build
GCC_ARM_FLAGS=-march=armv8.4-a+sve -g -static -O2
GCC_RISCV_FLAGS=-march=rv64gcv_zvbc_zba -mabi=lp64d -g -static -O2
QEMU_ARM_FLAGS=-L /usr/aarch64-linux-gnu/ -cpu max,pmu=off,sve=on,sve128=on
QEMU_RISCV_FLAGS=-cpu rv64,v=true,zba=true,vlen=128,f=true,vext_spec=v1.0
GCC_ARM=aarch64-none-linux-gnu-gcc
GCC_RISCV=riscv64-unknown-linux-gnu-gcc

all: vec_len_sve vec_len_rvv

go_arm: vec_len_sve Makefile
	qemu-aarch64 $(QEMU_ARM_FLAGS) ./$(BUILD)/vec_len_sve
	
go_riscv: vec_len_rvv Makefile
	qemu-riscv64 $(QEMU_RISCV_FLAGS) ./$(BUILD)/vec_len_rvv

vec_len_sve: Makefile
	$(GCC_ARM) $(SVE)/vec_len.c $(SVE)/vec_len.S -o $(BUILD)/$@ $(GCC_ARM_FLAGS)

vec_len.o: Makefile
	clang -c $(RVV)/vec_len.c -o $(BUILD)/vec_len.o $(GCC_RISCV_FLAGS)

vec_len_asm.o: Makefile
	clang -c $(RVV)/vec_len.S -o $(BUILD)/vec_len_asm.o $(GCC_RISCV_FLAGS)

vec_len_rvv: vec_len.o vec_len_asm.o Makefile
	$(GCC_RISCV) $(BUILD)/vec_len.o $(BUILD)/vec_len_asm.o -o $(BUILD)/$@ $(GCC_RISCV_FLAGS)
