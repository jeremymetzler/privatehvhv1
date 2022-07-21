#pragma once

using CreateMoveFn = bool(__thiscall*)(void*, float, CUserCmd*);
extern CreateMoveFn oCreateMove;

using LockCursorFn = void(__thiscall*)(void*);
extern LockCursorFn oLockCursor;

using PaintTraverseFn = void(__fastcall*)(void*, void*, unsigned int, bool, bool);
extern PaintTraverseFn oPaintTraverse;

using FrameStageNotifyFn = void(__fastcall*)(void*, void*, int istage);
extern FrameStageNotifyFn oFrameStageNotify;
enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

using BeginFrameFn = void(__fastcall*)(void*, void*);
extern BeginFrameFn oBeginFrame;

using DrawModelExecuteFn = void(__fastcall*)(void*, void*, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
extern DrawModelExecuteFn oDrawModelExecute;

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
extern OverrideViewFn oOverrideView;

class C_Event : public IGameEventListener2
{
public:
	void FireGameEvent(IGameEvent* pevent);
	int  EventDebugId = 42;
	bool Init()
	{
		if (!I::GameEventManager->AddListener(this, "player_hurt", false)
			|| !I::GameEventManager->AddListener(this, "weapon_fire", false)
			|| !I::GameEventManager->AddListener(this, "bullet_impact", false))
			return false;
		else
			return true;
	}

};

extern C_Event EventListener;

namespace Hooks {
	extern bool __stdcall CreateMove( float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall LockCursor();
	extern void __fastcall PaintTraverse(void*, void*, unsigned int iPanel, bool bForceRepaint, bool bAllowForce);
	extern void __fastcall FramStageNotify(void*, void*, int iStage);
	extern void __fastcall BeginFrame(void*, void*);
	extern void __fastcall DrawModelExecute(void*, void*, i_mat_render_context* context, const draw_model_state_t& state, const model_render_info_t& render_info, matrix3x4_t* matrix);
	extern void __fastcall OverrideView(void*, void*, CViewSetup* view);
	extern void __fastcall CLMove(void*, void*, char idk);

	extern WNDPROC oldWindowProc;
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}