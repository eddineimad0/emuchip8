#include<chip8_memory.h>
#include<stdlib.h>
#include<stdio.h>
#include <assert.h>
#define chip8_memory_is_index_bound(index) assert(index>=0x00 && index< CHIP8_MEMORY_SIZE)


void chip8_memory_write(struct chip8_memory * memory,uint32_t index,uint8_t data)
{
    chip8_memory_is_index_bound(index);
    if(index <0x200)
    {
        printf("[ERROR]: Illegal Memory access 0x%x {@chip8_memory.c->chip8_memory_write()}\n",index);
        exit(-1);
    }
    memory->memory[index]=data;
}


uint8_t chip8_memory_read_byte(struct chip8_memory * memory,uint32_t index)
{
    chip8_memory_is_index_bound(index);
    return memory->memory[index];
}

uint16_t chip8_memory_read_short(struct chip8_memory * _memory,uint32_t index)
{
    uint8_t hi_byte = chip8_memory_read_byte(_memory,index);
    uint8_t lo_byte = chip8_memory_read_byte(_memory,index+1);
    return (hi_byte << 8 | lo_byte);
}