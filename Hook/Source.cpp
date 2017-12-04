#include <Windows.h>
#include <iostream>
#include <detours.h>
#pragma comment(lib, "detours.lib")

int (WINAPI *pMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;
int WINAPI HookedMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	int ret = pMessageBoxW(hWnd, L"hooked", lpCaption, uType);
	return ret;
}

BOOL WINAPI DllMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pMessageBoxW, HookedMessageBoxW);

		if (DetourTransactionCommit() == NO_ERROR)
		{
			std::cout << "function hook successfully finished" << std::endl;
		}
		else
		{
			std::cout << "error" << std::endl;
		}
	}

	return TRUE;
}
