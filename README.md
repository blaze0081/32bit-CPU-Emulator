# 32bit-CPU-Emulator

## Compile the code
gcc -o cpu main.c cpu.c compiler.c

## Run the program:
./cpu run program.asm

## Available Instructions
### Basic instructions:

~~nop - No operation~~
halt - Stop execution
add reg - Add register to A
sub reg - Subtract register from A
mul reg - Multiply A by register
div reg - Divide A by register
inc reg - Increment register
dec reg - Decrement register
~~loop label - Loop if C != 0~~
mov reg num - Move number to register
~~load reg num - Load from stack~~
~~store reg num - Store to stack~~
in reg - Input number to register
~~get reg - Input character to register~~
out reg - Output number from register
~~put reg - Output character from register~~
~~swap reg1 reg2 - Swap registers~~
~~push reg - Push register to stack~~
~~pop reg - Pop stack to register~~
cmp reg1 reg2 - Compare registers
~~jmp label - Jump to label~~
~~jz label - Jump if zero~~
~~jnz label - Jump if not zero~~
~~jgt label~~

### Registers:
a, b, c, d - General purpose registers
result (bonus) - Result register