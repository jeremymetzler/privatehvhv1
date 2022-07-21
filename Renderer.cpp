#include "Main.h"

void CDraw::Update(int ix, int iy, int iw, int ih, Color cclr)
{
	x = ix;
	y = iy;
	w = iw;
	h = ih;
	clr = cclr;
}

void CDraw::Update(int ix, int iy,int iw,int ih)
{
	x = ix;
	y = iy;
	w = iw;
	h = ih;
}

void CDraw::Update(int ix, int iy)
{
	x = ix;
	y = iy;
}

void CDraw::Update(Color cclr)
{
	clr = cclr;
}

void CDraw::Update( std::string ctext)
{
	text = ctext;
}

void CDFilledRect::Render()
{
	if (clr.a() == 0)
		return;
	I::Surface->SetDrawingColor(clr.r(), clr.g(), clr.b(), clr.a());
	I::Surface->DrawFilledRectangle(x, y, w, h);
}

void CDRect::Render()
{
	if (clr.a() == 0)
		return;
	I::Surface->SetDrawingColor(clr.r(), clr.g(), clr.b(), clr.a());
	I::Surface->DrawRectangle(x, y, w, h);
}

void CDLine::Render()
{
	if (clr.a() == 0)
		return;
	I::Surface->SetDrawingColor(clr.r(), clr.g(), clr.b(), clr.a());
	I::Surface->DrawLine(x, y, w, h);
}

void CDLine::Update(int ix, int iy)
{
	w += ix - x;
	h += iy - y;
	x = ix;
	y = iy;
}

void CDText::Render()
{
	if (clr.a() == 0)
		return;
	I::Surface->SetTextColor(clr.r(), clr.g(), clr.b(), clr.a());
	I::Surface->DrawTextFont(font);
	I::Surface->DrawTextPos(x, y);
	std::wstring widestr = std::wstring(text.begin(), text.end());
	I::Surface->DrawRenderText(widestr.c_str(), text.size());
}

void CDGradRect::Render()
{
	if (clr.a() == 0)
		return;
	for (int gay = 0; ((gradw > 0) ? (gay <= gradw) : (gay >= gradw)); ((gradw > 0) ? gay++ : gay--))
	{
		I::Surface->SetDrawingColor(clr.r(), clr.g(), clr.b(), clr.a() - std::clamp(fabs(static_cast<float>(gay)) * scale, 0.f, static_cast<float>(clr.a())));
		I::Surface->DrawRectangle(gay + (x), gay + (y), (w)-(gay * 2), (h)-(gay * 2));
	}
}

