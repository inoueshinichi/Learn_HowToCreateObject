#include <windows_version.hpp>

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