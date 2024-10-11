#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define memory_size 1024


static void resetRegisters(struct cpu *cpu){
    cpu->A = 0;
    cpu->B = 0;
    cpu->C = 0;
    cpu->D = 0;
    cpu->status = 0;
    cpu->stackSize = 0;
    cpu->instructionPointer = 0;
    cpu->result = 0;
}

void cpuCreate(struct cpu *cpu, int32_t *memory, int32_t *stackBottom, size_t stackCapacity){
    resetRegisters(cpu);
    cpu->memory = memory;
    cpu->stackBottom = stackBottom;
    cpu->stackLimit = stackBottom - stackCapacity;
}

void cpuDestroy(struct cpu *cpu){
    free(cpu->memory);
    resetRegisters(cpu);
    cpu->memory = NULL;
    cpu->stackBottom = NULL;
    cpu->stackLimit = NULL;
}

void cpuReset(struct cpu *cpu){
    memset(cpu->stackLimit + 1, 0, (cpu->stackBottom - cpu->stackLimit) * sizeof(int32_t));
    resetRegisters(cpu);
}

int cpuStatus(struct cpu *cpu){
    return cpu->status;
}

int32_t cpuPeek(struct cpu *cpu, char reg){
    switch (reg) {
    case 'A':
        return cpu->A;
    case 'B':
        return cpu->B;
    case 'C':
        return cpu->C;
    case 'D':
        return cpu->D;
    case 'S':
        return cpu->stackSize;
    case 'I':
        return cpu->instructionPointer;
    case 'R':
        return cpu->result;
    default:
        return 0; // Invalid register
    }
}

int cpuRun(struct cpu *cpu, size_t steps){
    if (cpuStatus(cpu) != cpuOK) {
        return 0;
    }

    uint32_t counter = 0;
    while (steps > counter) {
        cpuStep(cpu);
        counter++;

        if (cpu->status == cpuHalted) {
            break;
        }
        if (cpu->status != cpuOK) {
            return counter * (-1); // Error code
        }
    }
    return counter; // OK
}

static int32_t next32Bits(struct cpu *cpu, int32_t addition){
    if (cpu->memory + cpu->instructionPointer + addition > cpu->stackLimit) {
        cpu->status = cpuInvalidAddress;
        return 0;
    }
    return *(cpu->memory + cpu->instructionPointer + addition);
}

static int32_t getRegister(struct cpu *cpu, int32_t index){
    switch (index) {
    case 0:
        return cpu->A;
    case 1:
        return cpu->B;
    case 2:
        return cpu->C;
    case 3:
        return cpu->D;
    case 4:
        return cpu->result;
    }
    return 0;
}

static int operationInstruction(struct cpu *cpu, char operand){
    int32_t registerIndex = next32Bits(cpu, 1);
    if (cpuStatus(cpu) == cpuInvalidAddress) {
        return 0;
    }

    if (registerIndex < 0 || registerIndex > 3) {
        cpu->status = cpuIllegalOperand;
        return 0;
    }

    int32_t registerInValue = getRegister(cpu, registerIndex);
    if (operand == '/' && registerInValue == 0) {
        cpu->status = cpuDivByZero;
        return 0;
    }

    int32_t value = cpuPeek(cpu, 'A');

    switch (operand) {
    case '+':
        value += registerInValue;
        break;
    case '-':
        value -= registerInValue;
        break;
    case '*':
        value *= registerInValue;
        break;
    case '/':
        value /= registerInValue;
        break;
    }
    cpu->A = value;
    cpu->result = value;

    cpu->instructionPointer += 2;
    return 1;
}

static void setRegister(struct cpu *cpu, int32_t index, int32_t value)
{
    switch (index) {
    case 0:
        cpu->A = value;
        break;
    case 1:
        cpu->B = value;
        break;
    case 2:
        cpu->C = value;
        break;
    case 3:
        cpu->D = value;
        break;
    }
}

static int movInstruction(struct cpu *cpu)
{
    int32_t registerIndex = next32Bits(cpu, 1);
    int32_t numValue = next32Bits(cpu, 2);
    if (cpuStatus(cpu) == cpuInvalidAddress) {
        return 0;
    }

    if (registerIndex < 0 || registerIndex > 3) {
        cpu->status = cpuIllegalOperand;
        return 0;
    }

    setRegister(cpu, registerIndex, numValue);
    cpu->instructionPointer += 3;
    return 1;
}

enum instructionKeyWords
{
    nope,
    halt,
    add,
    sub,
    mul,
    divI,
    inc,
    dec,
    loop,
    mov,
    load,
    store,
    in,
    get,
    out,
    put,
    swap,
    push,
    pop,
    cmp,
    jmp,
    jz,
    jnz,
    jgt,
    call,
    ret
};

int cpuStep(struct cpu *cpu)
{

    if (cpuStatus(cpu) != cpuOK) {
        return 0;
    }

    if (cpu->memory + cpu->instructionPointer < cpu->memory ||
            cpu->memory + cpu->instructionPointer > cpu->stackLimit) {
        cpu->status = cpuInvalidAddress;
        return 0;
    }

    int32_t instruction = next32Bits(cpu, 0);
    if (cpuStatus(cpu) == cpuInvalidAddress) {
        return 0;
    }

    switch (instruction) {
    case nope:
        cpu->instructionPointer++;
        return 1;
    case halt:
        cpu->status = cpuHalted;
        cpu->instructionPointer++;
        return 0;
    case add:
        return operationInstruction(cpu, '+');
    case sub:
        return operationInstruction(cpu, '-');
    case mul:
        return operationInstruction(cpu, '*');
    case divI:
        return operationInstruction(cpu, '/');
    case mov:
        return movInstruction(cpu);
    default:
        cpu->status = cpuIllegalInstruction;
        return 0;
    }
}