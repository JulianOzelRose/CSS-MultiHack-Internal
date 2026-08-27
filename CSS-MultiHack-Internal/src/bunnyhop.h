#pragma once

#include "offsets.h"
#include <Windows.h>

namespace bunnyhop
{
	constexpr uint32_t FL_ONGROUND = (1 << 0);
	constexpr uint32_t JUMP_PRESS = 6;

	void Bunnyhop(uintptr_t client, uintptr_t localPlayer)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			const uint32_t flags = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_fFlags);

			if (flags & FL_ONGROUND)
			{
				*reinterpret_cast<uint32_t*>(client + offset::m_dwForceJump) = JUMP_PRESS;
			}
		}
	}
}