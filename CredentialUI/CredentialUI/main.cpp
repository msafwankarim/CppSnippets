#include<Windows.h>     //Global functions and types of Win32
#include<wincred.h>     //Required for Credential UI functions
#include <iostream>     //Required for console io

#pragma comment(lib,"Credui.lib")       //Dynamic linking with Credui.lib


/*
#=======================================================================================#
*	Author: 		Safwan																*
*	Date:			Monday, ‎22 ‎March ‎2021, ‏‎5:23:34 PM									*
*	Brief:			Clones windows Administrator Password prompt using WinCred API		*
*																						*
#=======================================================================================#
*/

//int main() {
    /*
    CREDUI_INFO cui;
    TCHAR pszName[CREDUI_MAX_USERNAME_LENGTH + 1];
    TCHAR pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1];
    BOOL fSave;
    DWORD dwErr;

    cui.cbSize = sizeof(CREDUI_INFO);
    cui.hwndParent = NULL;
    //  Ensure that MessageText and CaptionText identify what credentials
    //  to use and which application requires them.
    cui.pszMessageText = TEXT("Enter administrator account information");
    cui.pszCaptionText = TEXT("Administrator Access Required");
    cui.hbmBanner = NULL;
    fSave = FALSE;
    SecureZeroMemory(pszName, sizeof(pszName));
    SecureZeroMemory(pszPwd, sizeof(pszPwd));
    dwErr = CredUIPromptForCredentials(
        &cui,                         // CREDUI_INFO structure
        TEXT("Host"),            // Target for credentials
                                      //   (usually a server)
        NULL,                         // Reserved
        0,                            // Reason
        pszName,                      // User name
        CREDUI_MAX_USERNAME_LENGTH + 1, // Max number of char for user name
        pszPwd,                       // Password
        CREDUI_MAX_PASSWORD_LENGTH + 1, // Max number of char for password
        &fSave,                       // State of save check box
        CREDUI_FLAGS_GENERIC_CREDENTIALS |  // flags
        CREDUI_FLAGS_ALWAYS_SHOW_UI |
        CREDUI_FLAGS_DO_NOT_PERSIST);

    if (!dwErr)
    {
        FILE* stream;
        _wfreopen_s(&stream,L"log.txt", L"w", stdout);
        //  Put code that uses the credentials here.
        std::wcout << L"Username: " << pszName << std::endl << L"Password: " << pszPwd << std::endl;
        //  When you have finished using the credentials,
        //  erase them from memory.
        SecureZeroMemory(pszName, sizeof(pszName));
        SecureZeroMemory(pszPwd, sizeof(pszPwd));
    }
    system("type log.txt");
    */
    //std::wstring strDbPath = L"...";
    //std::wstring strMsg = L"Enter your password to connect to: " + strDbPath;
    //wchar_t init[] = L"Empty";
    //wchar_t* pszUserName = init;
    //wchar_t* pszPassword = init;
    //wchar_t* pszDomainName = init;
    //CREDUI_INFOW credui = {};
    //credui.cbSize = sizeof(credui);
    //credui.hwndParent = nullptr;
    //credui.pszMessageText = strMsg.c_str();
    //credui.pszCaptionText = L"Enter Network Password";
    //credui.hbmBanner = nullptr;
    ////SEC_WINNT_AUTH_IDENTITY authPackage;
    //ULONG authPackage = 0;
    //LPVOID outCredBuffer = nullptr;
    //ULONG outCredSize = sizeof(outCredBuffer);
    //BOOL save = false;
    //DWORD dwFlags = CRED_PACK_GENERIC_CREDENTIALS;
    //ULONG* pAuthBuffer = &authPackage;
    //int cbAuthBuffer = sizeof(pAuthBuffer);
    //DWORD pcchlMaxUserName = CREDUI_MAX_USERNAME_LENGTH + 1;
    //
    
   

  /*  int result = CredUIPromptForWindowsCredentialsW(
        &credui,
        0,
        &authPackage,
        nullptr,
        NULL,
        &outCredBuffer,
        &outCredSize, 
        &save, 1);
    if (!result)
    {
        DWORD dwCredSize = outCredSize;
        BOOL bresult = CredUnPackAuthenticationBuffer(
            dwFlags,
            outCredBuffer,
            dwCredSize,
            pszUserName,
            &pcchlMaxUserName,
            pszDomainName,
            &pcchlMaxUserName,
            pszPassword,
            &pcchlMaxUserName
        );*/

       /* if (bresult || !bresult)
        {
            std::wcout << L"Username: " << pszUserName << std::endl << L"Password: " << pszPassword << std::endl;
            int err = GetLastError();
            if (err == ERROR_NOT_CAPABLE)
                std::cout << "ERROR_NOT_CAPABLE" << std::endl;
            else if (err == ERROR_INSUFFICIENT_BUFFER)
                std::cout << "ERROR_INSUFFICIENT_BUFFER" << std::endl;
            else if (err == ERROR_NOT_SUPPORTED)
                std::cout << "ERROR_NOT_SUPPORTED" << std::endl;
            else
                std::cout << err << std::endl;
        }

    }*/
    //
   
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

        LPWSTR szUserName = new WCHAR[maxUserNameSize];
        LPWSTR szDomain = new WCHAR[maxDomainNameSize];
        LPWSTR szPassword = new WCHAR[maxPasswordLength];

        DWORD dwCredBufferSize = outCredBufferSize;     //ULONG to DWORD

        DWORD lastError = 0;
        dwError = CredUnPackAuthenticationBuffer(
            CRED_PACK_GENERIC_CREDENTIALS,
            outCredBuffer,
            dwCredBufferSize,
            szUserName,
            &maxUserNameSize,
            szDomain,
            &maxDomainNameSize,
            szPassword,
            &maxPasswordLength
        );
        lastError = GetLastError();

        //Check for error
        if (dwError == FALSE)
        {
            DisplayConsoleError(L"Error", L"CredUnPackAuthenticationBuffer", lastError);
        }
        else
        {
            std::wcout << L"username " << szUserName << std::endl;
            std::wcout << L"domain " << szDomain << std::endl;
            std::wcout << L"password " << szPassword << std::endl;

           
        }

    }
    else
    {
        lastError = dwError;
    }

    SecureZeroMemory(outCredBuffer, outCredBufferSize);
    CoTaskMemFree(outCredBuffer);

    return lastError;
}