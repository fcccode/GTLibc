#include "GTLibc.h"

/*Global variables for storing game information*/
DWORD process_id = NIL;
HANDLE game_handle = (HANDLE)NULL;
CHAR game_name[MAX_PATH] = { NUL };
HWND game_hwnd = (HWND)NULL;


/*Global variable for storing error code*/
DWORD error_code = NIL;

/*Setting private methods inaccessible*/
BOOL private_field = FALSE;

/*Setting add Logs to disable by default.*/
BOOL logs_enabled = FALSE;

/*GTLibc -library to make game trainer for c/c++.*/

/****************************************************************************/
/*********************-PUBLIC-METHODS-***************************************/
/****************************************************************************/

/**
 * @description - Find game by process name.
 * @param - Game name in string format (CASE INSENSTIVE). and without '.exe' extension.
 * @return - If game found it returns HANDLE to game otherwise returns NULL
 * NOTE : Process name is name of your .exe file loaded in Memory.
 * PS : If multiple versions of game found then you have to update the GUI logic for this.
 */

HANDLE findGameProcess(LPCSTR game_name)
{
    private_field = TRUE;
    SetLastError(NO_ERROR);

    UINT index = NIL,game_len = NIL,sz_exe_len = NIL,game_exe_len = lstrlen(game_name);
    LPSTR game_name_exe = NULL,game_exe = NULL;
	
 	//process info variables.
  	CHAR p_name[MAX_PATH] = { '\0' };
   	DWORD p_id = NIL;
   	HANDLE p_handle = (HANDLE)NULL;
    HWND p_hwnd = (HWND)NULL;
	
 	LPCSTR sz_exe_file = NULL;
	
 	PROCESSENTRY32 entry;
	HANDLE snapshot = NULL;
	BOOL game_found = FALSE;
	
    if (logs_enabled)
    {
        addLog("%s -> game_name input : %s\n", FUNC_NAME, game_name);
        private_field = TRUE;
    }

    try
    {
        entry.dwSize = sizeof(PROCESSENTRY32);

		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NIL);
  		game_found = FALSE;
        error_code = NIL;

        //allocate enough memory to store game name + ".exe" extension.
        game_exe = GT_MemAlloc(HEAP_ZERO_MEMORY, game_exe_len + 4);

        //append ".exe" extension to match with szExeFile.
        lstrcpy(game_exe, game_name);
        lstrcat(game_exe,".exe");

        private_field = TRUE;
        //Main loop for iterating input game in process list.
        if (Process32First(snapshot, &entry))
        {
            while (Process32Next(snapshot, &entry))
            {
                if (!lstrcmpi(entry.szExeFile, game_exe))
                {
                    game_found = TRUE;
                    game_len = lstrlen(entry.szExeFile);
                    game_name_exe = GT_MemAlloc(HEAP_ZERO_MEMORY,game_len);
                    private_field = TRUE;
                    
					sz_exe_len = lstrlen(entry.szExeFile);
     				sz_exe_file = GT_MemAlloc(HEAP_ZERO_MEMORY, sz_exe_len);
                    private_field = TRUE;

                    //copy exe file name.
                    lstrcpy(sz_exe_file, entry.szExeFile);

                    //remove '.exe' part from game name.
                    for (index = 0; index < lstrlen(sz_exe_file); index++)
                    {
                        if (sz_exe_file[index] == '.')
                            break;

                        game_name_exe[index] = sz_exe_file[index];
                    }

                    GT_MemFree(game_exe);
                    private_field = TRUE;

                    GT_MemFree(game_name_exe);
                    private_field = TRUE;

                    GT_MemFree(sz_exe_file);
                    private_field = TRUE;
                    break;
                }
            }

            //if game found.
            if (game_found)
            {
                //copy current process info.
                lstrcpy(p_name,game_name_exe);
                p_id = entry.th32ProcessID;
                p_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                //set current process info.
                setGameName(p_name);
                setProcessID(p_id);
                setGameHandle(p_handle);
                setGameHWND(p_id);

                p_hwnd = getGameHWND();
                private_field = TRUE;


                if (logs_enabled)
                {
                    addLog("Game \tname %s\tid : %u\tHandle : 0x%X\thwnd : 0x%X\n", p_name, p_id, p_handle, p_hwnd);
                    private_field = TRUE;
                }

            }

            else
            {
                CloseHandle(snapshot);
                CloseHandle(game_handle);

                showError(ERROR_FILE_NOT_FOUND, FUNC_NAME, LINE_NO);
                private_field = TRUE;
            }
        }
        else
        {
            CloseHandle(snapshot);
            CloseHandle(game_handle);

            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if (logs_enabled)
    {
        addLog("%s  ->  returned handle : %p\n", FUNC_NAME, game_handle);
    }
    private_field = FALSE;
    return p_handle;
}

/**
 * @description - Find game by window name.
 * @param - window name in string format (CASE INSENSITIVE).
 * @return - if game window found then it returns HWND to that window otherwise returns NULL
 * NOTE : Windows name is name of your Game Process Window not the .exe file.
 */

HWND findGameWindow(LPCSTR window_name)
{
    private_field = TRUE;
    HWND game_window = NULL;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s -> window_name input : %s\n",FUNC_NAME,window_name);
        private_field = TRUE;
    }


    try
    {
		game_window = FindWindowEx((HWND)NULL, (HWND)NULL, NUL, window_name);
        
		if (game_window == NULL || *window_name == NUL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned game_window : %p\n",FUNC_NAME,game_window);
    }
    private_field = FALSE;
    return game_window;
}

/**
 * @description - Read value from provided address.
 * @param - Address in void* format.
 * @return - If read succeeded then it returns new value otherwise returns NIL.
 */

DWORD readAddress(LPVOID address)
{
    HANDLE game_handle = getGameHandle();
    DWORD dw_value = NIL;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s -> trying to read from address %p\n",FUNC_NAME,address);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NULL)
        {
            if(ERROR_INVALID)
            {
            error_code = GetLastError();
            throw(error_code);	
            }
        }

        else
        {
            if(!ReadProcessMemory(game_handle,address,&dw_value, sizeof(dw_value),NULL))
            {
                if(ERROR_INVALID)
                {
                error_code = GetLastError();
                throw(error_code);	
                }
            }

        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned value : %u\n",FUNC_NAME,dw_value);
    }

    private_field = FALSE;
    return dw_value;
}

/**
 * @description - Write value at provided address.
 * @param - Address in void* format and value to be written in DWORD format.
 * @return - If write is succeeded then it returns TRUE otherwise returns FALSE
 */

BOOL writeAddress(LPVOID address,DWORD value)
{

    HANDLE game_handle = getGameHandle();
    BOOL write_status = FALSE;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s -> Trying to write value %p at address %p\n",FUNC_NAME,value,address);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            if(!(write_status =  WriteProcessMemory(game_handle,address,&value,sizeof(value),NULL)))
            {
                if(!write_status)
                {
                    if(ERROR_INVALID)
                    {
                        error_code = GetLastError();
                        throw(error_code);
                    }
                }
            }

        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned status : %d\n",FUNC_NAME,write_status);
    }

    private_field = FALSE;
    return write_status;
}


