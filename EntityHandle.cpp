#include "Main.h"

EntityHandle g_Ent = EntityHandle();

PRecord::PRecord(EntInfo& lst)
{
	C_PlayerAnimstate* AnimState = lst.ent->GetAnimstate();

	if (!AnimState)
	{
		flSimtime = -1.f;
		return;
	}

	C_AnimationLayer* Animlayers = lst.ent->GetAnimLayers();
	if (!Animlayers)
	{
		flSimtime = -1.f;
		return;
	}

	bool LastRecordValid = lst.NewData.flSimtime != -1.f;

	if (LastRecordValid)
	{
		vVelocity = lst.NewData.vVelocity;
		flSimtime = lst.NewData.flSimtime;
		*AnimState = lst.NewData.animstate;
		memcpy(Animlayers, lst.NewData.animlayers, (sizeof(C_AnimationLayer) * 13));
		memcpy(animlayers, lst.NewData.animlayers, (sizeof(C_AnimationLayer) * 13));
		lst.ent->GetPosParams() = lst.NewData.flPosParams;
		lst.NewData.vVelocity = Vector(AnimState->m_vVelocityX, AnimState->m_vVelocityY, lst.ent->GetAbsVelocity().z);

	}
	else
	{
		vVelocity = lst.ent->GetVelocity();
		flSimtime = lst.ent->GetSimTime();
		memcpy(animlayers, Animlayers, (sizeof(C_AnimationLayer) * 13));
		AnimState->m_flLastClientSideAnimationUpdateTime = I::Globals->curtime - I::Globals->intervalPerTick;
	}	

	if (lst.RData.LastLby != lst.NewData.flLowerBodyYaw)
	{
		LbyUpdate = true;
	}

	AnimState->m_iLastClientSideAnimationUpdateFramecount = 0;

	if (g_Vars.Cfg.resolver)
	{
		ShotBullet = lst.NewData.ShotBullet;
		ShotReliable = lst.NewData.ShotReliable;
		g_Ent.Resolve(lst);
		LbyUpdate = lst.RData.LbyUpdated;
	}

	vViewAng = lst.ent->GetEyeAng();
	lst.NewData.LbyUpdate = LbyUpdate;

	index = lst.ent->GetIndex();
	float ogCurtime = I::Globals->curtime;
	I::Globals->curtime = flSimtime;

	lst.ent->AllowAnimUpdate() = true;

	lst.ent->SetAbsVelocity(lst.NewData.vVelocity);
	lst.ent->SetAbsPos(lst.NewData.vOrigin);
	lst.ent->GetOrigin() = lst.NewData.vOrigin;
	
	lst.ent->GetAbsOrigin() = lst.NewData.vOrigin;

	g_Ent.UpdateAnimationState(AnimState, vViewAng);
	lst.ent->UpdateClientAnims();

	lst.NewData.vVelocity = Vector(AnimState->m_vVelocityX, AnimState->m_vVelocityY, lst.ent->GetAbsVelocity().z);

	flPosParams = lst.ent->GetPosParams();

//	animlayers[0] = Animlayers[0];

	animstate = *AnimState;
	lst.NewData.animstate = animstate;

	lst.ent->AllowAnimUpdate() = false;

	AnimState->m_iLastClientSideAnimationUpdateFramecount = I::Globals->framecount;
	AnimState->m_flLastClientSideAnimationUpdateTime = flSimtime;

	//fix fov cull thingy
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(lst.ent) + 0xA30) = I::Globals->framecount; // *(_DWORD *)(v6 + 0xA30) = *(_DWORD *)(GlobalVars_0 + 4);
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(lst.ent) + 0xA28) = 0; // *(_DWORD *)(v6 + 0xA28) = 0;

	//invalidate bone cache
	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(lst.ent) + 0x2914) = FLT_MAX; //bone setup time  mov dword ptr [ecx+2924h], 0FF7FFFFFh
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(lst.ent) + 0x2680) = NULL; //bone model counter mov [ecx+2690h], eax

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(lst.ent) + 0xA5C) = 0; // m_nLastNonSkippedFrame set to 0 to force !ShouldSkipAnimationFrame *(_DWORD *)(v7 + 0xA64 + 4) = *(_DWORD *)(GlobalVars_0 + 4);

	int OldEffects = lst.ent->GetEffects();
	int OldEFlags = lst.ent->GetEFlags();

	lst.ent->GetEffects() |= EF_NOINTERP;
	lst.ent->GetEFlags() |= EFL_SETTING_UP_BONES;

	BYTE ThisThing = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(lst.ent) + 0x270);
	if (ThisThing & 1)
		*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(lst.ent) + 0x270) &= ~1; 

	lst.ent->SetAbsAngles(Vector(0, AnimState->m_flGoalFeetYaw, 0));
	lst.ent->GetAbsAngle() = Vector(0, AnimState->m_flGoalFeetYaw, 0);

	lst.ent->SetupBones(bones, 128, BONE_USED_BY_ANYTHING, flSimtime);
	memcpy(lst.NewData.bones, bones, (sizeof(matrix3x4_t) * 128));

	lst.ent->GetEffects() = OldEffects;
	lst.ent->GetEFlags() = OldEFlags;
	
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(lst.ent) + 0xA5C) = I::Globals->framecount;
	*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(lst.ent) + 0x2914) = I::Globals->curtime;
	*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(lst.ent) + 0x270) = ThisThing;

	flLowerBodyYaw = lst.ent->GetLby();
	flGoalFeetYaw = AnimState->m_flGoalFeetYaw;
	vOrigin = lst.ent->GetOrigin();
	I::Globals->curtime = ogCurtime;
}

