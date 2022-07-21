#pragma once


#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800
#define BONE_USED_BY_VERTEX_LOD2        0x00001000
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it
#define BONE_ALWAYS_SETUP			0x00080000

enum
{
	EFL_KILLME = (1 << 0),	// This entity is marked for death -- This allows the game to actually delete ents at a safe time
	EFL_DORMANT = (1 << 1),	// Entity is dormant, no updates to client
	EFL_NOCLIP_ACTIVE = (1 << 2),	// Lets us know when the noclip command is active.
	EFL_SETTING_UP_BONES = (1 << 3),	// Set while a model is setting up its bones.
	EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4), // This is a special entity that should not be deleted when we restart entities only

	EFL_HAS_PLAYER_CHILD = (1 << 4),	// One of the child entities is a player.

	EFL_DIRTY_SHADOWUPDATE = (1 << 5),	// Client only- need shadow manager to update the shadow...
	EFL_NOTIFY = (1 << 6),	// Another entity is watching events on this entity (used by teleport)

	// The default behavior in ShouldTransmit is to not send an entity if it doesn't
	// have a model. Certain entities want to be sent anyway because all the drawing logic
	// is in the client DLL. They can set this flag and the engine will transmit them even
	// if they don't have a model.
	EFL_FORCE_CHECK_TRANSMIT = (1 << 7),

	EFL_BOT_FROZEN = (1 << 8),	// This is set on bots that are frozen.
	EFL_SERVER_ONLY = (1 << 9),	// Non-networked entity.
	EFL_NO_AUTO_EDICT_ATTACH = (1 << 10), // Don't attach the edict; we're doing it explicitly

	// Some dirty bits with respect to abs computations
	EFL_DIRTY_ABSTRANSFORM = (1 << 11),
	EFL_DIRTY_ABSVELOCITY = (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
	//	UNUSED						= (1<<16),

	EFL_IN_SKYBOX = (1 << 17),	// This is set if the entity detects that it's in the skybox.
											// This forces it to pass the "in PVS" for transmission.
											EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),	// Entities with this flag set show up in the partition even when not solid
											EFL_TOUCHING_FLUID = (1 << 19),	// Used to determine if an entity is floating

											// FIXME: Not really sure where I should add this...
											EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
											EFL_NO_ROTORWASH_PUSH = (1 << 21),		// I shouldn't be pushed by the rotorwash
											EFL_NO_THINK_FUNCTION = (1 << 22),
											EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),

											EFL_CHECK_UNTOUCH = (1 << 24),
											EFL_DONTBLOCKLOS = (1 << 25),		// I shouldn't block NPC line-of-sight
											EFL_DONTWALKON = (1 << 26),		// NPC;s should not walk on this entity
											EFL_NO_DISSOLVE = (1 << 27),		// These guys shouldn't dissolve
											EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),	// Mega physcannon can't ragdoll these guys.
											EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),	// Don't adjust this entity's velocity when transitioning into water
											EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),	// Physcannon can't pick these up or punt them
											EFL_NO_DAMAGE_FORCES = (1 << 31),	// Doesn't accept forces from physics damage
};

class C_BaseCombatWeapon;
class model_t;

