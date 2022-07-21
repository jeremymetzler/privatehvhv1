#pragma once

struct NMsg
{
	std::string text = "";
	float time = 0.f;
	Color clr = Color(255, 255, 255, 255);
};

class Globals
{
public:
	HWND CSGOWindow;
	bool RenderInit;

	C_BaseEntity* LocalPlayer = nullptr;
	int KeyDown = 0;
	int KeyHeld = 0;
	POINT MousePos;
	Vector MoveAng;
	Vector ViewAng;
	Vector AntiAng;
	int LocalTeam = 0;
	bool SyncFrame = false;
	float LocalRealTime = 0.f;
	bool* SendPacket = nullptr;
	int ChokeAmnt = 0;
	bool GameReady = false;
	Vector LocalLeash;
	char hit_side;

	bool LocalShot = false;
	Vector BulletHitPos;
	int HitIndex = 0;
	
	std::deque<NMsg> Notif = std::deque<NMsg>();
	inline void AddNotif(std::string text, float duration = 0.5f, Color nclr = Color(255,255,255,255))
	{
		if (!I::Globals)
			return;
		Notif.push_back({ text, I::Globals->realtime + duration, nclr });
	}
private:
}; extern Globals g_Globals;