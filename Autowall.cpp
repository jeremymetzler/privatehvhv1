#include "Main.h"

Autowall g_Autowall = Autowall();

float Autowall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case hitgroup_head:
		return 4.0f;
	case hitgroup_chest:
	case hitgroup_leftarm:
	case hitgroup_rightarm:
		return 1.0f;
	case hitgroup_stomach:
		return 1.25f;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		return 0.75f;
	default:
		return 1.0f;
	}
}

bool Autowall::VecToVecVisible(const Vector& src, const Vector& point)
{
	CGameTrace trace;
	ray_t ray;
	ray.Init(src, point);
	trace_world_only filter;

	I::Trace->TraceRay(ray, CS_MASK_SHOOT, &filter, &trace);

	if (trace.fraction == 1.0f)
		return true;


	return false;
}

void Autowall::ScaleDamage(int hitgroup, C_BaseEntity* enemy, float weapon_armor_ratio, float& current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);

	if (enemy->GetArmor() > 0)
	{
		if (hitgroup == hitgroup_head)
		{
			if (enemy->HasHelmet())
				current_damage *= weapon_armor_ratio * 0.5f;
		}
		else
			current_damage *= weapon_armor_ratio * 0.5f;
	}
}

using FClassnameIsFn = int(__fastcall*)(void*, const char*);

bool Autowall::IsBreakableEntity(C_BaseEntity* pEnt)
{
	if (!pEnt)
		return false;
	static FClassnameIsFn FClassnameIs = nullptr; 
	if (!FClassnameIs)
	{
		FClassnameIs = reinterpret_cast<FClassnameIsFn>(g_Utils.FindPatternIDA("client.dll", "56 8B F2 85 C9 74 36 8B 01 FF 90 2C 02 ? ? ? ?")); // omg i actually did this on my own o:
	}

	if (FClassnameIs)
	{
		if (FClassnameIs(pEnt, "func_breakable") || FClassnameIs(pEnt, "func_breakable_surf"))
			return true;
	}
	return false;
}

bool Autowall::TraceToExit(Vector& end, CGameTrace* enter_trace, Vector start_pos, Vector dir, CGameTrace* exit_trace, bool mod) // op!!!
{
	Vector start = start_pos;
	float  max_distance = 90.f,
		ray_extension = 4.f,
		current_distance = 0;
	int    first_contents = 0;

	while (current_distance <= max_distance) {
		current_distance += ray_extension;

		start += dir * ray_extension;

		if (!first_contents) {
			first_contents = I::Trace->GetPointContents(start, CS_MASK_SHOOT | (mod ? 0 : CONTENTS_HITBOX), nullptr); /*0x4600400B*/
		}

		int point_contents = I::Trace->GetPointContents(start, CS_MASK_SHOOT | (mod ? 0 : CONTENTS_HITBOX), nullptr);

		if (!(point_contents & CS_MASK_SHOOT) || ((point_contents & CONTENTS_HITBOX) && point_contents != first_contents)) /*0x600400B, *0x40000000*/ {

			end = start - (dir * ray_extension);

			TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exit_trace);

			if (mod)
			{
				if (VecToVecVisible(start, Data.dest))
					return true;
			}
			else if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
				TraceLine(start, start_pos, MASK_SHOT_HULL, reinterpret_cast<C_BaseEntity*>(exit_trace->hit_entity), exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startsolid) {
					start = exit_trace->endpos;

					return true;
				}
			}

			if (exit_trace->DidHit() && !exit_trace->startsolid) {
				bool start_is_nodraw = !!(enter_trace->surface.flags & (SURF_NODRAW));
				bool exit_is_nodraw = !!(exit_trace->surface.flags & (SURF_NODRAW));

				if (exit_is_nodraw && IsBreakableEntity(reinterpret_cast<C_BaseEntity*>(enter_trace->hit_entity)) && IsBreakableEntity(reinterpret_cast<C_BaseEntity*>(exit_trace->hit_entity))) {
					return true;
				}

				if (!exit_is_nodraw || (start_is_nodraw && exit_is_nodraw) && (dir.Dot(exit_trace->plane.normal) <= 1.f)) {
					float mult_amount = exit_trace->fraction * 4.f;

					exit_trace->endpos = start - (dir * mult_amount);

					return true;
				}


			}

			if (!exit_trace->DidHit() || exit_trace->startsolid) {
				if ((enter_trace->hit_entity != nullptr && reinterpret_cast<C_BaseEntity*>(enter_trace->hit_entity)->GetIndex() != 0) && IsBreakableEntity(reinterpret_cast<C_BaseEntity*>(enter_trace->hit_entity))) {
					exit_trace = enter_trace;

					exit_trace->endpos = start_pos + dir;

					return true;
				}
			}			
		}
	}
	return false;
}

