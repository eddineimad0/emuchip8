#ifndef CHIP8_H
#define CHIP8_H

#include<config.h>
#include<chip8_memory.h>
#include<chip8_registers.h>
#include<chip8_stack.h>
#include<chip8_keyboard.h>
#include<chip8_screen.h>


// interpreter internals (main class)
struct chip8
{
    struct chip8_memory _memory;
    struct chip8_registers _registers;
    struct chip8_stack _stack;
    struct chip8_keyboard _keyboard;
    struct chip8_screen _screen;
};

// Initialize Memory and registers
void chip8_init(struct chip8 * _chip8);
// Decode OPCode and execute associated Instructions
void chip8_exec(struct chip8* _chip8,uint16_t OPcode);
// Load Program Code Into Chip8 Memory starting at address 0x200
void chip8_load(struct chip8* _chip8,const uint8_t* prog_buffer,size_t size);

#endif