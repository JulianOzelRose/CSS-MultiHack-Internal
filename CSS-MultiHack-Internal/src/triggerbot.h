#pragma once

#include "offsets.h"

namespace triggerbot
{
	void Triggerbot(uintptr_t client, uintptr_t localPlayer)
	{
		int crosshairId = *reinterpret_cast<uint32_t*>(localPlayer + offset::m_iCrosshairId);

		if (crosshairId != 0 && crosshairId <= 64)
		{
			uintptr_t targetEntity = *reinterpret_cast<std::uintptr_t*>(client + (offset::m_dwEntityList + (crosshairId - 1) * 0x20));

			if (targetEntity)
			{
				int targetTeam = *reinterpret_cast<uint32_t*>(targetEntity + offset::m_iTeamNum);
				int playerTeam = *reinterpret_cast<uint32_t*>(localPlayer + offset::m_iTeamNum);

				if (playerTeam != targetTeam)
				{
					int attackFlag = *reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack);

					if (attackFlag == 4)
					{
						*reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack) = 5;
						std::this_thread::sleep_for(std::chrono::milliseconds(1));

						*reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack) = 4;
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
				}
			}
		}
	}
}