/**
 * @description - Read value from provided address with offset.
 * @param - Address in void* format and offset in DWORD format.
 * @return - If read succeeded then it returns address of pointer otherwise returns NIL.
 */

DWORD readAddressOffset(LPVOID lp_address, DWORD dw_offset)
{
    HANDLE game_handle = getGameHandle();
    DWORD dw_value = NIL;

    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;
    private_field = TRUE;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s -> Trying to read value from address %p with offset %p\n",FUNC_NAME,lp_address,dw_offset);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            dw_value = readAddress((LPVOID)lpdw_address);
            private_field = TRUE;
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned value : %u\n",FUNC_NAME,dw_value);
    }

    private_field = FALSE;
    return dw_value;
}

/**
 * @description - Write value at provided address with offset.
 * @param - Address in void* format and offset in DWORD format,value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise returns FALSE.
 */

BOOL writeAddressOffset(LPVOID lp_address, DWORD dw_offset, DWORD dw_value)
{
    HANDLE game_handle = getGameHandle();
    BOOL write_status = FALSE;
    error_code = NIL;
    private_field = TRUE;


    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;

    if(logs_enabled)
    {
        addLog("%s -> Trying to write value %u at address %p with offset %p\n",FUNC_NAME,dw_value,lp_address,dw_offset);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            write_status = writeAddress((LPVOID)lpdw_address,dw_value);
            private_field = TRUE;


            if(!write_status)
            {
                if(ERROR_INVALID)
                    error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned status : %d\n",FUNC_NAME,write_status);
    }

    private_field = FALSE;
    return write_status;
}

/**
 * @description - Read value from provided address with provided offsets.
 * @param - Address in void* format, offsets in DWORD* format and size of offsets.
 * @return - If read succeeded then it returns list of values otherwise returns NULL
 * NOTE : This will be useful in reading multiple values at a time like multiple Ammos/Clips from Ammos/Clips offsets list.
 * PS : FREE this memory after using it to avoid memory leaks use HeapFree() Method from (windows.h).
 */

DWORD* readAddressOffsets(LPVOID lp_address, DWORD *dw_offsets,SIZE_T sz_offsets)
{
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    DWORD *dw_values = NULL;
    private_field = TRUE;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s -> Trying to read values from address %p with offsets\n",FUNC_NAME,lp_address);
        private_field = TRUE;
    }

    try
    {
  		dw_values = GT_MemAlloc(HEAP_ZERO_MEMORY,sz_offsets);
    		
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else if(dw_values == NULL)
        {
            if(ERROR_INVALID)
            {
                if(logs_enabled)
                {
                    addLog("%s -> %s\n",FUNC_NAME,"offset_values is empty");
                    private_field = TRUE;
                }
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            for (index = 0; index < sz_offsets/sizeof(DWORD); index++)
            {
                dw_values[index] = readAddressOffset(lp_address,dw_offsets[index]);
                private_field = TRUE;

                if(logs_enabled)
                {
                    addLog("Values[%u] : %p\tOffsets[%u] : %u\n",index,dw_values[index],index,dw_offsets[index]);
                    private_field = TRUE;
                }
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned values : %p\n",FUNC_NAME,dw_values);
    }

    private_field = FALSE;
    return dw_values;
}

/**
 * @description - Write value at address with offsets.
 * @param - Address in void* format and offset in DWORD* format,Size in SIZE_T format and value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise returns FALSE.
  * NOTE : This will be useful in writing multiple values at a time like multiple ammo/clips values at Ammos/Clips offsets list.
 */

BOOL writeAddressOffsets(LPVOID lp_address, DWORD *dw_offsets,SIZE_T sz_offsets,DWORD dw_value)
{
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s -> Trying to write value %u at address %p with offsets\n",FUNC_NAME,dw_value,lp_address);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {

            for (index = 0; index < sz_offsets/sizeof(DWORD); index++, dw_offsets++)
            {
                write_status = writeAddressOffset(lp_address,*dw_offsets,dw_value);
                private_field = TRUE;

                if(logs_enabled)
                {
                    addLog("Offsets[%u] : %p\t with write status %d\n",index,dw_offsets[index],write_status);
                    private_field = TRUE;
                }


                if(!write_status)
                {
                    if(ERROR_INVALID)
                        error_code = GetLastError();
                    throw(error_code);
                }
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned status : %d\n",FUNC_NAME,write_status);
    }

    private_field = FALSE;
    return write_status;
}


/**
 * @description - Read pointer's address from provided address with offset.
 * @param - Address in void* format and offset in DWORD format.
 * @return - If read succeeded then it returns address of pointer otherwise returns NULL.
 */

LPVOID readPointerOffset(LPVOID lp_address, DWORD dw_offset)
{
    HANDLE game_handle = getGameHandle();
    LPVOID lp_address_value = NULL;
    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;
    private_field = TRUE;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s -> Trying to read pointer from address %p with offset %p\n",FUNC_NAME,lp_address,dw_offset);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            lp_address_value = (LPVOID)readAddress((LPVOID)lpdw_address);
            private_field = TRUE;


            if (lp_address_value == NULL)
            {
                if(ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }


                //if Error not set explicitly by readAddress.
                else if(error_code == ERROR_SUCCESS)
                {
                    error_code = ERROR_PARTIAL_COPY;
                    SetLastError(error_code);
                    throw(error_code);
                }
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned pointer : %p\n",FUNC_NAME,lp_address_value);
    }

    private_field = FALSE;
    return lp_address_value;
}


/**
 * @description - Read pointer's address from provided address with provided offsets.
 * @param - Address in void* format, offsets in DWORD* format and size of offsets.
 * @return - If read succeeded then it returns address of pointer otherwise returns NULL.
 */

LPVOID readPointerOffsets(LPVOID lp_address, DWORD *dw_offsets,SIZE_T sz_offsets)
{
    HANDLE game_handle = NULL;
    UINT index = NIL;
    LPDWORD lpdw_address = NULL;
    error_code = NIL;
    private_field = TRUE;
		
    if(logs_enabled)
    {
        addLog("%s  ->  Trying to read pointer from address %p with offsets\n",FUNC_NAME,lp_address);
        private_field = TRUE;
    }


    try
    {
    	game_handle = getGameHandle();
    	private_field = TRUE;
    	
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {
			lpdw_address = (LPDWORD)lp_address;
				
            for (index = 0; index < sz_offsets/sizeof(DWORD); index++)
            {
                lpdw_address = (LPDWORD)readPointerOffset((LPVOID)lpdw_address,dw_offsets[index]);
                private_field = TRUE;

                if(logs_enabled)
                {
                    addLog("offsets[%u] : %p read pointer address : %p\n",index,dw_offsets[index],lpdw_address);
                    private_field = TRUE;
                }

                if (lpdw_address == NULL)
                {

                    if(ERROR_INVALID)
                        error_code = GetLastError();

                    //if Error not set explicitly by readAddress.
                    else if(error_code == ERROR_SUCCESS)
                    {
                        error_code = ERROR_PARTIAL_COPY;
                        SetLastError(error_code);
                    }

                    throw(error_code);
                }
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned pointer : %p\n",FUNC_NAME,(LPVOID)lpdw_address);
    }
    private_field = FALSE;
    return (LPVOID)lpdw_address;
}

/**
 * @description - Write value at pointer's address with offset.
 * @param - Address in void* format and offset in DWORD format,value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise returns FALSE.
 */

BOOL writePointerOffset(LPVOID lp_address, DWORD dw_offset, DWORD dw_value)
{
    HANDLE game_handle = getGameHandle();
    DWORD dw_base_address_value = NIL,dw_real_address = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s  ->  Trying to write value %u at pointer %p with offset %p\n",FUNC_NAME,dw_value,lp_address,dw_offset);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        dw_base_address_value = readAddress(lp_address);
        private_field = TRUE;

        if(dw_base_address_value == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            dw_real_address = dw_base_address_value + dw_offset;
            write_status = writeAddress((LPVOID)dw_real_address,dw_value);
            private_field = TRUE;


            if(!write_status)
            {
                if(ERROR_INVALID)
                    error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned status : %d\n",FUNC_NAME,write_status);
    }

    private_field = FALSE;
    return write_status;
}

/**
 * @description - Write value at pointer's address with offsets.
 * @param - Address in void* format and offset in DWORD* format,Size in SIZE_T format and value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise returns FALSE.
 */

BOOL writePointerOffsets(LPVOID lp_address, DWORD *dw_offsets,SIZE_T sz_offsets,DWORD dw_value)
{
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s  ->  Trying to write value : %u at pointer %p with offsets\n",FUNC_NAME,dw_value,lp_address);
        private_field = TRUE;
    }


    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {

            for (index = 0; index < sz_offsets/sizeof(DWORD); index++, dw_offsets++)
            {
                write_status = writePointerOffset(lp_address,*dw_offsets,dw_value);

                if(logs_enabled)
                {
                    addLog("Offsets[%u] : %u\twith write status : %d\n",index,dw_offsets[index],write_status);
                    private_field = TRUE;
                }



                if(!write_status)
                {
                    if(ERROR_INVALID)
                        error_code = GetLastError();
                    throw(error_code);
                }
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned status : %d\n",FUNC_NAME,write_status);
    }

    private_field = FALSE;
    return write_status;
}

/**
 * @description -  Get current game name from memory.
 * @return - If game found returns game name otherwise returns NUL.
 */

LPCSTR getGameName()
{
    private_field = TRUE;
    LPCSTR g_name = NUL;

    if((game_name[0] != NUL))
    {
        g_name = game_name;
    }


    else
    {
        g_name = NUL;
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned : %s\n",FUNC_NAME,g_name);
    }

    private_field = FALSE;
    return g_name;
}

/**
 * @description -  Get process ID of game from memory.
 * @return - If game found returns process ID of current game otherwise returns NIL.
 */

DWORD getProcessID()
{
    private_field = TRUE;
    DWORD p_id = NIL;

    if((process_id	!= NIL))
    {
        p_id = process_id;
    }

    else
    {
        p_id = NIL;
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned : %u\n",FUNC_NAME,p_id);
    }

    private_field = FALSE;
    return p_id;
}

/**
 * @description - Get game handle from HWND (handle to window).
 * @param - Handle to current window of game in HWND format.
 * @return - Handle to process on success otherwise returns NULL.
 * NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
 */

HANDLE getGameHandle4mHWND(HWND g_hwnd)
{
    private_field = TRUE;
    DWORD p_id = NIL;
    HANDLE g_handle = NULL;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s  ->  input Handle :  %p\n",FUNC_NAME,g_hwnd);
        private_field = TRUE;
    }


    try
    {
        if (g_hwnd == NULL || g_hwnd == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            p_id = getProcessID4mHWND(g_hwnd);

            if (p_id == NIL)
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }
            g_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, p_id);
        }

        //set new valid game handle from HWND.
        private_field = TRUE;
        if (g_handle != NULL)
            setGameHandle(g_handle);

        else
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned handle :  %p\n",FUNC_NAME,g_handle);
    }

    private_field = FALSE;
    return g_handle;
}

/**
 * @description - Get process ID of game from HWND (handle to window).
 * @param - Handle to current window of game in HWND format.
 * @return - On success it returns game's process ID otherwise returns NIL.
 * NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
 */

DWORD getProcessID4mHWND(HWND g_hwnd)
{
    private_field = TRUE;
    DWORD p_id = NIL;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s  ->  input Handle :  %p\n",FUNC_NAME,g_hwnd);
        private_field = TRUE;
    }

    try
    {
        if (g_hwnd == NULL || g_hwnd == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }

        }

        else
            GetWindowThreadProcessId(g_hwnd, &p_id);
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned process id :  %u\n",FUNC_NAME,p_id);
    }

    private_field = FALSE;
    return p_id;
}

/**
 * @description - Get base address of current game.
 * @param - Process ID in DWORD format.
 * @return - On success it returns base address of game in (LPBYTE) format otherwise returns NUL.
 */

LPBYTE getGameBaseAddress(DWORD process_id)
{
    private_field = TRUE;
    SetLastError(NO_ERROR);

    HANDLE snapshot = NULL;
    MODULEENTRY32 module;
    LPBYTE base_address = NUL;
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s  ->  input process id :  %u\n",FUNC_NAME,process_id);
        private_field = TRUE;
    }


    try
    {
        if (snapshot == NULL || snapshot == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
            else if(error_code == NIL)
            {
                error_code = ERROR_INVALID_PARAMETER;
                throw(error_code);
            }
        }

        else
        {
            module.dwSize = sizeof(MODULEENTRY32);
            Module32First(snapshot, &module);
            CloseHandle(snapshot);
            base_address = module.modBaseAddr;
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned address :  %p\n",FUNC_NAME,base_address);
    }

    private_field = FALSE;
    return base_address;
}

/**
 * INFO : Hot keys can be in combination of like hotKeysDown(LCTRL,VK_F1) or hotKeysDown(LSHIFT,'F')
 * @description - check for Hot keys combinations pressed or not.
 * @param - Combination of hot keys using virtual key_codes or characters A-Z,a-z.
 * @return - If keys pressed it will return TRUE otherwise returns FALSE.
 * PS : Don't use this method directly instead use 'hotKeysPressed' MACRO.
 */

BOOL hotKeysDown(int key, ...)
{
    short result;
    va_list list;

    result = GetAsyncKeyState(key);
    for (va_start(list, key); key; key = va_arg(list, int))
        result &= GetAsyncKeyState(key);

    va_end(list);

    return ((result & 0x8000) != 0);
}

/**
 * @description - Check if provided key is pressed or not.
 * @param - virtual key_code in int format. ex : (VK_SHIFT).
 * @return - If key is pressed it returns TRUE otherwise returns FALSE.
 * NOTE : This method must be in main game running loop or any continuous loop.
 */

BOOL isKeyPressed(const int key)
{
    SHORT key_state = GetAsyncKeyState(key);
    BOOL is_down = key_state & 0x8000;
    return is_down;
}

/**
 * @description - Check if provided key is toggled or not.
 * @param - virtual key_code in int format. ex : (VK_SHIFT).
 * @return - If key is toggled it returns TRUE otherwise returns FALSE.
 * NOTE : This method must be in main game running loop or any continuous loop.
 */

BOOL isKeyToggled(const int key)
{
    SHORT key_state = GetAsyncKeyState(key);
    BOOL is_toggled = key_state & 0x1;
    return is_toggled;
}

/****************************************************************************/
/****************-SEMI-PRIVATE-METHODS-**************************************/
/****************************************************************************/

/**
 * INFO : Send Mouse input to current game.
 * @description - Send mouse input control to game.
 * @param - mouse code in int format. ex : use these macros MOUSE_LEFT_ID or MOUSE_RIGHT_ID.
 * NOTE : This will be useful if you want to create some automated scripting for your game.
 */

void doMousePress(int mouse_code)
{
    private_field = TRUE;
    doVirtualKeyPress(INPUT_MOUSE, NIL, mouse_code);
    private_field = FALSE;
}

/**
 * INFO : Send Keyboard input to current game.
 * @description - Send keyboard input control to game.
 * @param - Virtual key_code in int format. ex : VK_CONTROL,VK_SHIFT. (see winuser.h for full list of Key codes.)
 * NOTE : This will be useful if you want to create some automated scripting for your game.
 */

void doKeyPress(int key_code)
{
    private_field = TRUE;
    doVirtualKeyPress(INPUT_KEYBOARD, key_code, NIL);
    private_field = FALSE;
}

/**
 * @description - Apply provided cheat code to current game.
 * @param - cheat code in string format, (NULL terminated).
 */
void setCheatCode(LPCSTR cheat_code)
{
	private_field = TRUE;
	UINT index = NIL;
	size_t cheat_len = lstrlen(cheat_code);
	LPSTR cheat_buf = GT_MemAlloc(HEAP_ZERO_MEMORY, cheat_len + 1);

	if (cheat_buf == NULL)
	{
		if (ERROR_INVALID)
		{
			private_field = TRUE;
			error_code = GetLastError();
			showError(error_code, FUNC_NAME, LINE_NO);
		}
	}

	CopyMemory(cheat_buf, cheat_code, sizeof(CHAR) * cheat_len + 1);

	//convert cheat code to upper case for better mapping of characters.
	LPCSTR lp_cheat_upper = CharUpper(cheat_buf);

	//Time delay before entering first char of cheat.
	Sleep(200);

	//Press all the cheat keys from cheat code.
	for (index = 0; index < cheat_len; index++)
	{
		doKeyPress(lp_cheat_upper[index]);
	}

	GT_MemFree(cheat_buf);
	private_field = FALSE;
}

/**
 * INFO : Search any value in current offset ie. (base_address + offset) for finding new heath/ammos/weapons in game.
 * @description -  Search value in offset area.
 * @param - base address of Ammo/health/clips etc in void*,offset limit in size_t,offset size and value for searching.
 * @return - If value found it returns its address and offset in formatted string otherwise returns NULL
 * NOTE : FREE this memory after using it to avoid memory leaks use HeapFree() Method from (windows.h).
 */

LPSTR searchOffsetArea(LPVOID offset_base_address, const size_t offset_limit, const size_t offset_size, DWORD search)
{
    private_field = TRUE;
    int value = 0, offset_index = 0, offset = 0,offset_len = (offset_limit / offset_size);
    DWORD offset_base = (DWORD)offset_base_address;
    DWORD offset_address = offset_base;
    int search_list_len = offset_len * 0x40;
    error_code = NIL;

    if(logs_enabled)
    {
        addLog("%s  ->  Trying to search value : %u from address %p with offset limit : %u and offset size : %u\n",FUNC_NAME,search,offset_base_address,offset_limit,offset_size);
        private_field = TRUE;
    }


    DWORD dw_search_size = search_list_len * sizeof(char);
    LPSTR search_list = GT_MemAlloc(HEAP_ZERO_MEMORY,dw_search_size);


    try
    {
        if (search_list == NULL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }

        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> Search list size is : %u\n",FUNC_NAME,sizeof(search_list));
        private_field = TRUE;
    }

    //copy headline status to search list.
    wsprintf(search_list, "%s\n", "SearchOffsetArea status : ");

    for (offset_index = 0; offset_index < offset_len; offset_index++)
    {
        offset_address += offset_size;
        offset = (int)(offset_address - offset_base);
        value = readAddress((LPVOID)offset_address);
        private_field = TRUE;


        //if value found then copy its offset address etc in formatted string.
        if (value == search)
            wsprintf(search_list + lstrlen(search_list), "Value : %d\tAddress : %p\tOffset : %p\n", value, offset_address, offset);
    }

    if(logs_enabled)
    {
        addLog("%s  ->  returned values : %p\n",FUNC_NAME,search_list);
    }

    private_field = FALSE;
    return search_list;
}


/**
 * INFO : Inject your custom code into the game.
 * @description - Inject single assembly/opcode direcltly into target process.
 * @param - Address where to inject, opcode to inject ,length of opcode in bytes.
 * @return - On success of injection it returns TRUE otherwise returns FALSE.

 * WARNING : This is advanced stuff so be careful while injecting custom code it should be exact opcode
 * for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defind by ISA
 * otherwise target process could result in SEGFAULT causing program to crash.
 */

BOOL injectCode(LPVOID lp_address,LPCVOID lp_opcode,SIZE_T sz_opcode_len)
{
    HANDLE game_handle = getGameHandle();
    BOOL inject_status = FALSE;
    DWORD old_protection = NIL;
    error_code = NIL;
    private_field = TRUE;

    if(logs_enabled)
    {
        addLog("%s -> Trying to inject code at address %p with opcode : %p of length : %lu\n",FUNC_NAME,lp_address,*(PUCHAR)(lp_opcode),sz_opcode_len);
        private_field = TRUE;
    }

    try
    {
        if (game_handle == NIL)
        {
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            //change protection of (.text) segment to EXECUTE/READWRITE.
            VirtualProtect((LPVOID)lp_address,sz_opcode_len,PAGE_EXECUTE_READWRITE,&old_protection);

            SetLastError(NO_ERROR);
            if(!(inject_status =  WriteProcessMemory(game_handle,lp_address,lp_opcode,sz_opcode_len,NULL)))
            {
                if(ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }
            private_field = TRUE;
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned status : %d\n",FUNC_NAME,inject_status);
    }

    //revert back to original protection of (.text) segment to READ_ONLY.
    VirtualProtect((LPVOID)lp_address,sz_opcode_len,old_protection,&old_protection);
    private_field = FALSE;
    return inject_status;
}

/**
 * INFO : Inject your multiple custom codes into the game.
 * @description - Inject multiple assembly/opcodes directly into target process.
 * @param - List of Address where to inject, List of opcodes to inject , List of length of opcode in bytes, Total no. of Addresses.
 * @return - On success of injection it returns TRUE otherwise returns FALSE.

 * WARNING : This is advanced stuff so be careful while injecting custom code it should be exact opcode
 * for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
 * otherwise target process could result in SEGFAULT causing program to crash.
 */

BOOL injectCodes(LPVOID lp_addresses[],LPBYTE lp_opcode[],SIZE_T sz_opcode_lens[],SIZE_T n_addresses)
{
    UINT index = NIL;
    BOOL inject_status = FALSE;

    for(index = 0; index < n_addresses; index++)
    {
        inject_status = injectCode(lp_addresses[index],lp_opcode[index],sz_opcode_lens[index]);
    }
    return inject_status;
}

/**
 * INFO : Write NOP (No-Operation) code into the game.
 * @description - Inject single assembly instruction NOP direcltly into target process.
 * @param - Address where to inject,size of opcode currently present at that address in bytes.
 * @return - On success of writing NOP it returns TRUE otherwise returns FALSE.

 * PS - size of opcode is needed because it will write NOP of N-byte length
 * where 'N' is size of current instruction present at that address.
 */

BOOL writeNOP(LPVOID lp_address, SIZE_T sz_opcode_len)
{
	BYTE NOP = '\x90';
	BOOL fill_status = FALSE;
	HANDLE game_handle = getGameHandle();
	DWORD old_protection = NIL;
	private_field = TRUE;

	LPBYTE lp_opcode = GT_MemAlloc(HEAP_ZERO_MEMORY, sz_opcode_len);
	private_field = TRUE;

	try
	{
		if (lp_opcode == NULL)
		{
			if (ERROR_INVALID)
			{
				error_code = GetLastError();
				throw(error_code);
			}
		}

		if (logs_enabled)
		{
			addLog("%s -> Trying to fill NOPS at address 0x%X of length : %lu\n", FUNC_NAME, lp_address, sz_opcode_len);
			private_field = TRUE;
		}

		//Fill opcode with NOP.
		FillMemory(lp_opcode, sz_opcode_len, NOP);

		if (game_handle == NIL)
		{
			if (ERROR_INVALID)
			{
				error_code = GetLastError();
				throw(error_code);
			}
		}

		else
		{
			//change protection of (.text) segment to EXECUTE/READWRITE.
			VirtualProtect(lp_address, sz_opcode_len, PAGE_EXECUTE_READWRITE, &old_protection);

			//inject NOP into at provided address.
			if (!(fill_status = WriteProcessMemory(game_handle, lp_address, lp_opcode, sz_opcode_len, NULL)))
			{
				if (ERROR_INVALID)
				{
					error_code = GetLastError();
					throw(error_code);
				}
			}
		}

		GT_MemFree(lp_opcode);
		private_field = TRUE;
	}
	catch (error_code)
	{
		private_field = TRUE;
		showError(error_code, FUNC_NAME, LINE_NO);
	}

	//revert back to original protection of (.text) segment to READ_ONLY.
	VirtualProtect(lp_address, sz_opcode_len, old_protection, &old_protection);

	private_field = FALSE;
	return fill_status;
}

/**
 * INFO : Write multiple NOP (No-Operation) code into the game.
 * @description - Inject multiple assembly instruction NOP directly into target process.
 * @param - List of Address where to inject,List of size of opcode present at that address, in bytes. Total no. of address.
 * @return - On success of writing NOP it returns TRUE otherwise returns FALSE.

 * PS - size of opcode is needed because it will write NOP of N-byte length
 * where 'N' is size of current instruction present at that address.
 */

BOOL writeNOPs(LPVOID lp_addresses[],SIZE_T sz_opcode_lens[],SIZE_T n_addresses)
{
    UINT index = NIL;
    BOOL nop_status = FALSE;

    for(index = 0; index < n_addresses; index++)
    {
        nop_status = writeNOP(lp_addresses[index],sz_opcode_lens[index]);
    }
    return nop_status;
}

/**
 * @description - Write the JMP or CALL assembly instruction at an address.

 * @param - source - Address where to set the jmp/call instruction,
 * destination - Address where you want to JMP or CALL.
 * opcode_type - Use ENUM opcode to provide opcode type values.
 * nops_amount - Amount of NOPs to fill after JMP or CALL.
 * @return - On success of writing it returns TRUE otherwise returns FALSE.
 */

BOOL writeJmpOrCall(LPVOID lp_source,LPVOID lp_destination,OPCODE opcode_type,UINT nops_amount)
{
    BOOL write_status = FALSE;
    DWORD old_protection = NIL,relative_offset = 0x5;
    UINT nop_amount = NIL;
    HANDLE game_handle = getGameHandle();
    BYTE jmp_call_opcode = NUL;
    SIZE_T opcode_size = NIL;
    UINT index = NIL;
    private_field = TRUE;

    LPDWORD source = (LPDWORD)lp_source;
    LPDWORD lpd_destination = (LPDWORD)lp_destination;
    DWORD destination = (DWORD)lpd_destination;

    //get the opcode type.
    switch(opcode_type)
    {
    case OPCODE_SHORT_JUMP :
    {
        jmp_call_opcode = '\xEB';
        opcode_size = 0x2;
        break;
    }

    case OPCODE_NEAR_JUMP :
    {
        jmp_call_opcode = '\xE9';
        opcode_size = 0x5;
        break;
    }

    case OPCODE_CALL :
    {
        jmp_call_opcode = '\xE8';
        opcode_size = 0x5;
        break;
    }
    }

    try
    {
        if (game_handle == NIL)
        {
            private_field = TRUE;
            if(ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            //inject jmp or call opcode.
            if(!(write_status = injectCode((LPVOID)source,(LPCVOID)&jmp_call_opcode,sizeof(BYTE))))
            {
                private_field = TRUE;
                if(ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }
            private_field = TRUE;

            //get the relative address.
            destination = destination - (DWORD)source - relative_offset;

            //change protection of (.text) segment to EXECUTE/READWRITE.
            VirtualProtect((LPVOID)source,opcode_size,PAGE_EXECUTE_READWRITE, &old_protection);

            if(logs_enabled)
            {
                addLog("%s -> Trying to write at address : %p\tinstruction : %p\tof length : %lu\n",FUNC_NAME,(LPVOID)((DWORD)source + 0x1),(LPCVOID)&destination,opcode_size - 1);
                private_field = TRUE;
            }

            //write destination address right after jmp or call opcode.
            if(!(write_status =  WriteProcessMemory(game_handle,(LPVOID)((DWORD)source + 0x1),(LPCVOID)&destination,opcode_size - 1,NULL)))
            {
                if(ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }

            //If JMP short selected then fill rest of bytes with NOP.
            if(opcode_type == OPCODE_SHORT_JUMP)
            {
                nop_amount = relative_offset - opcode_size;

                write_status = writeNOP((LPVOID)((DWORD)source + 0x2),nops_amount);
                private_field = TRUE;
            }

            //If NOPS enabled then write amount of NOPs after JMP/CALL instruction.
            if(nops_amount > 0)
            {

                for(index = 0; index < nops_amount; index++)
                    write_status = writeNOP((LPVOID)((DWORD)source + opcode_size + index),0x1);
                private_field = TRUE;
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }

    //revert back to origional protection of (.text) segment to READ_ONLY.
    VirtualProtect((LPVOID)source,opcode_size,old_protection,&old_protection);
    return write_status;
}

/**
 * INFO : Inject custom procedure into the game using shellcode method.
 * @description - Inject your assembly procedure directly into target process.
 * @param - Address where to inject procedure, Pointer to procedure's opcodes,size of procedure in bytes, amount of NOPs to fill after injecting proc.
 * @return - On success of injection it returns TRUE otherwise returns FALSE.

 * WARNING : This is advanced stuff so be careful while injecting custom procedure it should be exact opcode
 * for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
 * otherwise target process could result in SEGFAULT causing program to crash.
 *
 * NOTE : Amount of NOPs to fill in necessary after instruction because CALL instruction is 5 bytes but current instruction could be lower
 * or higher , so in order to avoid crashing we will fill NOPs in empty locations.
 * Calculate using disassembler the amount of NOPs needed after instruction depending upon current architecture.
 */

BOOL injectProc(LPVOID lp_address,LPCVOID lp_proc,SIZE_T sz_proc,UINT nops_amount)
{
    BOOL inject_status = FALSE;

    if(logs_enabled)
    {
        private_field = TRUE;
        addLog("%s -> Injecting proc at address : %p\twith size : %lu\n",FUNC_NAME,lp_address,sz_proc);
    }

    LPVOID shell_code_address = injectShellCode(lp_proc,sz_proc);
    inject_status = writeJmpOrCall(lp_address,shell_code_address,OPCODE_CALL,nops_amount);
    private_field = TRUE;

    if(logs_enabled)
    {
        private_field = TRUE;
        addLog("%s -> returned status : %d\n",FUNC_NAME,inject_status);
    }

    private_field = FALSE;
    return inject_status;
}

/**
 * INFO : Inject custom shellcode into the game.
 * @description - Inject your custom shellcode procedure directly into target process (Virtual Memory).
 * @param - Pointer to procedure's opcodes,size of procedure in bytes.
 * @return - On success of injection it returns address where shellcode was injected otherwise returns NULL.

 * WARNING : This is advanced stuff so be careful while injecting custom shellcode it should be exact opcode
 * for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
 * otherwise target process could result in SEGFAULT causing program to crash.
 */

LPVOID injectShellCode(LPCVOID lp_shellcode,SIZE_T sz_shellcode)
{
    HANDLE game_handle = NULL,h_thread_snap = NULL,h_thread = NULL,target_thread = NULL;
    LPVOID shell_code_address = NULL;
    DWORD tid = NIL;
    UINT index = NIL;
    PUCHAR lpu_shellcode = NULL,ret_opcode = NULL;
    THREADENTRY32 te32;
    CONTEXT context;

    try
    {
        game_handle = getGameHandle();

        if(game_handle == NULL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        // Takes a snapshot of all threads in the system, 0 to current process
        h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (h_thread_snap == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog(" CreateToolhelp32Snapshot success\n");
                private_field = TRUE;
            }
        }

        // Retrieves information about the first thread of any process encountered in a system snapshot.
        te32.dwSize = sizeof(THREADENTRY32);
        if (Thread32First(h_thread_snap, &te32) == FALSE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog(" Thread32First success\n");
                private_field = TRUE;
            }
        }


        /* Now walk the thread list of the system,
        and display information about each thread
        associated with the specified process*/

        do
        {
            if (te32.th32OwnerProcessID == GetProcessId(game_handle))
            {
                if (tid == 0)
                    tid = te32.th32ThreadID;
                h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
                if (h_thread == NULL)
                {
                    if (ERROR_INVALID)
                    {
                        error_code = GetLastError();
                        throw(error_code);
                    }
                }
                else
                {
                    SuspendThread(h_thread);
                    CloseHandle(h_thread);

                    if(logs_enabled)
                    {
                        addLog(" Suspend thread 0x%08X\n", te32.th32ThreadID);
                        private_field = TRUE;
                    }

                }
            }
        }
        while (Thread32Next(h_thread_snap, &te32));
        CloseHandle(h_thread_snap);

        // Get context.
        context.ContextFlags = CONTEXT_FULL;

        // Open targeted thread
        target_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
        if (target_thread == NULL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog(" Target thread 0x%08X\n", target_thread);
                private_field = TRUE;
            }
        }


        // Get eip & esp adresses
        if (!GetThreadContext(target_thread, &context))
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("\tEIP : 0x%08x\n\tESP : 0x%08x\n\tEBP : 0x%08x\n", context.Eip, context.Esp, context.Ebp);
                private_field = TRUE;
            }
        }

        /* Save eip, esp & ebp
        Allocate 4 bytes on the top of the stack for the RET*/
        context.Esp -= sizeof(unsigned int);
        if (!WriteProcessMemory(game_handle, (LPVOID)context.Esp, (LPCVOID)&context.Eip, sizeof(unsigned int), NULL))
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("writing ESP success\n");
                addLog("\tEIP : 0x%08x\n\tESP : 0x%08x\n\tEBP : 0x%08x\n", context.Eip, context.Esp, context.Ebp);
                private_field = TRUE;
            }
        }

        //Allocate memory to append return opcode.
        private_field = TRUE;
        lpu_shellcode = GT_MemAlloc(HEAP_ZERO_MEMORY,sz_shellcode + 1);
        CopyMemory(lpu_shellcode,lp_shellcode,sz_shellcode);

        //Append return opcode to current shellcode.
        ret_opcode = (PUCHAR)"\xC3";
        CopyMemory(lpu_shellcode + sz_shellcode,ret_opcode,sizeof(UCHAR));

        //point to new shellcode with return opcode.
        lp_shellcode = lpu_shellcode;

        //Add shellcode to Logs
        addLog(" Shellcode:\n");
        for (index = 0; index < sz_shellcode + 1; index++)
            addLog("0x%X\n",*(PUCHAR)(((DWORD)lp_shellcode + index)));
        addLog("\n");

        // Allocate memory in the targeted process for our shellcode
        shell_code_address = VirtualAllocEx(game_handle, NULL, sz_shellcode + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (shell_code_address == NULL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("Allocating %d bytes for our shellcode\n", sz_shellcode + 1);
                private_field = TRUE;
            }
        }

        // Write the shellcode into the targeted thread
        if (!WriteProcessMemory(game_handle, shell_code_address, (LPCVOID)lp_shellcode, sz_shellcode + 1, NULL))
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("shellcode inject success\n");
                private_field = TRUE;
            }
        }


        // Redirect eip to the s address
        context.Eip = (DWORD)shell_code_address;
        if(logs_enabled)
        {
            addLog("\tEIP : 0x%08x\n\tESP : 0x%08x\n\tEBP : 0x%08x\n", context.Eip, context.Esp, context.Ebp);
            private_field = TRUE;
        }


        if (!SetThreadContext(target_thread, &context))
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("SetThreadContext success\n");
                private_field = TRUE;
            }
        }


        // Resume Threads
        // Takes a snapshot of all threads in the system, 0 to current process
        h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        te32.dwSize = sizeof(THREADENTRY32);
        if (h_thread_snap == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("CreateToolhelp32Snapshot success\n");
                private_field = TRUE;
            }
        }

        // Retrieves information about the first thread of any process encountered in a system snapshot.
        if (Thread32First(h_thread_snap, &te32) == FALSE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }
        else
        {
            if(logs_enabled)
            {
                addLog("Thread32First success\n");
                private_field = TRUE;
            }
        }


        // Now walk the thread list of the system,
        // and display information about each thread
        // associated with the specified process
        do
        {
            if (te32.th32OwnerProcessID == GetProcessId(game_handle))
            {
                addLog("\tTHREAD ID = 0x%08X\n", te32.th32ThreadID);
                h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
                if (h_thread == NULL)
                {
                    if (ERROR_INVALID)
                    {
                        error_code = GetLastError();
                        throw(error_code);
                    }
                }
                else
                {
                    ResumeThread(h_thread);
                    if (te32.th32ThreadID == tid)
                        WaitForSingleObject(h_thread, 5000);
                    CloseHandle(h_thread);

                    if(logs_enabled)
                    {
                        addLog("Resume thread success\n");
                        private_field = TRUE;
                    }

                }
            }
        }
        while (Thread32Next(h_thread_snap, &te32));

        //Close handle an free memory.
        CloseHandle(h_thread_snap);
        private_field = TRUE;
        GT_MemFree(lpu_shellcode);
    }

    catch(error_code)
    {
        private_field = TRUE;
        showError(error_code,FUNC_NAME,LINE_NO);
    }

    if(logs_enabled)
    {
        addLog("%s -> returned address : %p\n",FUNC_NAME,shell_code_address);
    }

    return shell_code_address;
}




