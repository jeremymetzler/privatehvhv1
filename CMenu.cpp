#include "Main.h"

const float fade_speed = 600.f;

void CMenu::Render( bool disable_ctrl) {

	if (g_Vars.Menu.Opened) {

		if (disable_ctrl)
			FocusControlIndex = 99999999;
		else if (!disable_ctrl && FocusControlIndex == 99999999)
			FocusControlIndex = -1;

		if (LClick() && FocusControlIndex == -1)
		{
			if (MouseInRegion(x, y, w, 20) && !Grab)
			{
				Grab = true;
				LastMousePos = MousePos();
				LastMenuPos = { static_cast<long>(x), static_cast<long>(y) };
			}

			if (Grab)
			{
				oldx = x;
				oldy = y;
				x = LastMenuPos.x + (MousePos().x - LastMousePos.x);
				y = LastMenuPos.y + (MousePos().y - LastMousePos.y);
			}
		}
		else
			Grab = false;


		for (CDraw* Obj : Draw)
		{
			if (Grab)
				Obj->Update(Obj->x + (x - oldx), Obj->y + (y - oldy));

			Obj->Render();
		}

		int i = 0;
		for (CControl* Obj : Controls)
		{
			if (Grab)
			{
				Obj->x += (x - oldx);
				Obj->y += (y - oldy);
			}

			if (!disable_ctrl && (FocusControlIndex == i || FocusControlIndex == -1))
				Obj->Update();
			i += 1;
		}
	}

	for (CControl* Obj : Controls)
	{
		if (Obj->TypeName() == "CCheckBox")
		{
			CCheckBox* chck = reinterpret_cast<CCheckBox*>(Obj);
			if (g_Globals.KeyDown == chck->Keybind)
				*chck->Variable = !*chck->Variable;
		}
	}
}

void CMenu::Init()
{
	MainFont = I::Surface->FontCreate();
	I::Surface->SetFontGlyph(MainFont, "Lucida Sans Unicode", 15, 1000, 0, 0, FontFlags::Outline);

	Draw.push_back(new CDFilledRect(x + 5, y + 25, w - 10, h - 30, Color(50, 50, 50, 255)));
	Draw.push_back(new CDFilledRect(x, y, w, 20, Color(247, 139, 244, 255)));//255, 191, 228//169, 136, 242
	Draw.push_back(new CDGradRect(x, y, w, 20, 10, Color(232, 190, 247, 200)));
	Draw.push_back(new CDFilledRect(x, y + 20, w, h - 20, Color(110, 110, 110, 188)));
	Draw.push_back(new CDGradRect(x + 5, y + 25, w - 10, h - 30, -5, Color(50, 50, 50, 255)));
	Draw.push_back(new CDRect(x, y, w, h, Color(0, 0, 0, 255)));
	Draw.push_back(new CDLine(x, y + 20, x + w, y + 20, Color(0, 0, 0, 255)));

	std::wstring widestr = std::wstring(text.begin(), text.end());
	Vector2D txs = I::Surface->GetTextSize(MainFont, widestr.c_str());
//	Draw.push_back(new CDFilledRect(x + (w / 2) - (txs.x / 2) - 4, y + 2, txs.x + 8, 16, Color(110, 110, 110, 255)));
//	Draw.push_back(new CDGradRect(x + (w / 2) - (txs.x / 2) - 4, y + 2, txs.x + 8, 16, 6, Color(50, 50, 50, 255)));
//	Draw.push_back(new CDGradRect(x + (w / 2) - (txs.x / 2) - 4, y + 2, txs.x + 8, 16, -3, Color(150, 50, 50, 255)));
//	Draw.push_back(new CDRect(x + (w / 2) - (txs.x / 2) - 4, y + 2, txs.x + 8, 16, Color(247, 139, 244, 255)));
	Draw.push_back(new CDText(x + w / 2, y + 10, text, MainFont, Color(255, 255, 255, 255), 1));

	//ctrl insert point
	ctrlx = x + 10;
	ctrly = y + 30;
}

CGroupBox::CGroupBox(double percw, std::string ctext, CMenu* mwindow)
{
	text = ctext;
	Window = mwindow;

	w = (Window->w - (10 + 10 / percw)) * percw;
	h = 20;
	Window->oldctrlw = Window->ctrlw;

	x = Window->ctrlx;
	y = Window->ctrly;

	Window->ctrlx += 10;//indent

	Window->ctrlh = h;
	Window->ctrlw = w;
	Window->ctrly += 10;
}

