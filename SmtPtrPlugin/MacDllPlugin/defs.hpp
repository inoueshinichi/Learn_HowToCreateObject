/**
 * @file defs.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#if defined(_MSC_VER)
    #if defined(PLUGIN_EXPORTS) || defined(PLUGIN_dbg_EXPORTS)
    #define PLUGIN_API __declspec(dllexport)
    #else
    #define PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define PLUGIN_API 
#endif

#if defined(_MSC_VER)
// __func__の定義
#define __func__ __FUNCTION__
#endif

// Dll Plugin Version
#include <version.hpp>
