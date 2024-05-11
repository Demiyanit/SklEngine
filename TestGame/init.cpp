#include <entry.hpp>
#include "TestGame.hpp"
#include <iostream>
#include <windows.h>

IApplication* GetConfiguredApplication() {
    char currentDir[MAX_PATH];
    DWORD result = GetCurrentDirectoryA(MAX_PATH, currentDir);
    if (result > 0) {
        std::cout << "Current directory: " << currentDir << std::endl;
    }
    else {
        std::cerr << "Error: Unable to get current directory" << std::endl;
    }
	return new TestGame();
}