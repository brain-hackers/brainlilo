VERSION_MAJOR=2
VERSION_MINOR=0
VERSION_PATCH=0

PREFIX?=/opt/cegcc

CC=$(PREFIX)/bin/arm-mingw32ce-gcc
CXX=$(PREFIX)/bin/arm-mingw32ce-g++
LD=$(PREFIX)/bin/arm-mingw32ce-g++
STRIP=$(PREFIX)/bin/arm-mingw32ce-strip
DLLTOOL=$(PREFIX)/bin/arm-mingw32ce-dlltool
AS=$(PREFIX)/bin/arm-mingw32ce-as
NM=$(PREFIX)/bin/arm-mingw32ce-nm
WINDRES=$(PREFIX)/bin/arm-mingw32ce-windres

OUTPUT=BrainLILO.dll BrainLILODrv.dll BrainLILO.exe

CXXFLAGS= -DEV_PLATFORM_WIN32 -DUNICODE -D_UNICODE -DEV_UNSAFE_SWPRINTF -mwin32 \
-O0 -mcpu=arm926ej-s -D_WIN32_WCE=0x600 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 \
-D_FILE_OFFSET_BITS=64 -static

DLLFLAGS=-DEV_PLATFORM_WIN32 -DUNICODE -D_UNICODE -DEV_UNSAFE_SWPRINTF -mwin32 \
-O0 -mcpu=arm926ej-s -D_WIN32_WCE=0x600 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 \
-D_FILE_OFFSET_BITS=64 -DNDEBUG -Wall -static \
 -Wl,--image-base,0x100000 \
 -shared
 
DRVFLAGS= -DEV_PLATFORM_WIN32 -DUNICODE -D_UNICODE -DEV_UNSAFE_SWPRINTF -mwin32 \
-O0 -mcpu=arm926ej-s -D_WIN32_WCE=0x600 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 \
-D_FILE_OFFSET_BITS=64 -DNDEBUG -Wall -static \
 -Wl,--image-base,0x100000,--allow-multiple-definition \
 -lcoredll -shared

.PHONY:		all clean

all:				$(OUTPUT)

clean:			
				rm -f $(OUTPUT)

BrainLILO.dll:	BrainLILO.cpp
				$(CXX) BrainLILO.cpp -o BrainLILO.dll $(DLLFLAGS)
				$(STRIP) BrainLILO.dll
				
BrainLILODrv.dll:	BrainLILODrv.cpp
				$(CXX) BrainLILODrv.cpp -o BrainLILODrv.dll $(DRVFLAGS)
				$(STRIP) BrainLILODrv.dll

BrainLILO.exe:	bootloader.cpp
				$(CXX) bootloader.cpp -o BrainLILO.exe $(CXXFLAGS)
				$(STRIP) BrainLILO.exe