enum ClassId
{
	ClassId_CAI_BaseNPC = 0,
	ClassId_CAK47,
	ClassId_CBaseAnimating,
	ClassId_CBaseAnimatingOverlay,
	ClassId_CBaseAttributableItem,
	ClassId_CBaseButton,
	ClassId_CBaseCombatCharacter,
	ClassId_CBaseCombatWeapon,
	ClassId_CBaseCSGrenade,
	ClassId_CBaseCSGrenadeProjectile,
	ClassId_CBaseDoor,
	ClassId_CBaseEntity,
	ClassId_CBaseFlex,
	ClassId_CBaseGrenade,
	ClassId_CBaseParticleEntity,
	ClassId_CBasePlayer,
	ClassId_CBasePropDoor,
	ClassId_CBaseTeamObjectiveResource,
	ClassId_CBaseTempEntity,
	ClassId_CBaseToggle,
	ClassId_CBaseTrigger,
	ClassId_CBaseViewModel,
	ClassId_CBaseVPhysicsTrigger,
	ClassId_CBaseWeaponWorldModel,
	ClassId_CBeam,
	ClassId_CBeamSpotlight,
	ClassId_CBoneFollower,
	ClassId_CBreakableProp,
	ClassId_CBreakableSurface,
	ClassId_CC4,
	ClassId_CCascadeLight,
	ClassId_CChicken,
	ClassId_CColorCorrection,
	ClassId_CColorCorrectionVolume,
	ClassId_CCSGameRulesProxy,
	ClassId_CCSPlayer,
	ClassId_CCSPlayerResource,
	ClassId_CCSRagdoll,
	ClassId_CCSTeam,
	ClassId_CDEagle,
	ClassId_CDecoyGrenade,
	ClassId_CDecoyProjectile,
	ClassId_CDynamicLight,
	ClassId_CDynamicProp,
	ClassId_CEconEntity,
	ClassId_CEconWearable,
	ClassId_CEmbers,
	ClassId_CEntityDissolve,
	ClassId_CEntityFlame,
	ClassId_CEntityFreezing,
	ClassId_CEntityParticleTrail,
	ClassId_CEnvAmbientLight,
	ClassId_CEnvDetailController,
	ClassId_CEnvDOFController,
	ClassId_CEnvParticleScript,
	ClassId_CEnvProjectedTexture,
	ClassId_CEnvQuadraticBeam,
	ClassId_CEnvScreenEffect,
	ClassId_CEnvScreenOverlay,
	ClassId_CEnvTonemapController,
	ClassId_CEnvWind,
	ClassId_CFEPlayerDecal,
	ClassId_CFireCrackerBlast,
	ClassId_CFireSmoke,
	ClassId_CFireTrail,
	ClassId_CFish,
	ClassId_CFlashbang,
	ClassId_CFogController,
	ClassId_CFootstepControl,
	ClassId_CFunc_Dust,
	ClassId_CFunc_LOD,
	ClassId_CFuncAreaPortalWindow,
	ClassId_CFuncBrush,
	ClassId_CFuncConveyor,
	ClassId_CFuncLadder,
	ClassId_CFuncMonitor,
	ClassId_CFuncMoveLinear,
	ClassId_CFuncOccluder,
	ClassId_CFuncReflectiveGlass,
	ClassId_CFuncRotating,
	ClassId_CFuncSmokeVolume,
	ClassId_CFuncTrackTrain,
	ClassId_CGameRulesProxy,
	ClassId_CHandleTest,
	ClassId_CHEGrenade,
	ClassId_CHostage,
	ClassId_CHostageCarriableProp,
	ClassId_CIncendiaryGrenade,
	ClassId_CInferno,
	ClassId_CInfoLadderDismount,
	ClassId_CInfoOverlayAccessor,
	ClassId_CItem_Healthshot,
	ClassID_CItemDogTags,
	ClassId_CKnife,
	ClassId_CKnifeGG,
	ClassId_CLightGlow,
	ClassId_CMaterialModifyControl,
	ClassId_CMolotovGrenade,
	ClassId_CMolotovProjectile,
	ClassId_CMovieDisplay,
	ClassId_CParticleFire,
	ClassId_CParticlePerformanceMonitor,
	ClassId_CParticleSystem,
	ClassId_CPhysBox,
	ClassId_CPhysBoxMultiplayer,
	ClassId_CPhysicsProp,
	ClassId_CPhysicsPropMultiplayer,
	ClassId_CPhysMagnet,
	ClassId_CPlantedC4,
	ClassId_CPlasma,
	ClassId_CPlayerResource,
	ClassId_CPointCamera,
	ClassId_CPointCommentaryNode,
	ClassId_CPointWorldText,
	ClassId_CPoseController,
	ClassId_CPostProcessController,
	ClassId_CPrecipitation,
	ClassId_CPrecipitationBlocker,
	ClassId_CPredictedViewModel,
	ClassId_CProp_Hallucination,
	ClassId_CPropDoorRotating,
	ClassId_CPropJeep,
	ClassId_CPropVehicleDriveable,
	ClassId_CRagdollManager,
	ClassId_CRagdollProp,
	ClassId_CRagdollPropAttached,
	ClassId_CRopeKeyframe,
	ClassId_CSCAR17,
	ClassId_CSceneEntity,
	ClassId_CSensorGrenade,
	ClassId_CSensorGrenadeProjectile,
	ClassId_CShadowControl,
	ClassId_CSlideshowDisplay,
	ClassId_CSmokeGrenade,
	ClassId_CSmokeGrenadeProjectile,
	ClassId_CSmokeStack,
	ClassId_CSpatialEntity,
	ClassId_CSpotlightEnd,
	ClassId_CSprite,
	ClassId_CSpriteOriented,
	ClassId_CSpriteTrail,
	ClassId_CStatueProp,
	ClassId_CSteamJet,
	ClassId_CSun,
	ClassId_CSunlightShadowControl,
	ClassId_CTeam,
	ClassId_CTeamplayRoundBasedRulesProxy,
	ClassId_CTEArmorRicochet,
	ClassId_CTEBaseBeam,
	ClassId_CTEBeamEntPoint,
	ClassId_CTEBeamEnts,
	ClassId_CTEBeamFollow,
	ClassId_CTEBeamLaser,
	ClassId_CTEBeamPoints,
	ClassId_CTEBeamRing,
	ClassId_CTEBeamRingPoint,
	ClassId_CTEBeamSpline,
	ClassId_CTEBloodSprite,
	ClassId_CTEBloodStream,
	ClassId_CTEBreakModel,
	ClassId_CTEBSPDecal,
	ClassId_CTEBubbles,
	ClassId_CTEBubbleTrail,
	ClassId_CTEClientProjectile,
	ClassId_CTEDecal,
	ClassId_CTEDust,
	ClassId_CTEDynamicLight,
	ClassId_CTEEffectDispatch,
	ClassId_CTEEnergySplash,
	ClassId_CTEExplosion,
	ClassId_CTEFireBullets,
	ClassId_CTEFizz,
	ClassId_CTEFootprintDecal,
	ClassId_CTEFoundryHelpers,
	ClassId_CTEGaussExplosion,
	ClassId_CTEGlowSprite,
	ClassId_CTEImpact,
	ClassId_CTEKillPlayerAttachments,
	ClassId_CTELargeFunnel,
	ClassId_CTEMetalSparks,
	ClassId_CTEMuzzleFlash,
	ClassId_CTEParticleSystem,
	ClassId_CTEPhysicsProp,
	ClassId_CTEPlantBomb,
	ClassId_CTEPlayerAnimEvent,
	ClassId_CTEPlayerDecal,
	ClassId_CTEProjectedDecal,
	ClassId_CTERadioIcon,
	ClassId_CTEShatterSurface,
	ClassId_CTEShowLine,
	ClassId_CTesla,
	ClassId_CTESmoke,
	ClassId_CTESparks,
	ClassId_CTESprite,
	ClassId_CTESpriteSpray,
	ClassId_CTest_ProxyToggle_Networkable,
	ClassId_CTestTraceline,
	ClassId_CTEWorldDecal,
	ClassId_CTriggerPlayerMovement,
	ClassId_CTriggerSoundOperator,
	ClassId_CVGuiScreen,
	ClassId_CVoteController,
	ClassId_CWaterBullet,
	ClassId_CWaterLODControl,
	ClassId_CWeaponAug,
	ClassId_CWeaponAWP,
	ClassId_CWeaponBaseItem,
	ClassId_CWeaponBizon,
	ClassId_CWeaponCSBase,
	ClassId_CWeaponCSBaseGun,
	ClassId_CWeaponCycler,
	ClassId_CWeaponElite,
	ClassId_CWeaponFamas,
	ClassId_CWeaponFiveSeven,
	ClassId_CWeaponG3SG1,
	ClassId_CWeaponGalil,
	ClassId_CWeaponGalilAR,
	ClassId_CWeaponGlock,
	ClassId_CWeaponHKP2000,
	ClassId_CWeaponM249,
	ClassId_CWeaponM3,
	ClassId_CWeaponM4A1,
	ClassId_CWeaponMAC10,
	ClassId_CWeaponMag7,
	ClassId_CWeaponMP5Navy,
	ClassId_CWeaponMP7,
	ClassId_CWeaponMP9,
	ClassId_CWeaponNegev,
	ClassId_CWeaponNOVA,
	ClassId_CWeaponP228,
	ClassId_CWeaponP250,
	ClassId_CWeaponP90,
	ClassId_CWeaponSawedoff,
	ClassId_CWeaponSCAR20,
	ClassId_CWeaponScout,
	ClassId_CWeaponSG550,
	ClassId_CWeaponSG552,
	ClassId_CWeaponSG556,
	ClassId_CWeaponSSG08,
	ClassId_CWeaponTaser,
	ClassId_CWeaponTec9,
	ClassId_CWeaponTMP,
	ClassId_CWeaponUMP45,
	ClassId_CWeaponUSP,
	ClassId_CWeaponXM1014,
	ClassId_CWorld,
	ClassId_DustTrail,
	ClassId_MovieExplosion,
	ClassId_ParticleSmokeGrenade,
	ClassId_RocketTrail,
	ClassId_SmokeTrail,
	ClassId_SporeExplosion,
	ClassId_SporeTrail,
	MAX_CLASSID
};

