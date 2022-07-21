#pragma once

#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture
#define	SURF_HINT		0x0100	// make a primary bsp splitter
#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	0x2000	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define	CONTENTS_EMPTY			0		/**< No contents. */
#define	CONTENTS_SOLID			0x1		/**< an eye is never valid in a solid . */
#define	CONTENTS_WINDOW			0x2		/**< translucent, but not watery (glass). */
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		/**< alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't. */
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_MIST			0x40
#define CONTENTS_OPAQUE			0x80		/**< things that cannot be seen through (may be non-solid though). */
#define	LAST_VISIBLE_CONTENTS	0x80
#define ALL_VISIBLE_CONTENTS 	(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED5		0x200
#define CONTENTS_UNUSED6		0x4000
#define CONTENTS_TEAM1			0x800		/**< per team contents used to differentiate collisions. */
#define CONTENTS_TEAM2			0x1000		/**< between players and objects on different teams. */
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000		/**< ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW. */
#define CONTENTS_MOVEABLE		0x4000		/**< hits entities which are MOVETYPE_PUSH (doors, plats, etc) */
#define	CONTENTS_AREAPORTAL		0x8000		/**< remaining contents are non-visible, and don't eat brushes. */
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

/**
* @section currents can be added to any other contents, and may be mixed
*/
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

/**
* @endsection
*/

#define	CONTENTS_ORIGIN			0x1000000	/**< removed before bsping an entity. */
#define	CONTENTS_MONSTER		0x2000000	/**< should never be on a brush, only in game. */
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	/**< brushes to be added after vis leafs. */
#define	CONTENTS_TRANSLUCENT	0x10000000	/**< auto set if any surface has trans. */
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	/**< use accurate hitboxes on trace. */

/**
* @section Trace masks.
*/
#define	MASK_ALL				(0xFFFFFFFF)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 			/**< everything that is normally solid */
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 	/**< everything that blocks player movement */
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 							/**< water physics in these contents */
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 							/**< everything that blocks line of sight for AI, lighting, etc */
#define MASK_OPAQUE_AND_NPCS	(MASK_OPAQUE|CONTENTS_MONSTER)										/**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
#define	MASK_VISIBLE			(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 								/**< everything that blocks line of sight for players */
#define MASK_VISIBLE_AND_NPCS	(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 							/**< everything that blocks line of sight for players, but with monsters added. */
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 	/**< bullets see these as solid */
#define MASK_SHOT_HULL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	/**< non-raycasted weapons see this as solid (includes grates) */
#define MASK_SHOT_PORTAL		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 							/**< hits solids (not grates) and passes through everything else */
#define MASK_SOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 					/**< everything normally solid, except monsters (world+brush only) */
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for player movement, except monsters (world+brush only) */
#define MASK_NPCSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for npc movement, except monsters (world+brush only) */
#define MASK_NPCWORLDSTATIC		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 					/**< just the world, used for route rebuilding */
#define MASK_SPLITAREAPORTAL	(CONTENTS_WATER|CONTENTS_SLIME) 									/**< These are things that can split areaportals */

#define	CS_MASK_SHOOT (MASK_SOLID|CONTENTS_DEBRIS)


struct ray_t 
{
	VectorAligned  m_Start;  // starting point, centered within the extents
	VectorAligned  m_Delta;  // direction + length of the ray
	VectorAligned  m_StartOffset; // Add this to m_Start to Get the actual ray start
	VectorAligned  m_Extents;     // Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_pWorldAxisTransform;
	bool m_IsRay;  // are the extents zero?
	bool m_IsSwept;     // is delta != 0?

	ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& start, Vector const& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis) {
			if (m_Delta[iAxis] != 0.0f) {
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else {
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}

private:
};


struct csurface_t {
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t {
	Vector normal;
	float m_dist;
	BYTE m_type;
	BYTE m_sign_bits;
	BYTE m_pad[2];
};


enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class i_trace_filter {
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class trace_filter : public i_trace_filter {
public:

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle != skip);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* skip;
};

class trace_filter_one_entity : public i_trace_filter {
public:

	bool ShouldHitEntity(void* pEntityHandle, int) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	trace_filter_one_entity(void* pPassEnt1) {
		pEntity = pPassEnt1;
	}

	void* pEntity;
};

class trace_filter_one_entity2 : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	trace_filter_one_entity2(void* pPassEnt1) {
		pEntity = pPassEnt1;
	}
	void* pEntity;
};

class trace_filter_skip_two_entities : public i_trace_filter {
public:

	virtual bool ShouldHitEntity(void* pEntityHandle, int ) {
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	trace_filter_skip_two_entities(void* pPassEnt1, void* pPassEnt2) {
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	void* passentity1;
	void* passentity2;
};

class trace_filter_skip_one_entity : public i_trace_filter {
public:

	virtual bool ShouldHitEntity(void* pEntityHandle, int ) {
		return !(pEntityHandle == passentity1);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	trace_filter_skip_one_entity(void* pPassEnt1) {
		passentity1 = pPassEnt1;
	}

	void* passentity1;
};

class trace_entity : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int ) {
		return !(pEntityHandle == pSkip1);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	void* pSkip1;
};

class trace_world_only : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int ) {
		return false;
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector         startpos;            // start position
	Vector         endpos;              // final position
	cplane_t       plane;               // surface normal at impact

	float          fraction;            // time completed, 1.0 = didn't hit anything

	int            contents;            // contents on other side of surface hit
	unsigned short dispFlags;           // displacement flags for marking surfaces with data

	bool           allsolid;            // if true, plane is not valid
	bool           startsolid;          // if true, the initial point was in a solid area

	CBaseTrace() {}

};

class CGameTrace : public CBaseTrace // from gladiator cheats v2 not sure if sources class is the same dosnt matter
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;

public:

	float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
	csurface_t          surface;            // surface hit (impact surface)
	int                 hitgroup;           // 0 == generic, non-zero is specific body part
	short               physicsbone;        // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
	void* hit_entity;
	int                 hitbox;                       // box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& other) :
		fractionleftsolid(other.fractionleftsolid),
		surface(other.surface),
		hitgroup(other.hitgroup),
		physicsbone(other.physicsbone),
		worldSurfaceIndex(other.worldSurfaceIndex),
		hit_entity(other.hit_entity),
		hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}


class collideable_t;
class ITraceRay {
public:
	virtual int GetPointContents(const Vector& pos, int mask = MASK_ALL, IHandleEntity** ent = nullptr) = 0;
	virtual int GetPointContentsWorld(const Vector& pos, int mask = MASK_ALL) = 0;
	virtual int GetPointContentsCollideable(collideable_t* collide, const Vector& pos) = 0;
	virtual void ClipRayToEntity(const ray_t& ray, unsigned int mask, C_BaseEntity* ent, CGameTrace* trace) = 0;
	virtual void ClipRayToCollideable(const ray_t& ray, unsigned int mask, collideable_t* collide, CGameTrace* trace) = 0;
	virtual void TraceRay(const ray_t& ray, unsigned int mask, i_trace_filter* filter, CGameTrace* trace) = 0;
};