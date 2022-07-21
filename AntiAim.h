#pragma once

class AntiAim
{
public:
	bool HitInit = false;
	bool CanHit = false;
	bool HitCheck = false;
	bool LagPeek = false;
	bool DoScope = false;
	int LocalTicksToStop = 0;
	Vector FakeLocalHead;
	int TicksToStop[64];

	void PreRun(CUserCmd* cmd);
	void Prediction(CUserCmd* cmd, EntInfo& lst, C_BaseCombatWeapon* pWeapon);
	void Run(CUserCmd* cmd);
	void FakeLag(CUserCmd* cmd);
};

extern AntiAim g_AntiAim;