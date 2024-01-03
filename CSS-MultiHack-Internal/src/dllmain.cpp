#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <thread>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>

#include "hooks.h"
#include "globals.h"
#include "offsets.h"
#include "aimbot.h"
#include "triggerbot.h"
#include "bunnyhop.h"
#include "antiflash.h"

void Setup(const HMODULE instance)
{
	try
	{
		gui::Setup();
		hooks::Setup();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"Error",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_END))
	{
		uintptr_t client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));
		uintptr_t engine = reinterpret_cast<std::uintptr_t>(GetModuleHandle("engine.dll"));

		uintptr_t localPlayer = *reinterpret_cast<std::uintptr_t*>(client + offset::m_dwLocalPlayer);

		if (localPlayer)
		{

			if (globals::bAimbot)
			{
				aimbot::MyPlayer.ReadInformation(engine, client, localPlayer);
				aimbot::Aimbot(engine, client);
			}

			if (globals::bBunnyhop)
			{
				bunnyhop::Bunnyhop(client, localPlayer);
			}

			if (globals::bTriggerbot)
			{
				triggerbot::Triggerbot(client, localPlayer);
			}

			if (globals::bAntiFlash)
			{
				antiflash::AntiFlash(localPlayer);
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

UNLOAD:
	hooks::Destroy();
	gui::Destroy();
	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
			instance,
			0,
			nullptr
		);

		if (thread)
		{
			CloseHandle(thread);
		}
	}

	return TRUE;
}