/**
 * INFO : Whether library should maintain logs internally (enable this if you want this feature).
 * @description - Enable logs in library.
 * @return - Returns TRUE if enabled is success otherwise returns FALSE.
 */

BOOL enableLogs(void)
{
    private_field = TRUE;
    BOOL enable_status = FALSE;

    if(logs_enabled == FALSE)
    {
        logs_enabled = TRUE;
        enable_status = TRUE;
    }
    else
    {
        showWarning("Logs already enabled!");
        enable_status = FALSE;
    }
    private_field = FALSE;
    return enable_status;
}

/**
 * @description - Disable logs in library.
 * @return - Returns TRUE if disable is success otherwise returns FALSE.
 */

BOOL disableLogs(void)
{
    private_field = TRUE;
    BOOL disable_status = FALSE;

    if(logs_enabled == TRUE)
    {
        logs_enabled = FALSE;
        disable_status = TRUE;
    }
    else
    {
        showWarning("Logs already disabled!");
        disable_status = FALSE;
    }
    private_field = FALSE;
    return disable_status;
}

/**
 * @description - Get Handle to current game's process.
 * @return - If game found it return Handle to current game's process otherwise returns NULL.
 */

HANDLE getGameHandle()
{
    private_field = TRUE;
    HANDLE g_handle = NULL;

    if(game_handle != NULL)
    {
        g_handle = game_handle;
    }

    else
    {
        g_handle = NULL;
    }

    if(logs_enabled)
    {
        addLog("%s -> returned handle : %p\n",FUNC_NAME,g_handle);
    }

    private_field = FALSE;
    return g_handle;
}

