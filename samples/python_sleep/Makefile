##############################################################################
##
##  API Extention to Measure time slept.
##
##  Microsoft Research Detours Package
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!include ..\common.mak

LIBS=$(LIBS) kernel32.lib

##############################################################################

all: dirs \
    $(BIND)\python_sleep$(DETOURS_BITS).dll \
    \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\python_sleep$(DETOURS_BITS).bsc \
!ENDIF
    option

##############################################################################

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

$(OBJD)\python_sleep.obj : python_sleep.cpp

$(OBJD)\python_sleep.res : python_sleep.rc

$(BIND)\python_sleep$(DETOURS_BITS).dll $(BIND)\python_sleep$(DETOURS_BITS).lib: \
        $(OBJD)\python_sleep.obj $(OBJD)\python_sleep.res $(DEPS)
    cl /LD $(CFLAGS) /Fe$(@R).dll /Fd$(@R).pdb \
        $(OBJD)\python_sleep.obj $(OBJD)\python_sleep.res \
        /link $(LINKFLAGS) /subsystem:console \
        /export:DetourFinishHelperProcess,@1,NONAME \
        /export:NoWaitForSingleObjectEx \
        $(LIBS)

$(OBJD)\python_sleep$(DETOURS_BITS).bsc : $(OBJD)\python_sleep.obj
    bscmake /v /n /o $@ $(OBJD)\python_sleep.sbr

##############################################################################

clean:
    -del *~ 2>nul
    -del $(BIND)\python_sleep*.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

############################################### Install non-bit-size binaries.

!IF "$(DETOURS_OPTION_PROCESSOR)" != ""

$(OPTD)\python_sleep$(DETOURS_OPTION_BITS).dll:
$(OPTD)\python_sleep$(DETOURS_OPTION_BITS).pdb:

$(BIND)\python_sleep$(DETOURS_OPTION_BITS).dll : $(OPTD)\python_sleep$(DETOURS_OPTION_BITS).dll
    @if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\python_sleep$(DETOURS_OPTION_BITS).pdb : $(OPTD)\python_sleep$(DETOURS_OPTION_BITS).pdb
    @if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).

option: \
    $(BIND)\python_sleep$(DETOURS_OPTION_BITS).dll \
    $(BIND)\python_sleep$(DETOURS_OPTION_BITS).pdb \

!ELSE

option:

!ENDIF

##############################################################################

test: all
    @echo -------- Reseting test binaries to initial state. ---------------------
    $(BIND)\setdll.exe -r $(BIND)\sleep5.exe
    @echo.
    @echo -------- Should not load simple$(DETOURS_BITS).dll -----------------------------------
    $(BIND)\sleep5.exe
    @echo.
    @echo -------- Adding simple$(DETOURS_BITS).dll to sleep5.exe ------------------------------
    $(BIND)\setdll.exe -d:$(BIND)\simple$(DETOURS_BITS).dll $(BIND)\sleep5.exe
    @echo.
    @echo -------- Should load simple$(DETOURS_BITS).dll statically ----------------------------
    $(BIND)\sleep5.exe
    @echo.
    @echo -------- Removing simple$(DETOURS_BITS).dll from sleep5.exe --------------------------
    $(BIND)\setdll.exe -r $(BIND)\sleep5.exe
    @echo.
    @echo -------- Should not load simple$(DETOURS_BITS).dll -----------------------------------
    $(BIND)\sleep5.exe
    @echo.
    @echo -------- Should load simple$(DETOURS_BITS).dll dynamically using withdll.exe----------
    $(BIND)\withdll.exe -d:$(BIND)\simple$(DETOURS_BITS).dll $(BIND)\sleep5.exe
    @echo.

debug: all
    windbg -o $(BIND)\withdll.exe -d:$(BIND)\python_sleep$(DETOURS_BITS).dll $(BIND)\sleep5.exe


################################################################# End of File.