void EntityHandle::SetState(C_BaseEntity* ent, PRecord& rec)
{

	C_PlayerAnimstate* AnimState = ent->GetAnimstate();
	if (!AnimState)
	{
		return;
	}

	C_AnimationLayer* Animlayers = ent->GetAnimLayers();
	if (!Animlayers)
	{
		return;
	}

	*AnimState = rec.animstate;
	memcpy(Animlayers, rec.animlayers, (sizeof(C_AnimationLayer) * 13));
	ent->GetPosParams() = rec.flPosParams;
	
	ent->SetAbsPos(rec.vOrigin);
	ent->GetAbsOrigin() = rec.vOrigin;
	ent->GetOrigin() = rec.vOrigin;
	
	ent->AllowAnimUpdate() = true;
	ent->UpdateClientAnims();
	ent->AllowAnimUpdate() = false;
	
	//fix fov cull thingy
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA30) = I::Globals->framecount; // *(_DWORD *)(v6 + 0xA30) = *(_DWORD *)(GlobalVars_0 + 4);
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA28) = 0; // *(_DWORD *)(v6 + 0xA28) = 0;

	//invalidate bone cache
	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(ent) + 0x2914) = FLT_MAX; //bone setup time  mov dword ptr [ecx+2924h], 0FF7FFFFFh
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0x2680) = NULL; //bone model counter mov [ecx+2690h], eax

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA5C) = 0; // m_nLastNonSkippedFrame set to 0 to force !ShouldSkipAnimationFrame *(_DWORD *)(v7 + 0xA64 + 4) = *(_DWORD *)(GlobalVars_0 + 4);

	int OldEffects = ent->GetEffects();
	int OldEFlags = ent->GetEFlags();

	ent->GetEffects() |= EF_NOINTERP;
	ent->GetEFlags() |= EFL_SETTING_UP_BONES;

	BYTE ThisThing = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270);
	if (ThisThing & 1)
		*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270) &= ~1;

	memcpy(ent->GetBoneCache(), rec.bones, (sizeof(matrix3x4_t) * ent->GetBoneCount()));

	ent->SetAbsAngles(Vector(0, AnimState->m_flGoalFeetYaw, 0));
	ent->GetAbsAngle() = Vector(0, AnimState->m_flGoalFeetYaw, 0);

	ent->SetupBones(nullptr, 128, BONE_USED_BY_ANYTHING, rec.flSimtime);

	ent->GetEffects() = OldEffects;
	ent->GetEFlags() = OldEFlags;

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA5C) = I::Globals->framecount;
	*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ent) + 0x2914) = I::Globals->curtime;
	*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270) = ThisThing;
}

