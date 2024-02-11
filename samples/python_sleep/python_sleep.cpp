//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (simple.cpp of simple.dll)
//
//  Microsoft Research Detours Package
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  This DLL will detour the Windows SleepEx API so that TimedSleep function
//  gets called instead.  TimedSleepEx records the before and after times, and
//  calls the real SleepEx API through the TrueSleepEx function pointer.
//
#include <stdio.h>
#include <windows.h>
#include "detours.h"

//static LONG dwSlept = 0;
//static DWORD (WINAPI * TrueSleepEx)(DWORD dwMilliseconds, BOOL bAlertable) = SleepEx;
//
//DWORD WINAPI TimedSleepEx(DWORD dwMilliseconds, BOOL bAlertable)
//{
//    DWORD dwBeg = GetTickCount();
//    DWORD ret = TrueSleepEx(dwMilliseconds, bAlertable);
//    DWORD dwEnd = GetTickCount();
//
//    InterlockedExchangeAdd(&dwSlept, dwEnd - dwBeg);
//
//    return ret;
//}

DWORD (__stdcall * TrueWaitForSingleObjectEx)(HANDLE a0,
                                               DWORD a1,
                                               BOOL a2)
= WaitForSingleObjectEx;

DWORD WINAPI NoWaitForSingleObjectEx(
        HANDLE hHandle,
        DWORD  dwMilliseconds,
        BOOL   bAlertable
)
{
    if (dwMilliseconds == INFINITE || dwMilliseconds == 0 || dwMilliseconds == 10 || dwMilliseconds == 100) {
        return TrueWaitForSingleObjectEx(hHandle, dwMilliseconds, bAlertable);
    } else {
        return TrueWaitForSingleObjectEx(hHandle, 1, bAlertable);
    }
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    LONG error;
    (void)hinst;
    (void)reserved;

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        printf("python_sleep" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
               " Starting.\n");
        fflush(stdout);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TrueWaitForSingleObjectEx, NoWaitForSingleObjectEx);
        error = DetourTransactionCommit();

        if (error == NO_ERROR) {
            printf("python_sleep" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
                   " Detoured SleepEx().\n");
        }
        else {
            printf("python_sleep" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
                   " Error detouring SleepEx(): %ld\n", error);
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueWaitForSingleObjectEx, NoWaitForSingleObjectEx);
        error = DetourTransactionCommit();

        printf("python_sleep" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
               " Removed SleepEx() (result=%ld)\n", error);
        fflush(stdout);
    }
    return TRUE;
}

//
///////////////////////////////////////////////////////////////// End of File.
