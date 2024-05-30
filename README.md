# Vector Length Computation using SVE and RVV

This repository contains implementations of a simple vector length computation using ARM's Scalable Vector Extension (SVE) and RISC-V's Vector Extension (RVV). The vector length computation, often used in scientific and engineering applications, calculates the magnitude of a vector provided as an array of numbers. This project showcases how to utilize vector extensions to optimize such computations.

## Project Structure

- **src/**: Source files for both ARM SVE and RISC-V RVV implementations.
- **build/**: Directory for compiled binaries (create if does not exist).

## Prerequisites

To build and run the applications, ensure the following:

- A compatible compiler for ARM SVE and RISC-V RVV. This might be GCC or another compiler that supports these vector extensions.

## Building the Projects

To compile the implementations, you can use the provided Makefile. First, create a build directory and then use the make command to build the desired version:

```bash
mkdir build
make vec_len_rvv  # For the RISC-V RVV version
make vec_len_sve  # For the ARM SVE version
```
