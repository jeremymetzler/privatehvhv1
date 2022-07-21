#pragma once

enum hitgroups {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_neck = 8,
	hitgroup_gear = 10
};

class Autowall
{
public:
	struct FireBulletData
	{
		Vector src;
		Vector dest;
		CGameTrace enter_trace;
		Vector direction;
		i_trace_filter* filter = nullptr;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	}Data;

	void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr);
	bool VecToVecVisible(const Vector& src, const Vector& point);
	float GetDamage(C_BaseCombatWeapon* pWeapon, const Vector& src, const Vector& vecPoint );
	float HitFloatingPoint(C_BaseCombatWeapon* pWeapon, const Vector& source, const Vector& point);
	bool SimulateFireBullet(C_BaseCombatWeapon* pWeapon, bool teamCheck);
	bool HandleBulletPenetration(SWeaponInfo* weaponInfo, bool mod = false);
	bool IsBreakableEntity(C_BaseEntity* pEnt);
	bool TraceToExit(Vector& end, CGameTrace* enter_trace, Vector start, Vector dir, CGameTrace* exit_trace, bool mod = false);
	void ScaleDamage(int hitgroup, C_BaseEntity* enemy, float weapon_armor_ratio, float& current_damage);
	float GetHitgroupDamageMultiplier(int iHitGroup);
};

extern Autowall g_Autowall;