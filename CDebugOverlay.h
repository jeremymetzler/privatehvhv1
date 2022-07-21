#pragma once

class unk_0;
class CDebugOverlay
{
public:
	virtual void            AddEntityTextOverlay(int entIndex, int lineOffset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void            AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void            AddSphereOverlay(const Vector& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration) = 0;
	virtual void            AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void            AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void            AddTextOverlay(const Vector& origin, float duration, const char* format, ...) = 0;
	virtual void            AddTextOverlay(const Vector& origin, int lineOffset, float duration, const char* format, ...) = 0;
	virtual void            AddScreenTextOverlay(float xPos, float yPos, float duration, int r, int g, int b, int a, const char* text) = 0;
	virtual void            AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const Vector& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void            AddGridOverlay(const Vector& origin) = 0;
	virtual void            AddCoordFrameOverlay(const matrix3x4_t& frame, float scale, int colorTable[3][3] = nullptr) = 0;
	virtual int             ScreenPosition(const Vector& point, Vector& screen) = 0;
	virtual int             ScreenPosition(float xPos, float yPos, Vector& screen) = 0;
	virtual unk_0* GetFirst(void) = 0;
	virtual unk_0* GetNext(unk_0* current) = 0;
	virtual void            ClearDeadOverlays(void) = 0;
	virtual void            ClearAllOverlays(void) = 0;
	virtual void            AddTextOverlayRGB(const Vector& origin, int lineOffset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void            AddTextOverlayRGB(const Vector& origin, int lineOffset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void            AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void            AddBoxOverlaySecond(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
	virtual void			AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void            PurgeTextOverlays() = 0;

	bool m_bWorldToScreen(Vector world, Vector& screen)
	{
		return (ScreenPosition(world, screen) != 1);
	}
};