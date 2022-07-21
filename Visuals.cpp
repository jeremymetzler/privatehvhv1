#include "Main.h"

Visuals g_Visuals = Visuals();

static uint8_t secondaryclr[] = { 150,100,119,200 };
static uint8_t primaryclr[] = { 255,255,255,150 };

void Visuals::DrawHitboxes()
{
	if (!g_Vars.Cfg.chams)
		return;
	for (EntInfo& lst : g_Ent.List)
	{
		if (!lst.ent || !lst.ent->IsValid())
			continue;
		
		if (lst.NewData.bones)
		{
			for (int i = 0; i < hitbox_max; i++)
			{
				Vector ang, pos;
				g_Math.MatrixAngles(lst.NewData.bones[lst.HBIndxToBone[i]], ang, pos);

				if ((lst.ent->GetFlags() & FL_ONGROUND))
				{
					if (i == hitbox_left_foot)
						ang.x += 25;
					else if (i == hitbox_right_foot)
						ang.x -= 25;
				}

				if (ang.IsValid() && pos.IsValid())
					I::DebugOverlay->AddBoxOverlaySecond(pos, g_Vars.Off.MaxsMins[i][0], g_Vars.Off.MaxsMins[i][1], ang, primaryclr, secondaryclr, -1.f);
			}
		}		
	}
}

bool Visuals::BoundBox(EntInfo& pent)
{
	int index = pent.ent->GetIndex();

	if (!pent.NewData.bones)
		return false;

	int left = 999999, top = 999999, right = -999999, bottom = -999999;
	static Vector w2s;

	int best_hitboxes[4];
/*
	int CurCtrl = g_Vars.Cfg.hitbx;
	g_Aimbot.DirFwdAmnt[CurCtrl][0] = g_Vars.Cfg.fwd;
	g_Aimbot.DirFwdAmnt[CurCtrl][1] = g_Vars.Cfg.left;
	g_Aimbot.DirFwdAmnt[CurCtrl][2] = g_Vars.Cfg.right;
	g_Aimbot.DirFwdAmnt[CurCtrl][3] = g_Vars.Cfg.back;
	g_Aimbot.DirFwdAmnt[CurCtrl][4] = g_Vars.Cfg.up;
	g_Aimbot.DirFwdAmnt[CurCtrl][5] = g_Vars.Cfg.down;*/

	for (int i = 0; i < hitbox_max; i++)
	{
		Vector BonePos = pent.NewData.bones[pent.HBIndxToBone[i]].GetOrigin();
		if (g_Utils.WorldToScreen(BonePos, w2s))
		{
			if (w2s.x < left)
			{
				best_hitboxes[0] = i;
				left = w2s.x;
			}

			if (w2s.x > right)
			{
				best_hitboxes[1] = i;
				right = w2s.x;
			}

			if (w2s.y < top)
			{
				best_hitboxes[2] = i;
				top = w2s.y;
			}

			if (w2s.y > bottom)
			{
				best_hitboxes[3] = i;
				bottom = w2s.y;
			}
		}
		else
		{
			return false;
		}
/*
		if (i == hitbox_head)
		{
			Vector Ang = g_Math.CalcAng(pent.NewData.bones[pent.HBIndxToBone[hitbox_neck]].GetOrigin(),BonePos).NormalizeAngle();
			Vector fwd, right, up;
			Ang.x = g_Math.NormEuler(Ang.x + 78.f);
			g_Math.AngVec(Ang, &fwd, &right, &up);
			fwd.NormalizeInPlace();
			right.NormalizeInPlace();
			up.NormalizeInPlace();
			g_Utils.WorldToScreen(BonePos + fwd * g_Aimbot.DirFwdAmnt[i][0], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - right * g_Aimbot.DirFwdAmnt[i][1], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + right * g_Aimbot.DirFwdAmnt[i][2], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - fwd * g_Aimbot.DirFwdAmnt[i][3], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + up * g_Aimbot.DirFwdAmnt[i][4], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - up * g_Aimbot.DirFwdAmnt[i][5], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();

		}

		if (i == hitbox_pelvis)
		{
			Vector Ang = g_Math.CalcAng(BonePos, pent.NewData.bones[pent.HBIndxToBone[hitbox_neck]].GetOrigin()).NormalizeAngle();
			Ang.x = g_Math.NormEuler(Ang.x + 90.f);
			Vector fwd, right, up;

			g_Math.AngVec(Ang, &fwd, &right, &up);
			fwd.NormalizeInPlace();
			right.NormalizeInPlace();
			up.NormalizeInPlace();
			g_Utils.WorldToScreen(BonePos + fwd * g_Aimbot.DirFwdAmnt[i][0], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - right * g_Aimbot.DirFwdAmnt[i][1], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + right * g_Aimbot.DirFwdAmnt[i][2], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - fwd * g_Aimbot.DirFwdAmnt[i][3], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + up * g_Aimbot.DirFwdAmnt[i][4], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - up * g_Aimbot.DirFwdAmnt[i][5], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();

		}

		if (i == hitbox_chest)
		{
			Vector Ang = g_Math.CalcAng(BonePos, pent.NewData.bones[pent.HBIndxToBone[hitbox_neck]].GetOrigin()).NormalizeAngle();
			Ang.x = g_Math.NormEuler(Ang.x + 90.f);
			Vector fwd, right, up;

			g_Math.AngVec(Ang, &fwd, &right, &up);
			fwd.NormalizeInPlace();
			right.NormalizeInPlace();
			up.NormalizeInPlace();
			g_Utils.WorldToScreen(BonePos + fwd * g_Aimbot.DirFwdAmnt[i][0], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - right * g_Aimbot.DirFwdAmnt[i][1], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + right * g_Aimbot.DirFwdAmnt[i][2], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - fwd * g_Aimbot.DirFwdAmnt[i][3], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos + up * g_Aimbot.DirFwdAmnt[i][4], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();
			g_Utils.WorldToScreen(BonePos - up * g_Aimbot.DirFwdAmnt[i][5], w2s);
			CDFilledRect(w2s.x - 2, w2s.y - 2, 4, 4, Color(255, 255, 255, 255)).Render();

		}*/
	}

	Vector cam_ang = g_Globals.ViewAng;

	static Vector bruh[4];

	g_Math.AngVec(Vector(0.f, g_Math.NormEuler(cam_ang.y + 90.f), 0.f), &bruh[0]);
	g_Math.AngVec(Vector(0.f, g_Math.NormEuler(cam_ang.y - 90.f), 0.f), &bruh[1]);
	g_Math.AngVec(Vector(cam_ang.x - 90.f, g_Math.NormEuler(cam_ang.y), 0.f), &bruh[2]);
	g_Math.AngVec(Vector(cam_ang.x + 90.f, g_Math.NormEuler(cam_ang.y), 0.f), &bruh[3]);

	static Vector OK[4];

	for (int i = 0; i < 4; i++)
		if (g_Utils.WorldToScreen(Vector(pent.NewData.bones[pent.HBIndxToBone[best_hitboxes[i]]].GetOrigin() + Vector(bruh[i] * 20.f)), w2s))
			OK[i] = Vector(w2s.x, w2s.y, 0.f);

	BBox.left = OK[0].x;
	BBox.right = OK[1].x;
	BBox.top = OK[2].y;
	BBox.bottom = OK[3].y;
	return true;
}

