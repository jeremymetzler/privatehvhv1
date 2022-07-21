#pragma once

enum hitboxes {
	hitbox_head = 0,
	hitbox_neck = 1,
	hitbox_neck_lower = 2,
	hitbox_pelvis = 3,
	hitbox_stomach = 4,
	hitbox_lower_chest = 5,
	hitbox_chest = 6,
	hitbox_upper_chest = 7,
	hitbox_right_thigh = 8,
	hitbox_left_thigh = 9,
	hitbox_right_calf = 10,
	hitbox_left_calf = 11,
	hitbox_right_foot = 12,
	hitbox_left_foot = 13,
	hitbox_right_hand = 14,
	hitbox_left_hand = 15,
	hitbox_right_upper_arm = 16,
	hitbox_right_forearm = 17,
	hitbox_left_upper_arm = 18,
	hitbox_left_forearm = 19,
	hitbox_max = 20
};

struct EntInfo;

class PRecord
{
public:
	float flSimtime = -1.f, flLowerBodyYaw = 0.f, flGoalFeetYaw = 0.f;
	std::array<float, 24> flPosParams;
	int index = 0;
	matrix3x4_t bones[128];
	C_AnimationLayer animlayers[13] = { };
	C_PlayerAnimstate animstate = C_PlayerAnimstate();
	Vector vOrigin, vViewAng, vVelocity;
	bool LbyUpdate = false;
	float ShotTime = -1.f;
	bool ShotReliable = false;
	bool ShotBullet = false;

	PRecord()
	{
		flSimtime = -1.f;
		for (int i = 0; i < 13; i++)
			animlayers[i] = C_AnimationLayer();
		animstate = C_PlayerAnimstate();
	};

	PRecord(EntInfo& lst);
};

class ResolveData
{
public:
	std::string ResolveMode = "";
	float LastLby = 0.f;
	float StaticLby = 0.f;
	float LastStaticLby = 0.f;
	float LbyTimer = -1.f;
	float FakeLbyTime = -1.f;
	float LastRand = 0.f;
	float LastGoodRand = 0.f;
	Vector RandGoodPos;
	bool LbyUpdated = false;
	Vector Leash;
	int HitStack[7] = {0,0,0,0,0,0,0};
	bool SideSwitch[7] = { false,false,false,false,false,false,false };
	int RIndex = -1;
};

struct EntInfo
{
	C_BaseEntity* ent = nullptr;
	int index = -1;
	std::deque<PRecord> Records = std::deque<PRecord>();
	int HBIndxToBone[hitbox_max];

	bool DoAimbot = false;
	float LastVis = 0.f;
	Vector ValidPos;
	float LastSimtime = 0.f;
	ResolveData RData = ResolveData();
	PRecord NewData = PRecord();
	PRecord OldData = PRecord();
	
	EntInfo(C_BaseEntity* pent, int indx)
	{
		ent = pent;
		index = indx;

		model_t* model = ent->GetModel();

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

			HBIndxToBone[i] = hitbox->bone;
		}
	}
};

class EntityHandle
{
public:

	void SetState(C_BaseEntity* ent, PRecord& rec);
	void SetInfo(C_BaseEntity* ent, PRecord& rec);
	void LocalUpdate(C_BaseEntity* ent, Vector ang, bool update);
	void Resolve(EntInfo& lst);
	bool ValidTick(float& simtime);
	void UpdateAnimationState(C_PlayerAnimstate* state, Vector angle);

	std::deque<EntInfo> List = std::deque<EntInfo>();

	int ListCache[64] = { -2 };
	int LstSearch(int index);
	void LstUpdate(int start = -1);

	PRecord LocalRecord;
	int LHBIndxToBone[hitbox_max];
};

extern EntityHandle g_Ent;