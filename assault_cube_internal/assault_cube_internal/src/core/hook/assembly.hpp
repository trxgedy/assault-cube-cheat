#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "core\modules\modules.hpp"

namespace assembly
{
    static size_t player_addr = _modules->get_player_addr( );

    namespace god_mode
    {
        static size_t address_to_return {};

        __inline __declspec( naked ) void god_mode_function( )
        {
            __asm
            {
                lea eax, [ ebx - 0xE8 ]
                cmp eax, player_addr
                je its_me

                sub[ ebx + 0x04 ], esi

                its_me :
                    mov eax, esi

                jmp[ address_to_return ]
            }
        }
    }

    namespace rapid_fire
    {
        static size_t address_to_return {};
        static size_t recoil_offset( ) { return _modules->get_current_gun_recoil_addr( ); }

        __inline __declspec( naked ) void rapid_fire_function( )
        {
            __asm
            {
                mov ebx, eax
                call recoil_offset
                xchg eax, ebx

                sub eax, ebx
                cmp eax, player_addr
                je its_me

                add eax, ebx
                mov[ eax ], ecx
                mov eax, [ esi + 0x14 ]

                its_me:
                    add eax, ebx
                    mov[ eax ], 0
                    mov eax, [ esi + 0x14 ]

                jmp[ address_to_return ]
            }
        }
    }
}

#endif // !ASSEMBLY_HPP