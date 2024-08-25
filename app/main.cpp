#include <Windows.h>
#include <Shldisp.h>
#include "resource.h"
#include <fstream>
#include <iostream>
#include <tchar.h>
#include <filesystem>

#include <examplelib.h>

int Unzip2Folder( const TCHAR* zipFile, const TCHAR* folder )
{
    BSTR lpZipFile = SysAllocString(zipFile);
    BSTR lpFolder = SysAllocString(folder);
    CoInitialize( NULL);

    __try {
        IShellDispatch *pISD;

        Folder  *pZippedFile = 0L;
        Folder  *pDestination = 0L;

        long FilesCount = 0;
        IDispatch* pItem = 0L;
        FolderItems *pFilesInside = 0L;

        VARIANT Options, OutFolder, InZipFile, Item;        

        if (CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pISD) != S_OK)
            return 1;

        InZipFile.vt = VT_BSTR;
        InZipFile.bstrVal = lpZipFile;
        pISD->NameSpace( InZipFile, &pZippedFile);
        if (!pZippedFile)
        {
            pISD->Release();
            return 1;
        }

        OutFolder.vt = VT_BSTR;
        OutFolder.bstrVal = lpFolder;
        pISD->NameSpace( OutFolder, &pDestination);
        if(!pDestination)
        {
            pZippedFile->Release();
            pISD->Release();
            return 1;
        }

        pZippedFile->Items(&pFilesInside);
        if(!pFilesInside)
        {
            pDestination->Release();
            pZippedFile->Release();
            pISD->Release();
            return 1;
        }

        pFilesInside->get_Count( &FilesCount);
        if( FilesCount < 1)
        {
            pFilesInside->Release();
            pDestination->Release();
            pZippedFile->Release();
            pISD->Release();
            return 0;
        }

        pFilesInside->QueryInterface(IID_IDispatch,(void**)&pItem);

        Item.vt = VT_DISPATCH;
        Item.pdispVal = pItem;

        Options.vt = VT_I4;
        Options.lVal = 1024 | 512 | 16 | 4;//http://msdn.microsoft.com/en-us/library/bb787866(VS.85).aspx

        bool retval = pDestination->CopyHere( Item, Options) == S_OK;

        pItem->Release();pItem = 0L;
        pFilesInside->Release();pFilesInside = 0L;
        pDestination->Release();pDestination = 0L;
        pZippedFile->Release();pZippedFile = 0L;
        pISD->Release();pISD = 0L;        
    }
    __finally {
        SysFreeString(lpZipFile);
        SysFreeString(lpFolder);
        CoUninitialize();
    }

    return 0;
}

int main(int argc, char *argv[])
{
    const auto folderPath = TEXT("dlls");
    if (!CreateDirectory(folderPath, NULL))
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            // Failed to create the directory
            return 1;
        }
    }
    else {
        std::cout << "extracting dependencies...";
        HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
        unsigned int myResourceSize = ::SizeofResource(NULL, myResource);
        HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
        void* pMyBinaryData = ::LockResource(myResourceData);

        const TCHAR* tempZipFile = TEXT("data.zip");
        std::ofstream f(tempZipFile, std::ios::out | std::ios::binary);
        f.write((char*)pMyBinaryData, myResourceSize);
        f.close();

        TCHAR modulePath[MAX_PATH];
        GetModuleFileName(NULL, modulePath, MAX_PATH);
        std::filesystem::path moduleDir = std::filesystem::path(modulePath).parent_path();
        std::filesystem::path dllDir = moduleDir / folderPath;
        std::filesystem::path tempZipFilePath = moduleDir / tempZipFile;

        Unzip2Folder(tempZipFilePath.c_str(), dllDir.c_str());
        DeleteFile(tempZipFile);
        
        std::cout << "done!" << std::endl;
    }

    SetCurrentDirectory(folderPath);
    int result = doSomething(1, 2);
    std::cout << "Result from the delay load library is: " << result << std::endl;

    return 0;
}
