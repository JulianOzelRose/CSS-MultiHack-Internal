#pragma once

#include "offsets.h"

namespace triggerbot
{
	const int MAX_PLAYERS = 64;
	const int ATTACK_PRESS = 5;
	const int ATTACK_RELEASE = 4;

	void Triggerbot(uintptr_t client, uintptr_t localPlayer)
	{
		uint32_t crosshairId = *reinterpret_cast<uint32_t*>(localPlayer + offset::m_iCrosshairId);

		if (crosshairId == 0 || crosshairId > MAX_PLAYERS)
		{
			return;
		}

		uintptr_t targetEntity = *reinterpret_cast<std::uintptr_t*>(client + (offset::m_dwEntityList + (crosshairId - 1) * 0x20));

		if (targetEntity)
		{
			uint32_t targetTeam = *reinterpret_cast<uint32_t*>(targetEntity + offset::m_iTeamNum);
			uint32_t playerTeam = *reinterpret_cast<uint32_t*>(localPlayer + offset::m_iTeamNum);

			if (playerTeam != targetTeam)
			{
				int attackFlag = *reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack);

				if (attackFlag == ATTACK_RELEASE)
				{
					*reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack) = ATTACK_PRESS;
					std::this_thread::sleep_for(std::chrono::milliseconds(1));

					*reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack) = ATTACK_RELEASE;
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		}
	}
}