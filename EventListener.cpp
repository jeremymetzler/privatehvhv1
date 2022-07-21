#include "Main.h"

C_Event EventListener;

void C_Event::FireGameEvent(IGameEvent* pEvent)
{
	if (!pEvent)
		return;

	int index = I::Engine->GetPlayerForUserID(pEvent->GetInt("userid"));

	if (!(index >= 0 && index < 65))
		return;

	if (!pEvent->GetName())
		return;

	int LocalIndex = I::Engine->GetLocalPlayer();

	static bool shot_alrd = false;

	if (!strcmp(pEvent->GetName(), "player_hurt"))
	{
		int attacker_index = I::Engine->GetPlayerForUserID(pEvent->GetInt("attacker"));

		if (attacker_index == LocalIndex && index != LocalIndex)
		{
			g_Globals.HitIndex = index;

			PlayerInfo_t pinfo = {};
			pinfo.userId = -1;
			I::Engine->GetPlayerInfo(index, &pinfo);

			if (pinfo.userId != -1)
			{
				g_Globals.AddNotif( "hit: " + std::string(pinfo.szName) + " -" + std::to_string(pEvent->GetInt("dmg_health")) + "hp" ,  5.f);
			}

			if (g_Vars.Cfg.hitsound)
			{
				std::string Sound = "play physics\\metal\\metal_box_impact_soft" + std::to_string(1 + (rand() % 3));
				I::Engine->ExecuteClientCmd(Sound.c_str());
			}
		}
	}
	else if (!strcmp(pEvent->GetName(), "weapon_fire"))
	{
		if (LocalIndex == index)
		{
			shot_alrd = true;
			g_Globals.LocalShot = true;
		}
	}
	else if (!strcmp(pEvent->GetName(), "bullet_impact"))
	{
		if (LocalIndex == index)
		{
			Vector BulletPos = Vector(pEvent->GetFloat("x"), pEvent->GetFloat("y"), pEvent->GetFloat("z"));

			if (shot_alrd)// first bullet
			{
				g_Globals.BulletHitPos = BulletPos;
				if (g_Vars.Cfg.impacts)
					I::DebugOverlay->AddBoxOverlay(BulletPos, Vector(-2, -2, -2), Vector(2, 2, 2), Vector().Zero(), 100, 255, 100, 127, 4);
				shot_alrd = false;
			}
			else if (g_Vars.Cfg.impacts)
			{
				I::DebugOverlay->AddBoxOverlay(BulletPos, Vector(-2, -2, -2), Vector(2, 2, 2), Vector().Zero(), 255, 255, 255, 127, 4);
			}
		}
	}

	return;
}