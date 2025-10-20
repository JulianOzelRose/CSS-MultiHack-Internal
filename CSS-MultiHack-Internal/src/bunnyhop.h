#pragma once

#include "offsets.h"
#include <Windows.h>

namespace bunnyhop
{
	const int FLAG_STANDING = 257;
	const int FLAG_ONGROUND = 263;
	const int JUMP_PRESS = 6;

	void Bunnyhop(uintptr_t client, uintptr_t localPlayer)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			int flag = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_fFlags);

			if (flag == FLAG_STANDING || flag == FLAG_ONGROUND)
			{
				*reinterpret_cast<uint32_t*>(client + offset::m_dwForceJump) = JUMP_PRESS;
			}
		}
	}
}