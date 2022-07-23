#include "Main.h"

Aimbot g_Aimbot = Aimbot();

#define fwd_pnt (1)
#define left_pnt (1<<1)
#define right_pnt (1<<2)
#define back_pnt (1<<3)
#define up_pnt (1<<4)
#define down_pnt (1<<5)

float Aimbot::HitPoints(PointInfo& Src, C_BaseCombatWeapon* pWeapon, EntInfo& lst, PRecord& rec,  Vector& FwdAng, int Hitbox, char flag)
{
	static Vector dir[3];
	g_Math.AngVec(FwdAng, &dir[0], &dir[1], &dir[2]);
	dir[0].NormalizeInPlace();
	dir[1].NormalizeInPlace();
	dir[2].NormalizeInPlace();

	auto MPoint = [&]()
	{
		std::vector<Vector> pnts;
		pnts.clear();
		if (flag & fwd_pnt)
			pnts.push_back(Src.TargPos + dir[0] * (g_Aimbot.DirFwdAmnt[Hitbox][0] * g_Vars.Cfg.pointscale));
		if (flag & left_pnt)
			pnts.push_back(Src.TargPos - dir[1] * (g_Aimbot.DirFwdAmnt[Hitbox][1] * g_Vars.Cfg.pointscale));
		if (flag & right_pnt)
			pnts.push_back(Src.TargPos + dir[1] * (g_Aimbot.DirFwdAmnt[Hitbox][2] * g_Vars.Cfg.pointscale));
		if (flag & back_pnt)
			pnts.push_back(Src.TargPos - dir[0] * (g_Aimbot.DirFwdAmnt[Hitbox][3] * g_Vars.Cfg.pointscale));
		if (flag & up_pnt)
			pnts.push_back(Src.TargPos + dir[2] * (g_Aimbot.DirFwdAmnt[Hitbox][4] * g_Vars.Cfg.pointscale));
		if (flag & down_pnt)
			pnts.push_back(Src.TargPos - dir[2] * (g_Aimbot.DirFwdAmnt[Hitbox][5] * g_Vars.Cfg.pointscale));
		return pnts;
	};

	float Dmg1 = 0.f;

	int EntHP = lst.ent->GetHealth();


	for (Vector& mPnt : MPoint())
	{
		Dmg1 = g_Autowall.GetDamage(pWeapon, Src.EyePos, mPnt);

		if (g_Vars.Cfg.baim[0] && rec.flSimtime == lst.Records.back().flSimtime && Hitbox == hitbox_pelvis && (EntHP + 20) < Dmg1)
			Dmg1 += 500;

		if (g_Vars.Cfg.mindmg <= Dmg1 && Src.HighestDmg < Dmg1)
		{
			Src.FinalPos = mPnt;
			Src.HighestDmg = Dmg1;

			Src.FinalSimtime = rec.flSimtime;
			Src.BesRec = rec;
			Src.BesEnt = &lst;

			if (rec.flSimtime == lst.Records.back().flSimtime)
				Src.MainRec = true;
			else
				Src.MainRec = false;

		}
	}

	float Dmg2 = g_Autowall.GetDamage(pWeapon, Src.EyePos, Src.TargPos);

	if (g_Vars.Cfg.baim[0] && rec.flSimtime == lst.Records.back().flSimtime && Hitbox == hitbox_pelvis && (EntHP + 20) < Dmg2)
		Dmg2 += 1000;

	Dmg2 += 4.f;

	if (Dmg2 >= g_Vars.Cfg.mindmg && Src.HighestDmg <= Dmg2)
	{
		Src.FinalPos = Src.TargPos;
		Src.HighestDmg = Dmg2;
		Src.FinalSimtime = rec.flSimtime;
		Src.BesRec = rec;
		Src.BesEnt = &lst;

		if (rec.flSimtime == lst.Records.back().flSimtime)
			Src.MainRec = true;
		else
			Src.MainRec = false;
	}

	if (Dmg2 > Dmg1)
		return Dmg2;
	else
		return Dmg1;
}

