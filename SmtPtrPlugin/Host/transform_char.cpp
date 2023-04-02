#include <transform_char.hpp>

namespace is
{
    //////////////////////////////////
    /* Convert from UTF-8 to UTF-16 */
    //////////////////////////////////
    /**
     * @brief UTF-8 -> UTF-16
     *
     * @param utf8Char
     * @return std::wstring
     */
    std::wstring CvtUtf8ToUtf16(char const *utf8Char)
    {
        if (!utf8Char)
            return std::wstring();

        /* C++17よりstd::codecvtヘッダーが非推奨になったので、下記2行は使えない */
        // std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        // return converter.from_bytes(utf8_char);

        /* 代替案(プラットフォーム毎の変換APIを使うこと. 主にWindows OS) */
        // https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
        size_t necessarySize = ::MultiByteToWideChar(CP_UTF8, 0, utf8Char, -1, NULL, 0);
        std::wstring utf16Wstr(necessarySize, 0);
        ::MultiByteToWideChar(CP_UTF8, 0, utf8Char, -1, &utf16Wstr[0], necessarySize);
        utf16Wstr.resize(necessarySize - 1); // '\0'を消す
        return utf16Wstr;
    }

    /**
     * @brief UTF-8 -> UTF-16
     *
     * @param utf8String
     * @return std::wstring
     */
    std::wstring CvtUtf8ToUtf16(const std::string &utf8String)
    {
        return CvtUtf8ToUtf16(utf8String.c_str());
    }

    //////////////////////////////////
    /* Convert from UTF-16 to UTF-8 */
    //////////////////////////////////
    /**
     * @brief UTF-16 -> UTF-8
     *
     * @param utf16Wchar
     * @return std::string
     */
    std::string CvtUtf16ToUtf8(const wchar_t *utf16Wchar)
    {
        if (!utf16Wchar)
            return std::string();

        /* C++17よりstd::codecvtヘッダーが非推奨になったので、下記2行は使えない */
        // std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        // return converter.to_bytes(utf16_wchar);

        /* 代替案(プラットフォーム毎の変換APIを使うこと. 主にWindows OS) */
        // https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
        size_t necessarySize = ::WideCharToMultiByte(CP_UTF8, 0, utf16Wchar, -1, NULL, 0, NULL, NULL);
        std::string utf8Str(necessarySize, 0);
        ::WideCharToMultiByte(CP_UTF8, 0, utf16Wchar, -1, &utf8Str[0], necessarySize, NULL, NULL);
        utf8Str.resize(necessarySize - 1); // '\0'を消す
        return utf8Str;
    }

    /**
     * @brief UTF-16 -> UTF-8
     *
     * @param utf16Wstring
     * @return std::string
     */
    std::string CvtUtf16ToUtf8(const std::wstring &utf16Wstring)
    {
        return CvtUtf16ToUtf8(utf16Wstring.c_str());
    }

    /**
     * @brief UTF-16 -> UTF-8
     *
     * @param utf16Char16
     * @return std::string
     */
    std::string CvtUtf16ToUtf8(const char16_t *utf16Char16)
    {
        return CvtUtf16ToUtf8(reinterpret_cast<const wchar_t *>(utf16Char16));
    }

    /**
     * @brief UTF-16 -> UTF-8
     *
     * @param utf16String
     * @return std::string
     */
    std::string CvtUtf16ToUtf8(const std::u16string &utf16String)
    {
        return CvtUtf16ToUtf8(utf16String.c_str());
    }

    //////////////////////////////////////
    /* Convert from Shift-JIS to UTF-16 */
    //////////////////////////////////////
    /**
     * @brief Shift-JIS -> UTF-16
     *
     * @param shiftJisChar
     * @return std::wstring
     */
    std::wstring CvtShiftJisToUtf16(const char *shiftJisChar)
    {
        if (!shiftJisChar)
            return std::wstring();

        size_t necessarySize = ::MultiByteToWideChar(CP_ACP, 0, shiftJisChar, -1, NULL, 0);
        std::wstring utf16Wstr(necessarySize, 0);
        ::MultiByteToWideChar(CP_ACP, 0, shiftJisChar, -1, &utf16Wstr[0], necessarySize);
        utf16Wstr.resize(necessarySize - 1); // '\0'を消す
        return utf16Wstr;
    }