/**
 * @description - Get Handle to current game's window.
 * @return - If game found it return Handle to current game's windows otherwise returns NULL.
 */

HWND getGameHWND()
{
    private_field = TRUE;
    HWND g_hwnd = NULL;

    if(game_hwnd != NULL)
    {
        g_hwnd = game_hwnd;
    }


    else
    {
        g_hwnd = NULL;
    }

    if(logs_enabled)
    {
        addLog("%s -> returned handle : %p\n",FUNC_NAME,g_hwnd);
    }

    private_field = FALSE;
    return g_hwnd;
}

/****************************************************************************/
/****************-PRIVATE-METHODS-*******************************************/
/****************************************************************************/

static void showError(DWORD error_code, LPCSTR error_msg, DWORD line_no)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        char err_msg_buf[0xFF] = {'\0'};
        char sys_err_buf[0xFF] = {'\0'};
        LPSTR ptr = NUL;
        private_field = TRUE;


        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, error_code,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                      sys_err_buf, sizeof(sys_err_buf), (va_list *)NULL);

        // Trim the end of the line and terminate it with a null
        ptr = sys_err_buf;
        while ((*ptr > 31) || (*ptr == 9))
            ++ptr;

        do
        {
            *ptr-- = '\0';
        }
        while ((ptr >= sys_err_buf) && ((*ptr == '.') || (*ptr < 33)));

        //copy error from getLastError() to error buffer.
        wsprintf(err_msg_buf, "\nINFO : %s method failed!\nREASON : (%s)\nLINE. : occurred at line no. %d\n", error_msg, sys_err_buf, line_no);

        //Show error and exit afterwards.
        MessageBox((HWND)NULL, err_msg_buf, "ERROR!", MB_ICONERROR);

        if(logs_enabled)
        {
            addLog("Error occurred : %s\n", err_msg_buf);
            private_field = TRUE;
        }
    }
    private_field = FALSE;
}

