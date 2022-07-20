#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H
#include <config.h>
#include <stdint.h>


struct chip8_registers
{
   uint8_t V[CHIP8_NUMBER_OF_DATA_REGISTERS];
   uint16_t I;
   uint8_t delay_timer;
   uint8_t sound_timer;
   uint16_t PC;
   uint8_t SP; 
};

#endif