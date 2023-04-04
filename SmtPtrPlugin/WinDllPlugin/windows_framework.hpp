/**
 * @file windows_version.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

///////////////////////
/* Windows Framework */
///////////////////////
#define NOMAXMIN // Workaround for windows max, min macro
#include <windows.h>
#include <tlhelp32.h>

#include <atlstr.h> // CString (ATL)
#include <tchar.h>  // _TCHAR

void ShowWindowsOSVersion();