static void showInfo(LPCSTR info_message)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        MessageBox((HWND)NULL, info_message, "INFO!", MB_ICONINFORMATION);
        if(logs_enabled)
        {
            addLog("Information shown : %s\n", info_message);
        }
    }
}

static void showWarning(LPCSTR warning_message)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        MessageBox((HWND)NULL, warning_message, "WARNING!", MB_ICONWARNING);
        if(logs_enabled)
        {
            addLog("Warning shown : %s\n", warning_message);
        }
    }
}

/*Setters for Process id,name,handle.*/
static void setProcessID(DWORD p_id)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
        process_id = p_id;
}

static void setGameHandle(HANDLE g_handle)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
        game_handle = g_handle;
}

static void setGameName(LPCSTR g_name)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
        lstrcpy(game_name, g_name);
}

static void setGameHWND(DWORD process_id)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
        EnumWindows(EnumAllWindows,(LPARAM)process_id);
}

/*Custom logger to add logs for trainer.*/
static void addLog(LPCSTR format, ...)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        private_field = TRUE;
        error_code = NIL;
        char log_buf[0x400] = {NUL};
        static LPCSTR log_file_name = "GTLibc_logs.log";

        int date_len = NIL;
        static BOOL date_adder = FALSE;
        static BOOL file_checker = FALSE;

        //Only write data at beginning of file.
        if (!date_adder)
        {
            date_len = wsprintf(log_buf, "\nLog created by GTLibc at : %s", getCurrentTime());
            date_adder = TRUE;
        }

        va_list va_alist;
        va_start(va_alist, format);
        wvsprintf(log_buf + date_len, format, va_alist);
        va_end(va_alist);

        static HANDLE file_handle = NULL;

        if (!file_checker)
        {
            if (fileExist(log_file_name))
            {
                file_handle = CreateFileA(log_file_name, FILE_APPEND_DATA, NIL, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            }

            else
            {
                file_handle = CreateFile(log_file_name, FILE_WRITE_DATA, NIL, (LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            }
            file_checker = TRUE;
        }

        try
        {
            if (file_handle == NULL || file_handle == INVALID_HANDLE_VALUE)
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    CloseHandle(file_handle);
                }
                throw(error_code);
            }

            DWORD data_size = NIL;
            if (!WriteFile(file_handle,(LPCVOID)log_buf, lstrlen(log_buf), &data_size, (LPOVERLAPPED)NULL))
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }
        }
        catch (error_code)
        {
            showError(error_code, FUNC_NAME, LINE_NO);
        }
    }
    private_field = FALSE;
}

