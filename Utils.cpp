#include "Main.h"

void Utils::Init()
{
	std::unique_ptr<CVMTHookManager> Client;
	Client = std::make_unique<CVMTHookManager>(I::Client);
	std::unique_ptr<CVMTHookManager> ClientMode;
	ClientMode = std::make_unique<CVMTHookManager>(I::ClientMode);
	std::unique_ptr<CVMTHookManager> Surface;
	Surface = std::make_unique<CVMTHookManager>(I::Surface);
	std::unique_ptr<CVMTHookManager> Panel;
	Panel = std::make_unique<CVMTHookManager>(I::Panel);
	std::unique_ptr<CVMTHookManager> Studio;
	Studio = std::make_unique<CVMTHookManager>(I::StudioRender);
	std::unique_ptr<CVMTHookManager> ModelRender;
	ModelRender = std::make_unique<CVMTHookManager>(I::ModelRender);

	oLockCursor = reinterpret_cast<LockCursorFn>(Surface->dwHookMethod(reinterpret_cast<DWORD>(Hooks::LockCursor), 67));
	oCreateMove = reinterpret_cast<CreateMoveFn>(ClientMode->dwHookMethod(reinterpret_cast<DWORD>(Hooks::CreateMove), 24));
	oOverrideView = reinterpret_cast<OverrideViewFn>(ClientMode->dwHookMethod(reinterpret_cast<DWORD>(Hooks::OverrideView), 18));
	oPaintTraverse = reinterpret_cast<PaintTraverseFn>(Panel->dwHookMethod(reinterpret_cast<DWORD>(Hooks::PaintTraverse), 41));
	oFrameStageNotify = reinterpret_cast<FrameStageNotifyFn>(Client->dwHookMethod(reinterpret_cast<DWORD>(Hooks::FramStageNotify), 36));
	oBeginFrame = reinterpret_cast<BeginFrameFn>(Studio->dwHookMethod(reinterpret_cast<DWORD>(Hooks::BeginFrame), 9));
	oDrawModelExecute = reinterpret_cast<DrawModelExecuteFn>(ModelRender->dwHookMethod(reinterpret_cast<DWORD>(Hooks::DrawModelExecute), 21));

	EventListener.Init();
}

bool Utils::WorldToScreen(const Vector &origin, Vector &screen)
{
	const auto screenTransform = [&origin, &screen]() -> bool
	{
		static std::uintptr_t pViewMatrix;
		if (!pViewMatrix)
		{
			pViewMatrix = static_cast<std::uintptr_t>(g_Utils.FindPatternIDA("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			pViewMatrix += 3;
			pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
			pViewMatrix += 176;
		}

		const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
		screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];
		screen.z = 0.0f;

		float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

		if (w < 0.001f)
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if (!screenTransform())
	{
		int iScreenWidth, iScreenHeight;
		I::Engine->GetScreenSize(iScreenWidth, iScreenHeight);

		screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
		screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

		return true;
	}
	return false;
}

DWORD Utils::FindPatternIDA(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo;
	K32GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}

	return NULL;
}

unsigned int Utils::DataInMap(datamap_t* map, const char* name)//gladiator cheats
{
	while (map)
	{
		for (int i = 0; i < map->data_num_fields; i++)
		{
			if (map->data_desc[i].name == NULL)
				continue;

			if (strcmp(name, map->data_desc[i].name) == 0)
				return map->data_desc[i].offset[0];

			if (map->data_desc[i].type == 10)
			{
				if (map->data_desc[i].td)
				{
					unsigned int offset;

					if ((offset = DataInMap(map->data_desc[i].td, name)) != 0)
						return offset;
				}
			}
		}
		map = map->base_map;
	}
	return 0;
}