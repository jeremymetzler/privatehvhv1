#pragma once

class CDraw
{
public:
	virtual const std::string TypeName() { return "CDraw"; }
	virtual void Render() {};

	int x = 0, y = 0, w = 0, h = 0;
	Color clr = Color();
	std::string text = "";

	CDraw(){};

	virtual void Update(int ix, int iy, int iw, int ih, Color cclr = Color());
	virtual void Update(int ix, int iy, int iw, int ih);
	virtual void Update(int ix, int iy);
	virtual void Update(Color cclr);
	virtual void Update(std::string ctext);

private:
};

class CDRect :public CDraw
{
public:
	virtual const std::string TypeName() { return "CDRect"; }
	CDRect(int ix, int iy, int iw, int ih, Color cclr, std::string ctext = "")
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
		clr = cclr;
		text = ctext;
	}

	void Render();
private:
};

class CDFilledRect : public CDraw
{
public:
	virtual const std::string TypeName() { return "CDFilledRect"; }
	CDFilledRect(int ix, int iy, int iw, int ih, Color cclr, std::string ctext = "")
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
		clr = cclr;
		text = ctext;
	}

	void Render();
private:
};

class CDGradRect : public CDraw
{
public:
	virtual const std::string TypeName() { return "CDGradRect"; }
	CDGradRect(int ix, int iy, int iw, int ih, int igradw, Color cclr, std::string ctext = "")
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
		gradw = igradw;
		clr = cclr;
		text = ctext;
		scale = 300.f / fabs(static_cast<float>(igradw)); // ik its 255
	}

	void Render();
	int gradw = 0;
	float scale = 0;
private:
};

class CDLine :public CDraw
{
public:
	virtual const std::string TypeName() { return "CDLine"; }
	CDLine(int ix, int iy, int ix2, int iy2, Color cclr, std::string ctext = "")
	{
		x = ix;
		y = iy;
		w = ix2;
		h = iy2;
		clr = cclr;
		text = ctext;
	}

	void Update(int ix, int iy);
	void Render();
private:
};

class CDText :public CDraw
{
public:
	virtual const std::string TypeName() { return "CDText"; }
	inline void NewTextPose()
	{
		Vector2D Pos;
		if (!mode)
			Pos = { static_cast<float>(x), static_cast<float>(y) };
		else
		{
			std::wstring widestr = std::wstring(text.begin(), text.end());
			Vector2D Size = I::Surface->GetTextSize(font, widestr.c_str());

			if (mode == 1)// center
				Pos = Vector2D(static_cast<int>(x - (static_cast<int>(.5 + (Size.x / 2)))), static_cast<int>(y - (static_cast<int>(.5 + (Size.y / 2))) + 1));
			else if (mode == 2)// x backwards
				Pos = Vector2D(static_cast<int>(x - Size.x), static_cast<int>(y - (int(.5 + (Size.y / 2))) + 1));
			else if (mode == 3)// y center only
				Pos = Vector2D(x, static_cast<int>(y - (static_cast<int>(.5 + (Size.y / 2))) + 1));
			else if (mode == 4)// x center only
				Pos = Vector2D(static_cast<int>(x - (static_cast<int>(.5 + (Size.x / 2)))), y);
		}

		x = Pos.x;
		y = Pos.y;
	}

	CDText(int ix, int iy, std::string ctext, unsigned long lfont, Color cclr, int imode = 0)
	{
		x = ix;
		y = iy;

		text = ctext;
		font = lfont;
		clr = cclr;
		mode = imode;

		NewTextPose();
	}

	void Render();

private:
	unsigned long font = 0;
	int mode = 0;
};