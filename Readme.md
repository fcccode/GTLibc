![cover_logo](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/cover_logo.jpg?raw=true "")

**_GTLibc_** is **[Game Trainer](https://en.wikipedia.org/wiki/Trainer_(games)) library for _c/c++ in windows_** it provides all the necessary methods to make simple game trainer in
windows using **WIN32-API** with ease.
It uses only **WIN32-API** methods instead of **CRT** method because this is intended to work on **Windows** system only
and not shall be portable or to target other OS like **_Linux_,_MAC OS_** etc.

**NOTE** : This ain't memory scanning,hooking,analyzing library, it won't provide methods for scanning/signature or dumping RAW memory.
 
**AIM** : The aim of this library is only to provide the most efficient way of creating game trainer 
and to provide a layer on top of **WIN-32 API** _cumbersome_ methods and to make reading/writing ,finding Game process easier and convenient.


# Main Components :

## Finding game : 

Using **findGameProcess()** method.

![finding_game_process](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/finding_game_process.jpg?raw=true "")


Using **findGameWindow()** method.

![finding_game_window](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/finding_game_window.jpg?raw=true "")


## Reading Values : 

using **readAddress()** or **readAddressoffset()** methods.

![reading_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/reading_memory.jpg?raw=true "")

## Writing Values : 

using **writeAddress()** or **writeAddressOffset()** methods.

![writing_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/writing_memory.jpg?raw=true "")

## Creating Hot-keys :

using **hotKeysPressed()** **_MACRO_** or **isKeyPressed()/isKeyToggled()** methods.

![hotkeys](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/hotkeys.jpg?raw=true "")

# Additional Components :

## Applying cheat codes : 

using **setCheatCode()** method.

![set_cheat_code](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/set_cheat_code.jpg?raw=true "")

## Searching offset area : 

using **searchOffsetArea()** method.

![search_offset_area](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/search_offset_area.jpg?raw=true "")

## Automation scripting  : 

using **doMousePress()** and **doKeyPress()** methods.


# GTlibc Logs and errors :

## Multiple Games :
**NOTE** If Multiple games are found then you have to upate the logic of getting input from user again!

![multiple_games_warning](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/multiple_games_warning.jpg?raw=true "")


## Error/Exception Handling :

All the error/exception handling is done by library itself like if you tried read or write from **Invalid Memory section** or if process id,game handle/HWND are invalid  it will autmatically handle error and exits application with error code. So you dont have to check for any error.

![game_not_found](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/game_not_found.jpg?raw=true "")


![reading_invalid_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/reading_invalid_memory.jpg?raw=true "")


![writing_invalid_memory](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/writing_invalid_memory.jpg?raw=true "")


## Methods Accessibility :

All **Public** and **Semi-Public** methods are accessible . But **Private** methods are not and library will throw error if you tried to access them.

![private_method_error](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/private_method_error.jpg?raw=true "")

## Library Logs :

Logs are **disabled** by default but if you want library to maintain logs use **enableLogs()** method to **enable** logs.
or if you want to **disable** logs again you can use **disableLogs()** method.

![enable_disable_logs](https://github.com/haseeb-heaven/GTLibc/blob/master/resources/enable_disable_logs.jpg?raw=true "")


# Trainer Demo :
As a demo of this library GTA-SA Trainer is included to show demo of all the methods needed and how to use them in making simple game trainer.

# GTLibc Tutorial on YouTube :
[![GTLibc Demo](https://img.youtube.com/vi/cRCnN987gd8/0.jpg)](https://www.youtube.com/watch?v=cRCnN987gd8)

**DOCUMENTATION INFO :**
All Public and Semi-Private methods are well documented.
but private methods are not documented as it was not necessary to do so.

**VERSION INFO :**<br/>
GTLIBC Version : V 1.0. Dated : 23/03/2018.<br/>
GTLIBC Version : V 1.1. Dated : 12/04/2018.<br/>

**CHANGE LOGS.** <br/>
**[+]** Added Custom **_procedure_** injection and **_shellcode_** injection methods for advanced game hacking. <br/>
**[+]** Added Support for Microsoft's visual studio (_MSVC_ compiler) and for **Visual C/CPP**. <br/>
**[+]** Added wrapper memory methods for better Memory management.  <br/>
**[-]** Removed support for Multiple games found in memory. <br/>
**[-]** Removed feature where application used to exit after showing Error. <br/>

Written and maintained by HaseeB Mir (haseebmir.hm@gmail.com)