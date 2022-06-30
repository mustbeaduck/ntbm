#include "selection.hpp"
#include "offsets.hpp"
#include "Windows.h"
#include "stdint.h"

typedef unsigned int uint;

HINSTANCE dllHandle;


void entryPoint() {

    uintptr_t base = ( uintptr_t ) GetModuleHandle(0);
    setTheOffsets(base);

    createSelection({0, 0});

    FreeLibraryAndExitThread(dllHandle, 0);
}

bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		dllHandle = hinstDLL;
        CloseHandle(CreateThread(NULL, (SIZE_T)NULL, (LPTHREAD_START_ROUTINE)entryPoint, NULL, 0, NULL));

        return true; 
    }
    
	return false;
} 


