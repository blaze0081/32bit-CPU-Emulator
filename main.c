#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

const char *statusName(enum cpuStatus status){
    switch (status) {
        case cpuOK:
            return "cpuOK";
        case cpuHalted:
            return "cpuHalted";
        case cpuIllegalInstruction:
            return "cpuIllegalInstruction";
        case cpuIllegalOperand:
            return "cpuIllegalOperand";
        case cpuInvalidAddress:
            return "cpuInvalidAddress";
        case cpuInvalidStackOperation:
            return "cpuInvalidStackOperation";
        case cpuDivByZero:
            return "cpuDivByZero";
        case cpuIOError:
            return "cpuIOError";
        default:
            fprintf(stderr, "BUG: Unknown status (%d)\n", status);
            abort();
    }
    printf("\n");
}

static void state(struct cpu *cpu){
    printf("A: %d, B: %d, C: %d, D: %d\n", cpu->A, cpu->B, cpu->C, cpu->D);
    printf("Status: %s\n", statusName(cpu->status));
    printf("Result: %d\n", cpu->result);
}


int main(int argc, char *argv[]){
    //compile as: gcc -o cpu main.c cpu.c
    // run as: ./cpu progrma.bin
    if(argc != 2){
        printf("Invalid arguments");
        return 1;
    }

    size_t stackCapacity = 256; //initail stack size
    FILE *fptr;
    fptr = fopen(argv[1], "rb");

    int32_t *stackPtr;
    int32_t *memory = cpuCreateMemory(fptr, stackCapacity, &stackPtr);
    struct cpu cp;
    cpuCreate(&cp, memory, stackPtr, stackCapacity);

    //cpu run
    cpuRun(&cp, INT_MAX);
    state(&cp);

    //memory management
    fclose(fptr);
    cpuDestroy(&cp);
    return 0;

}