bool Aimbot::Hitchance(C_BaseCombatWeapon* pWeapon, C_BaseEntity* pEnt,Vector& Pos)// bameware (bolbi <3)
{
	auto RandomFloat = [](float min, float max) -> float
	{
		typedef float (*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
		return m_RandomFloat(min, max);
	};

	Vector angle = g_Math.CalcAng(g_Globals.LocalPlayer->GetEyePos(), Pos).NormalizeAngle();

	Vector forward, right, up;
	g_Math.AngVec(angle, &forward, &right, &up);

	int iHit = 0;

	for (int i = 0; i < 256; i++)
	{
		float RandomA = RandomFloat(0.0f, 1.0f);
		float RandomB = 1.0f - RandomA * RandomA;

		RandomA = RandomFloat(0.0f, M_PI_F * 2.0f);
		RandomB *= pWeapon->GetSpread();

		float SpreadX1 = (cos(RandomA) * RandomB);
		float SpreadY1 = (sin(RandomA) * RandomB);

		float RandomC = RandomFloat(0.0f, 1.0f);
		float RandomF = 1.0f - RandomC * RandomC;

		RandomC = RandomFloat(0.0f, M_PI_F * 2.0f);
		RandomF *= pWeapon->GetInac();

		float SpreadX2 = (cos(RandomC) * RandomF);
		float SpreadY2 = (sin(RandomC) * RandomF);

		float fSpreadX = SpreadX1 + SpreadX2;
		float fSpreadY = SpreadY1 + SpreadY2;

		Vector vSpreadForward;
		vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
		vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
		vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
		vSpreadForward.NormalizeInPlace();

		Vector qaNewAngle;
		g_Math.VecAng(vSpreadForward, qaNewAngle);
		qaNewAngle.NormalizeAngle();

		Vector vEnd;
		g_Math.AngVec(qaNewAngle, &vEnd);
		vEnd = g_Globals.LocalPlayer->GetEyePos() + (vEnd * 8192.f);

		CGameTrace trace;
		trace_filter_one_entity2 filter(pEnt);
		ray_t ray;
		ray.Init(g_Globals.LocalPlayer->GetEyePos(), vEnd);
	
		I::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
		if (trace.hit_entity == pEnt)
			iHit++;

		if (static_cast<float>(iHit) / 256.f >= g_Vars.Cfg.hitchance / 100.f)
			return true;
	}

	return false;
}

PointInfo MainInfo;
int Aimbot::Run(CUserCmd* cmd)
{
LOOP:
	if (Bullets.size() > 0 && (Bullets[0].PredDecay+ Bullets[0].FireRate + Bullets[0].Latency + (I::Globals->intervalPerTick * 2.f)) < g_Globals.LocalRealTime)
	{
		Bullets.erase(Bullets.begin());
		g_Globals.AddNotif("deleted last bullet", 5.f);
		goto LOOP;
	}

	g_Globals.hit_side = 0;

	static convar* weapon_recoil_scale = I::Cvar->FindVar("weapon_recoil_scale");
	static bool ShotLastTick = false;
	static int LastTick = 0;
	C_BaseCombatWeapon* Weapon = g_Globals.LocalPlayer->GetActiveWeapon();

	if (ShotLastTick)
	{
		*g_Globals.SendPacket = true;
		//	cmd->tick_count = LastTick + 1;
		ShotLastTick = false;
		return 0;
	}

	if (!Weapon)
	{
		return 0;
	}

	if (!g_Vars.Cfg.aimbot)
	{
		return 0;
	}

	SWeaponInfo* wInfo = Weapon->GetWeaponData();

	if (!wInfo || wInfo->WeaponType == WEAPONTYPE_GRENADE || wInfo->WeaponType == WEAPONTYPE_KNIFE || wInfo->WeaponType == WEAPONTYPE_C4)
	{
		return 1;
	}

	if (/*cmd->buttons & in_attack &&*/ g_Globals.LocalRealTime >= Weapon->GetNextPrimaryAttack())
	{
		MainInfo.reset();
		Vector EyePos = g_Globals.LocalPlayer->GetEyePos();
		Vector ExEyePos = EyePos + (g_Globals.LocalPlayer->GetOrigin() - g_Globals.LocalLeash);
		MainInfo.EyePos = EyePos;

		static const char flags[] = { (1), (1 << 1), (1 << 2), (1 << 3) };
		static Vector dir[2];
		Vector Right, Up, Test;
		for (EntInfo& lst : g_Ent.List)
		{
			if (!lst.ent || !lst.ent->IsValid() || lst.ent->IsBulletImmune())
				continue;

			Vector AngToLocal = g_Math.CalcAng(lst.NewData.vOrigin, g_Globals.LocalPlayer->GetOrigin());
			AngToLocal.NormalizeAngle();
			g_Math.AngVec(AngToLocal, nullptr, &Right, &Up);
			Right.NormalizeInPlace();
			Up.NormalizeInPlace();
			Vector Mid = lst.NewData.vOrigin;
			Mid.z += (lst.NewData.bones[lst.HBIndxToBone[hitbox_head]].GetOrigin().z - Mid.z) / 2.f;

			lst.DoAimbot = false;

			g_AntiAim.Prediction(cmd, lst, Weapon);

			if (fabs(lst.LastVis - g_Globals.LocalRealTime) > 5.f)
			{
				for (float x = -50.f; x <= 50.f; x += 16.66666f)
				{
					Vector RR = (Right * x);
					for (float y = -50.f; y <= 50.f; y += 16.66666f)
					{
						Vector Pnt = Mid + RR + (Up * y);
						float Dmg = g_Autowall.HitFloatingPoint(Weapon, EyePos, Pnt);
						if (Dmg != 0.f)
						{
							lst.LastVis = g_Globals.LocalRealTime;
							lst.ValidPos = Pnt;
							lst.DoAimbot = true;
							break;
						}
					}
					if (lst.DoAimbot)
						break;
				}

				if (!lst.DoAimbot)
					continue;
			}
			else
				lst.DoAimbot = true;



			/*	g_Math.AngVec(Vector(0, g_Globals.ViewAng.y, 0), &dir[0], &dir[1]);
				dir[0].NormalizeInPlace();
				dir[1].NormalizeInPlace();
				int i = 0;
				for (float ang = 0; ang >= -270.f; ang -= 90.f)
				{
					float DirYaw = g_Math.NormEuler(g_Globals.ViewAng.y + ang);
					switch (i)
					{
					case 0:
						Test = ExEyePos + dir[0] * 60.f;
						break;
					case 1:
						Test = ExEyePos + dir[1] * 60.f;
						break;
					case 2:
						Test = ExEyePos - dir[0] * 60.f;
						break;
					case 3:
						Test = ExEyePos - dir[1] * 60.f;
						break;
					}
					float Dmg = g_Autowall.HitFloatingPoint(Weapon, Test, lst.ValidPos);
					if (Dmg > 1.f)
					{
						if (!(g_Globals.hit_side & flags[i]))
							g_Globals.hit_side |= flags[i];
					}

					i += 1;
				}*/


			if (lst.Records.size() != 0)
			{
				if (g_Vars.Cfg.resolver && lst.Records.size() > 1)
				{
					int i = -1;
					for (PRecord& rec : lst.Records)
					{
						i += 1;
						if (i == lst.Records.size() - 1 || (rec.ShotBullet && !rec.ShotReliable))
							continue;

						if (!rec.LbyUpdate)
						{
							if (i == 0 && rec.vVelocity.Length() <= 12.0f && (rec.vOrigin - lst.Records.back().vOrigin).Length() < 15.f)
								continue;
							else if (i != 0)
								continue;
						}

						if (rec.LbyUpdate && g_Vars.Cfg.backtrack[0])
						{
							g_Ent.SetState(lst.ent, rec);
							lst.RData.ResolveMode = "lby_bt";

							MainInfo.TargPos = lst.ent->BonePos(rec.bones, lst.HBIndxToBone[hitbox_head]);
							Vector Ang = g_Math.CalcAng(rec.bones[lst.HBIndxToBone[hitbox_neck]].GetOrigin(), MainInfo.TargPos).NormalizeAngle();
							Ang.x = g_Math.NormEuler(Ang.x + 67.f);

							HitPoints(MainInfo, Weapon, lst, rec, Ang, hitbox_head, fwd_pnt | left_pnt | right_pnt | back_pnt | up_pnt);

						}
						else if (g_Vars.Cfg.backtrack[1])
						{
							g_Ent.SetState(lst.ent, rec);
							MainInfo.TargPos = lst.ent->BonePos(rec.bones, lst.HBIndxToBone[hitbox_pelvis]);
							Vector Ang = g_Math.CalcAng(MainInfo.TargPos, rec.bones[lst.HBIndxToBone[hitbox_neck]].GetOrigin()).NormalizeAngle();
							Ang.x = g_Math.NormEuler(Ang.x + 90.f);

							HitPoints(MainInfo, Weapon, lst, rec, Ang, hitbox_pelvis, fwd_pnt | left_pnt | right_pnt | back_pnt);
						}
					}

				}
				g_Ent.SetState(lst.ent, lst.Records.back());
				for (int i = 0; i < hitbox_max; i++)
				{
					MainInfo.TargPos = lst.ent->BonePos(lst.Records.back().bones, lst.HBIndxToBone[i]);

					switch (i)
					{
					case hitbox_head:
					{
						if (!g_Vars.Cfg.hitboxes[0])
							break;

						Vector Ang = g_Math.CalcAng(lst.Records.back().bones[lst.HBIndxToBone[hitbox_neck]].GetOrigin(), MainInfo.TargPos).NormalizeAngle();
						Ang.x = g_Math.NormEuler(Ang.x + 67.f);

						HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Ang, i, fwd_pnt | left_pnt | right_pnt | back_pnt | up_pnt);

						break;
					}
					case hitbox_chest:
					case hitbox_upper_chest:
					case hitbox_lower_chest:
					{
						if (!g_Vars.Cfg.hitboxes[1])
							break;

						if (i == hitbox_chest)
						{
							Vector Ang = g_Math.CalcAng(MainInfo.TargPos, lst.Records.back().bones[lst.HBIndxToBone[hitbox_neck]].GetOrigin()).NormalizeAngle();
							Ang.x = g_Math.NormEuler(Ang.x + 90.f);

							HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Ang, i, fwd_pnt | left_pnt | right_pnt | back_pnt);
						}
						else
							HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Vector().Zero(), i, 0);

						break;
					}
					case hitbox_pelvis:
					{
						if (!g_Vars.Cfg.hitboxes[2])
							break;

						Vector Ang = g_Math.CalcAng(MainInfo.TargPos, lst.Records.back().bones[lst.HBIndxToBone[hitbox_neck]].GetOrigin()).NormalizeAngle();
						Ang.x = g_Math.NormEuler(Ang.x + 90.f);

						HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Ang, i, fwd_pnt | left_pnt | right_pnt | back_pnt);

						break;
					}
					case hitbox_right_thigh:
					case hitbox_left_thigh:
					case hitbox_right_calf:
					case hitbox_left_calf:
					{
						if (!g_Vars.Cfg.hitboxes[4])
							break;

						HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Vector().Zero(), i, 0);
						break;
					}
					case hitbox_right_foot:
					case hitbox_left_foot:
					{
						if (!g_Vars.Cfg.hitboxes[5])
							break;

						HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Vector().Zero(), i, 0);
						break;
					}
					case hitbox_right_upper_arm:
					case hitbox_right_forearm:
					case hitbox_left_upper_arm:
					case hitbox_left_forearm:
					{
						if (!g_Vars.Cfg.hitboxes[3])
							break;

						HitPoints(MainInfo, Weapon, lst, lst.Records.back(), Vector().Zero(), i, 0);
						break;
					}
					default:
						break;
					} 
				}
			}
		}

		if (MainInfo.HighestDmg != 0.f && MainInfo.BesEnt)
		{
			if (g_Vars.Cfg.autostop)
				g_Misc.AutoStop(cmd);

			float FireRate = wInfo->FireRate();

			MainInfo.FinalAng = g_Math.CalcAng(EyePos, MainInfo.FinalPos).NormalizeAngle();

			if (!MainInfo.MainRec)
				g_Ent.SetState(MainInfo.BesEnt->ent, MainInfo.BesRec);

			bool hitch = Hitchance(Weapon, MainInfo.BesEnt->ent, MainInfo.FinalPos);

			if (!MainInfo.MainRec)
				g_Ent.SetState(MainInfo.BesEnt->ent, MainInfo.BesEnt->Records.back());

			g_Ent.LocalUpdate(g_Globals.LocalPlayer, MainInfo.FinalAng, true); // u cant use weapon stuff below this because it gets fucked for some reason

			EyePos = g_Globals.LocalPlayer->WeaponShootPos();

			MainInfo.FinalAng = g_Math.CalcAng(EyePos, MainInfo.FinalPos).NormalizeAngle();

			if (hitch)
			{
				if (!(cmd->buttons & in_attack))
					cmd->buttons |= in_attack;
				cmd->viewangles = MainInfo.FinalAng - (g_Globals.LocalPlayer->GetRecoilOff() * weapon_recoil_scale->ReadFloat());
				cmd->tick_count = g_Misc.TIME_TO_TICKS(g_Misc.LerpTime() + MainInfo.FinalSimtime);
				I::Globals->tickcount = cmd->tick_count;
				LastTick = cmd->tick_count;
				ShotLastTick = true;

				if (Bullets.size() > 0 && ((Bullets[0].PredDecay + (Bullets[0].FireRate * 0.85f))) >= g_Globals.LocalRealTime)
				{
					Bullets.erase(Bullets.begin());
				}

				if (MainInfo.MainRec)
					Bullets.push_back(Bullet_t{ MainInfo.BesEnt->index, MainInfo.BesEnt->RData.RIndex, FireRate,  (I::Engine->GetNetChannelInfo()->GetAvgLatency(FLOW_OUTGOING) + I::Engine->GetNetChannelInfo()->GetAvgLatency(FLOW_INCOMING)), MainInfo.BesEnt->RData.ResolveMode, g_Globals.LocalRealTime, EyePos, MainInfo.BesRec});
			}
			*g_Globals.SendPacket = true;
			return 2;
		}
	}

	return 4;
}

