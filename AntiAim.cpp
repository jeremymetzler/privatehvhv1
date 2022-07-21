#include "Main.h"

AntiAim g_AntiAim = AntiAim();

void AntiAim::PreRun(CUserCmd* cmd)
{
    static convar* sv_maxspeed = I::Cvar->FindVar("sv_maxspeed");
    static Vector transform;
    const float max_speed = sv_maxspeed->ReadFloat();
    const float acceleration = 5.5f;
    const float max_accelspeed = acceleration * max_speed * I::Globals->intervalPerTick;

    FakeLocalHead = (g_Globals.LocalPlayer->GetOrigin() + Vector(0.f, 0.f, 70.f) + (g_Globals.LocalPlayer->GetVelocity() * g_Misc.TICKS_TO_TIME(14)));
}

void AntiAim::Prediction(CUserCmd* cmd, EntInfo& lst, C_BaseCombatWeapon* pWeapon)
{
    Vector FakeHeadOrigin = ((lst.NewData.vOrigin + Vector(0.f, 0.f, 75.f)) - (g_Ent.LocalRecord.vVelocity / 2.f));
    if (g_Vars.Cfg.lagpeek && g_Autowall.HitFloatingPoint(pWeapon, FakeLocalHead, FakeHeadOrigin))
    {
        if (!HitInit && fabs(g_Ent.LocalRecord.vVelocity.Length2D()) > 1.0f)
        {
            CanHit = true;
            HitInit = true;
        }
        HitCheck = true;
    }
}

void AntiAim::Run(CUserCmd* cmd)
{
	/*	int SizeForAvg = 0;
	float LastYaw = 0;
	bool Flip = false;
	static const char flags[] = { (1), (1 << 1), (1 << 2), (1 << 3) };

	if (g_Globals.hit_side & flags[0])
	{
		SizeForAvg += 1;
		LastYaw = 0;
	}
	if (g_Globals.hit_side & flags[2])
	{
		SizeForAvg += 1;
		LastYaw += 180;
		Flip = true;
	}
	if (g_Globals.hit_side & flags[1])
	{
		SizeForAvg += 1;
		if (!Flip)
			LastYaw -= 90;
		else
			LastYaw += 270;
	}
	if (g_Globals.hit_side & flags[3])
	{
		SizeForAvg += 1;
		LastYaw += 90;
	}

	if (LastYaw != 0)
		LastYaw = g_Math.NormEuler(LastYaw / SizeForAvg);
	LastYaw = g_Math.NormEuler(LastYaw + 180.f + cmd->viewangles.y);

	g_Globals.AntiAng.y = LastYaw;

	if (*g_Globals.SendPacket)
		cmd->viewangles.y = g_Math.NormEuler(LastYaw + 180.f);
	else
		cmd->viewangles.y = LastYaw;*/

	float LastYaw = 0;
	/*
		static const char flags[] = { (1), (1 << 1), (1 << 2), (1 << 3) };

		if (!(g_Globals.hit_side & flags[2]))
		{
			LastYaw = g_Math.NormEuler(180.f + cmd->viewangles.y);
		}
		else if (!(g_Globals.hit_side & flags[1]))
		{
			LastYaw = g_Math.NormEuler(cmd->viewangles.y - 90.f);
		}
		else if (!(g_Globals.hit_side & flags[3]))
		{
			LastYaw = g_Math.NormEuler(cmd->viewangles.y + 90.f);
		}
		else if (!(g_Globals.hit_side & flags[0]))
		{
			LastYaw = cmd->viewangles.y;
		}
		else
		{
			LastYaw = g_Math.NormEuler(180.f + cmd->viewangles.y);
		}*/

	if (g_Vars.Cfg.flipaa)
		LastYaw = g_Math.NormEuler(cmd->viewangles.y + 90.f);
	else
		LastYaw = g_Math.NormEuler(cmd->viewangles.y - 90.f);

	g_Globals.AntiAng.y = LastYaw;

	if (*g_Globals.SendPacket && g_Globals.ChokeAmnt > 0)
		cmd->viewangles.y = g_Math.NormEuler(LastYaw + 180.f);
	else
		cmd->viewangles.y = LastYaw;
}

void AntiAim::FakeLag(CUserCmd* cmd)
{
    if (!g_Globals.SendPacket || static_cast<int>(g_Vars.Cfg.fakelag) == 0)
        return;

    bool skip_tick = false;

    if (!HitCheck)
        HitInit = false;

    if (CanHit)
    {
        *g_Globals.SendPacket = true;
        skip_tick = true;
        LagPeek = true;
        CanHit = false;
    }

    if (!LagPeek && !skip_tick)
    {
        *g_Globals.SendPacket = (g_Globals.ChokeAmnt >= static_cast<int>(g_Vars.Cfg.fakelag));
    }

    if (LagPeek && !skip_tick)
    {
        if (g_Globals.ChokeAmnt < 14)
            *g_Globals.SendPacket = false;
        else
        {
            *g_Globals.SendPacket = true;
            LagPeek = false;
        }
    }

    HitCheck = false;
}