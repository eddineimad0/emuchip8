#include<chip8.h>
#include<memory.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#define memcpy_safe_check(src,dst_size) assert(sizeof(src)>=dst_size)

const uint8_t chip8_Charcter_set[]={
    0xf0,0x90,0x90,0x90,0xf0, //0
    0x20,0x60,0x20,0x20,0x70, //1
    0xf0,0x10,0xf0,0x80,0xf0, //2
    0xf0,0x10,0xf0,0x10,0xf0, //3
    0x90,0x90,0xf0,0x10,0x10, //4
    0xf0,0x80,0xf0,0x10,0xf0, //5
    0xf0,0x80,0xf0,0x90,0xf0, //6
    0xf0,0x10,0x20,0x40,0x40, //7
    0xf0,0x90,0xf0,0x90,0xf0, //8
    0xf0,0x90,0xf0,0x10,0xf0, //9
    0xf0,0x90,0xf0,0x90,0x90, //A
    0xe0,0x90,0xe0,0x90,0xe0, //B
    0xf0,0x80,0x80,0x80,0xf0, //C
    0xe0,0x90,0x90,0x90,0xe0, //D
    0xf0,0x80,0xf0,0x80,0xf0, //E
    0xf0,0x80,0xf0,0x80,0x80, //F
};

const uint8_t map_keyboard[CHIP8_TOTAL_KEYS]=
{
    SDLK_0,SDLK_1,SDLK_2,SDLK_3,SDLK_4,SDLK_5,
    SDLK_7,SDLK_8,SDLK_9,SDLK_a,SDLK_b,SDLK_c,
    SDLK_d,SDLK_e,SDLK_f
};

void chip8_init(struct chip8 * _chip8)
{
    memset(_chip8,0,sizeof(struct chip8));
    memcpy_safe_check(_chip8->_memory.memory,sizeof(chip8_Charcter_set));
    memcpy(&(_chip8->_memory.memory),chip8_Charcter_set,sizeof(chip8_Charcter_set));
    chip8_set_keyboard_map(&_chip8->_keyboard,map_keyboard);
    _chip8->_registers.PC = CHIP8_PROGRAM_LOAD_ADDRESSE;
}

// Prompt user for a Valid key press
static int8_t chip8_wait_for_key_press(struct chip8* _chip8)
{
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        if(event.type!=SDL_KEYDOWN)continue;

        uint8_t c = event.key.keysym.sym;
        int8_t chip8_key=chip8_keyboard_map(&_chip8->_keyboard,c);
        if(chip8_key != -1)return chip8_key;
    }
    return -1;
}