void Visuals::DrawBox()
{
	CDRect(BBox.left, BBox.top, BBox.right - BBox.left, BBox.bottom - BBox.top, Color(0,0,0,255)).Render();
	CDRect(BBox.left - 1, BBox.top - 1, BBox.right - BBox.left + 2, BBox.bottom - BBox.top + 2, g_Menu->Draw[2]->clr).Render();
	CDRect(BBox.left - 2, BBox.top - 2, BBox.right - BBox.left + 4, BBox.bottom - BBox.top + 4, g_Menu->Draw[2]->clr).Render();
	CDRect(BBox.left - 3, BBox.top - 3, BBox.right - BBox.left + 6, BBox.bottom - BBox.top + 6, Color(0, 0, 0, 255)).Render();

	BBox.left -= 3;
	BBox.top -= 3;
	BBox.right += 3;
	BBox.bottom += 3;
}

void Visuals::DrawHealth(C_BaseEntity* ent)
{
	float delta = (static_cast<float>(BBox.bottom - BBox.top) - 2.f) / 100.f;
	int pos = (100 - ent->GetHealth()) * delta;

	CDFilledRect(BBox.left - 10, BBox.top + 1, 3, (BBox.bottom - BBox.top) - 2, Color(0, 0, 0, 150)).Render();

	if (ent->GetHealth() >= 100)
		CDFilledRect(BBox.left - 10, BBox.top + 1, 3, (BBox.bottom - BBox.top) - 2, Color(50, 255, 50, 255)).Render();
	else
		CDFilledRect(BBox.left - 10, BBox.top + pos + 1, 3, ((BBox.bottom - BBox.top) - pos) - 2, Color(50, 255, 50, 255)).Render();

	CDRect(BBox.left - 11, BBox.top, 5, (BBox.bottom - BBox.top), Color(0, 0, 0, 255)).Render();
}

void Visuals::ESP()
{
	if (I::Engine->IsInGame() && I::Engine->IsConnected()) {
		for (EntInfo& lst : g_Ent.List)
		{
			if (!lst.ent || !lst.ent->IsValid())
				continue;

			if (BoundBox(lst))
			{
				YOffset = BBox.top;

				if (g_Vars.Cfg.drawbox)
					DrawBox();

				if (g_Vars.Cfg.healthbar)
					DrawHealth(lst.ent);

				if (g_Vars.Cfg.name)
				{

					PlayerInfo_t pinfo;
					I::Engine->GetPlayerInfo(lst.index, &pinfo);

					if (!pinfo.szName)
						return;

					CDText(static_cast<int>(BBox.left + ((BBox.right - BBox.left) / 2)), static_cast<int>(BBox.top - 20), std::string(pinfo.szName),g_Menu->MainFont, Color(255, 255, 255, 255), 4).Render();
				}

				if (g_Vars.Cfg.weapon)
				{
					C_BaseCombatWeapon* Weapon = lst.ent->GetActiveWeapon();

					if (Weapon)
					{
						SWeaponInfo* wInfo = Weapon->GetWeaponData();

						if (wInfo)
						{
							CDText(BBox.right + 10, YOffset, wInfo->szWeaponName, g_Menu->MainFont, Color(255, 255, 255, 255)).Render();
							YOffset += 20;
						}
					}
				}

				if (g_Vars.Cfg.resolveinfo)
				{
					CDText(BBox.right + 10, YOffset, lst.RData.ResolveMode, g_Menu->MainFont, Color(255, 255, 255, 255)).Render();
					YOffset += 20;
				}

			}
		}
	}
}