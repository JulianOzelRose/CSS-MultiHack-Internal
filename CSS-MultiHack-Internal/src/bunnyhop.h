#pragma once

#include "offsets.h"
#include <Windows.h>

namespace bunnyhop
{
	void Bunnyhop(uintptr_t client, uintptr_t localPlayer)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			int flag = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_fFlags);

			if (flag == 257 || flag == 263)
			{
				*reinterpret_cast<uint32_t*>(client + offset::m_dwForceJump) = 6;
			}
		}
	}
}