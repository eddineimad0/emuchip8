#include<stdio.h>
#include<SDL2/SDL.h>
#include<chip8.h>
#include<stdbool.h>
#include<Windows.h>
#undef main

int main(int argc, const char** argv)
{
    if(argc < 2)
    {
        printf("<usage> : %s [ROM_FILE] \n",argv[0]);
        return -1;
    }
    // 0----------------------------------------------------0
    // | Loading the ROM content to Chip8 Memory addr(0x200)|
    // 0----------------------------------------------------0
    const char* filename=argv[1];
    printf("Application Start\n Program File : %s\n",filename);
    FILE* f = fopen(filename,"rb");
    if(!f)
    {
        printf("Failed to open the file [%s] \n",filename);
        fclose(f);
        return -1;
    }

    fseek(f,0,SEEK_END);
    size_t size = ftell(f);
    fseek(f,0,SEEK_SET);

    uint8_t* file_buffer=(uint8_t*) malloc(size * sizeof(uint8_t));
        if(!file_buffer)
        {
            printf("Failed to allocate file buffer \n");
            fclose(f);
            return -1;
        }

    int result=fread(file_buffer,size,sizeof(uint8_t),f);
    if(result !=1)
    {
        printf("Failed to read from file [%s] \n",filename);
        return 0;
    }
    fclose(f);
    // 0----------------------------------------------------0
    // |                  Region END                        |
    // 0----------------------------------------------------0

    // 0----------------------------------------------------0
    // | intialize Chip8 and SDL Window and Renderer        |
    // 0----------------------------------------------------0
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8,file_buffer,size);
    free(file_buffer);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow
    (
        EMULATOR_WINDOW_TITLE, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        CHIP8_DISPLAY_WIDTH * WINDOW_SIZE_MULTIPLIER,
        CHIP8_DISPLAY_HEIGHT * WINDOW_SIZE_MULTIPLIER, 
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_TEXTUREACCESS_TARGET);

    // 0----------------------------------------------------0
    // |                  Region END                        |
    // 0----------------------------------------------------0


    // 0----------------------------------------------------0
    // | Start Main Loop                                    |
    // 0----------------------------------------------------0
    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    goto clean_up;
                    break;
                case SDL_KEYDOWN:
                {
                    int vkey=chip8_keyboard_map(&chip8._keyboard,(uint8_t)(event.key.keysym.sym));
                    if(vkey!=-1)chip8_keyboard_down(&chip8._keyboard,vkey);
                }
                    break;

                case SDL_KEYUP:
                {
                    int vkey=chip8_keyboard_map(&chip8._keyboard,(uint8_t)(event.key.keysym.sym));
                    if(vkey!=-1)chip8_keyboard_up(&chip8._keyboard,vkey);
                }
                    break;
            };

        }
        
        //Draw Call
        chip8_render_to_window(&chip8._screen,renderer);
        

        if(chip8._registers.delay_timer > 0)
        {
            Sleep(10);
            chip8._registers.delay_timer -= 1;
            //printf("Delay!!!!\n");
        }

        if(chip8._registers.sound_timer > 0)
        {
            Beep(15000,10*chip8._registers.sound_timer);
            chip8._registers.sound_timer=0;
        }

        // Cycle : FETCH->INCR->DECODE->EXECUTE
        uint16_t opcode = chip8_memory_read_short(&chip8._memory,chip8._registers.PC);
        chip8._registers.PC+=2;
        chip8_exec(&chip8,opcode);

    }

    // 0----------------------------------------------------0
    // |                  Region END                        |
    // 0----------------------------------------------------0

clean_up:

   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
    return 0;
}