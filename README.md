# Counter-Strike: Source - Internal Multihack
This is a simple multihack for Counter-Strike: Source. Its features include
aimbot, triggerbot, anti-flash, and bunnyhop hack. It is an internal hack,
so you will need to use a DLL injector. For more details on installation and use,
scroll down to the section below. Later on this README are some
details about reversing the game as well as offset tables.

![MultiHack-Menu](https://github.com/JulianOzelRose/CSS-MultiHack-Internal/assets/95890436/4e31f08a-dab9-4977-b2ff-b7e8396da1eb)


## Installation and use
To download this hack, navigate to the [Releases](https://github.com/JulianOzelRose/CSS-MultiHack-Internal/releases)
page, then download the latest version of the DLL. You will then need to use a DLL injector.
I recommend using [this one](https://guidedhacking.com/resources/guided-hacking-dll-injector.4/).
Once you have the injector running, select the process for Counter-Strike: Source (`cstrike_win64.exe`). Then open and select the DLL,
then press `Inject`. The multihack should then be running with the menu visible. From the menu, you can toggle the different
features on or off. To run the aimbot, press the `V` key. To hide the menu, press the `INSERT` Key.
To unload the hack, press the `END` key on your numpad.

## Warning
This program reads and modifies memory internally. If you use it on a VAC-secured server,
you will be banned. To prevent this, be sure to add the `-insecure` flag to your
game's launch options. You can find this menu by right-clicking on your game from Steam,
then going to Properties. You can then safely play against bots without worrying
about catching a VAC ban.

## Demonstration
https://github.com/JulianOzelRose/CSS-MultiHack-Internal/assets/95890436/4f6940c2-bb24-451f-8c8a-2f06362440b4


## Bunnyhop
The bunnyhop hack is relatively straightforward. It works by first checking for ground flags. If the player
is on the ground or crouching, it forces a jump by setting `m_dwForceJump` to 6. When in the air,
the force jump variable resets. The result is perfectly timed jumps every time.

```
const uint32_t FLAG_STANDING = 257;
const uint32_t FLAG_ONGROUND = 263;
const uint32_t JUMP_PRESS = 6;

void Bunnyhop(uintptr_t client, uintptr_t localPlayer)
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		uint32_t flag = *reinterpret_cast<std::uint32_t*>(localPlayer + offset::m_fFlags);

		if (flag == FLAG_STANDING || flag == FLAG_ONGROUND)
		{
			*reinterpret_cast<uint32_t*>(client + offset::m_dwForceJump) = JUMP_PRESS;
		}
	}
}
```

## Anti-flash
For anti-flash, there are 2 relevant variables; `m_flFlashMaxAlpha`
determines how bright the flashbang's blinding effect is, and `m_flFlashMaxDuration` determines
how long the flashbang's effect lasts. Just setting `m_flFlashMaxAlpha` to 0 is sufficient to nullify the flashbang's blinding effect.
```
void AntiFlash(uintptr_t localPlayer)
{
	float flashMaxAlpha = *reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha);

	if (flashMaxAlpha > 0.0f)
	{
		*reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha) = 0.0f;
	}
}
```

## Triggerbot
The triggerbot uses `m_iCrosshairId` to determine what entity is in the player's
crosshairs. A value of 0 means no entity. A non-zero value could mean another player,
or it could mean a physics object. To prevent the triggerbot from firing at physics objects, the
crosshair ID condition should be capped at 64. Once an entity is detected in the crosshairs,
a check is performed to ensure the entity is on the opposing team. Then, an attack is forced
by setting `m_dwForceAttack` to 5. After a delay, this variable needs to be set back to
its default value of 4, or only 1 shot will be fired.
```
const uint32_t MAX_PLAYERS = 64;
const uint32_t ATTACK_PRESS = 5;
const uint32_t ATTACK_RELEASE = 4;

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
			uint32_t attackFlag = *reinterpret_cast<uint32_t*>(client + offset::m_dwForceAttack);

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
```

## Aimbot
The aimbot works by first iterating through the entity list. Each entity is 0x20 bytes apart,
so you must use 0x20 as the iterator. Then, the aimbot checks to ensure that the entity
is on the opposing team and is alive. Lastly, it performs calculations using trigonometry
to get the ideal angles, and updates the player's view angles, `m_angRotation`, to focus on the
closest enemy's head. Credit goes to [Guided Hacking](https://guidedhacking.com/) for the aimbot formula.
```
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
```

## Offset tables
Unfortunately, there is no offset dumper for CS:S that I am aware of. However, it
is possible to configure [hazedumper](https://github.com/frk1/hazedumper) with CS:S
offset signatures and get it to dump offsets that way. Below are the offsets I used
for this trainer. Note that the offsets for `CBasePlayer` are located on `client.dll`.

###               CBasePlayer                            ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x00D0          | Int32           | m_iHealth            |
| 0x00D8          | Int32           | m_iTeamNum           |
| 0x0320          | Float32[3]      | m_vecOrigin          |
| 0x0440          | Float32         | m_fFlags             |
| 0x127C          | Float32[3]      | m_vecPunchAngle      |
| 0x1A54          | Float32         | m_flFlashMaxAlpha    |
| 0x1A4C          | Float32         | m_flFlashMaxDuration |
| 0x1B20	      | Int32	        | m_iCrosshairId	   |

###                client.dll                            ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x5F4B68        | Pointer         | m_dwLocalPlayer      |
| 0x6098C8        | Pointer         | m_dwEntityList       |
| 0x677300        | DWORD           | m_dwForceJump        |
| 0x677310        | DWORD           | m_dwForceAttack      |

###               engine.dll                             ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x53E4E4        | Float32         | m_angRotation        |
| 0x6DA960        | Int32           | m_iNumPlayers        |

## Sources
I used the following sources to guide me through the creation of this trainer. The
tutorials for hooking DirectX and getting the aimbot working were instrumental. I recommend
checking them out for more in-depth explanations of the processes involved.
- [Guided Hacking - C++ Aimbot Tutorial](https://www.youtube.com/playlist?list=PLt9cUwGw6CYFgcPd7040cBPCq24maby8_)
- [cazz - ImGui Internal DirectX 9 Hook](https://www.youtube.com/watch?v=vF5fzIDUJVw)
