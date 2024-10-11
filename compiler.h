#include <stdint.h>
#include <stdlib.h>


typedef struct {
    int32_t* program;
    size_t size;
} CompiledProgram;

CompiledProgram compilerASMtoBinary(const char* filename);