#pragma once

class CVars {
public:
	struct {
		bool Opened;
	} Menu;

	struct {
		int m_lifeState = 0;
		int m_iHealth = 0;
		int m_vecOrigin = 0;
		int m_fFlags = 0;
		int m_vecVelocity = 0;
		int m_iTeamNum = 0;
		int m_vecViewOffset = 0;
		int m_mBoneCache = 0;
		int m_iBoneCount = 0;
		int m_flDuckAmount = 0;
		int m_flSimulationTime = 0;
		int m_nTickBase = 0;
		int m_angEyeAngles = 0;
		int m_somCmdOff = 0;
		int m_pBaseAnimState = 0;
		int m_pAnimOverlay = 0;
		int m_bClientSideAnimation = 0;
		int m_flPoseParameter = 0;
		int m_fEffects = 0;
		int m_iEFlags = 0;
		int m_flLowerBodyYawTarget = 0;
		int m_aimPunchAngle = 0;
		int m_ArmorValue = 0;
		int m_bHasHelmet = 0;
		int m_bGunGameImmunity = 0;
		int m_viewPunchAngle = 0;
		int m_bIsScoped = 0;

		int m_hActiveWeapon = 0;
		int m_flNextPrimaryAttack = 0;
		int m_fLastShotTime = 0;

		Vector MaxsMins[20][2] = { Vector(0,0,0) };

	}Off;

	struct {
		float testslid = 0.f;
		bool bhop = false;
		bool aimbot = false;
		float strafeang = 3.f;
		bool antiaim = false;
		int chams = 0;
		int resolver = 0;
		float fakelag = 0.f;
		float hitchance = 0.f;
		bool autostop = false;
		float mindmg = 0.f;
		bool thirdperson = false;
		bool novisrecoil = false;
		float tpdistance = 0.f;
		bool drawbox = false;
		bool flipaa = false;
		bool hitboxes[6] = { false, false, false, false, false, false };
	//	bool bthitboxes[6] = { false, false, false, false, false, false };
		float pointscale = 0.f;
		bool backtrack[2] = { false, false };
		bool baim[1] = {false};
		float fov = 0.f;
		bool healthbar = false;
		bool resolveinfo = false;
		bool name = false;
		bool weapon = false;
		bool impacts = false;
		bool hitsound = false;
		bool nozoom = false;
		bool noscope = false;
		bool lagpeek = false;

		int hitbx = 0;
		float fwd = 0.f;
		float left = 0.f;
		float right = 0.f;
		float back = 0.f;
		float up = 0.f;
		float down = 0.f;

	} Cfg;