static LPSTR getCurrentTime()
{
    SYSTEMTIME sys_time;
    static char time_buf[0x50] = {NUL};

    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        GetLocalTime(&sys_time);
        wsprintf(time_buf, "Time : %u:%u:%u\t{**[Day - %u], [Month - %u], [Year - %u]**}\n", sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wDay, sys_time.wMonth, sys_time.wYear);
    }
    return time_buf;
}

static BOOL fileExist(LPCSTR file_name)
{
    BOOL does_exist = FALSE;
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        OFSTRUCT offstr;
        HFILE file = OpenFile(file_name, &offstr, OF_EXIST);

        if (file == HFILE_ERROR)
            does_exist = FALSE;
        else
            does_exist = TRUE;

        CloseHandle((HANDLE)file);
    }
    return does_exist;
}

static void doVirtualKeyPress(int input_type, int key_code, int mouse_code)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        //input event.
        INPUT input;

        if (input_type == INPUT_KEYBOARD)
        {
            // Set up a generic keyboard event.
            input.type = INPUT_KEYBOARD;
            input.ki.wScan = NIL; // hardware scan code for key
            input.ki.time = NIL;
            input.ki.dwExtraInfo = NIL;

            // Set the key to be pressed
            input.ki.wVk = key_code; // virtual-key code
            input.ki.dwFlags = 0;    // 0 for key press
        }

        else if (input_type == INPUT_MOUSE)
        {
            input.type = INPUT_MOUSE;
            input.mi.dx = NIL;
            input.mi.dy = NIL;
            input.mi.mouseData = NIL;
            input.mi.dwExtraInfo = NIL;
            input.mi.time = NIL;
            input.mi.dwFlags = (mouse_code == 1) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
        }

        //Press Key/Mouse down
        SendInput(1, &input, sizeof(INPUT));
        //Sleep(1000);
        Sleep(200);

        //Setting Release Keyboard key
        if (input_type == INPUT_KEYBOARD)
            input.ki.dwFlags = KEYEVENTF_KEYUP;

        //Setting Release Mouse key
        if (input_type == INPUT_MOUSE)
            input.mi.dwFlags = (mouse_code == 1) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;

        //Release Key/Mouse down
        SendInput(1, &input, sizeof(INPUT));
        Sleep(200);
    }
}

