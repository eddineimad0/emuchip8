#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H
#include <config.h>

struct chip8;

struct chip8_stack
{
    uint16_t stack[CHIP8_STACK_DEPTH];
};


// Increment stack Pointer and store Program Counter Address in
void chip8_stack_push(struct chip8* _chip8, uint16_t _address);
// Return Address Pointed by Stack pointer
uint16_t chip8_stack_pop(struct chip8* _chip8);


#endif