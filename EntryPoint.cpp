#include "Main.h"

// I`ll update this base when i have free time.

void Initialize() {

	while (!(g_Globals.CSGOWindow = FindWindowA("Valve001", NULL)))
		Sleep(200);

	g_Interfaces.Init();
	g_pNetvars.Init();
	g_Vars.Init();

	Hooks::oldWindowProc = (WNDPROC)SetWindowLongPtr(g_Globals.CSGOWindow, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc);
	g_Utils.Init();
}

bool __stdcall DllMain(void* dll_instance, unsigned long reason_to_call, void* reserved) {
	switch (reason_to_call) {
	case DLL_PROCESS_ATTACH: // if DLL attached to CSGO
		DisableThreadLibraryCalls((HMODULE)dll_instance);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Initialize, dll_instance, 0, 0);
		break;
	}

	return true;
}