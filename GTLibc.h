#ifndef _GTLIBC_H_
#define _GTLIBC_H_

/*
BRIEF : GTLibc is library to make game trainer in c/c++ it provide all the necessary methods to make simple game trainer in
windows using win32-API with ease.
It uses only WIN32 API methods instead of CRT method because this is intended to work on Windows system only
and not shall be portable or to target other OS like Linux,MAC OS etc.

From beginning of trainer development till the end, it provides all necessary methods needed.

*****************************
*******Main components.******
*****************************

1)Finding game --> use findGameProcess()  or findGameWindow() method.
2)Reading values Health,XP,Ammos from game --> use readAddress() or readAddressoffset() methods.
3)Writing values Health,XP,Ammos to  game --> use writeAddress() or writeAddressOffset() methods.
4)Creating Hot-keys for trainer --> use hotKeysPressed() MACRO or isKeyPressed()/isKeyToggled() methods.

*****************************
****Additional components.***
*****************************

5)Additional Automation of scripting for trainer --> use doMousePress() and doKeyPress() methods.
6)Cheat code applying tool included in this library --> use setCheatCode() method.
7)Offset area searching tool included in this library --> use searchOffsetArea() method.

***********************************************
****Advanced components for Game Hacking.*****
**********************************************

8)Custom code injection tool included in this library --> use injectCode()/injectCodes() methods.
9)NOP instruction Filling tool included in this library --> use writeNOP()/writeNOPs() methods.
10)Write JMP/CALL assembly instruction --> use  writeJmpOrCall() method.

NOTE : This ain't memory scanning,hooking,analyzing library, it won't provide methods for scanning/signature or dumping RAW memory.

AIM : The aim of this library is only to provide the most efficient way of creating game trainer
and to provide a layer on top of WIN-32 API cumbersome methods and to make reading/writing ,finding Game process easier and convenient.

DOCUMENTATION INFO :
All Public and Semi-Private methods are well documented.
but private methods are not documented as it was not necessary to do so.

VERSION INFO :
GTLIBC Version : V 1.1.

WHATS NEW IN THIS VERSION :
[+] Added Custom procedure injection and shellcode injection methods for advanced game hacking.
[+] Added Support for Microsoft's visual studio (MSVC compiler) and for Visual C/CPP.
[+] Added wrapper memory methods for better Memory management.
[-] Removed support for Multiple games found in memory.


11)Custom procedure injection tool included in this library --> use injectProc() method.
12)Custom shellcode injection tool included in this library --> use injectShellCode() method.


Written by HaseeB Mir (haseebmir.hm@gmail.com)

V 1.0 : Dated : 23/03/2018
V 1.1 : Dated : 11/04/2018
*/

/*Undefine UNICODE if already defined*/
#ifdef UNICODE
#undef UNICODE
#endif

/*Defining WIN32 Constants*/
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

/*Including WIN32 libraries*/
#include <windows.h>
#include <tlhelp32.h>

/*Defining constants*/
#define NIL 0x0 	/*Integer NULL*/
#define NUL '\0' 	/*Character NULL*/
#define MOUSE_LEFT_ID 1
#define MOUSE_RIGHT_ID 2
#define hotKeysPressed(...) hotKeysDown(__VA_ARGS__, NULL)
#define ERROR_INVALID (GetLastError() != ERROR_SUCCESS || GetLastError() == ERROR_ALREADY_EXISTS)

/*Re-Defining standard file constants*/
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__

/*Defining exception handling constants*/
#define try BOOL __HadError=FALSE;
#define catch(x) ExitJmp:if(__HadError)
#define throw(x) __HadError=TRUE;goto ExitJmp;

/*Enum to store OPCODE type*/
typedef enum OPCODE {
	OPCODE_SHORT_JUMP,
	OPCODE_NEAR_JUMP,
	OPCODE_CALL
}OPCODE;

/****************************************************************************/
/*********************-PUBLIC-METHODS-***************************************/
/****************************************************************************/
/*Public methods to find Game process/window.*/
HANDLE findGameProcess(LPCSTR);
HWND findGameWindow(LPCSTR);

/*Public methods to Read/Write values from/at Address.*/
DWORD readAddress(LPVOID);
DWORD readAddressOffset(LPVOID, DWORD);
DWORD* readAddressOffsets(LPVOID, DWORD*, SIZE_T);
BOOL writeAddress(LPVOID, DWORD);
BOOL writeAddressOffset(LPVOID, DWORD, DWORD);
BOOL writeAddressOffsets(LPVOID, DWORD*, SIZE_T, DWORD);

