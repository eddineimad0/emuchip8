#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H
#include<stdbool.h>
#include<config.h>

struct chip8_keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
    const uint8_t* keyboard_map;
};

// Set keyboard map pointer(_map) in structure pointer variable
void chip8_set_keyboard_map(struct chip8_keyboard* _keyboard,const uint8_t* _map);
// Return chip8 Key Value([0,15]) associated with key from the map
int8_t chip8_keyboard_map(struct chip8_keyboard* _keyboard,uint8_t key);
// Set chip8 Key to pressed(true)
void chip8_keyboard_down(struct chip8_keyboard* _keyboard,uint32_t key);
// Set chip8 Key to released(false)
void chip8_keyboard_up(struct chip8_keyboard* _keyboard,uint32_t key);
// Return the state of a given Key
bool chip8_keyboard_is_down(struct chip8_keyboard* _keyboard,uint32_t key);


#endif