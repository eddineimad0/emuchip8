#include<chip8_screen.h>
#include<assert.h>
#include<memory.h>
#include<SDL2/SDL.h>
#define chip8_pixel_bound(x,y) assert(x>=0x00 && y>=0x00)


void chip8_screen_set(struct chip8_screen* _screen,uint32_t x,uint32_t y,bool value)
{
    chip8_pixel_bound(x,y);
    x%=CHIP8_DISPLAY_WIDTH;
    y%=CHIP8_DISPLAY_HEIGHT;
    _screen->pixels[(y*CHIP8_DISPLAY_WIDTH)+x]=value;
}

bool chip8_screen_is_set(struct chip8_screen* _screen,uint32_t x,uint32_t y)
{
    chip8_pixel_bound(x,y);
    x%=CHIP8_DISPLAY_WIDTH;
    y%=CHIP8_DISPLAY_HEIGHT;
    return _screen->pixels[(y*CHIP8_DISPLAY_WIDTH)+x];
}

bool chip8_screen_draw_sprite(struct chip8_screen* _screen,uint32_t x ,uint32_t y ,const uint8_t* sprt,int num)
{
    bool pixel_overwritten = false;
    uint8_t c,pixel;
    uint32_t y_coord;
    for(uint32_t j = 0;j<num;j++)
    {
        c=sprt[j];
        y_coord=(y+j);

        for(uint32_t k =0;k<8;k++)
        {
            pixel = (c & (0b10000000 >> k));
            if(pixel==0)continue;
            else
            {
                uint32_t x_coord=(x+k);
                pixel_overwritten=chip8_screen_is_set(_screen,x_coord,y_coord);
                bool set_value=pixel_overwritten!=true;
                chip8_screen_set(_screen,x_coord,y_coord,set_value);
            }
            
        }
    }
    return pixel_overwritten;
}

void chip8_screen_clear(struct chip8_screen* _screen)
{
    memset(_screen->pixels,0,sizeof(_screen->pixels));
}

void chip8_render_to_window(struct chip8_screen* _screen,SDL_Renderer* renderer)
{
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,255,255,255,0);

        // screen Render call
        for(uint32_t i=0;i<CHIP8_DISPLAY_WIDTH*CHIP8_DISPLAY_HEIGHT;i++)
        {
            
            if(_screen->pixels[i])
            {
                SDL_Rect rect;
                rect.h=WINDOW_SIZE_MULTIPLIER;
                rect.w=WINDOW_SIZE_MULTIPLIER;
                rect.x=(i%CHIP8_DISPLAY_WIDTH)*WINDOW_SIZE_MULTIPLIER;
                rect.y=(i/CHIP8_DISPLAY_WIDTH)*WINDOW_SIZE_MULTIPLIER;
                SDL_RenderFillRect(renderer,&rect);
            }

        }

        SDL_RenderPresent(renderer);
}