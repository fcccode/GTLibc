#ifndef _GTLIBC_H_
#define _GTLIBC_H_

/*
BRIEF : GTLibc is library to make game trainer in c/c++ it provide all the necessary methods to make simple game trainer in
windows using win32-API with ease.
It uses only WIN32 API methods instead of CRT method because this is intended to work on Windows system only
and not shall be portable or to target other OS like Linux,MAC OS etc.

From the beginning of trainer development till end, it provides all necessary methods needed for game trainer.

*****************************
*******Main components.******
*****************************

1)Finding game --> use GT_FindGameProcess()  or GT_FindGameWindow() method.
2)Reading values Health,XP,Ammos from game --> use GT_ReadAddress() or GT_ReadAddressoffset() methods.
3)Writing values Health,XP,Ammos to  game --> use GT_WriteAddress() or GT_WriteAddressOffset() methods.
4)Creating Hot-keys for trainer --> use GT_HotKeysPressed() MACRO or GT_IsKeyPressed()/GT_IsKeyToggled() methods.

*****************************
****Additional components.***
*****************************

5)Additional Automation of scripting for trainer --> use GT_DoMousePress() and GT_DoKeyPress() methods.
6)Cheat code applying tool included in this library --> use GT_SetCheatCode() method.
7)Offset area searching tool included in this library --> use GT_SearchOffsetArea() method.

***********************************************
****Advanced components for Game Hacking.*****
**********************************************

8)Custom code injection tool included in this library --> use GT_InjectCode()/GT_InjectCodes() methods.
9)NOP instruction Filling tool included in this library --> use GT_WriteNOP()/GT_WriteNOPs() methods.
10)Write JMP/CALL assembly instruction --> use  GT_WriteJmpOrCall() method.
11)Custom procedure injection tool included in this library --> use GT_InjectProc() method.
12)Custom shellcode injection tool included in this library --> use GT_InjectShellCode() method.

NOTE : This ain't memory scanning,hooking,analyzing library, it won't provide methods for scanning/signature or dumping RAW memory.

AIM : The aim of this library is only to provide the most efficient way of creating game trainer
and to provide a layer on top of WIN-32 API cumbersome methods and to make reading/writing ,finding Game process easier and convenient.

DOCUMENTATION INFO :
All Public and Semi-Private methods are well documented.
but private methods are not documented as it was not necessary to do so.

VERSION INFO :
GTLIBC Version : V 1.2

WHATS NEW IN THIS VERSION  v1.1 :
[+] Added Custom procedure injection and shellcode injection methods for advanced game hacking.
[+] Added Support for Microsoft's visual studio (MSVC compiler) and for Visual C/CPP.
[+] Added wrapper memory methods for better Memory management.
[-] Removed support for Multiple games found in memory.
[-] Removed feature where application used to exit after showing Error.

WHATS NEW IN THIS VERSION  V 1.2 
[+] Added new Console UI integrated Library.
[+] Added GT Prefix to all methods to differentiate it from regular WINAPI Methods.
[+] Current time is now calculated from standard MACROS rather than WIN-API time methods. 
[+] Added Private Macros GT_BUILD_CLI for GTConsole library and GT_USE_SOUND to use sound methods.
[+] Changed try-catch prefix as-well to gt_try-gt-catch to differentiate it from regular CPP try-catch.
[+] Improved performance and removed buffer overflows.


V 1.0 -> Dated : 23/03/2018
V 1.1 -> Dated : 11/04/2018
V 1.2 -> Dated : 23/04/2018

Written by Ha5eeB Mir (haseebmir.hm@gmail.com)
*/

/*Undefine UNICODE if already defined*/
#ifdef UNICODE
#undef UNICODE
#endif

#ifdef GT_BUILD_CLI 
#include "GTConsole.c"
#endif

/*Defining WIN32 Constants*/
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

/*Including WIN32 libraries*/
#include <windows.h>
#include <tlhelp32.h>

/*Defining NULL constants*/
#define NIL 0x0 	/*Integer NULL*/
#define NUL '\0' 	/*Character NULL*/
#define GT_HotKeysPressed(...) GT_HotKeysDown(__VA_ARGS__, NULL)

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__
#endif

/*define date and time regardless if it will bind with GTConsole or not*/
#define CURR_DATE __DATE__
#define CURR_TIME __TIME__

/*Defining exception handling constants*/
#if !defined(gt_try) && !defined(gt_catch) && !defined(gt_throw)
#define gt_try BOOL GT_HadError=FALSE;
#define gt_catch(x) GT_ExitJump:if(GT_HadError)
#define gt_throw(x) GT_HadError=TRUE;goto GT_ExitJump;
#endif

/*Enum for OPCODE type*/
typedef enum GT_OPCODE {
	GT_OPCODE_SHORT_JUMP = 0x1,
	GT_OPCODE_NEAR_JUMP = 0x2,
	GT_OPCODE_CALL = 0x3
}GT_OPCODE;

/*Enum for SHELLCODE type*/
typedef enum GT_SHELL
{
	GT_ORIGINAL_SHELL,
	GT_PATCHED_SHELL,
}GT_SHELL; 


/****************************************************************************/
/*********************-PUBLIC-METHODS-***************************************/
/****************************************************************************/
/*Public methods to find Game process/window.*/
HANDLE GT_FindGameProcess(LPCSTR);
HWND GT_FindGameWindow(LPCSTR);

