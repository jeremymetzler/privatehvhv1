#include "Main.h"

Misc g_Misc = Misc();

void Misc::CircleStrafe(CUserCmd* cmd)
{
	if (!g_Globals.LocalPlayer || !g_Globals.LocalPlayer->IsAlive())
		return;

	static float AngRecord[15];
	float AngStepAvf = 0.f;
	static float CurrAng = 0.f;
	for (int i = 1; i < 15; i++)
		AngStepAvf = AngRecord[i];
	AngStepAvf /= 14.f;

	CurrAng = AngStepAvf;

	Vector Ang;
	Vector Forward;

/*	for (int i = 1; i < 15; i++)
	{

		g_Math.AngVec(Ang, &Forward);

		ray_t ray();
		if (I::Trace->TraceRay())
		{

		}
		AngRecord[i] = AngRecord[i - 1];
	}*/
}

void Misc::Bhop(CUserCmd* cmd)
{
	if (!g_Vars.Cfg.bhop)
		return;

	static bool SideSwch = false;
	const float TickScale = (0.015625f / I::Globals->intervalPerTick);

	if (cmd->buttons & in_jump)
	{
		if (g_Globals.LocalPlayer->GetFlags() & FL_ONGROUND)
		{
			cmd->sidemove = 0;
			cmd->forwardmove = 0;
		}
		else
		{
			cmd->buttons &= ~in_jump;
			cmd->forwardmove = 0;

			float VelAng = g_Math.NormEuler(g_Math.CalcAng(Vector().Zero(), g_Globals.LocalPlayer->GetVelocity()).y);

			int SizeForAvg = 0;
			float LastYaw = 0;
			bool Flip = false;

			if (cmd->buttons & in_forward)
			{
				SizeForAvg += 1;
				LastYaw = 0;
			}
			if (cmd->buttons & in_back)
			{
				SizeForAvg += 1;
				LastYaw += 180;
				Flip = true;
			}
			if (cmd->buttons & in_moveright)
			{
				SizeForAvg += 1;
				if (!Flip)
					LastYaw -= 90;
				else
					LastYaw += 270;
			}
			if (cmd->buttons & in_moveleft)
			{
				SizeForAvg += 1;
				LastYaw += 90;
			}

			if (SizeForAvg != 0)
			{
				float AvgYaw = g_Math.NormEuler(LastYaw / SizeForAvg);
				g_Globals.MoveAng.y = g_Math.NormEuler(g_Globals.MoveAng.y + AvgYaw);
			}

			if (g_Math.NormEuler(g_Globals.MoveAng.y - VelAng) > g_Vars.Cfg.strafeang)
			{
				g_Globals.MoveAng.y = g_Math.NormEuler(VelAng + 1.0f * TickScale);
				cmd->sidemove = -450;
				SideSwch = true;
			}
			else if (g_Math.NormEuler(g_Globals.MoveAng.y - VelAng) < -g_Vars.Cfg.strafeang)
			{
				g_Globals.MoveAng.y = g_Math.NormEuler(VelAng - 1.0f * TickScale);
				cmd->sidemove = 450;
				SideSwch = false;
			}
			else
			{
				if (SideSwch)
				{
					g_Globals.MoveAng.y = g_Math.NormEuler(VelAng + .911f * TickScale);
					cmd->sidemove = -450;
				}
				else
				{
					cmd->sidemove = 450;
					g_Globals.MoveAng.y = g_Math.NormEuler(VelAng - .911f * TickScale);
				}
			}
		}
	}
}

void Misc::CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove) {
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);
}

using PhysicsRunThinkFn = char(__thiscall*)(C_BaseEntity*, int);

