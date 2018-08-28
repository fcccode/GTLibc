#define GT_BUILD_CLI /*Define this to use GTConsole library.*/
#define GT_USE_SOUND /*Define this to use Sound*/

#include "GTLibc.c"

/*Defining trainer bg and fg colors*/
#define TRAINER_FG FG_LIGHTCYAN
#define TRAINER_BG FG_BLACK

/*@anony enum for enable/disable COLORS options*/
enum {ENABLE_COLOR = FG_LIGHTGREEN  | TRAINER_BG ,DISABLE_COLOR = FG_LIGHTRED  | TRAINER_BG , STATUS_COLOR = TRAINER_FG  | TRAINER_BG };

/*@anony enum for status options*/
enum {STATUS_ENABLE,STATUS_DISABLE};

/*@anony enum for Weapon set*/
enum {NUTTER_TOOLS,THUG_TOOLS,PROFESSIONAL_TOOLS};

/************************************
*GTA-SA Trainer using GTLibc library*
*************************************/

BOOL initGameTrainer();
void runGameTrainer();
BOOL setUnlimitedMoney();
BOOL setUnlimitedHealth();
BOOL setUnlimitedAmmo();
void showStatus(LPSTR,UINT);

void testProcess();
void testWindow();
void testSearch();

LPVOID money_address = NULL,health_address = NULL,weapon_base_address = NULL;
LPSTR status = "Undefined!";
UINT status_type = NIL,status_len = NIL;

/*Global constants for sound paths*/
LPCSTR enable_path = "resources\\enable_sfx.wav",disable_path = "resources\\disable_sfx.wav",loading_path = "resources\\loading_sfx.wav";

int main()
{
    GT_EnableLogs();
    if(initGameTrainer())
    {
        //initialize complete now run game trainer.
        runGameTrainer();
    }

    return 0;
}

//initialize game trainer with process ID, handle , base address, weapon address etc.
BOOL initGameTrainer()
{
    DWORD processID = NIL;
    LPBYTE base_address = NULL;
    HANDLE game_handle = NULL;
    BOOL game_found = FALSE;
    LPCSTR game_name = "gta_sa", window_name = "GTA_SA +6 Trainer - Ha5eeB";

    //Making GTA:SA trainer example.
    //Open game and get process id, handle and base address.
    game_handle = GT_FindGameProcess(game_name);
    if (game_handle != NULL)
    {
   		/*Set window properties*/

		//set console background color.
		GT_SetConsoleBGColor(TRAINER_BG);

        //Set console windows text.
        GT_SetWindowTitle(window_name);
        GT_SetConsoleTextColor(TRAINER_FG  | TRAINER_BG );

        //set console window size.
        GT_ResizeWindow(250,250,485,600);

        //set console window style.
		GT_DisableWindowStyle(GWL_STYLE,WS_SIZEBOX | WS_MAXIMIZEBOX);

        //remove vertical scroll bar.
        GT_DisableWindowScrollbar(WS_VSCROLL);

        //remove blinking cursor.
        GT_ShowConsoleCursor(FALSE);

		/*Get process ID handle etc*/
        processID = GT_GetProcessID();
        game_handle = GT_GetGameHandle();
        base_address = GT_GetGameBaseAddress(processID);
    }
    //don't check for if game not found because that is handled by Library itself.


    //check for valid process ID and print game info.
    if (processID != 0)
    {
        game_found = TRUE;
        //GT_WriteConsole("Game Found %s\n",GT_GetGameName());
        //GT_WriteConsole("processID = %d\ngame_handle : %p\n\n", processID,game_handle);

        //example of static address.
        money_address = (LPVOID)0xB7CE50;

        //Example of getting base weapon address.
        weapon_base_address =  (LPVOID)((DWORD)GT_ReadPointerOffset(base_address,0x0076F3B8) + (DWORD)0x5E0);

        //Example of GT_ReadPointerOffset & GT_ReadPointerOffsets.
        DWORD health_offsets[] = {0x144,0x160};
        LPVOID base_health = GT_ReadPointerOffset(base_address,0x0076F3B8);
        health_address = (LPVOID)((DWORD)GT_ReadPointerOffsets(base_health,health_offsets,sizeof(health_offsets)) + (DWORD)0x540);

        /*base address is address of pistol in this case.
           GT_ReadAddress example.*/
        DWORD pistol_value = *(PDWORD)GT_ReadAddress(weapon_base_address);

        //GT_ReadAddressOffset example.
        GT_ReadAddressOffset(weapon_base_address,0x20);
    }
    return game_found;
}

