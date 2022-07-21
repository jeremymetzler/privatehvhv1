#include "Main.h"

BeginFrameFn oBeginFrame;
void __fastcall Hooks::BeginFrame(void* ecx, void* edx)
{
	oBeginFrame(ecx, edx);

	if (g_Globals.SyncFrame)
	{
		g_Visuals.DrawHitboxes();
	}	
	g_Globals.SyncFrame = false;
}