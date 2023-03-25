/**
 * @file windows_version.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#define NOMAXMIN // Workaround for windows max, min macro
#include <windows.h>
#include <tlhelp32.h>

#include <atlstr.h> // CString (ATL)
#include <tchar.h>  // _TCHAR

#include <iostream>
#include <sstream>
#include <string>

void ShowWindowsOSVersion()
{
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

#if defined(UNICODE) || defined(_UNICODE)
    std::wostringstream woss;
    woss << L"Windows Operating System Version" << std::endl;
    woss << L"--------------------------------" << std::endl;
    woss << L"Major version: " << osvi.dwMajorVersion << L"; ";
    woss << L"Minor version: " << osvi.dwMinorVersion << L"; ";
    woss << L"szCSDVersion: " << osvi.szCSDVersion << L";";
    woss << std::endl;
    std::wcout << woss.str();
#else
    std::ostringstream oss;
    oss << "Windows Operating System Version" << std::endl;
    oss << "--------------------------------" << std::endl;
    oss << "Major version: " << osvi.dwMajorVersion << "; ";
    oss << "Minor version: " << osvi.dwMinorVersion << "; ";
    oss << "szCSDVersion: " << osvi.szCSDVersion << ";";
    oss << std::endl;
    std::cout << oss.str();
#endif
}