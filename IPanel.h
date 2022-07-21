#pragma once

class IPanel {

	using PanelNamefn = const char* (__thiscall*)(IPanel*, unsigned int);
public:
	const char* GetPanelName(unsigned int panel_id) 
	{
		return g_Utils.GetVFunc<PanelNamefn>(this, 36)(this, panel_id);
	}
};