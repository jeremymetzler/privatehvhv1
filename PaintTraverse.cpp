#include "Main.h"

PaintTraverseFn oPaintTraverse;

std::deque<CMenu*>MenuCollection = std::deque<CMenu*>();

void __fastcall Hooks::PaintTraverse(void* ecx, void* edx, unsigned int iPanel, bool bForceRepaint, bool bAllowForce)
{

	static unsigned int vguiMatSystemTopPanelID = NULL;
	static unsigned int vguiHudZoomID = NULL;

	if (vguiHudZoomID && g_Vars.Cfg.noscope && vguiHudZoomID == iPanel)
		return;

	oPaintTraverse(ecx, edx, iPanel, bForceRepaint, bAllowForce);

	if (I::EntityList && I::EntityList->GetHighestEntityIndex() == -1) // disconnected
	{
		if (g_Globals.GameReady)
			g_Globals.GameReady = false;
		if (g_Ent.List.size() != 0)
		{
			for (EntInfo& lst : g_Ent.List)
			{
				lst.ent = nullptr;
				lst.index = -1;
				if (lst.Records.size() != 0)
					lst.Records.clear();
			}
			g_Ent.LstUpdate();
			g_Ent.List.clear();
		}
	}

	if (!vguiHudZoomID && !strcmp("HudZoom", I::Panel->GetPanelName(iPanel)))
		vguiHudZoomID = iPanel;
	if (!vguiMatSystemTopPanelID && !strcmp("MatSystemTopPanel", I::Panel->GetPanelName(iPanel)))
		vguiMatSystemTopPanelID = iPanel;
	else if (iPanel != vguiMatSystemTopPanelID)
		return;

	static int iClick = 0;
	iClick = ((g_Globals.KeyHeld == VK_LBUTTON) ? VK_LBUTTON : ((g_Globals.KeyHeld == VK_RBUTTON) ? VK_RBUTTON : 0));

	if (!g_Menu)
	{	
		MenuCollection.clear();

		g_Menu = new CMenu(10, 10, 575, 300, "private hvh v1");

		g_Menu->MousePosPtr = &g_Globals.MousePos;
		g_Menu->ClickPtr = &iClick;

		g_Menu->Init();

		g_Menu->Controls.push_back(new CGroupBox(0.25, "aim", g_Menu));
		{
			g_Menu->Controls.push_back(new CCheckBox("aimbot", &g_Vars.Cfg.aimbot, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("autostop", &g_Vars.Cfg.autostop, g_Menu));
			g_Menu->Controls.push_back(new CComboBox("resolver", { "off","lby", "1.0" }, &g_Vars.Cfg.resolver, g_Menu));
			g_Menu->Controls.push_back(new CMultiBox("baim", {"lethal"}, g_Vars.Cfg.baim, g_Menu));
			g_Menu->Controls.push_back(new CMultiBox("backtrack", { "lby", "pos"}, g_Vars.Cfg.backtrack, g_Menu));
			g_Menu->Controls.push_back(new CMultiBox("hitbox", {"head", "chest", "pelvis", "arms", "legs", "feet"}, g_Vars.Cfg.hitboxes, g_Menu));
			g_Menu->Controls.push_back(new CSlider("mindmg", 0.f, 114.f, &g_Vars.Cfg.mindmg, g_Menu));
			g_Menu->Controls.push_back(new CSlider("hitchance", 0.f, 100.f, &g_Vars.Cfg.hitchance, g_Menu));
			g_Menu->Controls.push_back(new CSlider("pointscale", 0.f, 1.f, &g_Vars.Cfg.pointscale, g_Menu));

		}
		g_Menu->ControlSearch("aim")->Init();

		g_Menu->Controls.push_back(new CGroupBox(0.25, "antiaim", g_Menu));
		{
			g_Menu->Controls.push_back(new CCheckBox("antiaim", &g_Vars.Cfg.antiaim, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("flipaa", &g_Vars.Cfg.flipaa, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("lagpeek", &g_Vars.Cfg.lagpeek, g_Menu));
			g_Menu->Controls.push_back(new CSlider("fakelag", 0.f, 17.f, &g_Vars.Cfg.fakelag, g_Menu));
		}
		g_Menu->ControlSearch("antiaim")->Init();

		g_Menu->Controls.push_back(new CGroupBox(0.25, "misc", g_Menu));
		{
			g_Menu->Controls.push_back(new CCheckBox("bhop", &g_Vars.Cfg.bhop, g_Menu));
			g_Menu->Controls.push_back(new CSlider("strafe range", 0.f, 100.f, &g_Vars.Cfg.strafeang, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("thirdperson", &g_Vars.Cfg.thirdperson, g_Menu));
			reinterpret_cast<CCheckBox*>(g_Menu->Controls.back())->Keybind = 'V';
			g_Menu->Controls.push_back(new CSlider("tpdistance", 60.f, 200.f, &g_Vars.Cfg.tpdistance, g_Menu));

			g_Menu->Controls.push_back(new CCheckBox("hitsounds", &g_Vars.Cfg.hitsound, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("impacts", &g_Vars.Cfg.impacts, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("no-zoom", &g_Vars.Cfg.nozoom, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("no-scope", &g_Vars.Cfg.noscope, g_Menu));
		}
		g_Menu->ControlSearch("misc")->Init();

		g_Menu->Controls.push_back(new CGroupBox(0.25, "visuals", g_Menu));
		{
			g_Menu->Controls.push_back(new CCheckBox("drawbox", &g_Vars.Cfg.drawbox, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("health bar", &g_Vars.Cfg.healthbar, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("name", &g_Vars.Cfg.name, g_Menu)); 
			g_Menu->Controls.push_back(new CCheckBox("weapon", &g_Vars.Cfg.weapon, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("resolve info", &g_Vars.Cfg.resolveinfo, g_Menu));
			g_Menu->Controls.push_back(new CCheckBox("no-recoil", &g_Vars.Cfg.novisrecoil, g_Menu));
			g_Menu->Controls.push_back(new CSlider("fov", 0.f, 144.f, &g_Vars.Cfg.fov, g_Menu));
			g_Menu->Controls.push_back(new CComboBox("chams", { "off","tansparent" }, &g_Vars.Cfg.chams, g_Menu));
		}
		g_Menu->ControlSearch("visuals")->Init();
/*
		g_Menu->Controls.push_back(new CGroupBox(0.25, "test", g_Menu));
		{
			g_Menu->Controls.push_back(new CComboBox("hitbx", {"head","neck","Lneck","pelvis","stomach","Lchest","chest","Uchest","Rthigh","Lthigh","Rcalf","Lcalf","Rfoot","Lfoot","Rhand","Lhand","RUarm","RLarm","LUarm","LLarm" }, &g_Vars.Cfg.hitbx, g_Menu));
			g_Menu->Controls.push_back(new CSlider("fwd", 0.f, 7.f, &g_Vars.Cfg.fwd, g_Menu));
			g_Menu->Controls.push_back(new CSlider("left", 0.f, 7.f, &g_Vars.Cfg.left, g_Menu));
			g_Menu->Controls.push_back(new CSlider("right", 0.f, 7.f, &g_Vars.Cfg.right, g_Menu));
			g_Menu->Controls.push_back(new CSlider("back", 0.f, 7.f, &g_Vars.Cfg.back, g_Menu));
			g_Menu->Controls.push_back(new CSlider("up", 0.f, 7.f, &g_Vars.Cfg.up, g_Menu));
			g_Menu->Controls.push_back(new CSlider("down", 0.f, 7.f, &g_Vars.Cfg.down, g_Menu));

		}
		g_Menu->ControlSearch("test")->Init();*/

		
		MenuCollection.push_back(g_Menu);
	}

	static CMenu* g_CfgMenu = nullptr;
	static bool save = false, load = false;
	if (!g_CfgMenu)
	{
		int x = 0, y= 0;
		I::Surface->GetScreenSize(x, y);
		g_CfgMenu = new CMenu(x -100, y - 100, 100, 100, "cfgs");

		g_CfgMenu->MousePosPtr = &g_Globals.MousePos;
		g_CfgMenu->ClickPtr = &iClick;

		g_CfgMenu->Init();

		g_CfgMenu->Controls.push_back(new CGroupBox(1, "v", g_CfgMenu));
		{
			g_CfgMenu->Controls.push_back(new CCheckBox("Save", &save, g_CfgMenu));
			g_CfgMenu->Controls.push_back(new CCheckBox("Load", &load, g_CfgMenu));
		}
		g_CfgMenu->ControlSearch("v")->Init();

		MenuCollection.push_back(g_CfgMenu);
	}


	if (g_CfgMenu)
	{
		if (save)
		{
			reinterpret_cast<CCheckBox*>(g_CfgMenu->ControlSearch("Save"))->Switch = true;
			save_config();
		}
		if (load)
		{
			reinterpret_cast<CCheckBox*>(g_CfgMenu->ControlSearch("Load"))->Switch = true;
			load_config();
		}
	}
		
	static int CurrentMainMenu = 0;
	static int ScreenSize[2] = {0,0};
	if (g_Vars.Cfg.noscope && g_Globals.LocalPlayer && g_Globals.LocalPlayer->IsAlive() && g_Globals.LocalPlayer->IsScoped())
	{
		if (ScreenSize[0] == 0)
			I::Engine->GetScreenSize(ScreenSize[0], ScreenSize[1]);

		static int Midx = ScreenSize[0] / 2;
		static int Midy = ScreenSize[1] / 2;

		CDLine(0, Midy, ScreenSize[0], Midy, Color(0, 0, 0, 255)).Render();
		CDLine(Midx, 0, Midx, ScreenSize[1], Color(0, 0, 0, 255)).Render();
	}

	g_Visuals.ESP();

	for (int i = 0; i < MenuCollection.size(); i++)
		if (i == CurrentMainMenu)
			continue;
		else
			MenuCollection[i]->Render(!load);

	CMenu* MainMenu = MenuCollection[CurrentMainMenu];

	MainMenu->Render();

	if (iClick == VK_LBUTTON && !MainMenu->MouseInRegion(MainMenu->x, MainMenu->y, MainMenu->w, MainMenu->h) && MainMenu->FocusControlIndex == -1)
	{
		for (int i = 0; i < MenuCollection.size(); i++)
		{
			if (MenuCollection[i]->MouseInRegion(MenuCollection[i]->x, MenuCollection[i]->y, MenuCollection[i]->w, MenuCollection[i]->h))
			{
				CurrentMainMenu = i;
				break;
			}
		}
	}

	if (g_Globals.KeyDown == 'G')
		g_Globals.AddNotif(std::to_string(g_Misc.LerpTime()), 0.f);

	if (g_Globals.Notif.size() != 0)
	{
		int i = 0;
		for (NMsg& msg : g_Globals.Notif)
		{
			CDText(10, 20 + (i * 10), msg.text, g_Menu->MainFont, msg.clr).Render();
			i += 1;
		}
		if (g_Globals.Notif.front().time < I::Globals->realtime)
		{
			g_Globals.Notif.front().text = "echo " + g_Globals.Notif.front().text;
			I::Engine->ExecuteClientCmd(g_Globals.Notif.front().text.c_str());
			g_Globals.Notif.pop_front();
		}
	}

	g_Globals.KeyDown = 0;
}