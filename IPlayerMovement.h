#pragma once

class IPlayerMoveHelper {
public:
	virtual void SetHost(C_BaseEntity* host) = 0;
};

class CMoveData
{
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	int	m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	QAngle			m_vecViewAngles;	// Command view angles (local space)
	QAngle			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector			m_vecVelocity;			// edict::velocity		// Current movement direction.
	Vector			m_vecTrailingVelocity;
	float			m_flTrailingVelocityTime;
	Vector			m_vecAngles;			// edict::angles
	Vector			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	Vector			m_outWishVel;		// This is where you tried 
	Vector			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;		///< If no, do no constraining past Radius.  If yes, cap them to SpeedFactor past radius

	void			SetAbsOrigin(const Vector& vec);
	const Vector& GetAbsOrigin() const;

private:
	Vector			m_vecAbsOrigin;		// edict::origin
};

inline const Vector& CMoveData::GetAbsOrigin() const
{
	return m_vecAbsOrigin;
}

inline void CMoveData::SetAbsOrigin(const Vector& vec)
{
	m_vecAbsOrigin = vec;
}

class IPlayerGameMovement {
private:

	using ProcessMovementFn = void(__thiscall*)(void*, C_BaseEntity* player, CMoveData* move);
	using ResetFn = void(__thiscall*)(void*);
	using StartTrackPredictionErrorsFn = void(__thiscall*)(void*, C_BaseEntity*);
	using FinishTrackPredictionErrorsFn = void(__thiscall*)(void*, C_BaseEntity*);
public:
	void ProcessMovement(C_BaseEntity* player, CMoveData* move)
	{
		g_Utils.GetVFunc<ProcessMovementFn>(this, 1)(this, player, move);
	}
	void Reset()
	{
		g_Utils.GetVFunc<ResetFn>(this, 2)(this);
	}
	void StartTrackPredictionErrors(C_BaseEntity* player)
	{
		g_Utils.GetVFunc<StartTrackPredictionErrorsFn>(this, 3)(this, player);
	}
	void FinishTrackPredictionErrors(C_BaseEntity* player)
	{
		g_Utils.GetVFunc<FinishTrackPredictionErrorsFn>(this, 4)(this, player);
	}

};

class IPlayerPrediction {

	using UpdateFn = void(__thiscall*)(void*, int startframe, bool validframe, int incoming_acknowledged, int outgoing_command);
	using LocalViewAngFn = void(__thiscall*)(void*, Vector&);
	using InPredictionFn = bool(__thiscall*)(void*);
	using RunCommandFn = void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, IPlayerMoveHelper*);
	using SetupMoveFn = void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, IPlayerMoveHelper*, CMoveData*);
	using FinishMoveFn = void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, void*);
	using RunPreThinkFn = void(__thiscall*)(void*, C_BaseEntity*);
	using RunThinkFn = void(__thiscall*)(void*, C_BaseEntity*, double);
	using StartCommandFn = void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*);
public:


	void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command)
	{
		g_Utils.GetVFunc<UpdateFn>(this, 3)(this, startframe, validframe, incoming_acknowledged, outgoing_command);
	}
	void GetLocalViewAng(Vector& ang)
	{
		g_Utils.GetVFunc<LocalViewAngFn>(this, 12)(this, ang);
	}
	void SetLocalViewAng(Vector& ang)
	{
		g_Utils.GetVFunc<LocalViewAngFn>(this, 13)(this, ang);
	}
	bool InPrediction() 
	{
		return g_Utils.GetVFunc<InPredictionFn>(this, 14)(this);
	}
	void RunCommand(C_BaseEntity* player, CUserCmd* cmd, IPlayerMoveHelper* helper) 
	{
		g_Utils.GetVFunc<RunCommandFn>(this, 19)(this, player, cmd, helper);
	}
	void SetupMove(C_BaseEntity* player, CUserCmd* cmd, IPlayerMoveHelper* helper, CMoveData* data)
	{
		g_Utils.GetVFunc<SetupMoveFn>(this, 20)(this, player, cmd, helper, data);
	}
	void FinishMove(C_BaseEntity* player, CUserCmd* cmd, void* data) 
	{
		g_Utils.GetVFunc<FinishMoveFn>(this, 21)(this, player, cmd, data);
	}
	void RunPreThink(C_BaseEntity* player)
	{
		g_Utils.GetVFunc<RunPreThinkFn>(this, 26)(this, player);
	}
	void RunThink(C_BaseEntity* player, double frametime)
	{
		g_Utils.GetVFunc<RunThinkFn>(this, 27)(this, player, frametime);
	}
	void RunPostThink(C_BaseEntity* player)
	{
		g_Utils.GetVFunc<RunPreThinkFn>(this, 28)(this, player);
	}
	void StartCommand(C_BaseEntity* player, CUserCmd* cmd)
	{
		g_Utils.GetVFunc<StartCommandFn>(this, 24)(this, player, cmd);
	}
	void FinishCommand(C_BaseEntity* player)
	{
		g_Utils.GetVFunc<RunPreThinkFn>(this, 25)(this, player);
	}
};
