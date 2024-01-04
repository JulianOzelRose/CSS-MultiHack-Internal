#pragma once

#include "offsets.h"
#include <cstdint>

namespace antiflash
{
	void AntiFlash(uintptr_t localPlayer)
	{
		float flashMaxAlpha = *reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha);
		float flashMaxDuration = *reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxDuration);

		if (flashMaxAlpha > 0.0f)
		{
			*reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha) = 0.0f;
		}

		if (flashMaxDuration > 0.0f)
		{
			*reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxDuration) = 0.0f;
		}
	}
}