// Handles Complex OPCodes
static void chip8_exec_ext(struct chip8* _chip8,uint16_t OPcode)
{
    uint16_t temp=0;
    uint8_t nibble=(OPcode & 0x000f);
    uint8_t y = ((OPcode >> 4 ) & 0x000F);
    uint8_t x = ((OPcode >> 8 ) & 0x000F);
    uint8_t KK = (OPcode & 0x00FF);
    uint16_t nnn = (OPcode & 0x0FFF);
    switch(OPcode & 0xF000)
    {
        // JP : jump to address
        case 0x1000:
            _chip8->_registers.PC=nnn;
            break;
        // CALL : call subroutine
        case 0x2000:
            chip8_stack_push(_chip8,_chip8->_registers.PC);
            _chip8->_registers.PC=nnn;
            break;
        // SE VX,byte : skip next inst if Vx=kk
        case 0x3000:
            if(_chip8->_registers.V[x]==KK)_chip8->_registers.PC +=2;
            break;
        // SNE Vx,byte : skip next inst if VX!=kk
        case 0x4000:
            if(_chip8->_registers.V[x]!=KK)_chip8->_registers.PC +=2;
            break;
        // SE Vx,Vy : skip next inst if vx==vy
        case 0x5000:
            if(_chip8->_registers.V[x]==_chip8->_registers.V[y])_chip8->_registers.PC +=2;
            break;
        //LD Vx,byte : load byte into vx
        case 0x6000:
            _chip8->_registers.V[x]=KK;
            break;
        // ADD Vx,byte : add the value kk to vx
        case 0x7000:
            _chip8->_registers.V[x]+=KK;
            break;
        
        case 0x8000:
            switch((OPcode & 0x00F))
            {
                // LD VX,vy : load value of vy into vx
                case 0x00:
                    _chip8->_registers.V[x] =_chip8->_registers.V[y];
                    break;
                // OR vx,vy
                case 0x01:
                    _chip8->_registers.V[x] |=_chip8->_registers.V[y];
                    break;
                // AND vx,vy
                case 0x02:
                    _chip8->_registers.V[x] &=_chip8->_registers.V[y];
                    break;
                // XOR Vx,Vy
                case 0x03:
                    _chip8->_registers.V[x] ^=_chip8->_registers.V[y];
                    break;
                // ADD Vx,Vy VF carry
                case 0x04:
                    temp= _chip8->_registers.V[x]+_chip8->_registers.V[y];
                    _chip8->_registers.V[0xF]=0;
                    if(temp>0xff)_chip8->_registers.V[0xF]=1;
                    //if((UINT8_MAX - _chip8->_registers.V[y])< _chip8->_registers.V[x])_chip8->_registers.V[0xF]=0x01;
                    _chip8->_registers.V[x]=temp;
                    break;
                // SUB Vx,Vy VF = !Borrow VX=VX-VY
                case 0x05:
                    _chip8->_registers.V[0xF]=0;
                    if(_chip8->_registers.V[y] < _chip8->_registers.V[x])_chip8->_registers.V[0xF]=1;
                    _chip8->_registers.V[x]-=_chip8->_registers.V[y];
                    break;
                // SHR Vx {,Vy} Vf=LSB(Vx), VX/2
                case 0x06:
                    _chip8->_registers.V[0xF]=(_chip8->_registers.V[x]&0x01);
                    _chip8->_registers.V[x]/=2;
                    break;
                // SUBN Vx,Vy VF=!Borrom Vx=Vy-Vx
                case 0x07:
                     _chip8->_registers.V[0xF]=0;
                    if(_chip8->_registers.V[y] > _chip8->_registers.V[x])_chip8->_registers.V[0xF]=1;
                    _chip8->_registers.V[x]=_chip8->_registers.V[y] - _chip8->_registers.V[x];
                    break;
                // SHL Vx {, Vy}
                case 0x0E:
                    _chip8->_registers.V[0xF]=(_chip8->_registers.V[x] & 0x80);
                    _chip8->_registers.V[x]*=2;
                    break;
                
            }
        break;
        // SNE vx,vy
        case 0x9000:
            if(_chip8->_registers.V[y] != _chip8->_registers.V[x])_chip8->_registers.PC +=2;
        break;
        // LD I,addr
        case 0xA000:
            _chip8->_registers.I=nnn;
            break;
        // JP V0,add : jump to location nnn+v0
        case 0xB000:
            _chip8->_registers.PC=nnn+_chip8->_registers.V[0x0];
            break;
        // RND Vx,byte : set Vx= random(byte) & KK
        case 0xC000:
            srand(clock());
            _chip8->_registers.V[x]=(rand() % 255) & KK;
            break;
        // DRW Vx,Vy,nibble : draw n-byte from I at (Vx,Vy) coords VF = collisions
        case 0xD000:
            _chip8->_registers.V[0x0f]=chip8_screen_draw_sprite(
                &_chip8->_screen,
                _chip8->_registers.V[x],
                _chip8->_registers.V[y],
                &(_chip8->_memory.memory[_chip8->_registers.I]),
                nibble
            );
            break;
        case 0xE000:
            switch(KK)
            {
                // SKP Vx : skip next inst if key with Value Vx is pressed
                case 0x9E:
                    if(chip8_keyboard_is_down(&_chip8->_keyboard,_chip8->_registers.V[x]))_chip8->_registers.PC+=2;
                    break;
                // SKNP Vx: skip next inst if key with Value Vx is not pressed
                case 0xA1:
                    if(!chip8_keyboard_is_down(&_chip8->_keyboard,_chip8->_registers.V[x]))_chip8->_registers.PC+=2;
                    break;

            };
            break;
        case 0xF000:
            switch(KK)
                {
                    // LD Vx,DT : set Vx to Delay Timer value
                    case 0x07:
                        _chip8->_registers.V[x]=_chip8->_registers.delay_timer;
                        break;
                    // LD Vx,k : wait for a key press and store it's value in Vx
                    case 0x0A:
                        _chip8->_registers.V[x]=chip8_wait_for_key_press(_chip8);
                        break;
                    // LD DT, Vx : set delay Timer to Vx
                    case 0x15:
                        _chip8->_registers.delay_timer=_chip8->_registers.V[x];
                        break;
                    // LD ST, Vx : set Sound Timer to Vx
                    case 0x18:
                        _chip8->_registers.sound_timer=_chip8->_registers.V[x];
                        break;
                    // ADD I, Vx : I = I + Vx
                    case 0x1E:
                        _chip8->_registers.I+=_chip8->_registers.V[x];
                        break;
                    // LD F, Vx : set I = location of sprite for digit Vx
                    case 0x29:
                        _chip8->_registers.I=(_chip8->_registers.V[x]*0x05);
                        break;
                    // LD B, Vx : set BCD(Vx) in Memory(I[0,2])
                    case 0x33:
                    {
                        uint8_t h,t,o;
                        h=_chip8->_registers.V[x]/100;
                        t=(_chip8->_registers.V[x])/10 %10 ;
                        o=(_chip8->_registers.V[x])%10;
                        _chip8->_memory.memory[_chip8->_registers.I]=h;
                        _chip8->_memory.memory[_chip8->_registers.I+1]=t;
                        _chip8->_memory.memory[_chip8->_registers.I+2]=o;
                    } 
                        break;
                    // LD [I],Vx : store registers V[0,x] in memory(I[0,x]);
                    case 0x55:
                    {
                        for(int i=0;i<=x;i++)
                        {
                            chip8_memory_write(&_chip8->_memory,_chip8->_registers.I+i,_chip8->_registers.V[i]);
                        }
                    }
                        break;
                    // LD Vx, [I]: read registers V[0,x] in memory(I[0,x]);
                    case 0x65:
                        for(int i=0;i<=x;i++)
                        {
                            _chip8->_registers.V[i]=chip8_memory_read_byte(&_chip8->_memory,_chip8->_registers.I+i);
                        }
                        break;
                };
            break;
            
        default:
            printf("[ERROR] OPCODE : 0x%x Not Found {@chip8.c->chip8_exec_ext()}\n",OPcode);
            exit(-1);
            break;
    }
}

void chip8_exec(struct chip8* _chip8,uint16_t OPcode)
{
    switch (OPcode)
    {
    // CLS : clear screen
    case 0x00E0:
        chip8_screen_clear(&_chip8->_screen);
        break;
    // RET : return from subroutine
    case 0x00EE:
        _chip8->_registers.PC = chip8_stack_pop(_chip8);
        break;
    // The Opcode holds data to be extracted and should be handeld by the extended function
    default:
        chip8_exec_ext(_chip8,OPcode);
        break;
    }

}

void chip8_load(struct chip8* _chip8,const uint8_t* prog_buffer,size_t size)
{
    memcpy_safe_check(_chip8->_memory.memory,(size+CHIP8_PROGRAM_LOAD_ADDRESSE));
    memcpy(&(_chip8->_memory.memory[CHIP8_PROGRAM_LOAD_ADDRESSE]),prog_buffer,size);  
}