static BOOL CALLBACK EnumAllWindows(HWND hwnd, LPARAM lParam)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        DWORD process_id = NIL;
        GetWindowThreadProcessId(hwnd, &process_id);
        if (process_id == lParam)
        {
            game_hwnd = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}

/*Private memory allocation wrapper methods*/
LPVOID GT_MemAlloc(DWORD dw_flag, SIZE_T sz_size)
{
    LPVOID gt_mem_block = NULL;
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        try
        {
            gt_mem_block = HeapAlloc(GetProcessHeap(), dw_flag, sz_size);

            if (gt_mem_block == NULL)
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }

        }
        catch (error_code)
        {
            private_field = TRUE;
            showError(error_code, FUNC_NAME, LINE_NO);
        }
    }

    private_field = FALSE;
    return gt_mem_block;
}

BOOL GT_MemFree(LPVOID gt_mem_block)
{
    BOOL free_status = FALSE;
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        try
        {
            if (!(free_status = HeapFree(GetProcessHeap(), NIL, gt_mem_block)))
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }
            }

        }
        catch (erro_code)
        {
            private_field = TRUE;
            showError(error_code, FUNC_NAME, LINE_NO);
        }
    }
    private_field = FALSE;
    return free_status;
}

static BOOL isPrivateField(BOOL private_field, LPCSTR proc_name, int line_no)
{
    if (private_field)
        return TRUE;

    else
    {
        char err_buf[0x100] = {NUL};
        wsprintf(err_buf, "ERROR : %s method failed!\nREASON : Access to private method! (ERROR_INVALID_FUNCTION) \nLINE : occurred at line no. %d", proc_name, line_no);
        MessageBox((HWND)NULL, err_buf, "ERROR!", MB_ICONERROR);

        DWORD exit_code = NIL;
        GetExitCodeProcess(GetCurrentProcess(), &exit_code);
        ExitProcess(exit_code);
    }
}
