/**
 * @file plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#if defined(_MSC_VER)
#define NOMAXMIN // Workaround for windows max, min macro
#include <windows.h>
#include <atlstr.h> // CString (ATL)
#include <tchar.h>  // _TCHAR

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

class Plugin
{
public:
    virtual ~Plugin(){};
    virtual const char *GetPluginName() const = 0;
    virtual int MajorVersion() const = 0;
    virtual int MinorVersion() const = 0;
    virtual int PatchVersion() const = 0;
    virtual const char *CompiledDatetime() const = 0;
    virtual const char *CompiledTime() const = 0;
    virtual const char *FilePath() const = 0;
    virtual void About() const = 0;
};