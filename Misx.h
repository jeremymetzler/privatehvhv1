#pragma once

class Misc
{
public:
	void Bhop(CUserCmd* cmd);
	void CircleStrafe(CUserCmd* cmd);
	void CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove);
	CMoveData* MoveData = new CMoveData;
	bool EngPrediction(CUserCmd* cmd);
	void AutoStop(CUserCmd* cmd);


	float LerpTime();
	inline int TIME_TO_TICKS(float dt) {
		return static_cast<int>(0.5f + (dt / I::Globals->intervalPerTick));
	}
	inline float TICKS_TO_TIME(int dt) {
		return static_cast<float>(I::Globals->intervalPerTick * static_cast<float>(dt));
	}
};

extern Misc g_Misc;