void Aimbot::BulletHandle()
{
	if (g_Globals.LocalShot && g_Globals.BulletHitPos != Vector(0, 0, 0))
	{
		if (Bullets.size() > 0)
		{
			int LstIndx = g_Ent.LstSearch(Bullets[0].TargIndex);

			if (Bullets[0].RIndex >= 0 && LstIndx != -1)
			{
				EntInfo* lst = &g_Ent.List[LstIndx];

				PlayerInfo_t pinfo;
				I::Engine->GetPlayerInfo(Bullets[0].TargIndex, &pinfo);

				std::string TargName = pinfo.szName;

				int& hit_ = lst->RData.HitStack[Bullets[0].RIndex];
				if (g_Globals.HitIndex == Bullets[0].TargIndex)
				{
					hit_++;

					if (hit_ > 2)
						hit_ = 2;
					g_Globals.AddNotif( TargName + " (" + Bullets[0].RMode + ")", 5.f, Color(0, 255, 0, 255));
				}
				else if (g_Globals.HitIndex != -1)
				{
					g_Globals.AddNotif("hit wrong target!", 5.f, Color(255, 255, 0, 255));
				}
				else
				{
					bool missed_due_to_resolver = false;

					
					if (lst->ent && lst->ent->IsAlive() && lst->ent->IsPlayer() && lst->ent->GetAnimstate() && !lst->ent->IsDormant())
					{
						g_Ent.SetState(lst->ent, Bullets[0].TargRec);

						Vector Fwd;
						g_Math.AngVec(g_Math.CalcAng(Bullets[0].LocalOrigin, g_Globals.BulletHitPos).NormalizeAngle(), &Fwd);
						Fwd.NormalizeInPlace();
						Fwd = Bullets[0].LocalOrigin + (Fwd * 7000.f);

						CGameTrace trace;
						ray_t ray;
						ray.Init(Bullets[0].LocalOrigin, Fwd);
						trace_filter_one_entity2 filter(lst->ent);
						I::Trace->TraceRay(ray, MASK_SHOT, &filter, &trace);

						if (trace.hit_entity == lst->ent)
						{
							hit_--;
							if (hit_ < 0)
								hit_ = 0;

							g_Globals.AddNotif("missed (" + TargName + ") due to bad resolve! " + Bullets[0].RMode, 5.f, Color(255, 0, 0, 255));
							if (hit_ == 0)
								lst->RData.SideSwitch[Bullets[0].RIndex] = !lst->RData.SideSwitch[Bullets[0].RIndex];

							if (lst->Records.size() > 0)
								g_Ent.SetState(lst->ent, lst->Records.back());
							else
								g_Ent.SetState(lst->ent, lst->NewData);
						}
						else
							g_Globals.AddNotif("missed (" + TargName + ") due to spread!", 5.f, Color(255, 255, 0, 255));

					}
					else
						g_Globals.AddNotif("bad bullet target", 5.f, Color(255, 0, 0, 255));

				}
			}
			else if (g_Globals.HitIndex == -1 && Bullets[0].RIndex == -1)
			{
				g_Globals.AddNotif("missed because???", 5.f, Color(255, 100, 50, 255));
			}
			else if (g_Globals.HitIndex != -1 && Bullets[0].RIndex != -1)
			{
				g_Globals.AddNotif("you hit but not correctly!" + Bullets[0].RMode, 5.f, Color(100, 255, 50, 255));
			}
			else if (g_Globals.HitIndex == -1 && Bullets[0].RIndex != -1)
			{
				g_Globals.AddNotif("missed due to wtf" + Bullets[0].RMode, 5.f, Color(100, 255, 255, 255));
			}

			Bullets.erase(Bullets.begin());
		}

		g_Globals.HitIndex = -1;
		g_Globals.LocalShot = false;
		g_Globals.BulletHitPos = Vector(0, 0, 0);
	}
}
