#pragma once

#include <iostream>

namespace offset
{
	constexpr std::ptrdiff_t m_dwLocalPlayer = 0x5F4B68;
	constexpr std::ptrdiff_t m_dwEntityList = 0x6098C8;
	constexpr std::ptrdiff_t m_dwForceAttack = 0x677310;
	constexpr std::ptrdiff_t m_dwForceJump = 0x677300;
	constexpr std::ptrdiff_t m_angRotation = 0x53E4E4;
	constexpr std::ptrdiff_t m_iNumPlayers = 0x6DA960;
	//constexpr std::ptrdiff_t m_vecPunchAngle = 0xE48;		// Need to find new offset for x64
	constexpr std::ptrdiff_t m_iHealth = 0xD0;
	constexpr std::ptrdiff_t m_iTeamNum = 0xD8;
	//constexpr std::ptrdiff_t m_hViewModel = 0x1118;		// Need to find new offset for x64
	constexpr std::ptrdiff_t m_vecOrigin = 0x320;
	constexpr std::ptrdiff_t m_fFlags = 0x440;
	constexpr std::ptrdiff_t m_iCrosshairId = 0x1B20;
	//constexpr std::ptrdiff_t m_hActiveWeapon = 0xD80;		// Need to find new offset for x64
	constexpr std::ptrdiff_t m_flFlashMaxAlpha = 0x1A54;
}