class C_CommandContext
{
public:
	CUtlVector< CUserCmd > cmds;

	int				numcmds;
	int				totalcmds;
	int				dropped_packets;
	bool			paused;
};

class C_PlayerAnimstate
{
public:
	char pad[0x3]; //3
	char bUnknown; //0x4
	char pad2[0x4E]; //74
	float m_flAnimUpdateDelta; //0x54 not atually the delta idfk what it is i was just sent this class but im p sure its always 0
	char pad3[0x8]; //8
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flDelta; //0x74 im guessing this is the real update time delta
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[0x4]; //4
	float m_flUnknownFloat1; //0xD4
	char pad6[0x8]; //8
	float m_flUnknownFloat2; //0xE0
	float m_flUnknownFloat3; //0xE4
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4
	float m_flFeetSpeedForwardsOrSideWays; //0xF8
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	bool m_bOnGround; //0x108
	char pad7[0x7]; //7
	float m_flTimeSinceInAir; //0x110
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118
	float m_flStopToFullRunningFraction; //0x11C
	char pad8[0x4]; //4
	float m_flMagicFraction; //0x0124
	char pad9[0x3C]; //60
	float m_flWorldForce; //0x0164
};

class C_AnimationLayer
{
public:
	char pad[0x14];
	unsigned int m_iOrder; //0x014
	unsigned int m_iSequence; //0x18
	float m_flPrevCycle; //0x1C
	float m_flWeight; //0x20
	float m_flWeightDeltaRate; //0x24
	float m_flPlaybackRate; //0x28
	float m_flCycle; //0x2C
	void* m_pOwner; //0x30
	char pad2[0x4];
};

