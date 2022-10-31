#define BRAINLILO_API extern "C" __declspec(dllexport)
#include "BrainLILO.h"
#include "BrainLILODrv.h"
#include <string.h>

static bool g_initialized = false;
static HANDLE g_helperHandle = NULL;
static HINSTANCE g_hInstance;

static void initialize()
{
    if (g_initialized)
        return;

    g_initialized = true;
}

static void getThisDllDirectoryPath(LPWSTR buffer)
{
    // retrive the path of the application.
    GetModuleFileName(g_hInstance, buffer, 512);

    const size_t notFound = (size_t)-1;
    size_t i = 0;
    size_t j = notFound;

    while (buffer[i])
    {
        if (buffer[i] == L'/' || buffer[i] == L'\\')
        {
            j = i;
        }
        i++;
    }

    if (j == notFound)
        return;

    buffer[j] = 0;
}

static bool isDriverLoaded()
{
    HANDLE handle;
    handle =
        CreateFile(L"LIN0:", GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE)
        handle = NULL;
    if (handle)
    {
        CloseHandle(handle);
        return true;
    }
    return false;
}

static void loadDriverIfNeeded()
{
    if (isDriverLoaded())
        return;

    OutputDebugString(L"BrainLILO: installing LILODriver...");

    DWORD err;
    HANDLE driver = NULL;

    wchar_t driverPath[1024];

    getThisDllDirectoryPath(driverPath);

    TCHAR newDriverPath[128];

    getThisDllDirectoryPath(driverPath);
    wcscat(driverPath, L"\\BrainLILODrv.dll");
    wcscpy(newDriverPath, L"\\Windows\\BrainLILODrv.dll");

    {
        wchar_t buf[1024];
        swprintf(buf, L"BrainLILO: copying \"%ls\" to \"%ls\"", driverPath, newDriverPath);
        OutputDebugString(buf);
    }

    if (!CopyFile(driverPath, newDriverPath, FALSE))
    {

        if (GetFileAttributes(newDriverPath) == (DWORD)-1)
        {
            OutputDebugString(L"BrainLILO: failed to copy");
            return;
        }
    }

    OutputDebugString(L"BrainLILO: registering BrainLILODriver...");

    try
    {
        driver = RegisterDevice(L"LIN", 0, L"\\Windows\\BrainLILODrv.dll", 0);

        if (driver == INVALID_HANDLE_VALUE)
            driver = NULL;

        if (!driver)
        {

            driver = RegisterDevice(L"LIN", 0, L"BrainLILODrv.dll", 0);

            if (driver == INVALID_HANDLE_VALUE)
                driver = NULL;
        }

        err = GetLastError();
    }
    catch (DWORD e)
    {
        err = e;
    }
    if (!driver && (err != 0x964))
    {
        OutputDebugString(L"BrainLILO: failed to install...");
        return;
    }
}

static void openDriver()
{
    if (g_helperHandle)
        return;
    g_helperHandle =
        CreateFile(L"LIN0:", GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (g_helperHandle == INVALID_HANDLE_VALUE)
        g_helperHandle = NULL;
}

extern "C" BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:

        g_hInstance = hInstance;

        initialize();
        loadDriverIfNeeded();
        openDriver();

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

BRAINLILO_API BOOL RKDoSoftReset()
{

    initialize();
    loadDriverIfNeeded();
    openDriver();

    return DeviceIoControl(g_helperHandle, IOCTL_LIN_DO_LINUX, NULL, 0, NULL, 0, NULL, NULL);
}