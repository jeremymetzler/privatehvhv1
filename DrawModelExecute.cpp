#include "Main.h"

DrawModelExecuteFn oDrawModelExecute;

void __fastcall Hooks::DrawModelExecute(void* ecx, void* edx, i_mat_render_context* context, const draw_model_state_t& state, const model_render_info_t& render_info, matrix3x4_t* matrix)
{
	const char* ModelName = I::ModelInfo->GetModelName(render_info.model);

	if (strstr(ModelName, "models/player/contactshadow"))
		return;

	if (!strstr(ModelName, "models/player"))
		return oDrawModelExecute(ecx, edx, context, state, render_info, matrix);

	int index = g_Ent.LstSearch( render_info.entity_index);

	if (render_info.entity_index == I::Engine->GetLocalPlayer() && g_Globals.LocalPlayer && g_Globals.LocalPlayer->IsAlive() && g_Vars.Cfg.thirdperson)
	{
		static matrix3x4_t absbones[128];
		memcpy(absbones, g_Ent.LocalRecord.bones, (sizeof(matrix3x4_t) * 128));
		for (int i = 0; i < 128; i++)
			absbones[i].SetOrigin(absbones[i].GetOrigin() + (g_Globals.LocalPlayer->GetAbsOrigin() - g_Ent.LocalRecord.vOrigin));
		return oDrawModelExecute(ecx, edx, context, state, render_info, absbones);
	}
	else if (index != -1 && g_Ent.List[index].Records.size() != 0)
	{
		if (g_Vars.Cfg.chams)
			return;
		else
			return oDrawModelExecute(ecx, edx, context, state, render_info, g_Ent.List[index].Records.back().bones);
	}

	return oDrawModelExecute(ecx, edx, context, state, render_info, matrix);
}