bool Autowall::HandleBulletPenetration(SWeaponInfo* weaponInfo, bool mod)
{
	surfacedata_t* enter_surface_data = I::SurfaceProps->GetSurfaceData(Data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	bool          solid_surf = ((Data.enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool          light_surf = ((Data.enter_trace.surface.flags >> 7) & SURF_LIGHT);

	Data.trace_length += Data.enter_trace.fraction * Data.trace_length_remaining;
	Data.current_damage *= powf(weaponInfo->flRangeModifier, Data.trace_length * 0.002f);

	if (Data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		Data.penetrate_count = 0;

	if (Data.penetrate_count <= 0)
		return false;

	Vector dummy;
	CGameTrace trace_exit;

	if (!TraceToExit(dummy, &Data.enter_trace, Data.enter_trace.endpos, Data.direction, &trace_exit, mod))
		return false;

	surfacedata_t* exit_surface_data = I::SurfaceProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.f;
	float combined_penetration_modifier = 0.f;

	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
		combined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}
	else if (light_surf || solid_surf) {
		combined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	else {
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
		final_damage_modifier = 0.16f;
	}

	if (enter_material == exit_material) {
		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD) {
			combined_penetration_modifier = 3.f;
		}

		else if (exit_material == CHAR_TEX_PLASTIC) {
			combined_penetration_modifier = 2.f;
		}
	}

	float thickness = (trace_exit.endpos - Data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= fmaxf(0.f, 1.0f / combined_penetration_modifier);
	thickness /= 24.0f;

	const auto lost_damage = fmaxf(0.0f, Data.current_damage * final_damage_modifier + fmaxf(0.f, 1.0f / combined_penetration_modifier)
		* 3.0f * fmaxf(0.0f, 3.0f / weaponInfo->flPenetration) * 1.25f + thickness);

	if (lost_damage > Data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		Data.current_damage -= lost_damage;

	if (Data.current_damage < 1.0f)
		return false;

	Data.src = trace_exit.endpos;
	Data.penetrate_count--;

	return true;
}

void Autowall::TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr)
{
	ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	trace_filter_skip_one_entity filter(ignore);

	I::Trace->TraceRay(ray, mask, &filter, ptr);
}

bool Autowall::SimulateFireBullet(C_BaseCombatWeapon* pWeapon, bool teamCheck)
{
	SWeaponInfo* weaponInfo = pWeapon->GetWeaponData();

	Data.penetrate_count = 4;
	Data.trace_length = 0.0f;
	Data.current_damage = static_cast<float>(weaponInfo->iDamage);

	while (Data.penetrate_count > 0 && Data.current_damage >= 1.0f)
	{
		Data.trace_length_remaining = weaponInfo->flRange - Data.trace_length;

		Vector end = Data.src + Data.direction * Data.trace_length_remaining;

		TraceLine(Data.src, end, MASK_SHOT_HULL | CONTENTS_HITBOX, g_Globals.LocalPlayer, &Data.enter_trace);

		if (Data.enter_trace.fraction == 1.0f)
			break;

		if (Data.enter_trace.hitgroup <= hitgroup_neck && Data.enter_trace.hitgroup > hitgroup_generic)
		{
			Data.trace_length += Data.enter_trace.fraction * Data.trace_length_remaining;
			Data.current_damage *= powf(weaponInfo->flRangeModifier, Data.trace_length * 0.002f);

			C_BaseEntity* player = reinterpret_cast<C_BaseEntity*>(Data.enter_trace.hit_entity);
			if (teamCheck && player->GetTeam() == g_Globals.LocalTeam)
				return false;

			ScaleDamage(Data.enter_trace.hitgroup, player, weaponInfo->flArmorRatio, Data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponInfo))
			break;
	}

	return false;
}

float Autowall::GetDamage(C_BaseCombatWeapon* pWeapon, const Vector& src, const Vector& point)
{
	float damage = 0.f;
	Data.dest = point;
	Data.src = src;
	if (!Data.filter)
		Data.filter = new trace_filter_skip_one_entity(g_Globals.LocalPlayer);

	Vector angles = g_Math.CalcAng(Data.src, Data.dest);
	g_Math.AngVec(angles, &Data.direction);

	Data.direction.NormalizeInPlace();

	if (SimulateFireBullet(pWeapon, true))
		damage = Data.current_damage;

	return damage;
}

float Autowall::HitFloatingPoint(C_BaseCombatWeapon* pWeapon, const Vector& source, const Vector& point)
{
	SWeaponInfo* weaponInfo = pWeapon->GetWeaponData();

	Data.dest = point;
	Data.src = source;
	if (!Data.filter)
		Data.filter = new trace_filter_skip_one_entity(g_Globals.LocalPlayer);

	Vector angles = g_Math.CalcAng(Data.src, Data.dest);
	g_Math.AngVec(angles, &Data.direction);
	Data.direction.NormalizeInPlace();

	Data.penetrate_count = 4;
	Data.trace_length = 0.0f;
	Data.current_damage = static_cast<float>(weaponInfo->iDamage);

	CGameTrace trace;
	ray_t ray;
	trace_world_only filter;

	while (Data.penetrate_count > 0 && Data.current_damage >= 1.0f)
	{
		Data.trace_length_remaining = weaponInfo->flRange - Data.trace_length;

		Vector end = Data.src + Data.direction * Data.trace_length_remaining;

		ray.Init(Data.src, end);
		I::Trace->TraceRay(ray, CS_MASK_SHOOT, &filter, &Data.enter_trace);

		if (VecToVecVisible(Data.src, point))
		{
			Data.trace_length += Data.enter_trace.fraction * Data.trace_length_remaining;
			Data.current_damage *= powf(weaponInfo->flRangeModifier, Data.trace_length * 0.002f);
			return Data.current_damage;
		}

		if (!HandleBulletPenetration(weaponInfo, true))
		{
			Data.current_damage = 0.f;
			break;
		}
	}

	Data.current_damage = 0.f;
	return Data.current_damage;
}