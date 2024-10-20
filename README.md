# 32bit-CPU-Emulator

## Compile the code
gcc -o cpu main.c cpu.c compiler.c

## Run the program:
./cpu run program.asm

## Available Instructions
### Basic instructions:

halt - Stop execution

add reg - Add register to A

sub reg - Subtract register from A

mul reg - Multiply A by register

div reg - Divide A by register

inc reg - Increment register

dec reg - Decrement register

mov reg num - Move number to register

in reg - Input number to register

out reg - Output number from register

cmp reg1 reg2 - Compare registers

### Registers:
a, b, c, d - General purpose registers
result (bonus) - Result register
