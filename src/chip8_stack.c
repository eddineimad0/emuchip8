#include<chip8_stack.h>
#include<assert.h>
#include<stdlib.h>
#include<chip8.h>
#include<stdio.h>

#define SP_SAFTEY_CHECK(SP) assert(SP >= 0 && SP < CHIP8_STACK_DEPTH)


void chip8_stack_push(struct chip8* _chip8,uint16_t _address)
{
    SP_SAFTEY_CHECK((uint32_t)(_chip8->_registers.SP));
    if((uint32_t)(_chip8->_registers.SP) == 15)
    {
        printf("[ERROR]: Stack Overflow Level : %d {@chip8_stack.c->chip8_stack_push()} \n",(uint32_t)(_chip8->_registers.SP));
        exit(-1);
    }

    _chip8->_registers.SP +=1;
    _chip8->_stack.stack[(uint32_t)(_chip8->_registers.SP)]=_address;
}


uint16_t chip8_stack_pop(struct chip8* _chip8)
{
    SP_SAFTEY_CHECK((uint32_t)(_chip8->_registers.SP));
    if((uint32_t)(_chip8->_registers.SP) == 0)
    {
        printf("[ERROR]: Stack Can't Be Unwinded Level : %d {@chip8_stack.c->chip8_stack_pop()} \n",(uint32_t)(_chip8->_registers.SP));
        exit(-1);
    }    

    uint16_t result = _chip8->_stack.stack[(uint32_t)(_chip8->_registers.SP)];
    _chip8->_registers.SP-=1;
    return result;
}