	inline void Init()
	{
		Off.m_lifeState = g_pNetvars.GetOffset("DT_BasePlayer", "m_lifeState");
		Off.m_iHealth = g_pNetvars.GetOffset("DT_BasePlayer", "m_iHealth");
		Off.m_vecOrigin = g_pNetvars.GetOffset("DT_BaseEntity", "m_vecOrigin");
		Off.m_fFlags = g_pNetvars.GetOffset("DT_BasePlayer", "m_fFlags");
		Off.m_vecVelocity = g_pNetvars.GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		Off.m_iTeamNum = g_pNetvars.GetOffset("DT_BaseEntity", "m_iTeamNum");
		Off.m_vecViewOffset = g_pNetvars.GetOffset("DT_BasePlayer","localdata", "m_vecViewOffset[0]");
		Off.m_flDuckAmount = g_pNetvars.GetOffset("DT_BasePlayer", "m_flDuckAmount");
		Off.m_flSimulationTime = g_pNetvars.GetOffset("DT_BaseEntity", "m_flSimulationTime");
		Off.m_nTickBase = g_pNetvars.GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		Off.m_angEyeAngles = g_pNetvars.GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Off.m_hActiveWeapon = g_pNetvars.GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		Off.m_flNextPrimaryAttack = g_pNetvars.GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		Off.m_bClientSideAnimation = g_pNetvars.GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		Off.m_flPoseParameter = g_pNetvars.GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		Off.m_fEffects = g_pNetvars.GetOffset("DT_BaseEntity", "m_fEffects");
		Off.m_flLowerBodyYawTarget = g_pNetvars.GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		Off.m_aimPunchAngle = g_pNetvars.GetOffset("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		Off.m_ArmorValue = g_pNetvars.GetOffset("DT_CSPlayer", "m_ArmorValue");
		Off.m_bHasHelmet = g_pNetvars.GetOffset("DT_CSPlayer", "m_bHasHelmet");
		Off.m_bGunGameImmunity = g_pNetvars.GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		Off.m_viewPunchAngle = g_pNetvars.GetOffset("DT_BasePlayer", "localdata", "m_Local", "m_viewPunchAngle");
		Off.m_fLastShotTime = g_pNetvars.GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		Off.m_bIsScoped = g_pNetvars.GetOffset("DT_CSPlayer", "m_bIsScoped");

		Off.m_pBaseAnimState = 0x3874;
		Off.m_pAnimOverlay = 0x2970;
		Off.m_mBoneCache = 0x2900;
		Off.m_iBoneCount = 0x290C;
		Off.m_somCmdOff = 0x3314;
		Off.m_iEFlags = 0xE4;

		//used HLMV
		Off.MaxsMins[0][0] = Vector(-2.77f, -2.88f, -3.10f); // head minsd
		Off.MaxsMins[0][1] = Vector(6.96f, 3.52f, 3.01f); // head maxsd

		Off.MaxsMins[1][0] = Vector(0.33f, -3.02f, -2.45f); // neck minsd
		Off.MaxsMins[1][1] = Vector(3.49f, 2.42f, 2.35f); // neck maxsd

		Off.MaxsMins[2][0] = Vector(0.33f, -3.02f, -2.45f); // neck2 minsd
		Off.MaxsMins[2][1] = Vector(3.49f, 2.42f, 2.35f); // neck2 maxsd

		Off.MaxsMins[3][0] = Vector(-6.42f, -5.75f, -6.86f); // pelvis mins
		Off.MaxsMins[3][1] = Vector(4.58f, 4.58f, 6.84f); // pelvis maxs

		Off.MaxsMins[4][0] = Vector(-4.28f, -4.58f, -6.39f); // belly minsd
		Off.MaxsMins[4][1] = Vector(3.21f, 5.89f, 6.28f); // belly maxsd

		Off.MaxsMins[5][0] = Vector(-4.28f, -5.35f, -5.89f); // thorax mins or chest
		Off.MaxsMins[5][1] = Vector(2.92f, 5.15f, 5.89f); // thorax maxs

		Off.MaxsMins[6][0] = Vector(-3.38f, -5.03f, -6.09f); // lowerchest minsd
		Off.MaxsMins[6][1] = Vector(3.21f, 5.89f, 5.98f); // lowerchest maxsd

		Off.MaxsMins[7][0] = Vector(-6.42f, -4.7459f, -6.9587f); // upperchest mins
		Off.MaxsMins[7][1] = Vector(7.4796f, 4.5796f, 6.8373f); // upperchest maxs

		Off.MaxsMins[8][0] = Vector(1.82f, -3.76f, -3.58f); // rightthigh minsd
		Off.MaxsMins[8][1] = Vector(22.15f, 3.72f, 2.14f); // rightthigh maxsd

		Off.MaxsMins[9][0] = Vector(1.82f, -3.26f, -2.14f); // leftthigh minsd
		Off.MaxsMins[9][1] = Vector(22.15f, 3.72f, 3.58f); // leftthigh maxsd

		Off.MaxsMins[10][0] = Vector(2.08f, -3.21f, -2.85f); // rightcalf minsd
		Off.MaxsMins[10][1] = Vector(19.26f, 2.68f, 2.25); // rightcalf maxsd

		Off.MaxsMins[11][0] = Vector(2.08f, -3.21f, -2.15f); // leftcalf minsd
		Off.MaxsMins[11][1] = Vector(19.26f, 2.68f, 3.05f); // leftcalf maxsd

		Off.MaxsMins[12][0] = Vector(-2.50f, -3.80f, -2.80f);//"ankle_L" ???
		Off.MaxsMins[12][1] = Vector(10.40f, -0.20f, 2.00f);

		Off.MaxsMins[13][0] = Vector(-10.40f, 0.30f, -2.00f);//"ankle_R" ???
		Off.MaxsMins[13][1] = Vector(2.60f, 4.00f, 2.80f);

		Off.MaxsMins[14][0] = Vector(-2.00f, -2.00f, -2.00f); // l hand
		Off.MaxsMins[14][1] = Vector(4.60f, 2.00f, 2.00f);

		Off.MaxsMins[15][0] = Vector(-5.00f, -2.00f, - 2.00f);// r hand
		Off.MaxsMins[15][1] = Vector(2.00f, 2.00f, 2.00f); 

		Off.MaxsMins[16][0] = Vector(-2.68f, -3.21f, -2.14f); // rightupperarm minsd
		Off.MaxsMins[16][1] = Vector(12.84f, 3.21f, 2.14f); // rightupperarm maxsd

		Off.MaxsMins[17][0] = Vector(-0.f, -2.14f, -2.14f); // rightforearm minsd
		Off.MaxsMins[17][1] = Vector(9.63f, 2.14f, 2.14f); // rightforearm maxsd

		Off.MaxsMins[18][0] = Vector(-2.68f, -3.21f, -2.14f); // leftupperarm minsd
		Off.MaxsMins[18][1] = Vector(12.84f, 3.21f, 2.14f); // leftupperarm maxsd

		Off.MaxsMins[19][0] = Vector(-0.f, -2.14f, -2.14f); // leftforearm minsd
		Off.MaxsMins[19][1] = Vector(9.63f, 2.14f, 2.14f); // leftforearm maxsd
	}

	// some vars for save/setup func.
}; extern CVars g_Vars;


inline void load_config() // kmeth 
{
	CreateDirectory("configs", NULL);
	std::string config_name;
	config_name = "configs/";
	config_name += "private_hvh_v1";
//	config_name += std::to_string(config_num);
	config_name += ".dat";

	std::ifstream f(config_name.c_str());
	if (!f.good()) return;
	FILE* fin = fopen(config_name.c_str(), "r");
	fread(&g_Vars.Cfg, sizeof(g_Vars.Cfg), 1, fin);
	fclose(fin);
}

inline void save_config()
{
	CreateDirectory("configs", NULL);
	std::string config_name;
	config_name = "configs/";
	config_name += "private_hvh_v1";
//	config_name += std::to_string(config_num);
	config_name += ".dat";
	FILE* fout = fopen(config_name.c_str(), "w");
	fwrite(&g_Vars.Cfg, sizeof(g_Vars.Cfg), 1, fout);
	fclose(fout);
}