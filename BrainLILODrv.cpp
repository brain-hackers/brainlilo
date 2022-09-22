/*
 * BrainLILODrv
 * U-Boot loader for electric dictionary.
 *
 * Copyright (C) 2019 C. Shirasaka <holly_programmer@outlook.com>
 * based on
 ** ResetKitHelper
 ** Soft/hard reset the electronic dictionary.
 **
 ** Copyright (C) 2012 T. Kawada <tcppjp [ at ] gmail.com>
 *
 * This file is licensed in MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <fstream>
#include <regex>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#define FSNOTIFY_POWER_OFF 1
#define FSNOTIFY_POWER_ON 0

#define BRAINLILODRV_API __declspec(dllexport)

#include "BrainLILODrv.h"
#include "models.h"

typedef void (*FileSystemPowerFunctionProc)(DWORD);
static FileSystemPowerFunctionProc FileSystemPowerFunction;

// GEN1
// VA
#define BOOTLOADER_PRELOADADDR_GEN1 (uint8_t *)0xa0250000
#define BOOTLOADER_LOADADDR_GEN1 (uint8_t *)0xa3f00000
#define PHYSICAL_INVOKER_INSTALLADDR_GEN1 (uint8_t *)0xb0000000
#define TMPA910_REMAP_REGISTER_ADDRESS (uint32_t *)0xaa000004
// PA
#define BOOTLOADER_LOADADDR_GEN1_PA (uint8_t *)0x43f00000

// DEFAULT(GEN2)
// VA
#define BOOTLOADER_PRELOADADDR (uint8_t *)0xa0250000
#define BOOTLOADER_LOADADDR (uint8_t *)0xa0200000
#define PHYSICAL_INVOKER_INSTALLADDR (uint8_t *)0xb0000000
// PA
#define BOOTLOADER_LOADADDR_PA (uint8_t *)0x40200000

// VA
uint8_t *bootloader_preload_address = NULL;
uint8_t *bootloader_load_address = NULL;
uint8_t *physical_invoker_install_address = NULL;
// PA
uint8_t *bootloader_load_address_pa = NULL;

DWORD fileSize;
int row;
int screenW;
int screenH;

static void outputDebugMessage(const wchar_t *format, ...)
{
    wchar_t buffer[256] = {0};
    va_list args;
    RECT rcScreen = {.left = 0, .top = 0, .right = screenW, .bottom = screenH};

    va_start(args, format);
    vswprintf(buffer, format, args);
    va_end(args);

    OutputDebugString(buffer);
    ExtTextOut(GetDC(NULL), 0, row * 14, ETO_CLIPPED, &rcScreen, buffer, wcslen(buffer), NULL);

    row++;
}

static void disableInterrupts()
{
    asm volatile("mrs	r0, cpsr\n"
                 "orr	r0,r0,#0x80\n"
                 "msr	cpsr_c,r0\n"
                 "mov	r0,#1" ::
                     : "r0");
}

extern "C" void EDNA2_physicalInvoker();

static void EnableMemoryRemapGen1()
{
    *TMPA910_REMAP_REGISTER_ADDRESS = 1;
    outputDebugMessage(L"BrainLILO: memory remap enable for Gen1\n");
}

static void EDNA2_installPhysicalInvoker(BrainGen gen)
{
    if (gen == Gen1)
    {
        EnableMemoryRemapGen1();
    }
    outputDebugMessage(L"BrainLILO: copying PhysicalInvoker to 0x%p from 0x%p\n", physical_invoker_install_address,
                       &EDNA2_physicalInvoker);
    memcpy(physical_invoker_install_address, (const void *)&EDNA2_physicalInvoker, 64 * 4);
    // clearCache();
}

__attribute__((noreturn)) static void EDNA2_runPhysicalInvoker()
{
    // r0=info
    asm volatile("msr	cpsr_c, #211\n"     // to supervisor mode
                 "mrc	p15,0,r0,c1,c0,0\n" // read ctrl regs
                 "bic	r0, r0, #8192\n"    // reset vector to lower
                 "mcr	p15,0,r0,c1,c0,0\n" // write ctrl regs
    );

    memcpy(bootloader_load_address, bootloader_preload_address, fileSize);

    asm volatile("ldr	r0, =0x0000\n"
                 "ldr	r1, =0x0000\n"
                 "ldr	r2, =0x0000\n"
                 "ldr	r3, =0x0000\n"
                 "ldr	r4, =0x0000\n"
                 "ldr	r5, =0x0000\n"
                 "ldr	r6, =0x0000\n"
                 "ldr	r7, =0x0000\n"
                 "mov	r8, %[textbase]\n"
                 "ldr	r9, =0x0000\n"

                 "mrc	p15,0,r10,c1,c0,0\n" // read ctrl regs
                 "bic	r10, r10, #5\n"      // disable MMU/DCache
                 "mcr	p15,0,r10,c1,c0,0\n" // write ctrl regs
                 "swi	#0\n"                // jump!
                 :                           /* no outputs */
                 : [textbase] "r"(bootloader_load_address_pa)
                 : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10");

    // never reach here
    while (true)
        ;
}