void CGroupBox::Init()
{
	std::wstring widestr = std::wstring(text.begin(), text.end());
	Vector2D TextBound = I::Surface->GetTextSize(Window->MainFont, widestr.c_str());

	Window->ctrlx -= 10;//cancel indent
	Window->ctrlh = Window->ctrly - y;
	h = Window->ctrlh;

//	Window->Draw.push_back(new CDFilledRect(x - 9, y, 9, h, Window->Draw[0]->clr));

	Window->Draw.push_back(new CDGradRect(x, y, w, h, -5, Color(Window->Draw[1]->clr).JermSetA(0), text));
	Window->Draw.push_back(new CDGradRect(x, y, w, h, 10, Window->Draw[0]->clr));
	Window->Draw.push_back(new CDGradRect(x, y, w + 1, h + 1, -3, Window->Draw[0]->clr));

	Window->Draw.push_back(new CDLine(x + w, y, x + w, y + 10, Window->Draw[1]->clr));
	Window->Draw.push_back(new CDLine(x, y, x, y + 10, Window->Draw[1]->clr));
	Window->Draw.push_back(new CDLine(x, y, x + w, y, Window->Draw[1]->clr));

	Window->Draw.push_back(new CDLine(x, Window->ctrly, x,  Window->ctrly - 10, Window->Draw[1]->clr));
	Window->Draw.push_back(new CDLine(x, Window->ctrly, x + w, Window->ctrly, Window->Draw[1]->clr));
	Window->Draw.push_back(new CDLine(x + w, Window->ctrly, x + w, Window->ctrly - 10, Window->Draw[1]->clr));

	Window->Draw.push_back(new CDLine(x + 8, y, x + 10 + TextBound.x , y, Window->Draw[0]->clr));
	Window->Draw.push_back(new CDText(x + 10, y, text, Window->MainFont, Color(255, 255, 255, 255), 3));

	int NewX = x;
	int NewY = y;
	int ClosestYXDist = 0;
	int ClosestYIndex = -1;

	if (Window->ctrly + 5 > Window->y + Window->h)
	{
		NewX = Window->ctrlx + Window->oldctrlw + 10;
		Window->ctrlx = NewX;
		NewY = Window->y + 30;
		Window->ctrly = NewY + Window->ctrlh;
	}

	for (int i = Window->Controls.size() - 1; i >= 0; i--)
	{
		CControl* Obj = Window->Controls[i];

		if (Obj->TypeName() == "CGroupBox" && Obj->text != text && ClosestYXDist < Obj->x + Obj->w && ((Obj->y >= y && Obj->y <= y + h) || (Obj->y + Obj->h >= y && Obj->y >= y && Obj->y <= y + h)))
		{
			ClosestYIndex = i;
			ClosestYXDist = Obj->x + Obj->w;
		}
	}

	if (ClosestYXDist != 0)
	{
		NewX = ClosestYXDist + 10;
		Window->ctrlx = NewX;
	}


	for (int i = Window->Draw.size() - 1; i >= 0; i--)
	{
		CDraw* Obj = Window->Draw[i];
		if (Obj->y >= y - 10)
		{
			Obj->Update(Obj->x + (NewX - x), Obj->y + (NewY - y));
		}
		else
			break;
	}

	for (int i = Window->Controls.size() - 1; i >= 0; i--)
	{
		CControl* Obj = Window->Controls[i];
		if (Obj->y >= y - 10 && Obj->text != text)
		{
			Obj->x += (NewX - x);
			Obj->y += (NewY - y);
		}
		else
			break;
	}

	x += (NewX - x);
	y += (NewY - y);
	
	Window->ctrly += 10;
}

void CGroupBox::Update()
{
	bool InBounds = Window->MouseInRegion(x, y, w, h);

	if (InBounds)
	{
		if (fade != 255.f)
			fade += I::Globals->frametime * fade_speed;
	}
	else if (!InBounds)
	{
		if (fade != 0.f)
			fade -= I::Globals->frametime * fade_speed;
	}

	if (fade != 255.f || fade != 0.f)
	{
		if (fade < 0.f)
			fade = 0.f;

		if (fade > 255.f)
			fade = 255.f;

		for (int drInd : Window->DrawSearch(text))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				if (ThisDraw->TypeName() == "CDGradRect")
				{
					ThisDraw->clr.JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[1]->clr.a()));
				}
			}
		}
	}
}

