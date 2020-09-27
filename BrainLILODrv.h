// This file is in public domain.

#pragma once
#include <winioctl.h>

#define _LIN_ACCESS_CTL_CODE(_Function) \
CTL_CODE(FILE_DEVICE_UNKNOWN, _Function, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_LIN_DO_LINUX _LIN_ACCESS_CTL_CODE(0x804)
