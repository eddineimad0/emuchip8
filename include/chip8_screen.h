#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include<stdbool.h>
#include<config.h>

typedef struct SDL_Renderer SDL_Renderer;

struct chip8_screen
{
    bool pixels[CHIP8_DISPLAY_HEIGHT*CHIP8_DISPLAY_WIDTH];
};

// Set A Pixel on the screen at coord(x,y)
void chip8_screen_set(struct chip8_screen* _screen,uint32_t x,uint32_t y,bool value);
// Clear the screen to black
void chip8_screen_clear(struct chip8_screen* _screen);
// Return the value of pixel on the screen at coord(x,y)
bool chip8_screen_is_set(struct chip8_screen* _screen,uint32_t x,uint32_t y);
// Draw Sprite data starting at Coord(x,y) from address(sprt) with a stride(num)
bool chip8_screen_draw_sprite(struct chip8_screen* _screen,uint32_t x ,uint32_t y ,const uint8_t* sprt,int num);
// Main Draw call for rendering Screen Data on the program window
void chip8_render_to_window(struct chip8_screen* _screen,SDL_Renderer* renderer);

#endif