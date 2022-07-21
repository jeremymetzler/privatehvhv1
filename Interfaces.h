#pragma once

#include "CHandle.h"
#include "IClientNetworkable.h"
#include "IBaseClientDLL.h"
#include "IVEngineClient.h"
#include "IEntity.h"
#include "ISurface.h"
#include "IPanel.h"
#include "ITraceRay.h"
#include "IPlayerMovement.h"
#include "IClientState.h"
#include "IStudioRender.h"
#include "CDebugOverlay.h"
#include "ICvar.h"
#include "IVModelRender.h"
#include "IPhysicsSurfaceProps.h"
#include "IGameEvenManager.h"

namespace I {
	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IClientEntityList* EntityList;
	extern IGlobalVars* Globals;
	extern IClientMode* ClientMode;
	extern ISurface* Surface;
	extern IPanel* Panel;
	extern ITraceRay* Trace;
	extern IPlayerMoveHelper* MoveHelper;
	extern IPlayerPrediction* Prediction;
	extern IPlayerGameMovement* GameMovement;
	extern IClientState* ClientState;
	extern IStudioRender* StudioRender;
	extern CDebugOverlay* DebugOverlay;
	extern ICvar* Cvar;
	extern IVModelRender* ModelRender;
	extern IVModelInfo* ModelInfo;
	extern IPhysicsSurfaceProps* SurfaceProps;
	extern IGameEventManager2* GameEventManager;
}

typedef void* (*create_interface_fn)(const char* name, int* return_code);

class Interfaces {
public:
	template <typename t = void*>
	t* GetInterface(const char* dll, const char* name, bool ver = false) {
		if (!dll || !name)
			return nullptr;

		void* pinterface = nullptr;
		static create_interface_fn create_interfaces = nullptr;
		while (!create_interfaces)
			create_interfaces = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA(dll), "CreateInterface"));

		if (ver)
		{
			pinterface = create_interfaces(name, 0);
		}
		else
		{
			char char_buffer[256];
			for (int i = 0; i < 100; i++)
			{
				sprintf_s(char_buffer, "%s%0.3d", name, i);
				pinterface = create_interfaces(char_buffer, 0);

				if (pinterface)
				{
					break;
				}
			}
		}

		if (!pinterface)
		{
			pinterface = new t*;
		}

		return reinterpret_cast<t*>(pinterface);
	}

	void Init();

private:
}; extern Interfaces g_Interfaces;