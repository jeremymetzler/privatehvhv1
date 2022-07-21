#pragma once

struct PointInfo
{
	float HighestDmg = 0.f;
	Vector FinalAng = Vector(0, 0, 0);
	Vector FinalPos = Vector(0, 0, 0);
	float FinalSimtime = 0.f;

	Vector EyePos = Vector(0,0,0);
	PRecord BesRec = PRecord();
	EntInfo* BesEnt = nullptr;

	bool MainRec = false;

	Vector TargPos = Vector(0, 0, 0);

	inline void reset()
	{
		HighestDmg = 0.f;
		FinalAng = Vector(0, 0, 0);
		FinalPos = Vector(0, 0, 0);
		FinalSimtime = 0.f;

		EyePos = Vector(0, 0, 0);
		BesRec = PRecord();
		BesEnt = nullptr;

		MainRec = false;
	}
};

struct Bullet_t
{
	int TargIndex = -1;
	int RIndex = -1;
	float FireRate = 0.f;
	float Latency = 0.f;
	std::string RMode = "";
	float PredDecay = 0.f;
	Vector LocalOrigin = Vector(0, 0, 0);

	PRecord TargRec = {};
};

class Aimbot
{
public:
	float DirFwdAmnt[hitbox_max][6];
	Aimbot() 
	{
		DirFwdAmnt[hitbox_head][0] = 4.55f;
		DirFwdAmnt[hitbox_head][1] = 4.00f;
		DirFwdAmnt[hitbox_head][2] = 4.00f;
		DirFwdAmnt[hitbox_head][3] = 2.88f;
		DirFwdAmnt[hitbox_head][4] = 5.00f;
		DirFwdAmnt[hitbox_head][5] = 3.30f;

		DirFwdAmnt[hitbox_pelvis][0] = 6.75f;
		DirFwdAmnt[hitbox_pelvis][1] = 7.00f;
		DirFwdAmnt[hitbox_pelvis][2] = 7.00f;
		DirFwdAmnt[hitbox_pelvis][3] = 5.11f;
		DirFwdAmnt[hitbox_pelvis][4] = 3.72f;
		DirFwdAmnt[hitbox_pelvis][5] = 2.90f;

		DirFwdAmnt[hitbox_chest][0] = 6.75f;
		DirFwdAmnt[hitbox_chest][1] = 7.00f;
		DirFwdAmnt[hitbox_chest][2] = 5.84f;
		DirFwdAmnt[hitbox_chest][3] = 5.11f;
		DirFwdAmnt[hitbox_chest][4] = 3.72f;
		DirFwdAmnt[hitbox_chest][5] = 2.90f;
	}
	std::deque<Bullet_t> Bullets = std::deque<Bullet_t>();
	void BulletHandle();

	int Run(CUserCmd* cmd);
	bool Hitchance(C_BaseCombatWeapon* pWeapon, C_BaseEntity* pEnt, Vector& Pos);
	float HitPoints(PointInfo& Src, C_BaseCombatWeapon* pWeapon, EntInfo& lst, PRecord& rec, Vector& FwdAng, int Hitbox, char flag);
};

extern Aimbot g_Aimbot;