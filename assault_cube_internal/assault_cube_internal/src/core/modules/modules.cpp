#include "stdafx.hpp"
#include "modules.hpp"

namespace modules
{
	void c_modules::radar( bool toggle )
	{
		std::array<std::uint8_t, 7> default_bytes = { 0x83, 0xb8, 0x0c, 0x03, 0x00, 0x00, 0x04 }; // cmp dword ptr [eax+0000030C],04
		std::array<std::uint8_t, 7> patched_bytes = { 0x83, 0x38, 0x04, 0x90, 0x90, 0x90, 0x90 }; // cmp dword ptr [eax],04 

		utils::memory::patch_bytes( this->radar_addr, ( toggle ? patched_bytes : default_bytes ) );
	}

	void c_modules::no_recoil( bool toggle )
	{
		std::array<std::uint8_t, 5> default_bytes = { 0xF3, 0x0F, 0x11, 0x56, 0x38 }; // movss [esi+38],xmm2
		std::array<std::uint8_t, 5> patched_bytes = { 0x90, 0x90, 0x90, 0x90, 0x90 };

		utils::memory::patch_bytes( this->recoil_addr, ( toggle ? patched_bytes : default_bytes ) );
	}

	void c_modules::infinite_ammo( bool toggle )
	{
		std::array<std::uint8_t, 2> default_bytes = { 0xFF, 0x08 }; // dec [eax]
		std::array<std::uint8_t, 2> patched_bytes = { 0x90, 0x90 };

		utils::memory::patch_bytes( this->ammo_addr, ( toggle ? patched_bytes : default_bytes ) );
	}

	size_t c_modules::get_current_gun_recoil_addr( )
	{
		for ( size_t i = 0x130; i <= 0x140; i += 4 )
		{
			if ( *reinterpret_cast< int * >( this->player_addr + i ) != 0 )          // each gun has its own ammo offset in the entity class ( 0x130 - 0x140 ) 
			{																		 // the recoil is organized in the same order as the ammo offsets ( 0x154 - 0x164 )
				return i + 0x24;													 // if the ammo offset being checked != 0, we know which gun is selected
			}																		 // the offset of the current gun's ammo + 0x24 = current gun's recoil offset
		}
	}

	std::vector<size_t> c_modules::get_entities( )
	{
		int entities  = *reinterpret_cast< int * >( base_address + offset_player + 0xC );
		int entity_list = *reinterpret_cast< int * >( base_address + offset_player + 0x4 );

		if ( !entities )
			return {};

		std::vector<size_t> buffer;
		buffer.reserve( entities - 1 );

		for ( int i = 1; i < entities; ++i ) // skip the entity 0, which is our local player
		{
			buffer.emplace_back( *reinterpret_cast< int * >( entity_list + i * 0x4 ) );
		}

		return buffer;
	}

	void c_modules::aimbot_thread( )
	{
		while ( !GetAsyncKeyState( VK_DELETE ) )
		{
			if ( GetAsyncKeyState( VK_XBUTTON2 ) )
			{
				static c_entity *local_player = reinterpret_cast< c_entity * >( this->get_player_addr( ) );
				const auto enemies_addresses = this->get_entities( );

				if ( enemies_addresses.size( ) == 0 )
					continue;

				float yaw { 0.f }, pitch { 0.f };
				Vector3 closest_enemy = { 1000.f, 1000.f, 1000.f };

				for ( int i = 0; i < enemies_addresses.size( ); ++i )
				{
					c_entity *enemy = reinterpret_cast< c_entity * >( enemies_addresses.at( i ) );

					if ( enemy->team == local_player->team )
						continue;

					if ( enemy->state != 0 ) // non zero values means that the entity is NOT alive
						continue;

					Vector3 current_enemy_pos = enemy->head_pos - local_player->head_pos;

					if ( current_enemy_pos.get_hypotenuse( ) < closest_enemy.get_hypotenuse( ) )
						closest_enemy = current_enemy_pos;

					yaw = ( std::atan2f( closest_enemy.y, closest_enemy.x ) * 180 / std::numbers::pi_v<float> ) + 90;
					pitch = std::atan2f( closest_enemy.z, std::hypot( closest_enemy.x, closest_enemy.y ) ) * 180 / std::numbers::pi_v<float>;
				}

				if ( yaw )
				{
					local_player->view_angle.x = yaw;
					local_player->view_angle.y = pitch;
				}
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}
}
