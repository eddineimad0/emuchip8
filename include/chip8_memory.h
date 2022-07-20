#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <config.h>

struct chip8_memory
{
  uint8_t memory[CHIP8_MEMORY_SIZE];  
};

// Write Byte(data) at address(index) in main memory 
void chip8_memory_write(struct chip8_memory * memory,uint32_t index,uint8_t data);
// Return byte from main memory address(index)
uint8_t chip8_memory_read_byte(struct chip8_memory * memory,uint32_t index);
// Return short from main memory address(index)
uint16_t chip8_memory_read_short(struct chip8_memory * _memory,uint32_t index);

#endif