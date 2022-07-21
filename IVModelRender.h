#pragma once

class i_mat_render_context;

struct model_t {
	void* handle;
	char	name[260];
	int	load_flags;
	int	server_count;
	int	type;
	int	flags;
	Vector	vec_mins;
	Vector	vec_maxs;
	float	radius;
};

struct model_render_info_t {
	Vector		origin;
	Vector		angles;
	//	char		u0[0x4];
	void* renderable;
	const model_t* model;
	const matrix3x4_t* model_to_world;
	const matrix3x4_t* lighting_offset;
	const Vector* lighting_origin;
	int		flags;
	int		entity_index;
	int		skin;
	int		body;
	int		hitboxset;
	unsigned short	instance;

	model_render_info_t() {
		model_to_world = nullptr;
		lighting_offset = nullptr;
		lighting_origin = nullptr;
	}
};

struct draw_model_state_t {
public:
	studio_hdr_t* studio_hdr;
	studiohwdata_t* studio_hw_data;
	IClientRenderable* renderable;
	const matrix3x4_t* model_to_world;
	studio_decal_handle_t	decals;
	int			drawflags;
	int			lod;
};

class IMaterial;

class IVModelRender {
	using OverridMatFn = void(__thiscall*)(IVModelRender*, IMaterial*, int, int);
public:
	void OverrideMaterial(IMaterial* material) {
		return g_Utils.GetVFunc<OverridMatFn>(this,1)(this, material, 0, 0);
	}
};

class IVModelInfo {
	using GetModelFn = model_t * (__thiscall*)(IVModelInfo*, int);
	using GetModelIndexFn = int(__thiscall*)(IVModelInfo*, const char*);
	using GetModelNameFn = const char* (__thiscall*)(IVModelInfo*, const model_t*);
	using GetStudioModelFn = studio_hdr_t * (__thiscall*)(IVModelInfo*, const model_t*);
public:
	model_t* GetModel(int index) 
	{
		return g_Utils.GetVFunc<GetModelFn>(this, 1)(this, index);
	}
	int GetModelIndex(const char* filename) 
	{
		return  g_Utils.GetVFunc<GetModelIndexFn>(this, 2)(this, filename);
	}
	const char* GetModelName(const model_t* model) 
	{
		return g_Utils.GetVFunc<GetModelNameFn>(this,3)(this, model);
	}
	studio_hdr_t* GetStudioModel(const model_t* model) 
	{
		return g_Utils.GetVFunc<GetStudioModelFn>(this,30)(this, model);
	}
};