#pragma once

#include "offsets.h"

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <vector>
#include <Windows.h>

namespace aimbot
{
	struct MyPlayer_t
	{
		uint32_t Team;
		uint32_t Health;
		float Position[3];

		void ReadInformation(uintptr_t engine, uintptr_t client, uintptr_t localPlayer)
		{
			if (localPlayer)
			{
				Team = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_iTeamNum);
				Health = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_iHealth);

				std::memcpy(Position, reinterpret_cast<float*>(localPlayer + offset::m_vecOrigin), sizeof(Position));
			}
		}
	} MyPlayer;

	struct TargetList_t
	{
		float Distance;
		float AimbotAngle[3];

		TargetList_t() { }

		TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
		{
			Distance = Get3DDistance(myCoords[0], myCoords[1], myCoords[2],
				enemyCoords[0], enemyCoords[1], enemyCoords[2]);

			AimbotAngle[0] = aimbotAngle[0];
			AimbotAngle[1] = aimbotAngle[1];
			AimbotAngle[2] = aimbotAngle[2];
		}

		float Get3DDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
			float enX, float enZ, float enY)
		{
			return float(sqrt(pow(float(enX - myCoordsX), 2.0) +
				pow(float(enY - myCoordsY), 2.0) +
				pow(float(enZ - myCoordsZ), 2.0)));
		}
	};

	struct PlayerList_t
	{
		uintptr_t CBaseEntity;
		uint32_t Team;
		uint32_t Health;
		float Position[3];
		float AimbotAngle[3];

		void ReadInformation(uintptr_t client, int index)
		{
			CBaseEntity = *reinterpret_cast<std::uintptr_t*>(client + (offset::m_dwEntityList + index * 0x20));

			if (CBaseEntity)
			{
				Team = *reinterpret_cast<std::uint32_t*>(CBaseEntity + offset::m_iTeamNum);
				Health = *reinterpret_cast<std::uint32_t*>(CBaseEntity + offset::m_iHealth);

				std::memcpy(Position, reinterpret_cast<float*>(CBaseEntity + offset::m_vecOrigin), sizeof(Position));
			}
		}
	} PlayerList[32];

	struct CompareTargetEnArray
	{
		bool operator()(TargetList_t& lhs, TargetList_t& rhs)
		{
			return lhs.Distance < rhs.Distance;
		}
	};

	void CalcAngle(float* src, float* dst, float* angles)
	{
		float delta[3] = { static_cast<float>(src[0] - dst[0]),
						   static_cast<float>(src[1] - dst[1]),
						   static_cast<float>(src[2] - dst[2]) };

		float hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

		angles[0] = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
		angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
		angles[2] = 0.0f;

		if (delta[0] >= 0.0f)
		{
			angles[1] += 180.0f;
		}
	}

	void Aimbot(uintptr_t engine, uintptr_t client)
	{
		uint32_t numPlayers = *reinterpret_cast<std::uint32_t*>(engine + offset::m_iNumPlayers);

		TargetList_t* TargetList = new TargetList_t[numPlayers];

		int targetLoop = 0;

		for (int i = 0; i < numPlayers; i++)
		{
			PlayerList[i].ReadInformation(client, i);

			if (PlayerList[i].Team != MyPlayer.Team && PlayerList[i].Health > 2)
			{
				CalcAngle(MyPlayer.Position, PlayerList[i].Position, PlayerList[i].AimbotAngle);

				TargetList[targetLoop] = TargetList_t(PlayerList[i].AimbotAngle, MyPlayer.Position, PlayerList[i].Position);

				targetLoop++;
			}
		}

		if (targetLoop > 0)
		{
			std::sort(TargetList, TargetList + targetLoop, CompareTargetEnArray());

			if (GetAsyncKeyState(0x56) & 0x8000)
			{
				std::memcpy(reinterpret_cast<void*>(engine + offset::m_angRotation),
					TargetList[0].AimbotAngle, sizeof(TargetList[0].AimbotAngle));
			}
		}

		targetLoop = 0;
		delete[] TargetList;
	}
}