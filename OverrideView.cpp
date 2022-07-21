#include "Main.h"

OverrideViewFn oOverrideView;


void __fastcall Hooks::OverrideView(void* ecx, void* edx, CViewSetup* view)
{
	oOverrideView(ecx, edx, view);
	if (!g_Globals.GameReady || !g_Globals.LocalPlayer)
		return;

	static bool InitTp = false;

	if (g_Globals.LocalPlayer->IsAlive())
	{
		if (!InitTp && g_Vars.Cfg.thirdperson)
		{
			I::Engine->ExecuteClientCmd("thirdperson");
			InitTp = true;
		}
		else if (InitTp && !g_Vars.Cfg.thirdperson)
		{
			I::Engine->ExecuteClientCmd("firstperson");
			InitTp = false;
		}

		if (g_Vars.Cfg.novisrecoil && !InitTp)
		{
			static convar* weapon_recoil_scale = I::Cvar->FindVar("weapon_recoil_scale");
			view->angles -= (g_Globals.LocalPlayer->GetViewPunch() + (g_Globals.LocalPlayer->GetRecoilOff() * weapon_recoil_scale->ReadFloat() * 0.4499999f));
		}

		if (g_Vars.Cfg.fov != 0.f && (g_Vars.Cfg.nozoom ? true : !g_Globals.LocalPlayer->IsScoped()))
			view->fov = g_Vars.Cfg.fov;

		if (InitTp)
		{
			Vector ViewAng, ViewFwd;
			I::Engine->GetViewAngles(ViewAng);

			view->angles = ViewAng;

			ViewAng.x = -ViewAng.x;
			ViewAng.y = g_Math.NormEuler(ViewAng.y + 180);

			g_Math.AngVec(ViewAng, &ViewFwd);

			Vector origin = g_Globals.LocalPlayer->GetAbsOrigin() + g_Globals.LocalPlayer->GetViewOff();

			CGameTrace trace;
			trace_world_only filter;
			ray_t ray;
			ray.Init(origin, (origin + (ViewFwd * g_Vars.Cfg.tpdistance)));
			I::Trace->TraceRay(ray, CS_MASK_SHOOT, &filter, &trace);

			ViewFwd = origin + (ViewFwd * (trace.fraction * (g_Vars.Cfg.tpdistance - 15)));

			view->origin = ViewFwd;
		}
	}
	else
	{
		if (InitTp)
		{
			I::Engine->ExecuteClientCmd("firstperson");
			InitTp = false;
		}
	}

}