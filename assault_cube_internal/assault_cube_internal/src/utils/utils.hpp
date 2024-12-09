#ifndef	UTILS_HPP
#define UTILS_HPP

#include "stdafx.hpp"

namespace utils
{
	namespace memory
	{
		__inline void patch_bytes( LPVOID address, std::span<std::uint8_t> bytes )
		{
			DWORD protect;

			VirtualProtect( address, bytes.size( ), PAGE_EXECUTE_READWRITE, &protect );

			std::memcpy( address, bytes.data( ), bytes.size( ) );

			VirtualProtect( address, bytes.size( ), protect, &protect );
		}
	}
}

#endif