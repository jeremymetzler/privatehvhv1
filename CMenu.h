#pragma once

class CMenu;

class CControl
{
public:
	virtual void Init() {};
	virtual void Update() {};
	virtual const std::string TypeName() { return "Base"; }
	CControl() { };


	int x = 0, y = 0, w = 0, h = 0;
	std::string text = "";

	float fade = 0;

	CMenu* Window = nullptr;
private:
};

class CGroupBox : public CControl
{
public:

	void Init();
	virtual const std::string TypeName() { return "CGroupBox"; }
//	void AddCtrl(CControl* ctrl);
	void Update();
	CGroupBox(double percw, std::string ctext, CMenu* mwindow);
	bool Switched = false;
private:
};

class CCheckBox : public CControl
{
public:
	virtual const std::string TypeName() { return "CCheckBox"; }
	//	void AddCtrl(CControl* ctrl);
	void Update();
	
	CCheckBox(std::string ctext, bool* var, CMenu * mwindow);
	bool* Variable = static_cast<bool*>(nullptr);
	bool OldVar = false;
	bool Switch = false;
	bool Switch2 = false;
	char Keybind = '=';
private:

};

class CSlider : public CControl
{
public:
	virtual const std::string TypeName() { return "CSlider"; }
	void Update();
	CSlider(std::string ctext, float fMin, float fMax, float* var, CMenu* mwindow);
	float* Variable = static_cast<float*>(nullptr);
	float OldVar = 0.f;
	float Min = 0.f, Max = 0.f;
	bool Switch = false;
private:

};

class CComboBox : public CControl
{
public:
	virtual const std::string TypeName() { return "CComboBox"; }
	void Update();
	CComboBox(std::string ctext, std::vector<std::string> vitems, int* var, CMenu* mwindow);
	int* Variable = static_cast<int*>(nullptr);
	int OldVar = 0;
	std::vector<std::string> items;
	bool Switch;
private:

};

class CMultiBox : public CControl
{
public:
	virtual const std::string TypeName() { return "CMultiBox"; }
	void Update();
	CMultiBox(std::string ctext, std::vector<std::string> vitems, bool* var, CMenu* mwindow);
	bool* Variable = static_cast<bool*>(nullptr);
	std::vector<std::string> items;
	bool Switch;
private:

};

class CMenu {
public:
	void Render(bool disable_ctrl =false);
	void Init();

	int x = 0, oldx = 0, y = 0, oldy = 0, w = 0, h = 0;
	int* ClickPtr = nullptr;
	inline bool LClick() { if (ClickPtr)return *ClickPtr == VK_LBUTTON; }
	inline bool RClick() { if (ClickPtr)return *ClickPtr == VK_RBUTTON; }
	POINT* MousePosPtr = nullptr;
	inline POINT MousePos() { if (MousePosPtr)return *MousePosPtr; }
	POINT LastMousePos = { 0,0 };
	POINT LastMenuPos = { 0,0 };
	bool Grab = false;
	std::string text = "";
	int FocusControlIndex = -1;

	inline std::vector<int> DrawSearch(std::string ctext)
	{
		std::vector<int> retVec;
		retVec.clear();
		for (int i = 0; i < Draw.size(); i++)
		{
			CDraw& ctrl = *Draw[i];
			if (ctrl.text == ctext)
			{
				retVec.push_back(i);
			}
		}

		return retVec;
	}

	inline CDraw* DrawSearchPtr(std::string ctext)
	{
		for (int i = 0; i < Draw.size(); i++)
		{
			CDraw* ctrl = Draw[i];
			if (ctrl->text == ctext)
			{
				return ctrl;
			}
		}
		return nullptr;
	}


	inline CControl* ControlSearch(std::string ctext)
	{
		for (int i = 0; i < Controls.size(); i++)
		{
			CControl* ret = Controls[i];
			if (ret->text == ctext)
			{
				return ret;
			}
		}
		
	}

	inline int ControlSearchIndx(std::string ctext)
	{
		for (int i = 0; i < Controls.size(); i++)
		{
			CControl* ret = Controls[i];
			if (ret->text == ctext)
			{
				return i;
			}
		}
		return -1;
	}
	CMenu() {}
	CMenu(int ix, int iy, int iw, int ih, std::string ctext)
	{
		x = ix;
		y = iy;
		w = iw;
		h = ih;
		text = ctext;
	}

	inline bool MouseInRegion(int x, int y, int x2, int y2)
	{
		return (MousePos().x >= x && MousePos().y >= y && MousePos().x <= x + x2 && MousePos().y <= y + y2);
	}

	int ctrlx = 0, ctrly = 0;
	int ctrlw = 0, ctrlh = 0;
	int oldctrlw = 0;
	unsigned long MainFont = 0;

	std::deque <CControl*> Controls = std::deque <CControl*>();
	std::deque <CDraw*> Draw = std::deque <CDraw*>();
private:
}; 

extern CMenu* g_Menu;