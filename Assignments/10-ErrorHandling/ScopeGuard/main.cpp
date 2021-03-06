#include "guard.h"
#include <Windows.h>
#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>
/*******************************************************************************************
                 ### INSTRUCTIONS ###
* Build a scopeguard that will take a void() function
* Modify readBin to use scopeguard to call closeHandle to make that code exception safe.
*******************************************************************************************/

//THIS SHOULD NEVER BE GLOBAL IT IS EVIL BUT I NEEDED IT TO BE ABLE TO TEST.
//DO NOT DO THIS IN PRODUCTION CODE!!!
HANDLE g_hFile = INVALID_HANDLE_VALUE;

struct BadFile : std::runtime_error {
    BadFile(const std::string& what) :std::runtime_error(what) {}
};

void processBin(std::vector<uint8_t>& bin)
{
    throw std::runtime_error("BAD");
}

std::vector<uint8_t> readBin(const std::wstring& filename)
{
    std::vector<uint8_t> bin;
    g_hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "ERROR: " << GetLastError() << "\n";
        throw BadFile("Can't Open File\n");
    }
    bin.resize(GetFileSize(g_hFile, nullptr));
    DWORD bytesRead = 0;
    ReadFile(g_hFile, bin.data(), bin.size(), &bytesRead, nullptr);

    processBin(bin);
    ////////////Protect with scopeGuard//////////////////
    CloseHandle(g_hFile);
    g_hFile = INVALID_HANDLE_VALUE;
    /////////////////////////////////////////////////////
    return bin;
}

int main()
{
    try {
        auto file = readBin(L"C:\\Windows\\System32\\calc.exe");
    }
    catch (const BadFile& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
    catch (...) {
        //don't terminate.
    }

    if (g_hFile == INVALID_HANDLE_VALUE) {
        std::cout << "PASS!\n";
        return 0;
    }

    std::cerr << "FAIL!\n";
    return -1;
}