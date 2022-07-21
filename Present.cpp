#include "Main.h"

LockCursorFn oLockCursor;

void __stdcall Hooks::LockCursor()
{
	if (g_Vars.Menu.Opened) {
		I::Surface->UnLockCursor();
		return;
	}

	oLockCursor(I::Surface);
}