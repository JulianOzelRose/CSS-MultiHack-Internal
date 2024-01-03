# Counter-Strike: Source - Internal Multihack
This is a simple multihack for Counter-Strike: Source. Its features include
aimbot, triggerbot, anti-flash, and bunnyhop hack. It is an internal hack,
so you will need a DLL injector. For more details on installation and use,
scroll down to the installation and use section. Later on this README are some
details about reversing the game as well as offset tables.

![MultiHack-Menu](https://github.com/JulianOzelRose/CSS-MultiHack-Internal/assets/95890436/4e31f08a-dab9-4977-b2ff-b7e8396da1eb)


## Installation and use
To download this hack, navigate to the [build](https://github.com/JulianOzelRose/CSS-MultiHack-Internal/tree/master/CSS-MultiHack-Internal/build)
folder, then download ```CSS-MultiHack-Internal.dll```. You will then need to use a DLL injector.
I recommend using [this one](https://guidedhacking.com/resources/guided-hacking-dll-injector.4/).
Once you have the injector, select the process for Counter-Strike: Source, then open the DLL,
then press inject. The multihack should then be running. From the menu, you can toggle the different
features on or off. To run the aimbot, press the ```V``` key. To hide the menu, press the ```INSERT``` Key.
To unload the hack, press the ```END``` key on your numpad.

## Warning
This program reads and modifies memory internally. If you use it on a VAC-secured server,
you will be banned. To prevent this, be sure to add the ```-insecure``` flag to your
game's launch options. You can find this menu by right-clicking on your game from Steam,
then going to Properties. You can then safely play against bots without worrying
about catching a VAC ban.

## Bunnyhop
The bunnyhop hack is relatively straightforward. It works by checking for ground flags. If the player
is on the ground or crouching, it forces a jump by setting ```m_dwForceJump``` to 6. When in the air,
the force jump variable resets. The result is perfectly timed jumps every time.

```
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
```

## Anti-flash
For anti-flash, there are 2 relevant variables to be changed; ```m_flFlashMaxAlpha```
determines how bright the flashbang's blinding effect is. ```m_flFlashMaxDuration``` determines
how long the flashbang's effect lasts. Setting either one to 0 will nullify the flashbang's effect.

```
void AntiFlash(uintptr_t localPlayer)
{
	float flashMaxAlpha = *reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha);
	float flashMaxDuration = *reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxDuration);

	if (flashMaxAlpha > 0.0)
	{
		*reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxAlpha) = 0;
	}

	if (flashMaxDuration > 0.0)
	{
		*reinterpret_cast<float*>(localPlayer + offset::m_flFlashMaxDuration) = 0;
	}
}
```

## Triggerbot
The triggerbot uses ```m_iCrosshairId``` to determine what entity is in the player's
crosshairs. A value of 0 means no entity. A non-zero value could mean another player,
or it could mean a barrel. To prevent the triggerbot from firing at barrels, the
```crosshairId``` condition should be capped at 64. Once an entity is detected in the
crosshairs, a check is performed to make sure the entity is on the opposing team.
Then, an attack is forced by setting ```m_dwForceAttack``` to 5. After a delay, this
variable needs to be set back to its default value of 4, or only 1 shot will be fired.
```
void Triggerbot(uintptr_t client, uintptr_t localPlayer)
{
	int crosshairId = *reinterpret_cast<uint32_t*>(localPlayer + offset::m_iCrosshairId);

	if (crosshairId != 0 && crosshairId <= 64)
	{
		uintptr_t targetEntity = *reinterpret_cast<std::uintptr_t*>(client + (offset::m_dwEntityList + (crosshairId - 1) * 0x10));

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
```

## Aimbot
Credit goes to [Guided Hacking](https://guidedhacking.com/) for the aimbot formula.
The aimbot works by iterating through the entity list. Each entity is 0x10 bytes apart,
so you must use 0x10 as the iterator. Then, the aimbot checks to ensure that the entity
is on the opposing team and is alive. Lastly, it performs calculations using trigonometry
to get the ideal angles, and updates the view angles, ```m_angRotation```, to focus on the
enemy's head.
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
Unfortunately there is no offset dumper for CS:S that I am aware of. However, it
is possible to configure [hazedumper](https://github.com/frk1/hazedumper) with CS:S
offset signatures and get it to dump offsets that way. Below are the offsets I used
for this trainer. Note that the offsets for ```CBaseEntity``` are located on ```client.dll```.

###                client.dll                            ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x47C33C        | Float           | m_angRotation        |
| 0x4C88E8        | Pointer         | m_dwLocalPlayer      |
| 0x4F5D24        | DWORD           | m_dwForceJump        |
| 0x4F5D30        | DWORD           | m_dwForceAttack      |

###               CBasePlayer                            ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x0094          | Integer         | m_iHealth            |
| 0x009C          | Integer         | m_iTeamNum           |
| 0x0290          | Float           | m_vecOrigin          |
| 0x0350          | Float           | m_fFlags             |
| 0x0D80          | Pointer         | m_hActiveWeapon      |
| 0x0E48          | Float           | m_vecPunchAngle      |
| 0x144C          | Float           | m_flFlashMaxAlpha    |
| 0x1450          | Float           | m_flFlashMaxDuration |


###               engine.dll                             ###
| Offset          | Type            | Variable             |
| --------------- | --------------- | -------------------- |
| 0x5EC82C        | Integer         | m_iNumPlayers        |

## Sources
I used the following sources to guide me through the creation of this trainer. The
tutorials for hooking DirectX and getting the aimbot working were instrumental. I recommend
checking them out for more in-depth explanations of the processes involved.
- [Guided Hacking - C++ Aimbot Tutorial](https://www.youtube.com/playlist?list=PLt9cUwGw6CYFgcPd7040cBPCq24maby8_)
- [cazz - ImGui Internal DirectX 9 Hook](https://www.youtube.com/watch?v=vF5fzIDUJVw)