bool Misc::EngPrediction(CUserCmd* cmd)//gud enuf
{

	if (!I::GameMovement || !I::MoveHelper || !I::Prediction)
	{
		if (MoveData)
		{
			delete[] MoveData;
			MoveData = nullptr;
		}
		return false;
	}


	if (!MoveData)
		MoveData = new CMoveData();

	static auto PredRandSeed = *reinterpret_cast<int**>(g_Utils.FindPatternIDA("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
	static auto PredPlayer = *reinterpret_cast<C_BaseEntity**>(g_Utils.FindPatternIDA("client.dll", "89 3D ? ? ? ? F3 0F 10 47") + 2);

//	static float m_flMovementTimeForUserCmdProcessingRemaining = 0;
	
/*	static convar* sv_maxusrcmdprocessticks = I::Cvar->FindVar("sv_maxusrcmdprocessticks");
	static convar* sv_maxusrcmdprocessticks_warning = I::Cvar->FindVar("sv_maxusrcmdprocessticks_warning");

	bool bRunNullCmd = false;
	if (int numUsrCmdProcessTicksMax = sv_maxusrcmdprocessticks->ReadInt())
	{
		// Grant the client some time buffer to execute user commands
		m_flMovementTimeForUserCmdProcessingRemaining += I::Globals->intervalPerTick;

		// but never accumulate more than N ticks
		if (m_flMovementTimeForUserCmdProcessingRemaining > numUsrCmdProcessTicksMax * I::Globals->intervalPerTick)
		{
			m_flMovementTimeForUserCmdProcessingRemaining = numUsrCmdProcessTicksMax * I::Globals->intervalPerTick;
			bRunNullCmd = true;
		}
	}

	auto ConsumeMovementTimeForUserCmdProcessing = [&](float flTimeNeeded)
	{
		if (m_flMovementTimeForUserCmdProcessingRemaining <= 0.0f)
		{
			return 0.0f;
		}
		else if (flTimeNeeded > m_flMovementTimeForUserCmdProcessingRemaining + FLT_EPSILON)
		{
			float flResult = m_flMovementTimeForUserCmdProcessingRemaining;
			m_flMovementTimeForUserCmdProcessingRemaining = 0.0f;
			return flResult;
		}
		else
		{
			m_flMovementTimeForUserCmdProcessingRemaining -= flTimeNeeded;
			if (m_flMovementTimeForUserCmdProcessingRemaining < 0.0f)
				m_flMovementTimeForUserCmdProcessingRemaining = 0.0f;
			return flTimeNeeded;
		}
	};

	float flTimeAllowedForProcessing = ConsumeMovementTimeForUserCmdProcessing(I::Globals->intervalPerTick);

	if (flTimeAllowedForProcessing < I::Globals->intervalPerTick)
	{
		return false; // Don't process this command
	}
*/

	cmd->random_seed = rand() % 0x7fffffff;
	*PredRandSeed = cmd->random_seed;
//	g_Globals.LocalPlayer->GetSomeCmd() = *cmd; crashes everything idk
	*PredPlayer = *g_Globals.LocalPlayer;

	float OgCurtime = I::Globals->curtime;
	float OgFrameTime = I::Globals->frametime;

	const float PlayerCurTime = static_cast<float>(g_Globals.LocalPlayer->GetTickBase()) * I::Globals->intervalPerTick;

	I::Prediction->Update(I::ClientState->m_nDeltaTick, I::ClientState->m_nDeltaTick > 0, I::ClientState->last_command_ack, I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands);

	I::MoveHelper->SetHost(g_Globals.LocalPlayer);

	I::GameMovement->StartTrackPredictionErrors(g_Globals.LocalPlayer);

	MoveData->m_vecOldAngles = g_Globals.LocalPlayer->GetEyeAng();

	I::Globals->curtime = PlayerCurTime;
	I::Globals->frametime = I::Globals->intervalPerTick;

	static PhysicsRunThinkFn PhysicsRunThink = nullptr;
	if (!PhysicsRunThink)
	{
		PhysicsRunThink = reinterpret_cast<PhysicsRunThinkFn>(g_Utils.FindPatternIDA("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ?"));
	}
	else if (PhysicsRunThink(g_Globals.LocalPlayer, 0))
	{
		g_Globals.LocalPlayer->PreThink();
	}

	const auto NextThink = reinterpret_cast<int*>(reinterpret_cast <uint32_t>(g_Globals.LocalPlayer) + 0xF8);
	if (*NextThink > 0 && *NextThink <= g_Globals.LocalPlayer->GetTickBase()) 
	{
		*NextThink = -1;
		g_Globals.LocalPlayer->Think();
	}

	I::Prediction->SetupMove(g_Globals.LocalPlayer, cmd, I::MoveHelper, MoveData);

	I::GameMovement->ProcessMovement(g_Globals.LocalPlayer, MoveData);

	I::Prediction->FinishMove(g_Globals.LocalPlayer, cmd, MoveData);

/*	I::Globals->curtime = OgCurtime;
	I::Globals->frametime = OgFrameTime;

	//process impacts

	I::Globals->curtime = PlayerCurTime;
	I::Globals->frametime = I::Globals->intervalPerTick;*/


	g_Globals.LocalPlayer->PostThink(); // updateaccpen done here

//	OldCmd.hasbeenpredicted = true;

//	*cmd = OldCmd;
//	g_Globals.LocalPlayer->GetSomeCmd() = *cmd;

	I::GameMovement->FinishTrackPredictionErrors(g_Globals.LocalPlayer);

	if (PredRandSeed)
		*PredRandSeed = -1;

	static int OldTickBase = 0;

	if (OldTickBase == g_Globals.LocalPlayer->GetTickBase())
	{
		g_Globals.LocalPlayer->GetTickBase() += 1;
	}

	OldTickBase = g_Globals.LocalPlayer->GetTickBase();

	g_Globals.LocalRealTime = static_cast<float>(OldTickBase) * I::Globals->intervalPerTick;

	I::GameMovement->Reset();

	I::Globals->curtime = OgCurtime;
	I::Globals->frametime = OgFrameTime;

	I::MoveHelper->SetHost(nullptr);

	return true;
}

float Misc::LerpTime()
{
	if (!I::Cvar)
		return 0.0f;
	static convar* cl_updaterate = I::Cvar->FindVar("cl_updaterate");
	int iUpdateRate = cl_updaterate->ReadInt();
	static convar* pMinUpdateRate = I::Cvar->FindVar("sv_minupdaterate");
	static convar* pMaxUpdateRate = I::Cvar->FindVar("sv_maxupdaterate");

	if (pMinUpdateRate && pMaxUpdateRate)
	{
		iUpdateRate = std::clamp(iUpdateRate, pMinUpdateRate->ReadInt(), pMaxUpdateRate->ReadInt());
	}
	static convar* cl_interp_ratio = I::Cvar->FindVar("cl_interp_ratio");
	float flLerpRatio = cl_interp_ratio->ReadFloat();
	if (flLerpRatio == 0)
	{
		flLerpRatio = 1.0f;
	}
	static convar* cl_interp = I::Cvar->FindVar("cl_interp");
	float flLerpAmount = cl_interp->ReadFloat();

	static convar* pMin = I::Cvar->FindVar("sv_client_min_interp_ratio");
	static convar* pMax = I::Cvar->FindVar("sv_client_max_interp_ratio");
	if (pMin && pMax && pMin->ReadFloat() != -1)
	{
		flLerpRatio = std::clamp(flLerpRatio, pMin->ReadFloat(), pMax->ReadFloat());
	}
	else
	{
		if (flLerpRatio == 0)
		{
			flLerpRatio = 1.0f;
		}
	}

	return max(flLerpAmount, flLerpRatio / iUpdateRate);
}

void Misc::AutoStop(CUserCmd* cmd)
{
	if (!(g_Globals.LocalPlayer->GetFlags() & FL_ONGROUND) || cmd->buttons & in_jump)
	{
		return;
	}
	Vector VelDir = g_Math.CalcAng(Vector().Zero(), g_Globals.LocalPlayer->GetVelocity());
	float Velocity = g_Globals.LocalPlayer->GetVelocity().Length2D();
	cmd->sidemove = 0.f;
	if (Velocity < 20.f)
		cmd->forwardmove = 0.f;
	else
		cmd->forwardmove = 450.f;
	g_Globals.MoveAng.y = g_Math.NormEuler(VelDir.y + 180.f);
}