CCheckBox::CCheckBox(std::string ctext, bool* var, CMenu* mwindow)
{
	Window = mwindow;
	w = (Window->ctrlw - 20);
	h = 10;

	fade = 0;
	text = ctext;
	x = mwindow->ctrlx;
	y = mwindow->ctrly;

	Variable = var;
	OldVar = !*Variable;
	mwindow->Draw.push_back(new CDGradRect(x + 1, y + 1, 8, h - 2, 3, Color(Window->Draw[2]->clr).JermSetA(0), text));
	mwindow->Draw.push_back(new CDRect(x, y, 10, h, Color(195, 195, 195, 255)));
	mwindow->Draw.push_back(new CDFilledRect(x + 1, y + 1, 8,  h - 2, Color(Window->Draw[1]->clr).JermSetA(*Variable ? 255 : 0), text));
	mwindow->Draw.push_back(new CDText(x + 20, y + 5, ctext, mwindow->MainFont, Color(255, 255, 255, 255), 3));
	mwindow->ctrly += h + 10;
}

void CCheckBox::Update()
{	
	bool InBounds = Window->MouseInRegion(x, y, w, h);

	if (Window->RClick() && InBounds && !Switch2)
	{
		Switch2 = true;
	}
	else if (!InBounds && Switch2)
		Switch2 = false;

	if (Switch2)
	{
		if (g_Globals.KeyDown != '\0' && g_Globals.KeyDown != Keybind && (g_Globals.KeyDown != VK_LBUTTON || g_Globals.KeyDown != VK_RBUTTON))
		{
			if (g_Globals.KeyDown == 27)
				Keybind = '=';
			else
				Keybind = g_Globals.KeyDown;
			Switch2 = false;
		}
	}

	if (Window->LClick() && !Switch)
	{
		if (InBounds && !Switch)
		{
			Window->FocusControlIndex = Window->ControlSearchIndx(text);
			Switch = true;
		}
	}
	else if (Switch)
	{
		if (!Window->LClick())
		{
			Window->FocusControlIndex = -1;
			*Variable = !*Variable;
			Switch = false;
		}
	}

	if (InBounds)
	{	
		if (fade != 255.f)
			fade += I::Globals->frametime * fade_speed;
		
	}
	else if (!InBounds)
	{
		if (fade != 0.f)
			fade -= I::Globals->frametime * fade_speed;
	}

	if (fade != 255.f || fade != 0.f)
	{
		if (fade < 0.f)
			fade = 0.f;

		if (fade > 255.f)
			fade = 255.f;

		for (int drInd : Window->DrawSearch(text))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				if (ThisDraw->TypeName() == "CDGradRect")
				{
					ThisDraw->clr.JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[2]->clr.a()));
				}
			}
		}
	}
	
	if (OldVar != *Variable)
	{
		for (int drInd : Window->DrawSearch(text))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				if (ThisDraw->TypeName() == "CDFilledRect")
				{
					if (*Variable && ThisDraw->clr.a() != 255)
					{
						Color rgb = ThisDraw->clr;
						ThisDraw->Update(Color(rgb.r(), rgb.g(), rgb.b(), 255));
					}
					else if (!*Variable && ThisDraw->clr.a() != 0)
					{
						Color rgb = ThisDraw->clr;
						ThisDraw->Update(Color(rgb.r(), rgb.g(), rgb.b(), 0));
					}
				}
			//	if (ThisDraw->TypeName() == "CDGradRect")
			//	{
			//		ThisDraw->clr.JermSetA(0);
			//	}

			}
		}
		OldVar = *Variable;
	}
}
#undef max //LMAO
CSlider::CSlider(std::string ctext,float fMin, float fMax, float* var, CMenu* mwindow)
{
	Window = mwindow;
	x = mwindow->ctrlx;
	y = mwindow->ctrly;
	w = (Window->ctrlw - 20);
	h = 10;
	Variable = var;
	OldVar = -std::numeric_limits<float>::max();//it just
	text = ctext;

	Min = fMin;
	Max = fMax;

	mwindow->Draw.push_back(new CDText(x, y + 5/* + 4 */ , ctext + ": " + std::format("{:.2f}", *Variable), mwindow->MainFont, Color(255, 255, 255, 255), 3));
//	mwindow->ctrly += h + 5;
	mwindow->Draw.push_back(new CDFilledRect(x, y + 1, w - 2, h - 2, Color(Window->Draw[1]->clr).JermSetA(0), ctext + ": " + std::format("{:.2f}", *Variable)));
	mwindow->Draw.push_back(new CDGradRect(x, y + 1, w, h - 2, 3, Color(Window->Draw[2]->clr).JermSetA(0), ctext + ": " + std::format("{:.2f}", *Variable)));
	mwindow->Draw.push_back(new CDRect(x, y, w, h, Color(195, 195, 195, 0), ctext + ": " + std::format("{:.2f}", *Variable)));

	mwindow->ctrly += h + 10;
}
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
void CSlider::Update()
{
	auto scale = [&]()
	{
		float ret = 0;

		if (Max >= 0 && Min >= 0)
			ret = Max - Min;
		else if (Max >= 0 && Min < 0)
			ret = Max + fabs(Min);
		else if (Max < 0 && Min < 0)
			ret = fabs(Min) - fabs(Max);
		return float(w) / ret;
	};
	
	bool InBounds = Window->MouseInRegion(x, y, w, h);

	float fscale = scale();

	if ((InBounds && Window->LClick()) || (Window->FocusControlIndex != -1 && Window->LClick()))
	{
		float delta = (Window->MousePos().x - x);

		*Variable = Min + (delta / scale());

		if (*Variable < Min)
		{
			*Variable = Min;
		}
		else if (*Variable > Max)
		{
			*Variable = Max;
		}
		if (Window->FocusControlIndex == -1)
		{
			Window->FocusControlIndex = Window->ControlSearchIndx(text);
		}
	}
	else if (Window->FocusControlIndex != -1 && !Window->LClick())
		Window->FocusControlIndex = -1;

	if (InBounds)
	{
		if (fade != 255.f)
			fade += I::Globals->frametime * fade_speed;
	}
	else
	{
		if (fade != 0.f)
			fade -= I::Globals->frametime * fade_speed;
	}

	if (fade != 255.f || fade != 0.f)
	{
		if (fade < 0.f)
			fade = 0.f;

		if (fade > 255.f)
			fade = 255.f;

		for (int drInd : Window->DrawSearch(text + ": " + std::format("{:.2f}", OldVar)))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				if (ThisDraw->TypeName() == "CDText")
				{
					ThisDraw->clr.JermSetA(static_cast<int>(255.f - fade));
				}
				else if (ThisDraw->TypeName() == "CDGradRect")
				{
					ThisDraw->clr.JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[2]->clr.a()));
				}
				else
				{
					ThisDraw->clr.JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[1]->clr.a()));
				}
			}
		}

		CDText(x + ((*Variable * fscale) - (Min * fscale)), y + 5, std::format("{:.2f}", *Variable), Window->MainFont, Color(255, 255, 255, static_cast<int>(fade)), 1).Render();
	}


	if (OldVar != *Variable)
	{
		for (int drInd : Window->DrawSearch(text + ": " + std::format("{:.2f}", OldVar)))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				if (ThisDraw->TypeName() == "CDFilledRect")
				{
					ThisDraw->w = (*Variable * fscale) - (Min * fscale);
				}

				ThisDraw->text = text + ": " + std::format("{:.2f}", *Variable);
			}
		}
		CDraw* DrawItem = Window->DrawSearchPtr(std::format("{:.2f}", OldVar));

		if (DrawItem)
		{
			DrawItem->x = x + ((*Variable * fscale) - (Min * fscale));
			DrawItem->text = std::format("{:.2f}", *Variable);
		}

		OldVar = *Variable;
	}
}
#undef max //LMAO
CComboBox::CComboBox(std::string ctext, std::vector<std::string> vitems, int* var, CMenu* mwindow)
{
	items.clear();
	items = vitems;
	Window = mwindow;
	x = mwindow->ctrlx;
	y = mwindow->ctrly;
	w = (Window->ctrlw - 20);
	h = 15;
	Variable = var;
	OldVar = *Variable;//it just
	text = ctext;

	mwindow->Draw.push_back(new CDText(x, y + 5, ctext + ": " + items[*Variable], mwindow->MainFont, Color(255, 255, 255, 255), 3));

	mwindow->ctrly += 20;
}
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
void CComboBox::Update()
{
	bool InBounds = Window->MouseInRegion(x, y, w, 10);

	if (fade != 0.f)
	{
		if (fade < 0.f)
			fade = 0.f;

		if (fade > 255.f)
			fade = 255.f;

		for (int i = 0; i < items.size(); i++)
		{
			if (Switch && Window->MouseInRegion(x, y + (i * h), w, h) && g_Globals.KeyDown == VK_LBUTTON)
				*Variable = i;
			if (*Variable == i)
				CDFilledRect(x, y + (i * h), w, h, Color(Window->Draw[2]->clr).JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[2]->clr.a()))).Render();
			else
				CDFilledRect(x, y + (i * h), w, h, Color(114, 114, 114, fade)).Render();

			CDText(x + w / 2, y + (i * h) + 6, items[i], Window->MainFont, Color(255, 255, 255, fade), 1).Render();
		}
		CDGradRect(x, y, w, items.size() * h, 5, Color(Window->Draw[1]->clr).JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[1]->clr.a()))).Render();

		if (fade != 255.f)
			for (int drInd : Window->DrawSearch(text + ": " + items[OldVar]))
			{
				if (drInd != -1)
				{
					CDraw* ThisDraw = Window->Draw[drInd];
					ThisDraw->clr.JermSetA(254.f - fade);
				}
			}
	}

	if (g_Globals.KeyDown == VK_LBUTTON && !Switch)
	{
		if (!Switch && InBounds)
		{
			Window->FocusControlIndex = Window->ControlSearchIndx(text);
			Switch = true;
		}
	}

	if (Switch && Window->LClick() && Window->FocusControlIndex != -1 && !Window->MouseInRegion(x, y, w, items.size() * h))
	{
		Window->FocusControlIndex = -1;
		Switch = false;
	}

	if (Switch && fade != 255.f)
		fade += I::Globals->frametime * fade_speed;

	if (!Switch && fade != 0.f)
		fade -= I::Globals->frametime * fade_speed;

	if (*Variable != OldVar)
	{
		for (int drInd : Window->DrawSearch(text + ": " + items[OldVar]))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				ThisDraw->text = text + ": " + items[*Variable];
			}
		}
		OldVar = *Variable;
	}
}

