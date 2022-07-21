#include "Main.h"

FrameStageNotifyFn oFrameStageNotify;

void __fastcall Hooks::FramStageNotify(void* ecx, void* edx, int iStage)
{
	if (iStage == FRAME_RENDER_START)
	{
		if (g_Globals.GameReady && g_Globals.LocalPlayer && g_Globals.LocalPlayer->IsAlive() && g_Globals.LocalPlayer->GetAnimstate())
		{
			float OldSimtime = g_Globals.LocalPlayer->GetSimTime();
			Vector OldOrigin = g_Globals.LocalPlayer->GetOrigin();
			g_Globals.LocalPlayer->SetAbsAngles(Vector(0, g_Globals.LocalPlayer->GetAnimstate()->m_flGoalFeetYaw, 0));
			g_Globals.LocalPlayer->GetSimTime() = OldSimtime;
			g_Globals.LocalPlayer->GetOrigin() = OldOrigin;
		}


		for (int i = 0; i < 64; i++) // this where the entity list is populated
		{		
			C_BaseEntity* Ent = I::EntityList->GetClientEntity(i);

			if (i != I::Engine->GetLocalPlayer())
			{
				int LstIndx = g_Ent.LstSearch(i);
				if (!Ent || !Ent->IsValid() || Ent->GetTeam() == g_Globals.LocalTeam)
				{
					if (LstIndx != -1)
					{
						g_Ent.LstUpdate(LstIndx);
						g_Ent.List[LstIndx].ent = nullptr;
						g_Ent.List[LstIndx].index = -1;
						if (g_Ent.List[LstIndx].Records.size() != 0)
							g_Ent.List[LstIndx].Records.clear();

						g_Ent.List.erase(g_Ent.List.begin() + LstIndx);
					}
				}
				else if (Ent->IsPlayer() && Ent->Animating() && Ent->Networkable() && Ent->GetClientClass()->iClassID == ClassId_CCSPlayer)
				{			
					if (LstIndx == -1)
					{
						g_Ent.List.push_back(EntInfo(Ent, i));
						LstIndx = g_Ent.List.size() - 1;
					}

					if (LstIndx != -1)
					{
						EntInfo* ThisEnt = &g_Ent.List[LstIndx];
						if (ThisEnt)
						{
							if (ThisEnt->NewData.index != i)
								ThisEnt->NewData.index = i;

							if (ThisEnt->LastSimtime != Ent->GetSimTime())
							{
								ThisEnt->NewData.ShotBullet = false;
								ThisEnt->NewData.ShotReliable = false;
								ThisEnt->OldData = ThisEnt->NewData;
								C_PlayerAnimstate* AnimState = Ent->GetAnimstate();
								C_AnimationLayer* Animlayers = Ent->GetAnimLayers();
								if (AnimState && Animlayers)
								{
									memcpy(ThisEnt->NewData.animlayers, Animlayers, (sizeof(C_AnimationLayer) * 13));
									ThisEnt->NewData.flGoalFeetYaw = AnimState->m_flGoalFeetYaw;
									ThisEnt->NewData.animstate = *AnimState;
								}

								ThisEnt->NewData.vOrigin = Ent->GetOrigin();
								ThisEnt->NewData.flPosParams = Ent->GetPosParams();
								ThisEnt->NewData.flLowerBodyYaw = Ent->GetLby();
								ThisEnt->NewData.vViewAng = Ent->GetEyeAng();
								ThisEnt->NewData.flSimtime = Ent->GetSimTime();
								ThisEnt->NewData.vVelocity = Ent->GetVelocity();
								ThisEnt->LastSimtime = Ent->GetSimTime();

								C_BaseCombatWeapon* pWep = ThisEnt->ent->GetActiveWeapon();
								if (pWep)
								{
									if (ThisEnt->NewData.ShotTime != pWep->GetLastShotTime())
									{
										if (pWep->GetLastShotTime() == ThisEnt->LastSimtime)
											ThisEnt->NewData.ShotReliable = true;
										else
										{
											ThisEnt->NewData.ShotReliable = false;
											ThisEnt->NewData.vViewAng.x += 3.f;
										}
										ThisEnt->NewData.ShotBullet = true;
										ThisEnt->NewData.ShotTime = pWep->GetLastShotTime();
									}
								}
							}							
						}
					}

					Ent->AllowAnimUpdate() = false;
					Ent->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount = I::Globals->framecount;
					Ent->GetAnimstate()->m_flLastClientSideAnimationUpdateTime = I::Globals->curtime;

					*reinterpret_cast<int*>(reinterpret_cast<DWORD>(Ent) + 0xA5C) = I::Globals->framecount;
					*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(Ent) + 0x2914) = I::Globals->curtime;
				}
				if (!g_Globals.GameReady)
					g_Globals.GameReady = true;
			}
		}
	}

	if (g_Globals.GameReady && iStage == FRAME_NET_UPDATE_END)
	{
		for (EntInfo& lst : g_Ent.List)
		{ 
			C_BaseEntity* Ent = I::EntityList->GetClientEntity(lst.index); // sry have todo this or big crash bec FRAME_NET_UPDATE_END is b4 FRAME_RENDER_START

			if (!Ent || !Ent->IsValid())
				continue;
			auto m_dwVarMap = reinterpret_cast<uintptr_t>(Ent) + 36;
			int thisthihnhh = *reinterpret_cast<int*>(m_dwVarMap + 20);
			for (auto size = 0; size < thisthihnhh; size++)
			{		
				uintptr_t* thisthing =reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(m_dwVarMap) + size * 12);
				if (thisthing)
				{
					*thisthing = 0;
				}
			}
		}
	}

	oFrameStageNotify(ecx, edx, iStage);
}