__attribute__((noreturn)) static DWORD EDNA2_callKernelEntryPoint(BrainGen gen)
{
    outputDebugMessage(L"BrainLILO: disabling interrupts");
    disableInterrupts();
    outputDebugMessage(L"BrainLILO: injecting code to internal ram");
    EDNA2_installPhysicalInvoker(gen);
    outputDebugMessage(L"BrainLILO: invoking");
    Sleep(1000);

    EDNA2_runPhysicalInvoker();
}

static void ShowMessage(std::wstring msg, std::wstring title, UINT typ)
{
    MessageBox(NULL, msg.c_str(), title.c_str(), typ);
}

static void SetAddress(BrainGen gen)
{
    switch (gen)
    {
    case Gen1:
        // VA
        bootloader_preload_address = BOOTLOADER_PRELOADADDR_GEN1;
        bootloader_load_address = BOOTLOADER_LOADADDR_GEN1;
        physical_invoker_install_address = PHYSICAL_INVOKER_INSTALLADDR_GEN1;
        // PA
        bootloader_load_address_pa = BOOTLOADER_LOADADDR_GEN1_PA;
        break;
    default:
        // Gen2?
        // VA
        bootloader_preload_address = BOOTLOADER_PRELOADADDR;
        bootloader_load_address = BOOTLOADER_LOADADDR;
        physical_invoker_install_address = PHYSICAL_INVOKER_INSTALLADDR;
        // PA
        bootloader_load_address_pa = BOOTLOADER_LOADADDR_PA;
    }
}

static BrainGen SelectGen(std::wstring model)
{
    BrainGen brainGen = UnknownGen;
    if (model == L"gen1.bin")
    {
        brainGen = Gen1;
    }
    else if (model == L"u-boot.bin")
    {
        brainGen = UnknownGen;
    }
    else
    {
        brainGen = Gen2Or3;
    }
    return brainGen;
}

