/**
 * @file defs_plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#if defined(PLUGIN_EXPORTS) || defined(PLUGIN_dbg_EXPORTS)
#   define PLUGIN_API __declspec(dllexport)
#else
#   define PLUGIN_API __declspec(dllimport)
#endif

// Dll Plugin Version
#include <version.hpp>

// Windows Framework
#include <windows_framework.hpp>

#if defined(_MSC_VER)
// __func__の定義
#define __func__ __FUNCTION__
#endif