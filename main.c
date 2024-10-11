#include "cpu.h"
#include "compiler.h"
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


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <program.asm>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    size_t stackCapacity = 256; // initial stack size

    int32_t *memory = malloc(1024 * sizeof(int32_t));

    // Compile the program
    CompiledProgram compiledProgram = compilerASMtoBinary(filename);

    // Copy program into memory
    memcpy(memory, compiledProgram.program, compiledProgram.size);

    // Set stack pointer to end of memory
    int32_t *stackPtr = memory + 1023;  // Point to last element

    struct cpu cp;
    cpuCreate(&cp, memory, stackPtr, stackCapacity);

    // CPU run
    cpuRun(&cp, INT_MAX);
    state(&cp);

    // Memory management
    cpuDestroy(&cp);
    free(memory);
    return 0;
}