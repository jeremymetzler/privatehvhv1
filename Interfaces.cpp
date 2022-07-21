#include "Main.h"

IBaseClientDLL* I::Client = nullptr;
IVEngineClient* I::Engine = nullptr;
IClientEntityList* I::EntityList = nullptr;
IGlobalVars* I::Globals = nullptr;
IClientMode* I::ClientMode = nullptr;
ISurface* I::Surface = nullptr;
IPanel* I::Panel = nullptr;
ITraceRay* I::Trace = nullptr;
IPlayerMoveHelper* I::MoveHelper = nullptr;
IPlayerPrediction* I::Prediction = nullptr;
IPlayerGameMovement* I::GameMovement = nullptr;
IClientState* I::ClientState = nullptr;
IStudioRender* I::StudioRender = nullptr;
CDebugOverlay* I::DebugOverlay = nullptr;
ICvar* I::Cvar = nullptr;
IVModelRender* I::ModelRender = nullptr;
IVModelInfo* I::ModelInfo = nullptr;
IPhysicsSurfaceProps* I::SurfaceProps = nullptr;
IGameEventManager2* I::GameEventManager = nullptr;

void Interfaces::Init()
{
	I::Client = GetInterface<IBaseClientDLL>("client.dll", "VClient");
	I::Engine = GetInterface<IVEngineClient>("engine.dll", "VEngineClient");
	I::EntityList = GetInterface<IClientEntityList>("client.dll", "VClientEntityList");
	I::Surface = GetInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface");
	I::Panel = GetInterface<IPanel>("vgui2.dll", "VGUI_Panel");
	I::Trace = GetInterface<ITraceRay>("engine.dll", "EngineTraceClient");
	I::Prediction = GetInterface<IPlayerPrediction>("client.dll", "VClientPrediction");
	I::GameMovement = GetInterface<IPlayerGameMovement>("client.dll", "GameMovement");
	I::StudioRender = GetInterface<IStudioRender>("studiorender.dll", "VStudioRender");
	I::DebugOverlay = GetInterface<CDebugOverlay>("engine.dll", "VDebugOverlay");
	I::Cvar = GetInterface<ICvar>("vstdlib.dll", "VEngineCvar");
	I::ModelRender = GetInterface<IVModelRender>("engine.dll", "VEngineModel");
	I::ModelInfo = GetInterface<IVModelInfo>("engine.dll", "VModelInfoClient");
	I::SurfaceProps = GetInterface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
	I::GameEventManager = GetInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002", true);

	I::ClientState = **reinterpret_cast<IClientState***>(g_Utils.FindPatternIDA("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	I::ClientMode = **reinterpret_cast<IClientMode***> ((*reinterpret_cast<uintptr_t**>(I::Client))[10] + 0x5u);
	I::Globals = **reinterpret_cast<IGlobalVars***>((*reinterpret_cast<uintptr_t**>(I::Client))[0] + 0x1Bu);
	I::MoveHelper = **reinterpret_cast<IPlayerMoveHelper***>(g_Utils.FindPatternIDA("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
}