/*
velocity isnt networked
tickbase isnt networked

simtime is set to curtime after using setabspos
*/
class C_BaseEntity
{
private:
	template<class T>
	T& GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T* GetPtr(const int offset)
	{
		return *reinterpret_cast<T**>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T& GetVFunc(void* class_pointer, size_t index)
	{
		return (*reinterpret_cast<T**>(class_pointer))[index];
	}

	using IsDormantFn = bool(__thiscall*)(void*);
	using SetupBonesFn = bool(__thiscall*)(void*, matrix3x4_t*, int, int, float);
	using ViewOffsetFn = Vector(__thiscall*)(C_BaseEntity*);
	using PreThinkFn = void(__thiscall*)(C_BaseEntity*);
	using ThinkFn = void(__thiscall*)(C_BaseEntity*);
	using PostThinkFn = void(__thiscall*)(C_BaseEntity*);
	using SetLocalViewAngFn = void(__thiscall*)(C_BaseEntity*, Vector&);
	using UpdateClientAnimsFn = void(__thiscall*)(C_BaseEntity*);
	using GetIndexFn = int(__thiscall*)(void*);
	using GetClientClassFn = ClientClass * (__thiscall*)(void*);
	using IsPlayerFn = bool(__thiscall*)(C_BaseEntity*);
	using GetModelFn = model_t * (__thiscall*)(void*);
	using DescMapFn = datamap_t * (__thiscall*)(void*);
	using WepShootPosFn = void (__thiscall*)(C_BaseEntity*, Vector&);
	using AbsTransFn = void(__thiscall*)(void*, const Vector&);
public:


	datamap_t* DescMap()
	{
		return GetVFunc<DescMapFn>(this, 17)(this);
	}

	void* Animating() { return reinterpret_cast<void*>(uintptr_t(this) + 0x4); }

	void* Networkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }

