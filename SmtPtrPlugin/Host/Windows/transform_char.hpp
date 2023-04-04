/**
 * @file transform_char.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-02
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

#include <string>

/**
 * @brief Windowsに関する文字周りの知識は下記を参照.
 * https://github.com/inoueshinichi/Dev_Win32NativeGuiApp/wiki/Windows周りの文字型&文字コード定義
 */

/**
 * @brief Windowsで標準入力をShiftJisからUTF-8に切り替える方法
 * https://qiita.com/tats-u/items/ef149aee6b69407db79b
 * 方法1. Windows10専用の手法: setlocale・std::locale::globalなどに空文字のデフォルトロケールを渡す.
 * 方法2. 実行プログラムへのマニフェストの埋め込み.
 *
 * 方法1.
 * #include <locale> or <locale.h>
 * setlocale(LC_ALL, ""); or std::locale::global(std::locale(""));
 *
 * 方法2. (CMakeでadd_executable()にマニフェストを埋め込む)
 * // char_code_manifest.xml
 * <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
 * <assembly manifestVersion="1.0" xmlns="urn:schemas-microsoft-com:asm.v1">
 * <application>
 *      <windowsSettings>
 *      <activeCodePage xmlns="http://schemas.microsoft.com/SMI/2019/WindowsSettings">UTF-8</activeCodePage>
 *      </windowsSettings>
 * </application>
 * </assembly>
 *
 * // CMakeLists.txt
 * add_ececutable(${TARGET_EXE} ... , char_code_manifest.xml)
 */

namespace is
{
    //////////////////////////////////
    /* Convert from UTF-8 to UTF-16 */
    //////////////////////////////////
    inline std::wstring CvtUtf8ToUtf16(char const *utf8Char);
    inline std::wstring CvtUtf8ToUtf16(const std::string &utf8String);

    //////////////////////////////////
    /* Convert from UTF-16 to UTF-8 */
    //////////////////////////////////
    inline std::string CvtUtf16ToUtf8(const wchar_t *utf16Wchar);
    inline std::string CvtUtf16ToUtf8(const std::wstring &utf16Wstring);
    inline std::string CvtUtf16ToUtf8(const char16_t *utf16Char16);
    inline std::string CvtUtf16ToUtf8(const std::u16string &utf16String);

    //////////////////////////////////////
    /* Convert from Shift-JIS to UTF-16 */
    //////////////////////////////////////
    inline std::wstring CvtShiftJisToUtf16(const char *shiftJisChar);
    inline std::wstring CvtShiftJisToUtf16(const std::string &shiftJisString);

    //////////////////////////////////////
    /* Convert from UTF-16 to Shift-JIS */
    //////////////////////////////////////
    inline std::string CvtUtf16ToShiftJis(const wchar_t *utf16Wchar);
    inline std::string CvtUtf16ToShiftJis(const std::wstring &utf16Wstring);
    inline std::string CvtUtf16ToShiftJis(const char16_t *utf16Char16);
    inline std::string CvtUtf16ToShiftJis(const std::u16string &utf16String);

    //////////////////////////////////////
    /* Convert from UTT-8 to Shift-JIS  */
    //////////////////////////////////////
    inline std::string CvtUtf8ToShiftJis(const char *utf8Char);
    inline std::string CvtUtf8ToShiftJis(const std::string &utf8String);

    //////////////////////////////////////
    /* Convert from Shift-JIS to UTF-8  */
    //////////////////////////////////////
    inline std::string CvtShiftJisToUtf8(const char *shiftJisChar);
    inline std::string CvtShiftJisToUtf8(const std::string &shiftJisString);

    // 文字列を16進数で標準出力にダンプする
    template <typename OStream, typename T>
    inline void DumpByHexLog(OStream &os, const T *t)
    {
        bool first = true;
        const auto *byte = reinterpret_cast<unsigned char const *>(t);
        os << std::hex << std::uppercase << std::setfill('0') << std::setw(2);
        while (*byte)
        {
            if (!std::exchange(first, false))
            {
                os << " ";
            }
            os << static_cast<unsigned int>(*byte);
            ++byte;
        }
        os << std::endl;
    }
}