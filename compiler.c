#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>


// Instruction codes matching the CPU's enum
enum Instructions {
    NOPE = 0,
    HALT = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    INC = 6,
    DEC = 7,
    LOOP = 8,
    MOV = 9,
    LOAD = 10,
    STORE = 11,
    IN = 12,
    GET = 13,
    OUT = 14,
    PUT = 15,
    SWAP = 16,
    PUSH = 17,
    POP = 18
};

// Convert register name to number
int reg_to_num(const char *reg) {
    if (strcmp(reg, "A") == 0 || strcmp(reg, "a") == 0) return 0;
    if (strcmp(reg, "B") == 0 || strcmp(reg, "b") == 0) return 1;
    if (strcmp(reg, "C") == 0 || strcmp(reg, "c") == 0) return 2;
    if (strcmp(reg, "D") == 0 || strcmp(reg, "d") == 0) return 3;
    return -1;
}

// Convert instruction name to opcode
int get_opcode(const char *instr) {
    if (strcmp(instr, "nope") == 0) return NOPE;
    if (strcmp(instr, "halt") == 0) return HALT;
    if (strcmp(instr, "add") == 0) return ADD;
    if (strcmp(instr, "sub") == 0) return SUB;
    if (strcmp(instr, "mul") == 0) return MUL;
    if (strcmp(instr, "div") == 0) return DIV;
    if (strcmp(instr, "inc") == 0) return INC;
    if (strcmp(instr, "dec") == 0) return DEC;
    if (strcmp(instr, "loop") == 0) return LOOP;
    if (strcmp(instr, "mov") == 0) return MOV;
    if (strcmp(instr, "load") == 0) return LOAD;
    if (strcmp(instr, "store") == 0) return STORE;
    if (strcmp(instr, "in") == 0) return IN;
    if (strcmp(instr, "get") == 0) return GET;
    if (strcmp(instr, "out") == 0) return OUT;
    if (strcmp(instr, "put") == 0) return PUT;
    if (strcmp(instr, "swap") == 0) return SWAP;
    if (strcmp(instr, "push") == 0) return PUSH;
    if (strcmp(instr, "pop") == 0) return POP;
    return -1;
}

// Parse a single line of assembly
int parse_line(const char *line, int32_t *program, int *prog_counter) {
    char instr[10] = {0};
    char arg1[10] = {0};
    char arg2[10] = {0};
    int num_args = 0;
    
    // Skip whitespace and empty lines
    while (isspace(*line)) line++;
    if (*line == '\0' || *line == ';' || *line == '#') return 0;
    
    // Parse instruction and arguments
    num_args = sscanf(line, "%s %s %s", instr, arg1, arg2);
    if (num_args < 1) return 0;
    
    // Convert instruction to lowercase
    for (int i = 0; instr[i]; i++) {
        instr[i] = tolower(instr[i]);
    }
    
    // Get opcode
    int opcode = get_opcode(instr);
    if (opcode == -1) {
        printf("Error: Unknown instruction '%s'\n", instr);
        return -1;
    }
    
    // Store opcode
    program[(*prog_counter)++] = opcode;
    
    // Handle arguments based on instruction type
    switch (opcode) {
        case HALT:
        case NOPE:
            // No arguments
            break;
            
        case INC:
        case DEC:
        case OUT:
        case PUT:
        case PUSH:
        case POP:
            // One register argument
            if (num_args < 2) {
                printf("Error: Missing register argument for '%s'\n", instr);
                return -1;
            }
            int reg = reg_to_num(arg1);
            if (reg == -1) {
                printf("Error: Invalid register '%s'\n", arg1);
                return -1;
            }
            program[(*prog_counter)++] = reg;
            break;
            
        case MOV:
            // Register and value
            if (num_args < 3) {
                printf("Error: Missing arguments for 'mov'\n");
                return -1;
            }
            reg = reg_to_num(arg1);
            if (reg == -1) {
                printf("Error: Invalid register '%s'\n", arg1);
                return -1;
            }
            program[(*prog_counter)++] = reg;
            program[(*prog_counter)++] = atoi(arg2);
            break;
            
        // Add more cases for other instruction types as needed
    }
    
    return 0;
}

CompiledProgram compilerASMtoBinary(const char* filename) {
    static int32_t program[1024] = {0};  // Program memory
    int prog_counter = 0;         // Current position in program
    char line[256];               // Input line buffer
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    // Read and parse assembly lines
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;
        
        if (parse_line(line, program, &prog_counter) != 0) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            fclose(file);
            exit(1);
        }
    }
    
    fclose(file);
    
    CompiledProgram result = {program, prog_counter * sizeof(int32_t)};
    return result;
}