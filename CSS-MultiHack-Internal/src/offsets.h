#pragma once

#include <iostream>

namespace offset
{
	constexpr std::ptrdiff_t m_dwLocalPlayer = 0x4C88E8;
	constexpr std::ptrdiff_t m_dwEntityList = 0x4D5AE4;
	constexpr std::ptrdiff_t m_dwForceAttack = 0x4F5D30;
	constexpr std::ptrdiff_t m_dwForceJump = 0x4F5D24;
	constexpr std::ptrdiff_t m_angRotation = 0x47C33C;
	constexpr std::ptrdiff_t m_iNumPlayers = 0x5EC82C;
	constexpr std::ptrdiff_t m_vecPunchAngle = 0xE48;
	constexpr std::ptrdiff_t m_iHealth = 0x94;
	constexpr std::ptrdiff_t m_iTeamNum = 0x9C;
	constexpr std::ptrdiff_t m_hViewModel = 0x1118;
	constexpr std::ptrdiff_t m_vecOrigin = 0x290;
	constexpr std::ptrdiff_t m_fFlags = 0x350;
	constexpr std::ptrdiff_t m_iCrosshairId = 0x14F0;
	constexpr std::ptrdiff_t m_hActiveWeapon = 0xD80;
	constexpr std::ptrdiff_t m_flFlashMaxAlpha = 0x144C;
	constexpr std::ptrdiff_t m_flFlashMaxDuration = 0x1450;
}