#include "Main.h"
WNDPROC Hooks::oldWindowProc;

LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool PressedButtons = false;
	if (uMsg == WM_MOUSEMOVE)
	{
		g_Globals.MousePos.x = GET_X_LPARAM(lParam);
		g_Globals.MousePos.y = GET_Y_LPARAM(lParam);
	}

	if ((uMsg == WM_LBUTTONDOWN || uMsg == WM_KEYDOWN || uMsg == WM_RBUTTONDOWN) && g_Globals.KeyDown != wParam && !(wParam == 'W' || wParam == 'A' || wParam == 'S' || wParam == 'D') )
	{
		g_Globals.KeyHeld = wParam;
		g_Globals.KeyDown = wParam;
	}
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_KEYUP || uMsg == WM_RBUTTONUP)
	{
		g_Globals.KeyHeld = 0;
	}

/*	if (uMsg == WM_KEYDOWN)
	{
		g_Globals.KeyDown = wParam;
	}
	else if (uMsg == WM_KEYUP)
	{
		g_Globals.KeyDown = 0;
	}*/

	if (!PressedButtons && g_Globals.KeyDown == VK_DELETE)
	{
		PressedButtons = true;
	}
	else if (PressedButtons && g_Globals.KeyDown != VK_DELETE)
	{
		PressedButtons = false;

		g_Vars.Menu.Opened = !g_Vars.Menu.Opened;
		if (g_Vars.Menu.Opened)
			I::Engine->ExecuteClientCmd("cl_mouseenable 0");
		else
			I::Engine->ExecuteClientCmd("cl_mouseenable 1");
	}

	if (g_Vars.Menu.Opened)
		return true;

	return CallWindowProcA(oldWindowProc, hWnd, uMsg, wParam, lParam);
}