#include "GTLibc.h"

/*GTLibc -library to make game trainer for c/c++.*/

/****************************************************************************/
/*********************-PUBLIC-METHODS-***************************************/
/****************************************************************************/

/**
 * @description - Find game by process name.
 * @param - Game name in string format (CASE INSENSTIVE). and without '.exe' extension.
 * @return - If game found it returns HANDLE to game otherwise exits the application with error code.
 * NOTE : Process name is name of your .exe file loaded in Memory.
 * PS : If multiple versions of game found then you have to update the GUI logic for this.
 */

HANDLE findGameProcess(LPCSTR game_name)
{
    private_field = TRUE;
    SetLastError(NO_ERROR);

    try
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NIL);
        BOOL game_found = FALSE;
        UINT process_index = NIL, process_count = NIL;
        error_code = NIL;

        //process info variables.
        char p_name[MAX_PATH] = {'\0'};
        DWORD p_id = NIL;
        HANDLE p_handle = (HANDLE)NULL;
        HWND p_hwnd = (HWND)NULL;

        //count total no. of active process from Memory.
        if (Process32First(snapshot, &entry))
        {
            while (Process32Next(snapshot, &entry))
                ++process_count;
        }

        DWORD dw_process_size = process_count * sizeof(process_table);
        process_table *process_list = (process_table *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dw_process_size);

        if (process_list == NULL)
        {
            if (logs_enabled)
            {
                addLog("%s->process_list -> Memory Error : %s\n", FUNC_NAME, strerror(errno));
                addLog("%s->process_list address : 0x%X\n", FUNC_NAME, process_list);
                private_field = TRUE;
            }

            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        //Main loop for iterating input game in process list.
        if (Process32First(snapshot, &entry))
        {
            while (Process32Next(snapshot, &entry))
            {
                if (strcasestr(entry.szExeFile, game_name) != NULL)
                {

                    game_found = TRUE;
                    const UINT game_len = lstrlen(entry.szExeFile);
                    char game_name_exe[game_len];
                    ZeroMemory(game_name_exe, game_len);

                    const UINT sz_exe_len = lstrlen(entry.szExeFile);
                    char sz_exe_file[sz_exe_len];
                    ZeroMemory(sz_exe_file, sz_exe_len);

                    //copy exe file name.
                    lstrcpy(sz_exe_file, entry.szExeFile);

                    //remove '.exe' part from game name.
                    UINT index;
                    for (index = 0; index < lstrlen(sz_exe_file); index++)
                    {
                        if (sz_exe_file[index] == '.')
                            break;

                        game_name_exe[index] = sz_exe_file[index];
                    }

                    //copy process name,id and handle to process_list.
                    lstrcpy(process_list[process_index].process_name, game_name_exe);
                    process_list[process_index].process_id = entry.th32ProcessID;
                    process_list[process_index].process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                    /*Throw error if process can't be opened/accessed*/
                    if (process_list[process_index].process_handle == NULL)
                    {
                        if (ERROR_INVALID)
                        {
                            error_code = GetLastError();
                            throw(error_code);
                        }
                    }

                    setGameHWND(entry.th32ProcessID);
                    process_list[process_index].process_hwnd = getGameHWND();
                    process_index++;
                }
            }

            if (process_index == process_count)
                showWarning("Games/Process list way too long to filter out!");

            //reallocate process list to no. of process found.
            process_table ptr_process_list[process_index];
            ZeroMemory(ptr_process_list, sizeof(ptr_process_list));

            if (process_list != NULL && process_index != NIL)
            {
                DWORD dw_bytes = process_index * sizeof(process_table);
                process_table *p_list = (process_table *)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, process_list, dw_bytes);

                if (p_list == NULL)
                {
                    if (logs_enabled)
                    {
                        addLog("%s->p_list -> Memory Error : %s\n", FUNC_NAME, strerror(errno));
                        addLog("%s->p_list address : 0x%X\n", FUNC_NAME, p_list);
                        private_field = TRUE;
                    }

                    if (!HeapFree(GetProcessHeap(), NIL, process_list))
                    {
                        if (ERROR_INVALID)
                        {
                            error_code = GetLastError();
                            throw(error_code);
                        }
                    }

                    if (ERROR_INVALID)
                    {
                        error_code = GetLastError();
                        throw(error_code);
                    }
                }

                else
                {
                    CopyMemory(ptr_process_list, p_list, process_index * sizeof(process_table));

                    if (!HeapFree(GetProcessHeap(), NIL, p_list))
                    {
                        if (ERROR_INVALID)
                        {
                            error_code = GetLastError();
                            throw(error_code);
                        }
                    }
                }
            }
            else
            {
                if (logs_enabled)
                {
                    addLog("%s->%s\n", FUNC_NAME, "process_list is empty");
                    private_field = TRUE;
                }

                if (!HeapFree(GetProcessHeap(), NIL, process_list))
                {
                    if (ERROR_INVALID)
                    {
                        error_code = GetLastError();
                        throw(error_code);
                    }
                }
            }
            //if found single version of game.
            if (game_found && process_index == 1)
            {
                //copy current process info.
                lstrcpy(p_name, ptr_process_list[0].process_name);
                p_id = ptr_process_list[0].process_id;
                p_handle = ptr_process_list[0].process_handle;

                //set current process info.
                setGameName(p_name);
                setProcessID(p_id);
                setGameHandle(p_handle);
                setGameHWND(p_id);

                p_hwnd = getGameHWND();

                if (logs_enabled)
                {
                    addLog("Game \tname %s\tid : %u\tHandle : 0x%X\tHWND : 0x%X\t", p_name, p_id, p_handle, p_hwnd);
                    private_field = TRUE;
                }
            }

            /*
                    if found multiple versions of game.

                    **TO-DO**
                    implement your logic for getting input from user again in case you have GUI application.

                    **IMPLEMENTED - IN COMMENT SECTION**
                    CLI application logic for getting input again from user has been implemented already.
                    */

            /*Getting input again from Multiple games (CLI-Logic)*/
            else if (game_found && process_index > 1)
            {

                /*Show incomplete warning*/
                char warn_msg[MAX_PATH] = {'\0'};
                wsprintf(warn_msg, "Logic for multiple games is incomplete !\nPlease refer FILE : %s\nline no : %d\n", FILE_NAME, LINE_NO);
                showWarning(warn_msg);
                private_field = TRUE;

                //print Multiple games name.
                UINT index = NIL;
                char multi_games[MAX_PATH * process_index];
                ZeroMemory(multi_games, sizeof(multi_games));

                wsprintf(multi_games, "Multiple games found by this name\nGames list : \n");

                for (index = 0; index < process_index; index++)
                {
                    lstrcpy(p_name, ptr_process_list[index].process_name);
                    p_id = ptr_process_list[index].process_id;
                    p_handle = ptr_process_list[index].process_handle;

                    //add multi games to LOG and format to buffer.
                    if (logs_enabled)
                    {
                        addLog("Process\tname %s\tid : %u\tHandle : 0x%X\n", p_name, p_id, p_handle);
                        private_field = TRUE;
                    }
                    wsprintf(multi_games + lstrlen(multi_games), "Game : %s\tProcess ID : %u\n", p_name, p_id);
                }
                wsprintf(multi_games + lstrlen(multi_games), "%s\n", "\nSelect at least one game from this list!");

                //Show Multi-games List.
                showInfo(multi_games);
                private_field = TRUE;

                /* -- COMMENT SECTION--
                            * CLI-Logic for multiple versions of game implemented
                             * to use it uncomment this section
                            */

                //ask again for input!
                /*char g_name[MAX_PATH];
                fgets(g_name,sizeof(g_name),stdin);
                int pos_index = NIL;

                //check for input process in process list.
                for(index = 0; index < process_index; index++)
                {
                    if(!strcasecmp(ptr_process_list[index].process_name,g_name))
                    {

                        //copy current process info.
                        lstrcpy(p_name,ptr_process_list[index].process_name);
                        p_id = ptr_process_list[index].process_id;
                        p_handle = ptr_process_list[index].process_handle;

                        //set current process info.
                        setGameName(p_name);
                        setProcessID(p_id);
                        setGameHandle(p_handle);
                        setGameHWND(p_id);

                        if(logs_enabled)
                        {
                            addLog("Game \tName : %s\t id : %u\tHandle 0x%X\tHWND : 0x%X\n",p_name,p_id,p_handle,getGameHWND());
                            private_field = TRUE;
                        }

                        game_found = TRUE;
                        pos_index = index;
                        private_field = TRUE;
                        break;
                    }
                }

                if(index == process_index)
                {
                    game_found = FALSE;
                    showInfo("Game not found! Please open the game first!");
                }

                //close all other handles.
                for(index = 0; index < process_index; index++)
                {
                    if(index != pos_index)
                        CloseHandle(ptr_process_list[index].process_handle);
                }*/
            }

            else
            {
                showInfo("Game not found! Please open the game first!");

                CloseHandle(snapshot);
                CloseHandle(game_handle);

                DWORD exit_code;
                GetExitCodeProcess(GetCurrentProcess(), &exit_code);
                if (logs_enabled)
                {
                    addLog("Process exited with exit code : %u\n", exit_code);
                }
                ExitProcess(exit_code);
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
    private_field = FALSE;
    return game_handle;
}

/**
 * @description - Find game by window name.
 * @param - window name in string format (CASE INSENSITIVE).
 * @return - if game window found then it returns HWND to that window otherwise exits the application with error code.
 * NOTE : Windows name is name of your Game Process Window not the .exe file.
 */

HWND findGameWindow(LPCSTR window_name)
{
    private_field = TRUE;
    HWND game_window = FindWindowEx((HWND)NULL, (HWND)NULL, NUL, window_name);
    error_code = NIL;

    try
    {

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
    private_field = FALSE;
    return game_window;
}

/**
 * @description - Read value from provided address.
 * @param - Address in void* format.
 * @return - If read succeeded then it returns new value otherwise exits the application with error code.
 */

DWORD readAddress(LPVOID address)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    DWORD value = NIL;
    error_code = NIL;

    try
    {
        if (game_handle == NULL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            if (!ReadProcessMemory(game_handle, address, &value, sizeof(value), NULL))
            {
                if (ERROR_INVALID)
                    error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }
    private_field = FALSE;
    return value;
}

/**
 * @description - Write value at provided address.
 * @param - Address in void* format and value to be written in DWORD format.
 * @return - If write is succeeded then it returns TRUE otherwise exits the application with error code.
 */

BOOL writeAddress(LPVOID address, DWORD value)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    BOOL write_status = FALSE;
    error_code = NIL;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            if (!(write_status = WriteProcessMemory(game_handle, address, &value, sizeof(value), NULL)))
            {
                if (!write_status)
                {
                    if (ERROR_INVALID)
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
    private_field = TRUE;
    error_code = NIL;

    HANDLE game_handle = getGameHandle();
    DWORD dw_value = NIL;

    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;

    try
    {
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
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            dw_value = readAddress((LPVOID)lpdw_address);
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }
    private_field = FALSE;
    return dw_value;
}

/**
 * @description - Write value at provided address with offset.
 * @param - Address in void* format and offset in DWORD format,value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise exits the application with error code.
 */

BOOL writeAddressOffset(LPVOID lp_address, DWORD dw_offset, DWORD dw_value)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    BOOL write_status = FALSE;
    error_code = NIL;

    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;

    try
    {
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
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            write_status = writeAddress((LPVOID)lpdw_address, dw_value);

            if (!write_status)
            {
                if (ERROR_INVALID)
                    error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }
    private_field = FALSE;
    return write_status;
}

/**
 * @description - Read value from provided address with provided offsets.
 * @param - Address in void* format, offsets in DWORD* format and size of offsets.
 * @return - If read succeeded then it returns list of values otherwise exits the application with error code.
 * NOTE : This will be useful in reading multiple values at a time like multiple Ammos/Clips from Ammos/Clips offsets list.
 * PS : FREE this memory after using it to avoid memory leaks use HeapFree() Method from (windows.h).
 */

DWORD *readAddressOffsets(LPVOID lp_address, DWORD *dw_offsets, SIZE_T sz_offsets)
{
    private_field = TRUE;
    error_code = NIL;

    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    DWORD *dw_values = (DWORD *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sz_offsets);

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else if (dw_values == NULL)
        {
            if (ERROR_INVALID)
            {
                if (logs_enabled)
                {
                    addLog("%s->%s\n", FUNC_NAME, "offset_values is empty");
                }
                error_code = GetLastError();
                throw(error_code);
            }
        }

        else
        {
            for (index = 0; index < sz_offsets / sizeof(DWORD); index++)
            {
                dw_values[index] = readAddressOffset(lp_address, dw_offsets[index]);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }
    private_field = FALSE;
    return dw_values;
}

/**
 * @description - Write value at address with offsets.
 * @param - Address in void* format and offset in DWORD* format,Size in SIZE_T format and value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise exits the application with error code.
  * NOTE : This will be useful in writing multiple values at a time like multiple ammo/clips values at Ammos/Clips offsets list.
 */

BOOL writeAddressOffsets(LPVOID lp_address, DWORD *dw_offsets, SIZE_T sz_offsets, DWORD dw_value)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {

            for (index = 0; index < sz_offsets / sizeof(DWORD); index++, dw_offsets++)
            {
                write_status = writeAddressOffset(lp_address, *dw_offsets, dw_value);

                if (!write_status)
                {
                    if (ERROR_INVALID)
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
    private_field = FALSE;
    return write_status;
}

/**
 * @description - Read pointer's address from provided address with offset.
 * @param - Address in void* format and offset in DWORD format.
 * @return - If read succeeded then it returns address of pointer otherwise exits the application with error code.
 */

LPVOID readPointerOffset(LPVOID lp_address, DWORD dw_offset)
{
    private_field = TRUE;
    error_code = NIL;

    HANDLE game_handle = getGameHandle();
    LPVOID lp_address_value = NULL;
    LPDWORD lpdw_address = (LPDWORD)lp_address;
    DWORD dw_address = (DWORD)lpdw_address;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            lpdw_address = (LPDWORD)(dw_address + dw_offset);
            lp_address_value = (LPVOID)readAddress((LPVOID)lpdw_address);

            if (lp_address_value == NULL)
            {
                if (ERROR_INVALID)
                {
                    error_code = GetLastError();
                    throw(error_code);
                }

                //if Error not set explicitly by readAddress.
                else if (error_code == ERROR_SUCCESS)
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
    private_field = FALSE;
    return lp_address_value;
}

/**
 * @description - Read pointer's address from provided address with provided offsets.
 * @param - Address in void* format, offsets in DWORD* format and size of offsets.
 * @return - If read succeeded then it returns address of pointer otherwise exits the application with error code.
 */

LPVOID readPointerOffsets(LPVOID lp_address, DWORD *dw_offsets, SIZE_T sz_offsets)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    error_code = NIL;

    LPDWORD lpdw_address = (LPDWORD)lp_address;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {

            for (index = 0; index < sz_offsets / sizeof(DWORD); index++)
            {
                lpdw_address = (LPDWORD)readPointerOffset((LPVOID)lpdw_address, dw_offsets[index]);
                if (lpdw_address == NULL)
                {

                    if (ERROR_INVALID)
                        error_code = GetLastError();

                    //if Error not set explicitly by readAddress.
                    else if (error_code == ERROR_SUCCESS)
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
    private_field = FALSE;
    return (LPVOID)lpdw_address;
}

/**
 * @description - Write value at pointer's address with offset.
 * @param - Address in void* format and offset in DWORD format,value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise exits the application with error code.
 */

BOOL writePointerOffset(LPVOID lp_address, DWORD dw_offset, DWORD dw_value)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    DWORD dw_base_address_value = NIL, dw_real_address = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        dw_base_address_value = readAddress(lp_address);
        if (dw_base_address_value == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }

        else
        {
            dw_real_address = dw_base_address_value + dw_offset;
            write_status = writeAddress((LPVOID)dw_real_address, dw_value);

            if (!write_status)
            {
                if (ERROR_INVALID)
                    error_code = GetLastError();
                throw(error_code);
            }
        }
    }
    catch (error_code)
    {
        showError(error_code, FUNC_NAME, LINE_NO);
    }
    private_field = FALSE;
    return write_status;
}

/**
 * @description - Write value at pointer's address with offsets.
 * @param - Address in void* format and offset in DWORD* format,Size in SIZE_T format and value in DWORD format.
 * @return - If write succeeded then it returns TRUE otherwise exits the application with error code.
 */

BOOL writePointerOffsets(LPVOID lp_address, DWORD *dw_offsets, SIZE_T sz_offsets, DWORD dw_value)
{
    private_field = TRUE;
    HANDLE game_handle = getGameHandle();
    UINT index = NIL;
    BOOL write_status = FALSE;
    error_code = NIL;

    try
    {
        if (game_handle == NIL)
        {
            if (ERROR_INVALID)
                error_code = GetLastError();
            throw(error_code);
        }
        else
        {

            for (index = 0; index < sz_offsets / sizeof(DWORD); index++, dw_offsets++)
            {
                write_status = writePointerOffset(lp_address, *dw_offsets, dw_value);

                if (!write_status)
                {
                    if (ERROR_INVALID)
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
    private_field = FALSE;
    return write_status;
}

/**
 * @description -  Get current game name from memory.
 * @return - current opened game name.
 */

LPCSTR getGameName()
{
    return (game_name[0] != NUL) ? game_name : NUL;
}

/**
 * @description -  Get process ID of game from memory.
 * @return - process ID of current opened game.
 */

DWORD getProcessID()
{
    return process_id;
}

/**
 * @description - Get game handle from HWND (handle to window).
 * @param - Handle to current window of game in HWND format.
 * @return - Handle to process on success otherwise exits the application with error code.
 * NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
 */

HANDLE getGameHandle4mHWND(HWND g_hwnd)
{
    private_field = TRUE;
    DWORD p_id = NIL;
    HANDLE g_handle = NULL;
    error_code = NIL;

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
    private_field = FALSE;
    return g_handle;
}

/**
 * @description - Get process ID of game from HWND (handle to window).
 * @param - Handle to current window of game in HWND format.
 * @return - On success it returns game's process ID otherwise exits the application with error code.
 * NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
 */

DWORD getProcessID4mHWND(HWND g_hwnd)
{
    private_field = TRUE;
    DWORD p_id = NIL;
    error_code = NIL;

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

    private_field = FALSE;
    return p_id;
}

/**
 * @description - Get base address of current game.
 * @param - Process ID in DWORD format.
 * @return - On success it returns base address of game in (LPBYTE) format otherwise exits the application with error code.
 * NOTE : This is required for games that don't have too many static pointers.
 */

LPBYTE getGameBaseAddress(DWORD process_id)
{
    private_field = TRUE;
    SetLastError(NO_ERROR);

    HANDLE snapshot = NULL;
    MODULEENTRY32 module = {};
    LPBYTE base_address = NUL;
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    error_code = NIL;

    try
    {
        if (snapshot == NULL || snapshot == INVALID_HANDLE_VALUE)
        {
            if (ERROR_INVALID)
            {
                error_code = GetLastError();
                throw(error_code);
            }
            else if (error_code == NIL)
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
    UINT index = NIL;
    size_t cheat_len = lstrlen(cheat_code);
    CHAR cheat_buff[cheat_len + 1];
    CopyMemory(cheat_buff, cheat_code, sizeof(CHAR) * cheat_len + 1);

    //convert cheat code to upper case for better mapping of characters.
    LPCSTR lp_cheat_upper = CharUpper(cheat_buff);

    //Time delay before entering first char of cheat.
    Sleep(200);

    //Press all the cheat keys from cheat code.
    for (index = 0; index < cheat_len; index++)
    {
        doKeyPress(lp_cheat_upper[index]);
    }
}

/**
 * INFO : Search any value in current offset ie. (base_address + offset) for finding new heath/ammos/weapons in game.
 * @description -  Search value in offset area.
 * @param - base address of Ammo/health/clips etc in void*,offset limit in size_t,offset size and value for searching.
 * @return - If value found it returns its address and offset in formatted string otherwise exits the application with error code.
 * NOTE : FREE this memory after using it to avoid memory leaks use HeapFree() Method from (windows.h).
 */

LPSTR searchOffsetArea(LPVOID offset_base_address, const size_t offset_limit, const size_t offset_size, DWORD search)
{
    private_field = TRUE;
    int value = 0, offset_index = 0, offset = 0, offset_len = (offset_limit / offset_size);
    DWORD offset_base = (DWORD)offset_base_address;
    DWORD offset_address = offset_base;
    int search_list_len = offset_len * 0x40;
    error_code = NIL;

    DWORD dw_search_size = search_list_len * sizeof(char);
    LPSTR search_list = (LPSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dw_search_size);

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

    if (logs_enabled)
    {
        addLog("%s->Search list size is : %u\n", FUNC_NAME, sizeof(search_list));
    }

    //copy headline status to search list.
    wsprintf(search_list, "%s\n", "SearchOffsetArea status : ");

    for (offset_index = 0; offset_index < offset_len; offset_index++)
    {
        offset_address += offset_size;
        offset = (int)(offset_address - offset_base);
        value = readAddress((LPVOID)offset_address);

        //if value found then copy its offset address etc in formatted string.
        if (value == search)
            wsprintf(search_list + lstrlen(search_list), "Value : %d\tAddress : 0x%X\tOffset : 0x%X\n", value, offset_address, offset);
    }
    private_field = FALSE;
    return search_list;
}

/**
 * INFO : Whether library should maintain logs internally (enable this if you want this feature).
 * @description - Enable logs in library.
 * @return - Returns true if enabled is success otherwise returns false.
 */

BOOL enableLogs(void)
{
    private_field = TRUE;
    BOOL enable_status = FALSE;

    if (logs_enabled == FALSE)
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
 * @return - Returns true if disable is success otherwise returns false.
 */

BOOL disableLogs(void)
{
    private_field = TRUE;
    BOOL disable_status = FALSE;

    if (logs_enabled == TRUE)
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
    return game_handle;
}

/**
 * @description - Get Handle to current game's window.
 * @return - If game found it return Handle to current game's windows otherwise returns NULL.
 */

HWND getGameHWND()
{
    return game_hwnd;
}

/****************************************************************************/
/****************-PRIVATE-METHODS-*******************************************/
/****************************************************************************/

static void showError(DWORD error_code, LPCSTR error_msg, DWORD line_no)
{
    private_field = TRUE;
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        char err_msg_buff[0xFF] = {'\0'};
        char sys_err_buff[0xFF] = {'\0'};
        LPSTR ptr = NUL;

        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, error_code,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                      sys_err_buff, sizeof(sys_err_buff), (va_list *)NULL);

        // Trim the end of the line and terminate it with a null
        ptr = sys_err_buff;
        while ((*ptr > 31) || (*ptr == 9))
            ++ptr;

        do
        {
            *ptr-- = '\0';
        } while ((ptr >= sys_err_buff) && ((*ptr == '.') || (*ptr < 33)));

        //copy error from getLastError() to error buffer.
        wsprintf(err_msg_buff, "\nINFO : %s method failed!\nREASON : (%s)\nLINE. : occurred at line no. %d\n", error_msg, sys_err_buff, line_no);

        //Show error and exit afterwards.
        MessageBox((HWND)NULL, err_msg_buff, "ERROR!", MB_ICONERROR);

        if (logs_enabled)
        {
            addLog("Error occurred : %s\n", err_msg_buff);
            private_field = TRUE;
        }

        DWORD exit_code;
        GetExitCodeProcess(GetCurrentProcess(), &exit_code);

        if (logs_enabled)
        {
            addLog("Process exited with exit code : %u\n", exit_code);
        }

        ExitProcess(exit_code);
    }
    private_field = FALSE;
}

static void showInfo(LPCSTR info_message)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        MessageBox((HWND)NULL, info_message, "INFO!", MB_ICONINFORMATION);
        if (logs_enabled)
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
        if (logs_enabled)
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
        EnumWindows(EnumAllWindows, (LPARAM)process_id);
}

/*Custom logger to add logs for trainer.*/
static void addLog(LPCSTR format, ...)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        private_field = TRUE;
        error_code = NIL;
        int log_len = 0x400;
        char log_buf[log_len];
        static LPCSTR log_file_name = "GTLibc_logs.log";
        ZeroMemory(log_buf, sizeof(log_buf));

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
            if (!WriteFile(file_handle, (LPCVOID)log_buf, lstrlen(log_buf), &data_size, (LPOVERLAPPED)NULL))
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
    static char time_buff[0x50] = {NUL};

    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        GetLocalTime(&sys_time);
        wsprintf(time_buff, "Time : %u:%u:%u\tYear : %u/%u/%u\n", sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wDay, sys_time.wMonth, sys_time.wYear);
    }
    return time_buff;
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

static LPSTR strcasestr(LPSTR str1, LPCSTR str2)
{
    if (isPrivateField(private_field, FUNC_NAME, LINE_NO))
    {
        LPCSTR a, b;
        for (; *str1; *str1++)
        {
            a = str1;
            b = str2;

            while ((*a++ | 32) == (*b++ | 32))
                if (!*b)
                    return (str1);
        }
    }
    return NUL;
}

static BOOL isPrivateField(BOOL private_field, LPCSTR proc_name, int line_no)
{
    if (private_field)
        return TRUE;

    else
    {
        char err_buff[0x100] = {NUL};
        wsprintf(err_buff, "ERROR : %s method failed!\nREASON : Access to private method! (ERROR_INVALID_FUNCTION) \nLINE : occurred at line no. %d", proc_name, line_no);
        MessageBox((HWND)NULL, err_buff, "ERROR!", MB_ICONERROR);

        DWORD exit_code = NIL;
        GetExitCodeProcess(GetCurrentProcess(), &exit_code);
        ExitProcess(exit_code);
    }
}
