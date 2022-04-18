// This file is in public domain.

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow)
{
    HINSTANCE lib = LoadLibrary(L"BrainLILO");
    if (!lib)
    {
        return 1;
    }

    typedef BOOL (*RKDoSoftResetProc)();
    RKDoSoftResetProc RKDoSoftReset = (RKDoSoftResetProc)GetProcAddress(lib, L"RKDoSoftReset");
    if (!RKDoSoftReset)
    {
        return 1;
    }

    if (!RKDoSoftReset())
    {
        return 1;
    }
    return 0;
}
