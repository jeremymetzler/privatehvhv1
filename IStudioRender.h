#pragma once

struct studiohwdata_t;
typedef void* studio_decal_handle_t;

struct studio_bone_t {
	int name_index;
	inline char* const name(void) const {
		return ((char*)this) + name_index;
	}
	int parent;
	int bone_controller[6];

	Vector pos;
	Quaternion quat;
	Vector rotation;

	Vector pos_scale;
	Vector rot_scale;

	matrix3x4_t pose_to_bone;
	Quaternion quat_alignement;
	int flags;
	int proc_type;
	int proc_index;
	mutable int physics_bone;

	inline void* procedure() const {
		if (proc_index == 0) return NULL;
		else return(void*)(((unsigned char*)this) + proc_index);
	};

	int surface_prop_idx;
	inline char* const surface_prop(void) const {
		return ((char*)this) + surface_prop_idx;
	}
	inline int get_surface_prop(void) const {
		return surf_prop_lookup;
	}

	int contents;
	int surf_prop_lookup;
	int unused[7];
};

struct studio_box_t {
	int bone;
	int group;
	Vector mins;
	Vector maxs;
	int name_index;
	int pad01[3];
	float radius;
	int pad02[4];
};

struct studio_hitbox_set_t {
	int name_index;
	inline char* const name(void) const {
		return ((char*)this) + name_index;
	}
	int hitbox_count;
	int hitbox_index;

	inline studio_box_t* hitbox(int i) const {
		return (studio_box_t*)(((unsigned char*)this) + hitbox_index) + i;
	}
};

class studio_hdr_t {
public:
	int id;
	int version;
	long checksum;
	char name_char_array[64];
	int length;
	Vector eye_pos;
	Vector illium_pos;
	Vector hull_mins;
	Vector hull_maxs;
	Vector mins;
	Vector maxs;
	int flags;
	int bones_count;
	int bone_index;
	int bone_controllers_count;
	int bone_controller_index;
	int hitbox_sets_count;
	int hitbox_set_index;
	int local_anim_count;
	int local_anim_index;
	int local_seq_count;
	int local_seq_index;
	int activity_list_version;
	int events_indexed;
	int textures_count;
	int texture_index;

	studio_hitbox_set_t* hitbox_set(int i) {
		if (i > hitbox_sets_count) return nullptr;
		return (studio_hitbox_set_t*)((uint8_t*)this + hitbox_set_index) + i;
	}
	studio_bone_t* bone(int i) {
		if (i > bones_count) return nullptr;
		return (studio_bone_t*)((uint8_t*)this + bone_index) + i;
	}
};

class IStudioRender
{
public:
};