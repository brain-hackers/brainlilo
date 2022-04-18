// This file is in public domain.

#include <algorithm>
#include <tchar.h>
#include <vector>
#include <windows.h>

#define MainWindowClassName L"SelectorMainWindowClass"

#define DataTypeAlertError 0x1000
#define DataTypeAlertInformation 0x1001
#define DataTypeAlertWarning 0x1002

static void showAlertWarning(LPCWSTR message, LPCWSTR title)
{

    HWND selectorMainWindow = FindWindow(MainWindowClassName, NULL);

    if (!selectorMainWindow)
    {
        MessageBox(NULL, message, title, MB_ICONWARNING);
        return;
    }

    wchar_t data[2048];
    wcscpy(data, message);
    data[wcslen(message)] = 0;
    wcscpy(data + wcslen(message) + 1, title);

    size_t dataLen = wcslen(message) + wcslen(title) + 1;

    COPYDATASTRUCT info;
    info.dwData = DataTypeAlertWarning;
    info.cbData = dataLen * sizeof(wchar_t);

    HGLOBAL global = GlobalAlloc(GPTR, info.cbData);
    memcpy((LPVOID)global, data, info.cbData);

    info.lpData = (LPVOID)global;

    SendMessage(selectorMainWindow, WM_COPYDATA, NULL, (LPARAM)&info);

    GlobalFree(global);
}

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