	bool& GetLifeState() { return GetValue<bool>(g_Vars.Off.m_lifeState); }

	int& GetHealth() { return GetValue<int>(g_Vars.Off.m_iHealth); }

	Vector& GetOrigin() { return GetValue<Vector>(g_Vars.Off.m_vecOrigin); }

	EntityFlags& GetFlags() { return GetValue<EntityFlags>(g_Vars.Off.m_fFlags); }

	Vector& GetVelocity() { return GetValue<Vector>(g_Vars.Off.m_vecVelocity); }

	int& GetTeam() { return GetValue<int>(g_Vars.Off.m_iTeamNum); }

	Vector& GetViewOff() { return GetValue<Vector>(g_Vars.Off.m_vecViewOffset); }

	matrix3x4_t* GetBoneCache() { return GetValue<matrix3x4_t*>(g_Vars.Off.m_mBoneCache); }

	int& GetBoneCount() { return GetValue<int>(g_Vars.Off.m_iBoneCount); }

	float& GetDuckAmount() { return GetValue<float>(g_Vars.Off.m_flDuckAmount); }

	float& GetSimTime() { return GetValue<float>(g_Vars.Off.m_flSimulationTime); }

	int& GetTickBase() { return GetValue<int>(g_Vars.Off.m_nTickBase); }

	Vector& GetEyeAng() { return GetValue<Vector>(g_Vars.Off.m_angEyeAngles); }

	CUserCmd* GetSomeCmd() { return GetValue<CUserCmd*>(g_Vars.Off.m_somCmdOff); }

	C_PlayerAnimstate* GetAnimstate() { return GetValue<C_PlayerAnimstate*>(g_Vars.Off.m_pBaseAnimState); }

	C_AnimationLayer* GetAnimLayers() { return GetValue<C_AnimationLayer*>(g_Vars.Off.m_pAnimOverlay); }

	bool& AllowAnimUpdate() { return GetValue<bool>(g_Vars.Off.m_bClientSideAnimation); }

	std::array<float, 24>& GetPosParams() { return GetValue<std::array<float, 24>>(g_Vars.Off.m_flPoseParameter); }

	int& GetEffects() { return GetValue<int>(g_Vars.Off.m_fEffects); }

	int& GetEFlags() { return GetValue<int>(g_Vars.Off.m_iEFlags); }

	float& GetLby() { return GetValue<float>(g_Vars.Off.m_flLowerBodyYawTarget); }

	Vector& GetRecoilOff() { return GetValue<Vector>(g_Vars.Off.m_aimPunchAngle); }

	Vector& GetViewPunch() { return GetValue<Vector>(g_Vars.Off.m_viewPunchAngle); }

	int& GetArmor() { return GetValue<int>(g_Vars.Off.m_ArmorValue); }

	bool& HasHelmet() { return GetValue<bool>(g_Vars.Off.m_bHasHelmet); }

	bool& IsBulletImmune() { return GetValue<bool>(g_Vars.Off.m_bGunGameImmunity); }

	bool& IsScoped() { return GetValue<bool>(g_Vars.Off.m_bIsScoped); }