//run game trainer after initializing.
void runGameTrainer()
{

    /***********************************************/
    /****************GTA-SA TRAINER*****************/
    /***********************************************/

	//Trainer Menu variables.
    LPCSTR lp_menu_items[] ={"Unlimited Money","Unlimited Health","Unlimited Ammo","Nutter Tools","Thug Tools","Professional Tools","Exit Trainer"};
    LPCSTR lp_hotkey_items[] = {"[F4]\n","[F5]\n","[F6]\n","[F7]\n","[F8]\n","[F9]\n","[INSERT]\n"};
    LPCSTR lp_menu_separators[] = {"\t\t","\t","\t\t","\t\t","\t\t","\t","\t\t"};
    SIZE_T sz_menu = sizeof(lp_menu_items) / sizeof(lp_menu_items[0]);

	WORD menu_colors[sz_menu],hkey_colors[sz_menu];
	SHORT op_x[sz_menu],op_y[sz_menu];

	/*Co-ordinates of hotkeys options*/
	COORD hkey_coords = {24,2};

	//Fill menu with same options for color and x,y co-ordinates.
	FillMemory(menu_colors,sizeof(menu_colors),ENABLE_COLOR);
	FillMemory(hkey_colors,sizeof(hkey_colors),DISABLE_COLOR);
	FillMemory(op_x,sizeof(op_x),-1);
	FillMemory(op_y,sizeof(op_y),-1);


    //Printing Heading.
    GT_SetConsoleTextColor(TRAINER_FG  | TRAINER_BG);
	GT_WriteConsoleXY(5,0,"GTA-SA +6 TRAINER (V 1.0.0)\n\n");

	//create Menu items and hotkeys.
	GT_CreateMenu(lp_menu_items,lp_hotkey_items,lp_menu_separators,sz_menu,menu_colors,hkey_colors,op_x,op_y);


    //Cheats list for weapons set.
    char *weapons_cheats[] = {"UZUMYMW","LXGIWYL","KJKSZPJ"};

    //running main game loop.
    while(TRUE)
    {

        //Applying unlimited money,health and ammo .

        if(GT_IsKeyToggled(VK_F4))
        {
            if(setUnlimitedMoney())
            {
            	status = "[+]Money Enabled!";
            	status_type = STATUS_ENABLE;

                //change hotkey COLORS to enable and play enable sound.
                GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y,"[F4]");
                GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);
                GT_StopSound();
            }
        }

        else if(GT_IsKeyToggled(VK_F5))
        {
            if(setUnlimitedHealth())
            {
            	status = "[+]Health Enabled!";
            	status_type = STATUS_ENABLE;

                //change hotkey COLORS to enable and play enable sound.
                GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y + 1,"[F5]");
                GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);
            }
        }

        else if(GT_IsKeyToggled(VK_F6))
        {
            if(setUnlimitedAmmo())
           	{
            	status = "[+]Ammo Enabled!";
            	status_type = STATUS_ENABLE;

                //change hotkey COLORS to enable and play enable sound.
                GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y + 2,"[F6]");
                GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);
            }
        }

        //Applying weapons cheat codes.
        else if(GT_IsKeyToggled(VK_F7))
        {
            GT_SetCheatCode(weapons_cheats[NUTTER_TOOLS]);
      		status = "[+]Nutter tools Enabled!";
       		status_type = STATUS_ENABLE;

         	//change hotkey COLORS to enable and play enable sound.
         	GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y + 3,"[F7]");
          	GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);
        }

        else if(GT_IsKeyToggled(VK_F8))
        {
            GT_SetCheatCode(weapons_cheats[THUG_TOOLS]);
      		status = "[+]Thug tools Enabled!";
       		status_type = STATUS_ENABLE;

         	//change hotkey COLORS to enable and play enable sound.
         	GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y + 4,"[F8]");
          	GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);

        }

        else if(GT_IsKeyToggled(VK_F9))
        {
            GT_SetCheatCode(weapons_cheats[PROFESSIONAL_TOOLS]);
      		status = "[+]Prof. tools Enabled!";
       		status_type = STATUS_ENABLE;

         	//change hotkey COLORS to enable and play enable sound.
         	GT_WriteConsoleXY(hkey_coords.X,hkey_coords.Y  + 5,"[F9]");
          	GT_PlaySound(enable_path,SND_FILENAME | SND_ASYNC);
        }

        else if(GT_IsKeyToggled(VK_INSERT))
        {
   	    	GT_WriteConsoleXY(0,9,"Trainer created using GTLibc ");
            break;
        }

     //print status info.
     showStatus(status,status_type);
    }
}

BOOL setUnlimitedMoney()
{
    DWORD unlimited_money = 0x7FFFFFFFUL;
    BOOL write_status = GT_WriteAddress(money_address,&unlimited_money);
    return write_status;
}

BOOL setUnlimitedHealth()
{
    DWORD unlimited_health = 0x62C80000;
    BOOL write_status = GT_WriteAddress(health_address,&unlimited_health);
    return write_status;
}


