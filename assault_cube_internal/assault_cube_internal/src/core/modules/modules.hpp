#ifndef MODULES_HPP
#define MODULES_HPP

namespace modules
{
	class c_modules
	{
	private:
		size_t base_address = reinterpret_cast< size_t > ( GetModuleHandleA( "ac_client.exe" ) );
		size_t player_addr = *reinterpret_cast< size_t * > ( base_address + offset_player );
		size_t *radar_addr = reinterpret_cast< size_t * >( base_address + offset_radar );
		size_t *recoil_addr = reinterpret_cast< size_t * >( base_address + offset_recoil );
		size_t *ammo_addr = reinterpret_cast< size_t * >( base_address + offset_ammo_function );

		std::vector<size_t> get_entities( );

	public:
		c_modules( ) = default;
		~c_modules( ) = default;

		template<typename type>
		void change_value( size_t address, type value )
		{
			*reinterpret_cast< type * > ( address ) = value;
		}

		void radar( bool toggle );
		void no_recoil( bool toggle );
		void infinite_ammo( bool toggle );

		size_t get_player_addr( ){ return this->player_addr; }
		size_t get_current_gun_recoil_addr( );

		void aimbot_thread( );
	};
}

inline auto _modules = std::make_unique< modules::c_modules >( );

#endif