    /**
     * @brief Shift-JIS -> UTF-16
     *
     * @param shiftJisString
     * @return std::wstring
     */
    std::wstring CvtShiftJisToUtf16(const std::string &shiftJisString)
    {
        return CvtShiftJisToUtf16(shiftJisString.c_str());
    }

    //////////////////////////////////////
    /* Convert from UTF-16 to Shift-JIS */
    //////////////////////////////////////
    /**
     * @brief UTF-16 -> Shift-JIS
     *
     * @param utf16Wchar
     * @return std::string
     */
    std::string CvtUtf16ToShiftJis(const wchar_t *utf16Wchar)
    {
        if (!utf16Wchar)
            return std::string();

        size_t necessarySize = ::WideCharToMultiByte(CP_ACP, 0, utf16Wchar, -1, NULL, 0, NULL, NULL);
        std::string shiftJisStr(necessarySize, 0);
        ::WideCharToMultiByte(CP_ACP, 0, utf16Wchar, -1, &shiftJisStr[0], necessarySize, NULL, NULL);
        shiftJisStr.resize(necessarySize - 1); // '\0'を消す
        return shiftJisStr;
    }

    /**
     * @brief UTF-16 -> Shift-JIS
     *
     * @param utf16Wstring
     * @return std::string
     */
    std::string CvtUtf16ToShiftJis(const std::wstring &utf16Wstring)
    {
        return CvtUtf16ToShiftJis(utf16Wstring.c_str());
    }

    /**
     * @brief UTF-16 -> Shift-JIS
     *
     * @param utf16Char16
     * @return std::string
     */
    std::string CvtUtf16ToShiftJis(const char16_t *utf16Char16)
    {
        return CvtUtf16ToShiftJis(reinterpret_cast<const wchar_t *>(utf16Char16));
    }

    /**
     * @brief UTF-16 -> Shift-JIS
     *
     * @param utf16String
     * @return std::string
     */
    std::string CvtUtf16ToShiftJis(const std::u16string &utf16String)
    {
        return CvtUtf16ToShiftJis(utf16String.c_str());
    }

    //////////////////////////////////////
    /* Convert from UTT-8 to Shift-JIS  */
    //////////////////////////////////////
    /**
     * @brief UTF-8 -> Shift-JIS
     *
     * @param utf8Char
     * @return std::string
     */
    std::string CvtUtf8ToShiftJis(const char *utf8Char)
    {
        /* UTF-8 -> UTF-16 -> Shift-JIS */
        std::wstring utf16Wstr = CvtUtf8ToUtf16(utf8Char);
        return CvtUtf16ToShiftJis(utf16Wstr);
    }

    /**
     * @brief UTF-8 -> Shift-JIS
     *
     * @param utf8String
     * @return std::string
     */
    std::string CvtUtf8ToShiftJis(const std::string &utf8String)
    {
        /* UTF-8 -> UTF-16 -> Shift-JIS */
        std::wstring utf16Wstr = CvtUtf8ToUtf16(utf8String);
        return CvtUtf16ToShiftJis(utf16Wstr);
    }

    //////////////////////////////////////
    /* Convert from Shift-JIS to UTF-8  */
    //////////////////////////////////////
    /**
     * @brief Shift-JIS -> UTF-8
     *
     * @param shiftJisChar
     * @return std::string
     */
    std::string CvtShiftJisToUtf8(const char *shiftJisChar)
    {
        /* Shift-JIS -> UTF-16 -> UTF-8 */
        std::wstring utf16Wstr = CvtShiftJisToUtf16(shiftJisChar);
        return CvtUtf16ToShiftJis(utf16Wstr);
    }

    /**
     * @brief Shift-JIS -> UTF-8
     *
     * @param shiftJisString
     * @return std::string
     */
    std::string CvtShiftJisToUtf8(const std::string &shiftJisString)
    {
        /* Shift-JIS -> UTF-16 -> UTF-8 */
        std::wstring utf16Wstr = CvtShiftJisToUtf16(shiftJisString);
        return CvtUtf16ToShiftJis(utf16Wstr);
    }
}