#undef max //LMAO
CMultiBox::CMultiBox(std::string ctext, std::vector<std::string> vitems, bool* var, CMenu* mwindow)
{
	items.clear();
	items = vitems;
	Window = mwindow;
	x = mwindow->ctrlx;
	y = mwindow->ctrly;
	w = (Window->ctrlw - 20);
	h = 15;
	Variable = var;
	text = ctext;

	mwindow->Draw.push_back(new CDText(x, y + 5, ctext + ":  . . .", mwindow->MainFont, Color(255, 255, 255, 255), 3));

	mwindow->ctrly += 20;
}
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
void CMultiBox::Update()
{
	bool InBounds = Window->MouseInRegion(x, y, w, 10);

	if (fade != 0.f)
	{
		if (fade < 0.f)
			fade = 0.f;

		if (fade > 255.f)
			fade = 255.f;

		for (int i = 0; i < items.size(); i++)
		{
			if (Switch && Window->MouseInRegion(x, y + (i * h), w, h) && g_Globals.KeyDown == VK_LBUTTON)
				Variable[i] = !Variable[i];
			if (Variable[i])
				CDFilledRect(x, y + (i * h), w, h, Color(Window->Draw[2]->clr).JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[2]->clr.a()))).Render();
			else
				CDFilledRect(x, y + (i * h), w, h, Color(114, 114, 114, fade)).Render();

			CDText(x + w / 2, y + (i * h) + 6, items[i], Window->MainFont, Color(255, 255, 255, fade), 1).Render();
		}
		CDGradRect(x, y, w, items.size() * h, 5, Color(Window->Draw[1]->clr).JermSetA(std::clamp(static_cast<int>(fade), 0, Window->Draw[1]->clr.a()))).Render();

		if (fade != 255.f)
			for (int drInd : Window->DrawSearch(text + ":  . . ."))
			{
				if (drInd != -1)
				{
					CDraw* ThisDraw = Window->Draw[drInd];
					ThisDraw->clr.JermSetA(254.f - fade);
				}
			}
	}

	if (Window->LClick() && !Switch)
	{
		if (!Switch && InBounds)
		{
			Window->FocusControlIndex = Window->ControlSearchIndx(text);
			Switch = true;
		}
	}

	if (Switch && Window->LClick() && Window->FocusControlIndex != -1 && !Window->MouseInRegion(x, y, w, items.size() * h))
	{
		Window->FocusControlIndex = -1;
		Switch = false;
	}

	if (Switch && fade != 255.f)
		fade += I::Globals->frametime * fade_speed;

	if (!Switch && fade != 0.f)
		fade -= I::Globals->frametime * fade_speed;

/*	if (*Variable != OldVar)
	{
		for (int drInd : Window->DrawSearch(text + ": " + items[OldVar]))
		{
			if (drInd != -1)
			{
				CDraw* ThisDraw = Window->Draw[drInd];
				ThisDraw->text = text + ": " + items[*Variable];
			}
		}
		OldVar = *Variable;
	}*/
}