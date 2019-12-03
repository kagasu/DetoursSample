#include <Windows.h>
#include <iostream>
#include "DetoursHelper.h"

void DetourMessageBoxW(BOOL hookEnabled)
{
	static decltype(&MessageBoxW) OriginalMessageBoxW = MessageBoxW;

	decltype(&MessageBoxW) HookMessageBox = [](HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) -> int
	{
		auto ret = OriginalMessageBoxW(hWnd, L"hooked", lpCaption, uType);
		return ret;
	};

	DetourFunction(hookEnabled, reinterpret_cast<LPVOID*>(&OriginalMessageBoxW), HookMessageBox);
}

BOOL WINAPI DllMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourMessageBoxW(true);
		std::cout << "function hook successfully finished" << std::endl;
	}

	return TRUE;
}
