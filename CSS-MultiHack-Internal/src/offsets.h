#pragma once

#include <iostream>

namespace offset
{
    // client.dll
    constexpr std::ptrdiff_t m_dwLocalPlayer = 0x5F3B98;
    constexpr std::ptrdiff_t m_dwEntityList = 0x6088F8;
    constexpr std::ptrdiff_t m_dwForceAttack = 0x675DF0;
    constexpr std::ptrdiff_t m_dwForceJump = 0x675DE0;

    // engine.dll
    constexpr std::ptrdiff_t m_angRotation = 0x53F4E4;
    constexpr std::ptrdiff_t m_iNumPlayers = 0x6DB960;

    // CBasePlayer
    constexpr std::ptrdiff_t m_iHealth = 0xD0;
    constexpr std::ptrdiff_t m_iTeamNum = 0xD8;
    constexpr std::ptrdiff_t m_vecOrigin = 0x320;
    constexpr std::ptrdiff_t m_fFlags = 0x440;
    constexpr std::ptrdiff_t m_iCrosshairId = 0x1B20;
    constexpr std::ptrdiff_t m_flFlashMaxAlpha = 0x1A54;
}