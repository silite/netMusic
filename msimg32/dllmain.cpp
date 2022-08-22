// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include<Windows.h>
#include "EasyCEFHooks.h"

using namespace std;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MessageBox(NULL, TEXT("test"), NULL, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		
	}
	return TRUE;
}