BOOL setUnlimitedAmmo()
{

    DWORD unlimited_ammo = 0x7FFFFFFFUL;
	BOOL write_status = FALSE;
    //list of weapons and clips offsets from base weapon address.
    static DWORD weapons_offsets[] = {0x0,0x4,0x1C,0x20,0x38,0x3C,0x54,0x58,0x74,0x90,0xAC,0xC4,0xC8};
    size_t offsets_size = sizeof(weapons_offsets);

    //GT_ReadAddressOffsets example.
    DWORD *ammos_list = (PDWORD)GT_ReadAddressOffsets(weapon_base_address,weapons_offsets,offsets_size);

    if(ammos_list == NULL)
    {
        GT_WriteConsole("ammos_list is NULL, error code : 0x%X\n",GetLastError());
    }

    //printing weapons ammo from offsets list.
    //UINT index = NIL;
    //for(index = 0; index < offsets_size / sizeof(DWORD); index++)
    //GT_WriteConsole("Weapon No %d Ammo at offset 0x%X : %u\n",(index + 1),weapons_offsets[index]);


    write_status = GT_WriteAddressOffsets(weapon_base_address,weapons_offsets,offsets_size,&unlimited_ammo);

    //free memory using HeapFree() example.
    if(!HeapFree(GetProcessHeap(),NIL,ammos_list))
    {
        GT_WriteConsole("Error while freeing memory, error code : 0x%X\n",GetLastError());

        DWORD exit_value;
        GetExitCodeProcess(GetCurrentProcess(),&exit_value);
        ExitProcess(exit_value);
    }

    return write_status;
}

void showStatus(LPSTR status,UINT status_type)
{
    UINT sz_status = lstrlen(status);

    if(sz_status > 0)
    {
        if(sz_status < status_len)
        {
        	//clear status.
            GT_ClearConsoleText(10,11,status_len);
        }
    }

	//print status static text.
    GT_SetConsoleTextColor(STATUS_COLOR);
    GT_WriteConsoleXY(0,11,"Status : ");

	//print status message.
    (status_type == STATUS_ENABLE) ? GT_SetConsoleTextColor(ENABLE_COLOR) : (status_type == STATUS_DISABLE) ? GT_SetConsoleTextColor(DISABLE_COLOR) : GT_SetConsoleTextColor(TRAINER_FG  | TRAINER_BG );
    GT_WriteConsole("%s",status);

    if(sz_status > 0)
        status_len = sz_status;
}


//Example of FindGame by Process name.
void testProcess()
{
	//enabling/disabling logs example.
    if(GT_EnableLogs())
    {
        GT_WriteConsole("GTLibc Logs are enabled in this trainer.\n");
    }

    else if(GT_DisableLogs())
    {
        GT_WriteConsole("GTLibc Logs are disabled in this trainer.\n");
    }

    DWORD processID = 0;
    HANDLE game_handle = NULL;
    HWND g_hwnd = NULL;

    //ask for input to open game.
    char g_name[MAX_PATH] = {NUL};
    GT_WriteConsole("Enter process/game name to open\n");
    GT_ReadConsole(g_name,sizeof(g_name),CONSOLE_READ_STR);

    //find game by process name.
    game_handle = GT_FindGameProcess(g_name);
    if(game_handle != NULL)
    {
        processID = GT_GetProcessID();
        g_hwnd = GT_GetGameHWND();
    }

    if (processID != 0)
    {
        GT_WriteConsole("Name : %s\tprocessID = %d\n",GT_GetGameName(),processID);
        GT_WriteConsole("Game handle = %p\nGame HWND = %p\n", game_handle,g_hwnd);
    }
}

//Example of FindGame by Window name.
void testWindow()
{
    DWORD processID = 0;
    HANDLE game_handle = NULL;

    //ask for input to open game.
    char g_window[MAX_PATH] = {NUL};
    GT_WriteConsole("Enter game window name to open\n");
    GT_ReadConsole(g_window,sizeof(g_window),CONSOLE_READ_STR);

    HWND g_hwnd = GT_FindGameWindow(g_window);

    if (g_hwnd != NULL)
    {
        game_handle = GT_GetGameHandle4mHWND(g_hwnd);
        processID = GT_GetProcessID4mHWND(g_hwnd);

        GT_WriteConsole("Window name : %s\tprocessID = %d\n",g_window,processID);
        GT_WriteConsole("Game handle = %p\tWindow handle = %p\n", game_handle,g_hwnd);
    }
}

//Example of SearchOffsetArea.
void testSearch()
{

    size_t offset_limit = 100; //limit of searching offset.
    size_t offset_size = 4; // size in bytes 2 bytes,4bytes etc...
    int search = 17; //value to search.

    char *search_list = GT_SearchOffsetArea(weapon_base_address,offset_limit,offset_size,search);
    GT_WriteConsole("%s\n",search_list);

    //free memory from search list after printing using HeapFree() method.
    if(search_list == NULL)
    {
        GT_WriteConsole("Search list is NULL, error code : 0x%X\n",GetLastError());
    }

    //if error occurred while freeing then exit with error code.
    else
    {
        if(!HeapFree(GetProcessHeap(),NIL,search_list))
        {
            GT_WriteConsole("Error while freeing memory, error code : 0x%X\n",GetLastError());
        }
    }

    //exit process with exit code.
    DWORD exit_value;
    GetExitCodeProcess(GetCurrentProcess(),&exit_value);
    ExitProcess(exit_value);
}