	C_BaseCombatWeapon* GetActiveWeapon();

	void UpdateClientAnims()
	{
		GetVFunc<UpdateClientAnimsFn>(this, 218)(this);
	}

	void SetLocalViewAng(Vector Ang)
	{
		GetVFunc<SetLocalViewAngFn>(this, 361)(this, Ang);
	}

	void PreThink()
	{
		GetVFunc<PreThinkFn>(this, 307)(this);
	}

	void Think()
	{
		GetVFunc<ThinkFn>(this, 137)(this);
	}

	void PostThink()
	{
		GetVFunc<PostThinkFn>(this, 308)(this);
	}

	bool IsAlive() 
	{
		if (!this)
			return false;
		return GetHealth() > 0;
	}

	bool SetupBones(matrix3x4_t* out, int max_bones, int mask, float time) 
	{
		if (!Animating())
			return false;
		return GetVFunc<SetupBonesFn>(Animating(), 13)(Animating(), out, max_bones, mask, time);
	}

	Vector BonePos(matrix3x4_t* in,int bone_index)
	{
		if (!in || bone_index < 0 || bone_index >= 128)
			return Vector();
		return { in[bone_index][0][3], in[bone_index][1][3], in[bone_index][2][3] };
	}

	bool IsDormant() 
	{
		return GetVFunc<IsDormantFn>(Networkable(), 9)(Networkable());
	}

	int GetIndex()
	{
		return GetVFunc<GetIndexFn>(Networkable(), 10)(Networkable());
	}

	ClientClass* GetClientClass()
	{
		return GetVFunc<GetClientClassFn>(Networkable(), 2)(Networkable());
	}

	model_t* GetModel() 
	{
		return GetVFunc<GetModelFn>(Animating(), 8)(Animating());
	}

	bool IsPlayer()
	{
		return GetVFunc<IsPlayerFn>(this, 152)(this);
	}

	bool IsValid()
	{
		return (IsAlive() && !IsDormant());
	}

	Vector GetEyePos()
	{
		return GetOrigin() + GetViewOff();
	}

	Vector WeaponShootPos()
	{
		Vector retVec;
		GetVFunc<WepShootPosFn>(this, 277)(this, retVec);
		return retVec;
	}

	Vector& GetAbsOrigin()
	{
		static int m_vecAbsOrigin = g_Utils.DataInMap(DescMap(), "m_vecAbsOrigin");
		return GetValue<Vector>(m_vecAbsOrigin);
	}

	Vector& GetAbsAngle()
	{
		static int m_angAbsRotation = g_Utils.DataInMap(DescMap(), "m_angAbsRotation");
		return GetValue<Vector>(m_angAbsRotation);
	}

	Vector& GetAbsVelocity()
	{
		static int m_vecAbsVelocity = g_Utils.DataInMap(DescMap(), "m_vecAbsVelocity");
		return GetValue<Vector>(m_vecAbsVelocity);
	}


