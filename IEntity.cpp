#include "Main.h"

C_BaseCombatWeapon* C_BaseEntity::GetActiveWeapon()
{
	ULONG  index = *reinterpret_cast<PULONG>(uintptr_t(this) + g_Vars.Off.m_hActiveWeapon);
	return reinterpret_cast<C_BaseCombatWeapon*>(I::EntityList->GetClientEntityFromHandle(reinterpret_cast<HANDLE>(index)));
}

float SWeaponInfo::FireRate() // dont look here :(
{
	if (!this)
		return 0.f;

	std::string weapon_name = szWeaponName;

	if (weapon_name == "weapon_g3sg1")
		return 0.25f;
	else if (weapon_name == "weapon_scar20")
		return 0.25f;
	else if (weapon_name == "weapon_ssg08")
		return 1.25f;
	else if (weapon_name == "weapon_awp")
		return 1.463f;
	else if (weapon_name == "weapon_ak47")
		return 0.1f;
	else if (weapon_name == "weapon_m4a1_silencer")
		return 0.089f;
	else if (weapon_name == "weapon_m4a1")
		return 0.089f;
	else if (weapon_name == "weapon_glock")
		return 0.15f;
	else if (weapon_name == "weapon_hkp2000")
		return 0.169f;
	else if (weapon_name == "weapon_usp_silencer")
		return 0.169f;
	else if (weapon_name == "weapon_elite")
		return 0.12f;
	else if (weapon_name == "weapon_deagle")
		return 0.224f;
	else if (weapon_name == "weapon_p250")
		return 0.15f;
	else if (weapon_name == "weapon_tec9")
		return 0.12f;
	else if (weapon_name == "weapon_fiveseven")
		return 0.15f;
	else if (weapon_name == "weapon_nova")
		return 0.882f;
	else if (weapon_name == "weapon_sawedoff")
		return 0.845f;
	else if (weapon_name == "weapon_mag7")
		return 0.845f;
	else if (weapon_name == "weapon_xm1014")
		return 0.35f;
	else if (weapon_name == "weapon_mac10")
		return 0.075f;
	else if (weapon_name == "weapon_ump45")
		return 0.089f;
	else if (weapon_name == "weapon_mp9")
		return 0.070f;
	else if (weapon_name == "weapon_bizon")
		return 0.08f;
	else if (weapon_name == "weapon_mp7")
		return 0.08f;
	else if (weapon_name == "weapon_p90")
		return 0.070f;
	else if (weapon_name == "weapon_galilar")
		return 0.089f;
	else if (weapon_name == "weapon_sg556")
		return 0.089f;
	else if (weapon_name == "weapon_aug")
		return 0.089f;
	else if (weapon_name == "weapon_m249")
		return 0.08f;
	else if (weapon_name == "weapon_negev")
		return 0.0008f;
	else if (weapon_name == "weapon_mp5sd")
		return 0.08f;
	else
		return 0.0f;
}