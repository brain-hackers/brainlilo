#pragma once

#include <windows.h>

#ifndef BRAINLILO_API
#define BRAINLILO_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    BRAINLILO_API BOOL RKDoSoftReset();

#ifdef __cplusplus
};
#endif
