#include <windows.h>
#include "libraries/MinHook/include/MinHook.h"

typedef BOOL (WINAPI *RegisterHotKeyFunc)(_In_ HWND, _In_ int, _In_ UINT, _In_ UINT);

static RegisterHotKeyFunc RegisterHotKey_original = NULL;

static BOOL WINAPI RegisterHotKeyCustom(
	_In_ HWND hWnd,
	_In_ int  id,
	_In_ UINT fsModifiers,
	_In_ UINT vk)
{
	if (vk == VK_MEDIA_PLAY_PAUSE)
	{
		// We're lying to Teams, just in case they check the result.
		return TRUE;
	}

	return RegisterHotKey_original(hWnd, id, fsModifiers, vk);
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH && IsMicrosoftTeamsProcess())
	{
		return RegisterHookForRegisterHotKey();
	}

	return FALSE;
}

#define try(__result) if (__result != MH_OK) return FALSE;

BOOL RegisterHookForRegisterHotKey()
{
	try(MH_Initialize());
	try(MH_CreateHookApi(L"User32.dll", "RegisterHotKey", &RegisterHotKeyCustom, (LPVOID*)&RegisterHotKey_original));
	try(MH_EnableHook(MH_ALL_HOOKS));
	OutputDebugStringA("Hey Teams, here's a special version of RegisterHotKey just for you");
	return TRUE;
}

BOOL IsMicrosoftTeamsProcess()
{
	wchar_t processFilePath[MAX_PATH];
	GetModuleFileNameW(NULL, processFilePath, MAX_PATH);

	if (GetLastError()) return FALSE;

	wchar_t* processFileName = wcsrchr(processFilePath, '\\') + 1;
	CharLowerW(processFileName);

	return wcscmp(processFileName, L"teams.exe") == 0;
}