/*Public methods to Read/Write pointer from/at Address.*/
LPVOID readPointerOffset(LPVOID, DWORD);
LPVOID readPointerOffsets(LPVOID, DWORD*, SIZE_T);
BOOL writePointerOffset(LPVOID, DWORD, DWORD);
BOOL writePointerOffsets(LPVOID, DWORD*, SIZE_T, DWORD);

/*Public getter methods to get Game Name,Handle,Process ID,base address.*/
LPCSTR getGameName(VOID);
DWORD getProcessID(VOID);
HANDLE getGameHandle4mHWND(HWND);
DWORD getProcessID4mHWND(HWND);
LPBYTE getGameBaseAddress(DWORD);

/*Public methods for creating hot-keys*/
BOOL hotKeysDown(INT, ...);
BOOL isKeyPressed(CONST INT);
BOOL isKeyToggled(CONST INT);

/****************************************************************************/
/****************-SEMI-PRIVATE-METHODS-**************************************/
/****************************************************************************/
/*Semi-private methods for pressing Keyboard and Mouse keys*/
VOID doMousePress(INT);
VOID doKeyPress(INT);

/*Semi-private Tool for Applying cheat codes*/
VOID setCheatCode(LPCSTR);

/*Semi-private Tool for searching in offset area*/
LPSTR searchOffsetArea(LPVOID, CONST size_t, CONST size_t, DWORD);

/*Semi-private Tool for Injecting custom code*/
BOOL injectCode(LPVOID, LPCVOID, SIZE_T);
BOOL injectCodes(LPVOID[], LPBYTE[], SIZE_T[], SIZE_T);

/*Semi-private Tool for writing assembly NOP instruction*/
BOOL writeNOP(LPVOID, SIZE_T);
BOOL writeNOPs(LPVOID[], SIZE_T[], SIZE_T);

/*Semi-private Tool for writing assembly JMP or CALL instruction*/
BOOL writeJmpOrCall(LPVOID, LPVOID, OPCODE, UINT);

/*Semi-private Tool for injecting custom Procedure into game*/
BOOL injectProc(LPVOID, LPCVOID, SIZE_T, UINT);

/*Semi-private Tool for injecting custom shellcode into game*/
LPVOID injectShellCode(LPCVOID, SIZE_T);

/*Semi private method for enabling/disabling Logs*/
BOOL enableLogs(VOID);
BOOL disableLogs(VOID);

/*Semi-private getter methods to get Game,Handle,HWND.*/
HANDLE getGameHandle(VOID);
HWND getGameHWND(VOID);

/****************************************************************************/
/****************-PRIVATE-METHODS-*******************************************/
/****************************************************************************/
/*Private methods for showing error/info/warning*/
static VOID showError(DWORD, LPCSTR, DWORD);
static VOID showInfo(LPCSTR);
static VOID showWarning(LPCSTR);

/*Private setter methods for setting Game Name, ID,Handle,HWND etc*/
static VOID setProcessID(DWORD);
static VOID setGameHandle(HANDLE);
static VOID setGameName(LPCSTR);
static VOID setGameHWND(DWORD);

/*Private utility methods for time and Logs*/
static LPSTR getCurrentTime(VOID);
static VOID addLog(LPCSTR, ...);
static BOOL fileExist(LPCSTR);

/*Private memory allocation wrapper methods*/
LPVOID GT_MemAlloc(DWORD, SIZE_T);
BOOL GT_MemFree(LPVOID);

/*Private miscellaneous methods*/
static VOID doVirtualKeyPress(INT, INT, INT);
static BOOL CALLBACK EnumAllWindows(HWND, LPARAM);
static BOOL isPrivateField(BOOL, LPCSTR, INT);

/*Global variables for storing game information*/
extern DWORD process_id;
extern HANDLE game_handle;
extern CHAR game_name[MAX_PATH];
extern HWND game_hwnd;

/*Global variable for storing error code*/
extern DWORD error_code;

/*Global variable for Setting private methods inaccessible*/
extern BOOL private_field;

/*Global variable for enabling/disabling logs*/
extern BOOL logs_enabled;

#endif	/* _GTLIBC_H_ */