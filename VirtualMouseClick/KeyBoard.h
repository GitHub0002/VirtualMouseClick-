#pragma once
#include "Log.h"
#include "VirtualMouseClickDlg.h"
extern HWND g_thisWnd;
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;


// This is the callback function. Consider it the event that is raised when, in this case,
// a key is pressed.
//LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)

{
	if (nCode >= 0)
	{
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			// a key (non-system) is pressed.
			if (kbdStruct.vkCode == VK_F3)
			{
				LOG("Hook VK_F3");
				CVirtualMouseClickDlg::m_bModeClickOnly = !CVirtualMouseClickDlg::m_bModeClickOnly;
			}
			if (kbdStruct.vkCode == VK_F2)
			{
				LOG("Hook VK_F2");
				SendMessageA(g_thisWnd, UM_DOWINTHRIGHTBUTTON, 0, 0);
			}
		}
	}

	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void ReleaseKeyBoardHook()
{
	UnhookWindowsHookEx(_hook);
}

void SetKeyBoardHook()
{
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook. If you create a hack you will not need the callback function
	// in another place then your own code file anyway. Read more about it at MSDN.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)))
	{
		MessageBoxA(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
		LOG("Hook Set Failed");
	}
}


