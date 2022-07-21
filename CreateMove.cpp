#include "Main.h"

CreateMoveFn oCreateMove;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* pCmd)
{
	auto bCreateMoveReturn = oCreateMove(I::ClientMode, flInputSampleTime, pCmd); // Cause that`s fix a lot of bugs w/ this hook

	if (!pCmd || !pCmd->command_number)
		return bCreateMoveReturn;

	if (!g_Globals.SendPacket)
	{
		uintptr_t* frame_pointer;
		__asm mov frame_pointer, ebp;
		g_Globals.SendPacket = reinterpret_cast<bool*>(*frame_pointer - 0x1C);
		*g_Globals.SendPacket = true;
	}
	else
	{
		*g_Globals.SendPacket = true;
	}

	static convar* sv_cheats = I::Cvar->FindVar("sv_cheats");
	if (!sv_cheats->ReadInt())
		sv_cheats->SetInt(1);

	g_Aimbot.BulletHandle();

	if (g_Globals.GameReady && g_Ent.List.size() > 0) // this is where entity lag records are made
	{
		for (EntInfo& lst : g_Ent.List)
		{
			if (!lst.ent || !lst.ent->IsValid())
				continue;

			if (lst.Records.size() != 0)
			{
				if (/*(lst.NewData.Shot ? true : (lst.ent->GetEyeAng().x == lst.NewData.vViewAng.x)) && */g_Ent.ValidTick(lst.NewData.flSimtime))
				{

					if (lst.Records.back().flSimtime != lst.NewData.flSimtime)
					{
						lst.RData.FakeLbyTime = lst.NewData.flSimtime;

						if (lst.RData.LbyTimer != -1.f && lst.RData.LbyTimer < lst.RData.FakeLbyTime)
						{
							if (lst.RData.LbyTimer <= lst.RData.FakeLbyTime - I::Globals->intervalPerTick)
							{
								lst.RData.LbyTimer = -1.f;
							}
							else
							{
								lst.RData.LastLby += 999.f;
								lst.NewData.flSimtime = lst.RData.FakeLbyTime;
							}
						}
						lst.Records.push_back(PRecord(lst));
					}
					else
					{
						if (lst.RData.LbyTimer != -1.f && lst.RData.LbyTimer < lst.RData.FakeLbyTime)
						{
							if (lst.RData.LbyTimer <= lst.RData.FakeLbyTime - I::Globals->intervalPerTick)
							{
								lst.RData.LbyTimer = -1.f;
							}
							else
							{
								lst.RData.LastLby += 999.f;
								lst.NewData.flSimtime = lst.RData.FakeLbyTime;
								lst.Records.push_back(PRecord(lst));
							}
						}
					}
				}
				else
					g_Ent.SetInfo(lst.ent, lst.Records.back());

				if (!g_Ent.ValidTick(lst.Records.front().flSimtime))
				{
					lst.Records.front().flPosParams = std::array<float, 24>();
					lst.Records.pop_front();
				}

			}
			else if (lst.Records.size() == 0 && lst.NewData.flSimtime != -1.f && g_Ent.ValidTick(lst.NewData.flSimtime)  /*(lst.NewData.Shot ? true : (lst.ent->GetEyeAng().x == lst.NewData.vViewAng.x)) && */)
			{

				if (lst.RData.LbyTimer != -1.f && lst.RData.LbyTimer < lst.NewData.flSimtime)
				{
					if (lst.RData.LbyTimer <= lst.NewData.flSimtime - I::Globals->intervalPerTick)
					{
						lst.RData.LbyTimer = -1.f;
					}
					else
					{
						lst.RData.LastLby += 999.f;
					}
				}
				lst.Records.push_back(PRecord(lst));
			}

			if (lst.RData.LbyTimer != -1.f && fabs(lst.RData.LbyTimer - lst.RData.FakeLbyTime) > 1.2f )
			{
				lst.RData.LbyTimer = -1.f;
			}

			lst.RData.FakeLbyTime += I::Globals->intervalPerTick;
		}		
	}

	g_Globals.MoveAng = pCmd->viewangles;
	g_Globals.ViewAng = pCmd->viewangles;
	g_Globals.AntiAng = pCmd->viewangles;
	if (g_Globals.GameReady)
	{
		if (!g_Globals.LocalPlayer)
		{
			g_Globals.LocalPlayer = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

			if (g_Globals.LocalPlayer && g_Globals.LocalPlayer->IsAlive())
			{
				model_t* model = g_Globals.LocalPlayer->GetModel();

				if (!model)
					return;

				studio_hdr_t* hdr = I::ModelInfo->GetStudioModel(model);

				if (!hdr)
					return;

				studio_hitbox_set_t* set = hdr->hitbox_set(0);

				if (!set)
					return;

				for (int i = 0; i < hitbox_max; i++)
				{
					studio_box_t* hitbox = set->hitbox(i);

					if (!hitbox)
						continue;

					if (hitbox->bone >= 128 || hitbox->bone < 0)
						continue;

					g_Ent.LHBIndxToBone[i] = hitbox->bone;
				}
			}		
		}
		else 
			g_Globals.LocalPlayer = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	}
	else
		g_Globals.LocalPlayer = nullptr;

	if (g_Globals.LocalPlayer && g_Globals.LocalPlayer->IsAlive() && g_Globals.LocalPlayer->GetActiveWeapon() && g_Globals.LocalPlayer->GetActiveWeapon()->GetWeaponData())
	{

		float LeashDist = (g_Globals.LocalLeash - g_Globals.LocalPlayer->GetOrigin()).Length();
		Vector LeashAng = g_Math.CalcAng(g_Globals.LocalLeash, g_Globals.LocalPlayer->GetOrigin());
		Vector Fwd;
		if (LeashDist > 30.f)
		{
			g_Math.AngVec(LeashAng, &Fwd);
			Fwd.NormalizeInPlace();
			g_Globals.LocalLeash += Fwd * (LeashDist - 30.f);
		}

		g_Globals.LocalTeam = g_Globals.LocalPlayer->GetTeam();

		g_Misc.Bhop(pCmd);

		int OgButtons = pCmd->buttons;
		if (g_Misc.EngPrediction(pCmd))
		{
			pCmd->buttons = OgButtons;

			g_AntiAim.PreRun(pCmd);

			int Code = g_Aimbot.Run(pCmd);

			if (g_Vars.Cfg.antiaim && (Code & (4 | 2) || Code == 0) && !(pCmd->buttons & (in_attack | in_use)))
			{
				if (Code & 4)
					g_AntiAim.FakeLag(pCmd);
				g_AntiAim.Run(pCmd);
				pCmd->viewangles.x = 89.f;
				g_Globals.AntiAng.x = pCmd->viewangles.x;
			}

			if (!(Code & 2))
			{
				pCmd->tick_count = g_Globals.LocalPlayer->GetTickBase();
				I::Globals->tickcount = pCmd->tick_count;
				g_Ent.LocalUpdate(g_Globals.LocalPlayer, g_Globals.AntiAng, *g_Globals.SendPacket);
			}		
		}

		g_Misc.CorrectMovement(g_Globals.MoveAng, pCmd, pCmd->forwardmove, pCmd->sidemove);

		pCmd->buttons &= ~in_moveright;
		pCmd->buttons &= ~in_moveleft;
		pCmd->buttons &= ~in_forward;
		pCmd->buttons &= ~in_back;

		if (pCmd->forwardmove > 0.f)
			pCmd->buttons |= in_forward;
		else if (pCmd->forwardmove < 0.f)
			pCmd->buttons |= in_back;
		if (pCmd->sidemove > 0.f)
			pCmd->buttons |= in_moveright;
		else if (pCmd->sidemove < 0.f)
			pCmd->buttons |= in_moveleft;

		pCmd->viewangles.NormalizeAngle();
		pCmd->viewangles.x = std::clamp(pCmd->viewangles.x, -89.0f, 89.0f);
		pCmd->viewangles.y = std::clamp(pCmd->viewangles.y, -180.0f, 180.0f);
		pCmd->viewangles.z = 0.0f;
	}
	else if (g_Misc.MoveData)
	{
		delete[] g_Misc.MoveData;
		g_Misc.MoveData = nullptr;

		I::Engine->ExecuteClientCmd("play bot\\aw_hell");
	}
	else
	{
		g_Globals.LocalRealTime = I::Globals->curtime;
	}

	if (g_Globals.SendPacket && *g_Globals.SendPacket)
		g_Globals.ChokeAmnt = 0;
	else
		g_Globals.ChokeAmnt += 1;

	g_Globals.SyncFrame = true;
	return false;
}
