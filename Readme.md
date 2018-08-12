![cover_logo](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/cover_logo.jpg?raw=true "")

**_GT_**_Libc_ is **[Game Trainer](https://en.wikipedia.org/wiki/Trainer_(games)) library for _c/c++ in windows_** it provides all the necessary methods to make simple game trainer in
windows using **WIN32-API** with ease.
It uses only **WIN32-API** methods instead of **CRT** method because this is intended to work on **Windows** system only
and not shall be portable or to target other OS like **_Linux_,_MAC OS_** etc.

**NOTE** : This ain't memory scanning,hooking,analyzing library, it won't provide methods for scanning/signature or dumping RAW memory.
 
**AIM** : The aim of this library is only to provide the most efficient way of creating game trainer 
and to provide a layer on top of **WIN-32 API** _cumbersome_ methods and to make reading/writing ,finding Game process easier and convenient.

## **_Your support is needed to keep this project alive, Feel free to donate._**
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GAS9SD3H7DYGY)

# Main Components :

## Finding game : 

Using **GT_FindGameProcess()** method.

![finding_game_process](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/finding_game_process.jpg?raw=true "")


Using **GT_FindGameWindow()** method.

![finding_game_window](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/finding_game_window.jpg?raw=true "")


## Reading Values : 

using **GT_ReadAddress()** or **GT_ReadAddressoffset()** methods.

![reading_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/reading_memory.jpg?raw=true "")

## Writing Values : 

using **GT_WriteAddress()** or **GT_WriteAddressOffset()** methods.

![writing_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/writing_memory.jpg?raw=true "")

## Creating Hot-keys :

using **GT_HotKeysPressed()** **_MACRO_** or **GT_IsKeyPressed()/GT_IsKeyToggled()** methods.

![hotkeys](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/hotkeys.jpg?raw=true "")

# Additional Components :

## Applying cheat codes : 

using **GT_SetCheatCode()** method.

![set_cheat_code](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/set_cheat_code.jpg?raw=true "")

## Searching offset area : 

using **GT_SearchOffsetArea()** method.

![search_offset_area](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/search_offset_area.jpg?raw=true "")

# Advanced game hacking methods.

## Injecting Opcode(s) into game : 

using **GT_InjectOpcode()/GT_InjectOpcodes()** method.

![inject_opcode](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/inject_opcode.jpg?raw=true "")

## Injecting Shellcode into game : 

using **GT_InjectShellCode()** method.

![inject_shellcode](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/inject_shellcode.jpg?raw=true "")

## Injecting DLL into game : 

using **GT_InjectDLL()** method.

![inject_dll](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/inject_dll.jpg?raw=true "")

## Automation scripting  : 

using **GT_DoMousePress()** and **GT_DoKeyPress()** methods.


# GTlibc Logs and errors :

## GTConsole Library Error :
If you tried to access methods of **GT**_Console_ library without using MACRO **_GT_BUILD_CLI_** you will get **undefined reference** error.

![gt_console_error](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/gtconsole_error.jpg?raw=true "")


## Error/Exception Handling :

All the error/exception handling is done by library itself like if you tried read or write from **Invalid Memory section** or if process id,game handle/HWND are invalid  it will automatically handle error and exits application with error code. So you don't have to check for any error.

![game_not_found](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/game_not_found.jpg?raw=true "")


![reading_invalid_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/reading_invalid_memory.jpg?raw=true "")


![writing_invalid_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/writing_invalid_memory.jpg?raw=true "")


## Methods Accessibility :

All **Public** and **Semi-Public** methods are accessible . But **Private** methods are not and library will throw error if you tried to access them.

![private_method_error](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/private_method_error.jpg?raw=true "")

## Library Logs :

Logs are **disabled** by default but if you want library to maintain logs use **GT_EnableLogs()** method to **enable** logs.
or if you want to **disable** logs again you can use **GT_DisableLogs()** method.

![enable_disable_logs](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/enable_disable_logs.jpg?raw=true "")


# Trainer Demo :
As a demo of this library GTA-SA Trainer is included to show demo of all the **GT**Libc and **GT**Console methods and how to use them in making simple game trainer.

# GTLibc Tutorial on YouTube :
[![GTLibc Demo](https://img.youtube.com/vi/cRCnN987gd8/0.jpg)](https://www.youtube.com/watch?v=cRCnN987gd8)

**DOCUMENTATION INFO :**
All Public and Semi-Private methods are well documented.
but private methods are not documented as it was not necessary to do so.

**VERSION INFO :**<br/>
GTLIBC Version : V 1.0  Dated : 23/03/2018.<br/>
GTLIBC Version : V 1.1  Dated : 12/04/2018.<br/>
GTLIBC Version : V 1.2  Dated : 23/04/2018.<br/>

**CHANGE LOGS FOR V 1.1** <br/>
**[+]** Added Custom **_procedure_** injection and **_shellcode_** injection methods for advanced game hacking. <br/>
**[+]** Added Support for Microsoft's visual studio (_MSVC_ compiler) and for **Visual C/CPP**. <br/>
**[+]** Added wrapper memory methods for better Memory management.  <br/>
**[-]** Removed support for Multiple games found in memory. <br/>
**[-]** Removed feature where application used to exit after showing Error. <br/>

**CHANGE LOGS FOR V 1.2** <br/>
**[+]** Added new **Console** UI integrated Library called **GT**_Console_.<br/>
**[+]** Added **GT** Prefix to all methods to differentiate it from regular **WINAPI** Methods.<br/>
**[+]** Time is now calculated from standard MACROS rather than WIN-API time methods. <br/>
**[+]** Added Private Macros **_GT_BUILD_CLI_** for **GT**_Console_ library and **_GT_USE_SOUND_** to use Sound methods.<br/>
**[+]** Changed try-catch prefix as-well to **gt**-_try_-**gt**-_catch_ to differentiate it from regular CPP try-catch.<br/>
**[+]** Improved performance and removed buffer overflows.<br/>

**CHANGE LOGS FOR V 1.3** <br/>
**[+]** Added architecture/machine detection support.<br/>
**[+]** Added 64-bit support for shellcode injection.<br/>
**[+]** Added advanced hacking method DLL injection.<br/>
**[+]** Improved logs detection method.<br/>

Written and maintained by HaseeB Mir (haseebmir.hm@gmail.com)