static bool doLinux()
{
    std::wifstream iVersion;
    std::wstring line, model;
    std::wregex modelRe(L"[A-Z]{2}-[A-Z0-9]+");
    std::wsmatch match;
    BrainGen brain_gen = UnknownGen;

    std::wstring fn(L"\\Storage Card\\loader\\");
    HANDLE hUBoot;
    DWORD wReadSize;

    iVersion.open("\\NAND\\version.txt");
    while (getline(iVersion, line))
    {
        if (regex_search(line, match, modelRe))
        {
            model = match[0].str();
            break;
        }
    }

    if (model.length() == 0)
    {
        outputDebugMessage(L"BrainLILO: failed to match the model name");
        MessageBox(NULL, L"Failed to match the model name", L"BrainLILO", MB_ICONWARNING);
        return false;
    }

    outputDebugMessage(L"BrainLILO: internal model name: %s", model.c_str());

    auto iter = models.find(model);
    if (iter != models.end())
    {
        model = iter->second;
    }
    else
    {
        outputDebugMessage(L"BrainLILO: internal model name %s is unknown, falling back to u-boot.bin", model.c_str());
        model = L"u-boot.bin";
    }

    fn += model;
    outputDebugMessage(L"BrainLILO: opening Bootloader file: %s", fn.c_str());

    brain_gen = SelectGen(model);
    SetAddress(brain_gen);

    hUBoot = CreateFile(fn.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hUBoot == INVALID_HANDLE_VALUE)
    {
        outputDebugMessage(L"BrainLILO: could not open the bootloader: %s", fn.c_str());
        ShowMessage(L"Could not open the bootloader: " + fn, L"BrainLILO", MB_ICONWARNING);
        return false;
    }

    outputDebugMessage(L"BrainLILO: bootloader file handle 0x%p", hUBoot);

    fileSize = GetFileSize(hUBoot, NULL);
    outputDebugMessage(L"BrainLILO: bootloader file size %d Byte", fileSize);

    outputDebugMessage(L"BrainLILO: preloading bootloader to 0x%p...", bootloader_preload_address);
    if (!ReadFile(hUBoot, (void *)bootloader_preload_address, fileSize, &wReadSize, NULL))
    {
        outputDebugMessage(L"BrainLILO: could not read the bootloader");
        ShowMessage(L"Could not read the bootloader", L"BrainLILO", MB_ICONWARNING);
        return false;
    }
    outputDebugMessage(L"BrainLILO: bootloader copied! Closing file handle...");
    CloseHandle(hUBoot);

    outputDebugMessage(L"BrainLILO: notifying power off to filesystems...");
    if (FileSystemPowerFunction)
        FileSystemPowerFunction(FSNOTIFY_POWER_OFF);

    outputDebugMessage(L"BrainLILO: starting bootloader call sequence...");
    EDNA2_callKernelEntryPoint(brain_gen);
    return true;
}

extern "C" BRAINLILODRV_API BOOL LIN_IOControl(DWORD handle, DWORD dwIoControlCode, DWORD *pInBuf, DWORD nInBufSize,
                                               DWORD *pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned)
{
    SetLastError(0);

    switch (dwIoControlCode)
    {
    case IOCTL_LIN_DO_LINUX:
        if (!doLinux())
        {
            if (FileSystemPowerFunction)
                FileSystemPowerFunction(FSNOTIFY_POWER_ON);
            return FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

extern "C" BRAINLILODRV_API BOOL LIN_Read(DWORD handle, LPVOID pBuffer, DWORD dwNumBytes)
{
    SetLastError(ERROR_INVALID_FUNCTION);
    return FALSE;
}

extern "C" BRAINLILODRV_API BOOL LIN_Write(DWORD handle, LPVOID pBuffer, DWORD dwNumBytes)
{
    SetLastError(ERROR_INVALID_FUNCTION);
    return FALSE;
}

extern "C" BRAINLILODRV_API DWORD LIN_Seek(DWORD handle, long lDistance, DWORD dwMoveMethod)
{
    SetLastError(ERROR_INVALID_FUNCTION);
    return FALSE;
}

extern "C" BRAINLILODRV_API void LIN_PowerUp(void)
{
    OutputDebugString(L"BrainLILO: resuming.");
}

extern "C" BRAINLILODRV_API void LIN_PowerDown(void)
{
}

extern "C" BRAINLILODRV_API DWORD LIN_Init(LPCTSTR pContext, DWORD dwBusContext)
{

    void *ctx;
    ctx = (void *)LocalAlloc(LPTR, sizeof(4));

    screenW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    screenH = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    return (DWORD)ctx;
}

extern "C" BRAINLILODRV_API DWORD LIN_Open(DWORD dwData, DWORD dwAccess, DWORD dwShareMode)
{

    void *hnd = (void *)LocalAlloc(LPTR, 4);

    row = 0;

    return (DWORD)hnd;
}

extern "C" BRAINLILODRV_API BOOL LIN_Close(DWORD handle)
{
    LocalFree((void *)handle);

    return TRUE;
}

extern "C" BRAINLILODRV_API BOOL LIN_Deinit(DWORD dwContext)
{

    LocalFree((void *)dwContext);
    return TRUE;
}

extern "C" BOOL APIENTRY DllMainCRTStartup(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        FileSystemPowerFunction =
            (FileSystemPowerFunctionProc)GetProcAddress(LoadLibrary(L"COREDLL"), L"FileSystemPowerFunction");

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
