#include <Windows.h>
#include <iostream>

#define WORLD_OFFSET 0x49CDA8
#define MONEY_OFFSET 0x461E74
#define LOCALPLAYER_OFFSET 0x58
#define NITRO_OFFSET 0x41C

DWORD base = (DWORD)GetModuleHandleA("SPEED2.EXE");

BOOL WINAPI MainThread(HMODULE hMod) {
    while (!GetAsyncKeyState(VK_DELETE)) {
        DWORD World = *(DWORD*)(base + WORLD_OFFSET);
        if (World) {
            DWORD LocalPlayer = *(DWORD*)(World + LOCALPLAYER_OFFSET);
            if (GetAsyncKeyState(VK_CONTROL) & 1) {
                *(int*)(LocalPlayer + NITRO_OFFSET) = 32000; // Fully charged
            }
            if (GetAsyncKeyState(0x4F) & 1) { // O key
                *(int*)(base + MONEY_OFFSET) += 10000;
            }
            if (GetAsyncKeyState(0x4C) & 1) { // L key
                *(int*)(base + MONEY_OFFSET) -= 10000;
            }
        }
    }
    FreeLibraryAndExitThread(hMod, 0);
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        std::cout << "Succeccfully injected" << std::endl;
        std::cout << "O-key +10000" << std::endl;
        std::cout << "L-key -10000" << std::endl;
        std::cout << "Ctrl to fully charged nitro" << std::endl;
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