void EntityHandle::SetInfo(C_BaseEntity* ent, PRecord& rec)
{
	C_PlayerAnimstate* AnimState = ent->GetAnimstate();
	if (!AnimState)
	{
		return;
	}

	C_AnimationLayer* Animlayers = ent->GetAnimLayers();
	if (!Animlayers)
	{
		return;
	}

	*AnimState = rec.animstate;
	memcpy(Animlayers, rec.animlayers, (sizeof(C_AnimationLayer) * 13));
	ent->GetPosParams() = rec.flPosParams;
	memcpy(ent->GetBoneCache(), rec.bones, (sizeof(matrix3x4_t) * ent->GetBoneCount()));
}

int EntityHandle::LstSearch(int index)
{
	if (index >= 64 || index < 0)
		return -1;

	if (ListCache[0] == -2)
		for (int i = 0; i < 64; i++)
			ListCache[i] = -1;

	if (ListCache[index] == -1 && List.size() != 0)
	{
		for (int i = 0; i < List.size(); i++)
		{
			if (index == List[i].index)
				ListCache[index] = i;
		}
	}
	else if (ListCache[index] >= List.size() || List[ListCache[index]].index != index)
	{
			ListCache[index] = -1;
	}

	return ListCache[index];
}

void EntityHandle::LstUpdate(int start)
{
	if (start == -1)
	{
		ListCache[0] = -2;
		return;
	}

	ListCache[List[start].index] = -1;
	for (int i = start + 1; i < List.size(); i++)
	{
		ListCache[List[i].index]--;
	}
}

void EntityHandle::LocalUpdate(C_BaseEntity* ent, Vector ang, bool update)
{
	C_PlayerAnimstate* AnimState = ent->GetAnimstate();
	if (!AnimState)
	{
		return;
	}

	C_AnimationLayer* Animlayers = ent->GetAnimLayers();
	if (!Animlayers)
	{
		return;
	}
	//op
	static C_AnimationLayer animlayers1[13] = { };
	static C_PlayerAnimstate animstate1 = C_PlayerAnimstate();
	static std::array<float, 24> flPosParams1;

	if (fabs(g_Ent.LocalRecord.animstate.m_flLastClientSideAnimationUpdateTime - g_Globals.LocalRealTime) < 2.f && g_Ent.LocalRecord.animstate.pBaseEntity == ent)
	{
		*AnimState = g_Ent.LocalRecord.animstate;
		//	memcpy(Animlayers, animlayers1, (sizeof(C_AnimationLayer) * 13));
		ent->GetPosParams() = g_Ent.LocalRecord.flPosParams;
	}

	ent->AllowAnimUpdate() = true;
	UpdateAnimationState(AnimState, ang);
	ent->UpdateClientAnims();
	ent->AllowAnimUpdate() = false;

	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(ent) + 0x2914) = FLT_MAX; //bone setup time  mov dword ptr [ecx+2924h], 0FF7FFFFFh
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0x2680) = NULL; //bone model counter mov [ecx+2690h], eax

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA5C) = 0; // m_nLastNonSkippedFrame set to 0 to force !ShouldSkipAnimationFrame *(_DWORD *)(v7 + 0xA64 + 4) = *(_DWORD *)(GlobalVars_0 + 4);

	int OldEffects = ent->GetEffects();
	int OldEFlags = ent->GetEFlags();

	ent->GetEffects() |= EF_NOINTERP;
	ent->GetEFlags() |= EFL_SETTING_UP_BONES;

	BYTE ThisThing = *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270);
	if (ThisThing & 1)
		*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270) &= ~1;

	if (update)
	{
		g_Ent.LocalRecord.vOrigin = ent->GetAbsOrigin();
		ent->SetupBones(g_Ent.LocalRecord.bones, 128, BONE_USED_BY_ANYTHING, g_Globals.LocalRealTime + I::Globals->intervalPerTick);
	}
	else
		ent->SetupBones(nullptr, 128, BONE_USED_BY_ANYTHING, g_Globals.LocalRealTime + I::Globals->intervalPerTick);


	ent->GetEffects() = OldEffects;
	ent->GetEFlags() = OldEFlags;

	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(ent) + 0xA5C) = I::Globals->framecount;
	*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ent) + 0x2914) = I::Globals->curtime;
	*reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(ent) + 0x270) = ThisThing;

	g_Ent.LocalRecord.animstate = *AnimState;
	g_Ent.LocalRecord.flPosParams = ent->GetPosParams();
	memcpy(g_Ent.LocalRecord.animlayers, Animlayers, (sizeof(C_AnimationLayer) * 13));
	g_Ent.LocalRecord.vVelocity = ent->GetVelocity();
}

