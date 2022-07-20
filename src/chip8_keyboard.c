#include<chip8_keyboard.h>
#include <assert.h>
#define chip8_key_is_bound(key) assert(key>=0x00 && key< CHIP8_TOTAL_KEYS)

void chip8_set_keyboard_map(struct chip8_keyboard*_keyboard,const uint8_t* _map)
{
    _keyboard->keyboard_map=_map;
}

int8_t chip8_keyboard_map(struct chip8_keyboard* _keyboard,uint8_t key)
{
    for(int i=0;i<CHIP8_TOTAL_KEYS;i++)
    {
        if(_keyboard->keyboard_map[i]==key)
        {
            return i;
        }
    }

    return -1;
}

void chip8_keyboard_down(struct chip8_keyboard* _keyboard,uint32_t key)
{
    chip8_key_is_bound((uint32_t)(key));
    _keyboard->keyboard[(uint32_t)(key)]=true;
}
void chip8_keyboard_up(struct chip8_keyboard* _keyboard,uint32_t key)
{
    chip8_key_is_bound((uint32_t)(key));
    _keyboard->keyboard[(uint32_t)(key)]=false;
}
bool chip8_keyboard_is_down(struct chip8_keyboard* _keyboard,uint32_t key)
{
    chip8_key_is_bound((uint32_t)(key));
    return  _keyboard->keyboard[(uint32_t)(key)];
}