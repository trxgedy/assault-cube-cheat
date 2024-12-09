#ifndef HOOK_HPP
#define HOOK_HPP

namespace hook
{
    template <size_t length>
    class c_hook
    {
    private:
        size_t base_address = reinterpret_cast< size_t > ( GetModuleHandleA( "ac_client.exe" ) );
        size_t function_offset;
        size_t *return_address;

        std::array<std::uint8_t, length> default_bytes;

        LPVOID target_function, custom_function;

    public:
        c_hook( size_t offset, LPVOID function, size_t *jump_back )
        {
            this->function_offset = offset; 
            this->custom_function = function;
            this->return_address = jump_back;
            this->target_function = reinterpret_cast< LPVOID >( base_address + function_offset );

            *return_address = reinterpret_cast< size_t >( this->target_function ) + 5;

            std::memcpy( default_bytes.data( ), this->target_function, default_bytes.size( ) );
        }

        ~c_hook( )
        {
            utils::memory::patch_bytes( this->target_function, default_bytes );
        }

        void toggle_hook( bool toggle )
        {
            if ( !toggle )
            {
                utils::memory::patch_bytes( this->target_function, default_bytes );
                return;
            }

            DWORD page_protect;
            VirtualProtect( this->target_function, length, PAGE_EXECUTE_READWRITE, &page_protect );

            DWORD address_distance = ( ( DWORD ) this->custom_function - ( DWORD ) this->target_function ) - 5;

            *reinterpret_cast< BYTE * >( this->target_function ) = 0xE9;
            *reinterpret_cast< DWORD * >( ( DWORD ) this->target_function + 1 ) = address_distance;

            VirtualProtect( this->target_function, length, page_protect, &page_protect );
        }
    };
}

#endif // !HOOK_HPP