#include "stdafx.hpp"

void __stdcall main_thread( LPVOID param )
{
    AllocConsole( );
    FILE *conout {};
    freopen_s( &conout, "conout$", "w", stdout );

    std::thread( &modules::c_modules::aimbot_thread, _modules.get( ) ).detach( );

    const auto _god_mode_hook = std::make_unique< hook::c_hook<5> >( offset_health_function, assembly::god_mode::god_mode_function, &assembly::god_mode::address_to_return );
    const auto _rapid_fire_hook = std::make_unique< hook::c_hook<5> >( offset_gun_delay, assembly::rapid_fire::rapid_fire_function, &assembly::rapid_fire::address_to_return );

    bool no_clip_toggle = false;
    bool radar_toggle = false;
    bool god_mode_toggle = false;
    bool infinite_ammo_toggle = false;\
    bool no_recoil_toggle = false;
    bool rapid_fire_toggle = false;

    std::println( "local player address: 0x{:X}", _modules->get_player_addr( ) );

    _modules->change_value<int>( _modules->get_player_addr( ) + offset_health, 9999 );
    _modules->change_value<int>( _modules->get_player_addr( ) + offset_ammo, 999 );
    _modules->change_value<int>( _modules->get_player_addr( ) + offset_grenade, 99 );

    while ( true )
    {
        if ( GetAsyncKeyState( VK_DELETE ) )
            break;

        if ( GetAsyncKeyState( VK_F1 ) & 1 )
        {
            no_clip_toggle = !no_clip_toggle;
            _modules->change_value<int>( _modules->get_player_addr( ) + offset_gamemode, ( no_clip_toggle ? 4 : 0 ) );

            no_clip_toggle ? std::println( "no clip on" ) : std::println( "no clip off" );
        }

        if ( GetAsyncKeyState( VK_F2 ) & 1 )
        {
            radar_toggle = !radar_toggle;
            _modules->radar( radar_toggle );

            radar_toggle ? std::println( "radar on" ) : std::println( "radar off" );
        }

        if ( GetAsyncKeyState( VK_F3 ) & 1 )
        {
            god_mode_toggle = !god_mode_toggle;
            _god_mode_hook->toggle_hook( god_mode_toggle );

            god_mode_toggle ? std::println( "god mode on" ) : std::println( "god mode off" );
        }

        if ( GetAsyncKeyState( VK_F4 ) & 1 )
        {
            infinite_ammo_toggle = !infinite_ammo_toggle;
            _modules->infinite_ammo( infinite_ammo_toggle );

            infinite_ammo_toggle ? std::println( "infinite ammo on" ) : std::println( "inifinite ammo off" );
        }

        if ( GetAsyncKeyState( VK_F5 ) & 1 )
        {
            no_recoil_toggle = !no_recoil_toggle;
            _modules->no_recoil( no_recoil_toggle );

            no_recoil_toggle ? std::println( "no recoil on" ) : std::println( "no recoil off" );
        }

        if ( GetAsyncKeyState( VK_F6 ) & 1 )
        {
            rapid_fire_toggle = !rapid_fire_toggle;
            _rapid_fire_hook->toggle_hook( rapid_fire_toggle );

            rapid_fire_toggle ? std::println( "rapid fire on" ) : std::println( "rapid fire off" );
        }

        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }

    ShowWindow( FindWindowA( "ConsoleWindowClass", nullptr ), SW_HIDE );
    FreeConsole( );

    FreeLibrary( reinterpret_cast< HMODULE > ( param ) );
}

bool __stdcall DllMain( HMODULE hModule, DWORD reason, LPVOID lpReserved )
{
    switch ( reason )
    {
        case DLL_PROCESS_ATTACH:
            std::thread( main_thread, hModule ).detach( );

        case DLL_PROCESS_DETACH:
            break;
    }

    return true;
}