/*Public methods to Read/Write values from/at Address.*/
DWORD GT_ReadAddress(LPVOID);
DWORD GT_ReadAddressOffset(LPVOID, DWORD);
DWORD* GT_ReadAddressOffsets(LPVOID, DWORD*, SIZE_T);
BOOL GT_WriteAddress(LPVOID, DWORD);
BOOL GT_WriteAddressOffset(LPVOID, DWORD, DWORD);
BOOL GT_WriteAddressOffsets(LPVOID, DWORD*, SIZE_T, DWORD);

/*Public methods to Read/Write pointer from/at Address.*/
LPVOID GT_ReadPointerOffset(LPVOID, DWORD);
LPVOID GT_ReadPointerOffsets(LPVOID, DWORD*, SIZE_T);
BOOL GT_WritePointerOffset(LPVOID, DWORD, DWORD);
BOOL GT_WritePointerOffsets(LPVOID, DWORD*, SIZE_T, DWORD);

/*Public getter methods to get Game Name,Handle,Process ID,base address.*/
LPCSTR GT_GetGameName(VOID);
DWORD GT_GetProcessID(VOID);
HANDLE GT_GetGameHandle4mHWND(HWND);
DWORD GT_GetProcessID4mHWND(HWND);
LPBYTE GT_GetGameBaseAddress(DWORD);

/*Public methods for creating hot-keys*/
BOOL GT_HotKeysDown(INT, ...);
BOOL GT_IsKeyPressed(CONST INT);
BOOL GT_IsKeyToggled(CONST INT);

/****************************************************************************/
/****************-SEMI-PRIVATE-METHODS-**************************************/
/****************************************************************************/
/*Semi-private methods for pressing Keyboard and Mouse keys*/
VOID GT_DoMousePress(INT);
VOID GT_DoKeyPress(INT);

/*Semi-private Tool for Applying cheat codes*/
VOID GT_SetCheatCode(LPCSTR);

/*Semi-private Tool for searching in offset area*/
LPSTR GT_SearchOffsetArea(LPVOID, CONST size_t, CONST size_t, DWORD);

/*Semi-private Tool for Injecting custom code*/
BOOL GT_InjectCode(LPVOID, LPCVOID, SIZE_T);
BOOL GT_InjectCodes(LPVOID[], LPBYTE[], SIZE_T[], SIZE_T);

/*Semi-private Tool for writing assembly NOP instruction*/
BOOL GT_WriteNOP(LPVOID, SIZE_T);
BOOL GT_WriteNOPs(LPVOID[], SIZE_T[], SIZE_T);

/*Semi-private Tool for writing assembly JMP or CALL instruction*/
BOOL GT_WriteJmpOrCall(LPVOID, LPVOID, GT_OPCODE, UINT);

/*Semi-private Tool for injecting custom Procedure into game*/
LPVOID GT_InjectProc(LPVOID, LPCVOID, SIZE_T, UINT,GT_SHELL,GT_OPCODE);

/*Semi-private Tool for injecting custom shellcode into game*/
LPVOID GT_InjectShellCode(LPVOID,LPCVOID, SIZE_T,GT_SHELL,GT_OPCODE);

/*Semi private method for enabling/disabling Logs*/
BOOL GT_EnableLogs(VOID);
BOOL GT_DisableLogs(VOID);

/*Semi-private getter methods to get Game,Handle,HWND.*/
HANDLE GT_GetGameHandle(VOID);
HWND GT_GetGameHWND(VOID);

/****************************************************************************/
/****************-PRIVATE-METHODS-*******************************************/
/****************************************************************************/
/*Private methods for showing error/info/warning*/
static VOID GT_ShowError(DWORD, LPCSTR, DWORD);
static VOID GT_ShowInfo(LPCSTR);
static VOID GT_ShowWarning(LPCSTR);
static DWORD GT_GetError(VOID);

/*Private setter methods for setting Game Name, ID,Handle,HWND etc*/
static VOID GT_SetProcessID(DWORD);
static VOID GT_SetGameHandle(HANDLE);
static VOID GT_SetGameName(LPCSTR);
static VOID GT_SetGameHWND(DWORD);

/*Private utility methods for Logs*/
static VOID GT_AddLog(LPCSTR, ...);
static BOOL GT_FileExist(LPCSTR);

/*Private memory allocation wrapper methods*/
LPVOID GT_MemAlloc(DWORD, SIZE_T);
BOOL GT_MemFree(LPVOID);

/*Private miscellaneous methods*/
static VOID GT_DoVirtualKeyPress(INT, INT, INT);
static BOOL CALLBACK GT_EnumAllWindows(HWND, LPARAM);
static BOOL GT_IsPrivateMethod(BOOL, LPCSTR, INT);

/*Global variables for storing game information*/
extern DWORD gt_process_id;
extern HANDLE gt_game_handle;
extern CHAR gt_game_name[MAX_PATH];
extern HWND gt_game_hwnd;

/*Global variable for storing error code*/
extern DWORD gt_error_code;

/*Global variable for Setting private methods inaccessible*/
extern BOOL gt_private_method;

/*Global variable for enabling/disabling logs*/
extern BOOL gt_logs_enabled;

#endif	/* _GTLIBC_H_ */