	void SetAbsAngles(Vector angles) {//just set abs_angle directly instead because this can mess with position
		float OgSimtime = GetSimTime();
		static AbsTransFn set_abs_angles_fn = (AbsTransFn)((DWORD)g_Utils.FindPatternIDA("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));
		GetEFlags() |= EFL_DIRTY_ABSTRANSFORM;
		set_abs_angles_fn(this, angles);
		GetSimTime() = OgSimtime;
	}

	void SetAbsPos(Vector position) {
		float OgSimtime = GetSimTime();
		static AbsTransFn set_position_fn = (AbsTransFn)((DWORD)g_Utils.FindPatternIDA("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D"));
		GetEFlags() |= EFL_DIRTY_ABSTRANSFORM;
		set_position_fn(this, position);
		GetSimTime() = OgSimtime;
	}

	void SetAbsVelocity(Vector velocity)
	{
		float OgSimtime = GetSimTime();
		static AbsTransFn set_velocity_fn = (AbsTransFn)((DWORD)g_Utils.FindPatternIDA("client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1 F3"));
		GetEFlags() |= EFL_DIRTY_ABSTRANSFORM;
		GetEFlags() |= EFL_DIRTY_ABSVELOCITY;
		set_velocity_fn(this, velocity);
		GetSimTime() = OgSimtime;
	}
};

class SWeaponInfo {
public:
	virtual ~SWeaponInfo() {};

	char* consoleName;			// 0x0004
	char		pad_0008[12];			// 0x0008
	int			iMaxClip1;				// 0x0014
	int			iMaxClip2;				// 0x0018
	int			iDefaultClip1;			// 0x001C
	int			iDefaultClip2;			// 0x0020
	char		pad_0024[8];			// 0x0024
	char* szWorldModel;			// 0x002C
	char* szViewModel;			// 0x0030
	char* szDroppedModel;			// 0x0034
	char		pad_0038[4];			// 0x0038
	char* N0000023E;				// 0x003C
	char		pad_0040[56];			// 0x0040
	char* szEmptySound;			// 0x0078
	char		pad_007C[4];			// 0x007C
	char* szBulletType;			// 0x0080
	char		pad_0084[4];			// 0x0084
	char* szHudName;				// 0x0088
	char* szWeaponName;			// 0x008C
	char		pad_0090[56];			// 0x0090
	int 		WeaponType;				// 0x00C8
	int			iWeaponPrice;			// 0x00CC
	int			iKillAward;				// 0x00D0
	char* szAnimationPrefix;		// 0x00D4
	float		flCycleTime;			// 0x00D8
	float		flCycleTimeAlt;			// 0x00DC
	float		flTimeToIdle;			// 0x00E0
	float		flIdleInterval;			// 0x00E4
	bool		bFullAuto;				// 0x00E8
	char		pad_0x00E5[3];			// 0x00E9
	int			iDamage;				// 0x00EC
	float		flArmorRatio;			// 0x00F0
	int			iBullets;				// 0x00F4
	float		flPenetration;			// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;				// 0x0104
	float		flRangeModifier;		// 0x0108
	float		flThrowVelocity;		// 0x010C
	char		pad_0x010C[12];			// 0x0110
	bool		bHasSilencer;			// 0x011C
	char		pad_0x0119[3];			// 0x011D
	char* pSilencerModel;			// 0x0120
	int			iCrosshairMinDistance;	// 0x0124
	int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;		// 0x012C
	float		flMaxPlayerSpeedAlt;	// 0x0130
	float		flSpread;				// 0x0134
	float		flSpreadAlt;			// 0x0138
	float		flInaccuracyCrouch;		// 0x013C
	float		flInaccuracyCrouchAlt;	// 0x0140
	float		flInaccuracyStand;		// 0x0144
	float		flInaccuracyStandAlt;	// 0x0148
	float		flInaccuracyJumpInitial;// 0x014C
	float		flInaccuracyJump;		// 0x0150
	float		flInaccuracyJumpAlt;	// 0x0154
	float		flInaccuracyLand;		// 0x0158
	float		flInaccuracyLandAlt;	// 0x015C
	float		flInaccuracyLadder;		// 0x0160
	float		flInaccuracyLadderAlt;	// 0x0164
	float		flInaccuracyFire;		// 0x0168
	float		flInaccuracyFireAlt;	// 0x016C
	float		flInaccuracyMove;		// 0x0170
	float		flInaccuracyMoveAlt;	// 0x0174
	float		flInaccuracyReload;		// 0x0178
	int			iRecoilSeed;			// 0x017C
	float		flRecoilAngle;			// 0x0180
	float		flRecoilAngleAlt;		// 0x0184
	float		flRecoilAngleVariance;	// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;		// 0x0190
	float		flRecoilMagnitudeAlt;	// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;	// 0x01A0
	float		flRecoveryTimeStand;	// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;		// 0x01B8
	bool		bHideViewModelZoomed;	// 0x01B9
	char		pad_0x01B5[2];			// 0x01BA
	char		iZoomLevels[3];			// 0x01BC
	int			iZoomFOV[2];			// 0x01C0
	float		fZoomTime[3];			// 0x01C4
	char* szWeaponClass;			// 0x01D4
	float		flAddonScale;			// 0x01D8
	char		pad_0x01DC[4];			// 0x01DC
	char* szEjectBrassEffect;		// 0x01E0
	char* szTracerEffect;			// 0x01E4
	int			iTracerFrequency;		// 0x01E8
	int			iTracerFrequencyAlt;	// 0x01EC
	char* szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		pad_0x01F4[4];			 // 0x01F4
	char* szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		pad_0x01FC[4];			// 0x01FC
	char* szMuzzleSmokeEffect;	// 0x0200
	float		flHeatPerShot;			// 0x0204
	char* szZoomInSound;			// 0x0208
	char* szZoomOutSound;			// 0x020C
	float		flInaccuracyPitchShift;	// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;		// 0x0218
	char		pad_0x0218[8];			// 0x0220
	char* pWrongTeamMsg;			// 0x0224
	bool		bHasBurstMode;			// 0x0228
	char		pad_0x0225[3];			// 0x0229
	bool		bIsRevolver;			// 0x022C
	bool		bCannotShootUnderwater;	// 0x0230

	float FireRate();
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

class C_BaseCombatWeapon
{
private:
	template<class T>
	T& GetVFunc(void* class_pointer, size_t index)
	{
		return (*reinterpret_cast<T**>(class_pointer))[index];
	}
	template<class T>
	T& GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	using GetInacFn = float(__thiscall*)(C_BaseCombatWeapon*);
	using GetSpreadFn = float(__thiscall*)(void*);
	using UpdateInacFn = void(__thiscall*)(void*);
	using WeaponDatFn = SWeaponInfo * (__thiscall*)(void*);
public:

	float& GetNextPrimaryAttack() { return GetValue<float>(g_Vars.Off.m_flNextPrimaryAttack); }

	float& GetLastShotTime() { return GetValue<float>(g_Vars.Off.m_fLastShotTime); }

	float GetInac()
	{
		float retnum = GetVFunc<GetInacFn>(this, 469)(this);
		if (isnan(retnum))
			return 0;
		else
    		return retnum;
	}
	float GetSpread()
	{
		return GetVFunc<GetSpreadFn>(this, 439)(this);
	}
	void UpdateInac()
	{
		GetVFunc<UpdateInacFn>(this, 470)(this);
	}

	SWeaponInfo* GetWeaponData()
	{
		static auto WeaponData = reinterpret_cast<WeaponDatFn>(g_Utils.FindPatternIDA("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B"));
		return WeaponData(this);
	}

};

struct SpatializationInfo_t;
class IClientEntity
{
public:
	virtual void             Release(void) = 0;
	virtual const Vector     GetAbsOrigin(void) const = 0;
	virtual const QAngle     GetAbsAngles(void) const = 0;
	virtual void* GetMouth(void) = 0;
	virtual bool             GetSoundSpatialization(SpatializationInfo_t info) = 0;
	virtual bool             IsBlurred(void) = 0;
};
typedef void* HANDLE;
class IClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable(int entnum) = 0;
	virtual void* vtablepad0x1(void) = 0;
	virtual void* vtablepad0x2(void) = 0;
	virtual C_BaseEntity* GetClientEntity(int entNum) = 0;
	virtual IClientEntity* GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex(void) = 0;
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
};


enum
{
	EF_BONEMERGE = 0x001,	// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004,	// player flashlight
	EF_NOINTERP = 0x008,	// don't interpolate the next frame
	EF_NOSHADOW = 0x010,	// Don't cast no shadow
	EF_NODRAW = 0x020,	// don't draw entity
	EF_NORECEIVESHADOW = 0x040,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
									// parent and uses the parent's bbox + the max extents of the aiment.
									// Otherwise, it sets up the parent's bones every frame to figure out where to place
									// the aiment, which is inefficient because it'll setup the parent's bones even if
									// the parent is not in the PVS.
									EF_ITEM_BLINK = 0x100,	// blink an item so that the user notices it.
									EF_PARENT_ANIMATES = 0x200,	// always assume that the parent entity is animating


};