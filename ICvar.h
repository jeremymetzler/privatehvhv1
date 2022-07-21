#pragma once

enum cvar_flags {
	fcvar_none = 0,
	fcvar_unregistered = (1 << 0),
	fcvar_developmentonly = (1 << 1),
	fcvar_gamedll = (1 << 2),
	fcvar_clientdll = (1 << 3),
	fcvar_hidden = (1 << 4),
	fcvar_protected = (1 << 5),
	fcvar_sponly = (1 << 6),
	fcvar_archive = (1 << 7),
	fcvar_notify = (1 << 8),
	fcvar_userinfo = (1 << 9),
	fcvar_printableonly = (1 << 10),
	fcvar_unlogged = (1 << 11),
	fcvar_never_as_string = (1 << 12),
	fcvar_replicated = (1 << 13),
	fcvar_cheat = (1 << 14),
	fcvar_ss = (1 << 15),
	fcvar_demo = (1 << 16),
	fcvar_dontrecord = (1 << 17),
	fcvar_ss_added = (1 << 18),
	fcvar_release = (1 << 19),
	fcvar_reload_materials = (1 << 20),
	fcvar_reload_textures = (1 << 21),
	fcvar_not_connected = (1 << 22),
	fcvar_material_system_thread = (1 << 23),
	fcvar_archive_xbox = (1 << 24),
	fcvar_accessible_from_threads = (1 << 25),
	fcvar_server_can_execute = (1 << 28),
	fcvar_server_cannot_query = (1 << 29),
	fcvar_clientcmd_can_execute = (1 << 30),
	fcvar_unused = (1 << 31),
	fcvar_material_thread_mask = (fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread)
};
// callback prototypes.
using ChangeCallbackV1_t = void(__cdecl*)();
using ChangeCallback_t = void(__cdecl*)(void* var, const char* old, float flold);

// do not use these, use the virtuals, please!
struct CVValue_t {
	char* m_string;
	int   m_str_len;
	float m_float;
	int   m_int;
};

class convar {
private:
	void* vtable; // 0x0000
public:
	convar* m_next; // 0x0004
	int                              m_registered; // 0x0008
	char* m_name;	// 0x000C
	char* m_help_string;// 0x0010
	int                              m_flags;// 0x0014
	ChangeCallbackV1_t				 m_callback;//0x0018
	convar* m_parent;
	char* m_default_value;
	CVValue_t                        m_value;
	int                              m_has_min;
	float                            m_min;
	int                              m_has_max;
	float                            m_max;
	CUtlVector< ChangeCallback_t >   m_callbacks;

	__forceinline float ReadFloat()
	{
		return g_Utils.GetVFunc<ReadFloatFn>(this, 12)(this);
	}

	__forceinline int ReadInt()
	{
		return g_Utils.GetVFunc<ReadIntFn>(this, 13)(this);
	}

	__forceinline void SetFloat(float val)
	{
		g_Utils.GetVFunc<SetFloatFn>(this, 15)(this, val);
	}

	__forceinline void SetInt(int val)
	{
		g_Utils.GetVFunc<SetIntFn>(this, 16)(this, val);
	}

/*	__forceinline void SetFloat(int val)
	{
		typedef void(__thiscall* set_i)(void*, int);
		utilities::call_virtual<set_i>(this, 16)(this, val);
	}*/

	//	VF(read_float, float, 12);

	//	VF(read_int, int, 13);

//	VF(set_string, void, 14, const char* value);

	//	VF(set_float, void, 15, float value);

	//	VF(set_int, void, 16, int value);
private:

	using ReadFloatFn = float(__thiscall*)(void*);
	using ReadIntFn = int(__thiscall*)(void*);
	using SetFloatFn = void(__thiscall*)(void*, float);
	using SetIntFn = void(__thiscall*)(void*, int);
};

class ICvar
{

	using FindVarFn = convar * (__thiscall*)(void*, const char*);
public:

	__forceinline convar* FindVar(const char* var_name)
	{
		return g_Utils.GetVFunc<FindVarFn>(this, 14)(this, var_name);
	}
};