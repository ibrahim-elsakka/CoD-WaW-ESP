#pragma once
#include <cstdint>


namespace Zombies
{
	//World at War
	uintptr_t AEntityList = 0x018E7448;
	uintptr_t AViewMatrix = 0x008E870C;
	namespace Offsets
	{
		uintptr_t OLocation = 0x18;
		uintptr_t OHealth = 0x1C8;
		uintptr_t ODistanceBetween = 0x88;
	}

	//Assault Cube
	/*uintptr_t AEntityList = 0x0010F4F8;
	uintptr_t AViewMatrix = 0x00501AE8;

	namespace Offsets
	{
		uintptr_t OLocation = 0x34;
		uintptr_t OHealth = 0xF8;
		uintptr_t ODistanceBetween = 0x04;
	}*/
}