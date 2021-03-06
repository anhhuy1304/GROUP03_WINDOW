// Dlllock.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#define EXPORT __declspec(dllexport)
HHOOK hHookMouse = NULL;
HHOOK hHookKey = NULL;
HINSTANCE hInst;

LRESULT CALLBACK LockMouse(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) return CallNextHookEx(hHookMouse, nCode, wParam, lParam);
	return TRUE; //lock mouse
}

LRESULT CALLBACK LockKey(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) return CallNextHookEx(hHookKey, nCode, wParam, lParam);
	if ((nCode == HC_ACTION) && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		if ((p->vkCode >= 0x41 && p->vkCode <= 0x5A) || //enable A-Z
			(p->vkCode == 0x08) || // enable backspace 
			(p->vkCode == 0x10 || p->vkCode == 0xA0 || p->vkCode == 0xA1) || //enable shift
			(p->vkCode >= 0x30 && p->vkCode <= 0x39) ||
			(p->vkCode >= 0x60 && p->vkCode <= 0x69) ||//enable 0-9
			(p->vkCode >= 0x25 && p->vkCode <= 0x28) || //enable top down left right arrow key
			(p->vkCode == 0x14) ||//enable capslock
			(p->vkCode == 0x0D) || //enter
			(p->vkCode == 0x09) //tab
			)
			return CallNextHookEx(hHookKey, nCode, wParam, lParam);

		return TRUE;
	}
	return CallNextHookEx(hHookKey, nCode, wParam, lParam);
}

extern "C" EXPORT void installLockMouseHook() {
	if (hHookMouse != NULL) return;
	hHookMouse = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LockMouse, hInst, 0);
			}

extern "C" EXPORT void installLockSomeKey() {
	if (hHookKey != NULL) return;
	hHookKey = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LockKey, hInst, 0);
}

extern "C" EXPORT void RemoveLockKey() {
	if (hHookKey == NULL) return;
	UnhookWindowsHookEx(hHookKey);
	hHookKey = NULL;
}

extern "C" EXPORT void RemoveLockMouse() {
	if (hHookMouse == NULL) return;
	UnhookWindowsHookEx(hHookMouse);
	hHookMouse = NULL;
}