void EntityHandle::Resolve(EntInfo& lst)
{
	lst.RData.RIndex = -1;
	if (!g_Globals.LocalPlayer || !g_Globals.LocalPlayer->IsAlive())
		return;

	C_BaseCombatWeapon* pWeapon = g_Globals.LocalPlayer->GetActiveWeapon();

	lst.RData.ResolveMode = "";

	if (!pWeapon)
		return;

	if (g_Vars.Cfg.resolver == 0)
		return;
	if (g_Vars.Cfg.resolver == 1)
	{
		lst.RData.ResolveMode = "lby";
		lst.ent->GetEyeAng().y = lst.NewData.flLowerBodyYaw;
		return;
	}

	float LeashDist = (lst.RData.Leash - lst.NewData.vOrigin).Length();
	Vector LeashAng = g_Math.CalcAng(lst.RData.Leash, lst.NewData.vOrigin);
	Vector Fwd;
	if (LeashDist > 50.f)
	{
		g_Math.AngVec(LeashAng, &Fwd);
		Fwd.NormalizeInPlace();
		lst.RData.Leash += Fwd * (LeashDist - 50.f);
	}

	float Speed = lst.ent->GetVelocity().Length();
	bool OnGround = (lst.ent->GetFlags() & FL_ONGROUND);

	float HeadToOriginLen = (lst.NewData.bones[lst.HBIndxToBone[hitbox_head]].GetOrigin() - lst.NewData.vOrigin).Length2D();

	auto PointFromRotation = [&](float yaw)
	{
		Vector vPoint = lst.NewData.vOrigin, NewAng = Vector(0, yaw, 0);
		g_Math.AngVec(NewAng, &Fwd);
		Fwd.NormalizeInPlace();
		vPoint.z += fabs(lst.NewData.bones[lst.HBIndxToBone[hitbox_head]].GetOrigin().z - lst.NewData.vOrigin.z);
		vPoint += Fwd * (HeadToOriginLen + 8.f);
		vPoint.z += 8.f;
		vPoint += (lst.NewData.vOrigin - lst.RData.Leash) / 4.f;
		return vPoint;
	};

	lst.RData.LbyUpdated = false;

	if (Speed > 0.1f)
	{
		lst.RData.FakeLbyTime = lst.NewData.flSimtime;
		lst.RData.LastLby = lst.NewData.flLowerBodyYaw;
		lst.RData.LastRand = lst.NewData.flLowerBodyYaw;
		lst.RData.LbyTimer = -1.f;
	}

	if (Speed > 12.f && lst.NewData.animlayers[6].m_flWeight > 0.f)
	{
		lst.RData.LastStaticLby = lst.RData.StaticLby;
		lst.RData.StaticLby = lst.NewData.flLowerBodyYaw;
		lst.ent->GetEyeAng().y = lst.RData.StaticLby;
		lst.RData.ResolveMode = "lby";

		for (int i = 0; i < 7; i++)
		{
			lst.RData.HitStack[i] = 0;
			lst.RData.SideSwitch[i] = false;
		}

	}
	else
	{

		if (lst.NewData.ShotReliable)
		{
			Vector Ang = g_Math.CalcAng(lst.NewData.vOrigin, g_Globals.LocalPlayer->GetOrigin());
			Ang.NormalizeAngle();
			lst.ent->GetEyeAng().y = Ang.y;
			lst.RData.ResolveMode = "shot";
			lst.RData.LbyTimer = -1.f;
			return;
		}


		if (lst.RData.LastLby != lst.NewData.flLowerBodyYaw)
		{
			lst.RData.ResolveMode = "lby_ch";
			lst.ent->GetEyeAng().y = lst.NewData.flLowerBodyYaw;
			lst.RData.LastLby = lst.NewData.flLowerBodyYaw;
			lst.RData.LbyUpdated = true;
			lst.RData.LbyTimer = lst.RData.FakeLbyTime + 1.1f;
			return;
		}

		//legendary
		Vector ExtendedEyePos = g_Globals.LocalPlayer->GetEyePos() + (g_Globals.LocalPlayer->GetOrigin() - g_Globals.LocalLeash);

		float HighestDmg = 999999.f;

		float YawList[] = { g_Math.NormEuler(lst.NewData.vViewAng.y + 180.f), g_Math.NormEuler(LeashAng.y + 180.f), lst.RData.LastStaticLby,lst.RData.StaticLby, lst.RData.LastRand, lst.NewData.flLowerBodyYaw, 0.f  };
		static std::string RList[] = {"yaw180", "leash","last_static_lby","static_lby" ,"rand", "lby", "avg_good_ang"};

		for (int i = 0; i < 7; i++)
		{
			if (i == 6)
			{
				float AngSum =0.f;
				float AngAvg = 0.f;

				if ((lst.RData.RandGoodPos - lst.NewData.vOrigin).Length() <= 60.f)
				{
					AngSum = lst.RData.LastStaticLby + lst.RData.LastGoodRand + YawList[1];
					AngAvg = g_Math.NormEuler(AngSum / 3.f);
				}
				else
				{
					AngSum = lst.RData.LastStaticLby + YawList[1];
					AngAvg = g_Math.NormEuler(AngSum / 2.f);
				}

				YawList[i] = AngAvg;
			}
			float Dmg = g_Autowall.HitFloatingPoint(pWeapon, ExtendedEyePos, PointFromRotation(g_Math.NormEuler(YawList[i])));
			if (Dmg <= HighestDmg)
			{
				lst.ent->GetEyeAng().y = g_Math.NormEuler( YawList[i] + (lst.RData.SideSwitch[i] ? 180.f : 0.f ));
				lst.RData.ResolveMode = RList[i] + (lst.RData.SideSwitch[i] ? "_switched" : "");
				if (i == 4)
				{
					lst.RData.RandGoodPos = lst.NewData.vOrigin;
					lst.RData.LastGoodRand == YawList[i];
				}
				HighestDmg = Dmg;

				lst.RData.RIndex = i;
			}
			else if (i == 4)
			{
				float AngDelt = fabs(g_Math.NormEuler(lst.RData.LastStaticLby - lst.RData.LastRand));
				float RevDelt = std::clamp(180.f - AngDelt, 45.f, 135.f);
				lst.RData.LastRand = g_Math.NormEuler(lst.RData.LastRand + (180.f - (RevDelt/2.f)) + (rand() % static_cast<int>(RevDelt)));
			}
		}
	}
	lst.NewData.ShotReliable = false;	
}

bool EntityHandle::ValidTick(float& simtime)
{
	static convar* sv_maxunlag = I::Cvar->FindVar("sv_maxunlag");
	float correct = std::clamp(I::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING) + I::Engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + g_Misc.LerpTime(), 0.f, sv_maxunlag->ReadFloat());
	float deltaTime = correct - ((g_Globals.LocalRealTime + I::Globals->intervalPerTick) - simtime);
	return fabsf(deltaTime) < 0.2f;
}

void EntityHandle::UpdateAnimationState(C_PlayerAnimstate* state, Vector angle) // gladiator cheats v2
{
	static auto UpdateAnimState = g_Utils.FindPatternIDA("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
	if (!UpdateAnimState)
		return;

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}
