#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void CopyFiles(const wstring& sourceDir, const wstring& destinationDir) {
    wstring searchPath = sourceDir + L"\\*.*";
    WIN32_FIND_DATA findData;
    HANDLE findHandle = FindFirstFile(searchPath.c_str(), &findData);

    if (findHandle != INVALID_HANDLE_VALUE) {
        do {
            wstring sourceFilePath = sourceDir + L"\\" + findData.cFileName;
            wstring destinationFilePath = destinationDir + L"\\" + findData.cFileName;
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0) {
                    CreateDirectory(destinationFilePath.c_str(), nullptr);
                    CopyFiles(sourceFilePath, destinationFilePath);
                }
            }
            else {
                CopyFile(sourceFilePath.c_str(), destinationFilePath.c_str(), FALSE);
            }
        } while (FindNextFile(findHandle, &findData));

        FindClose(findHandle);
    }
}

void CreateBinaryFile(const wstring& directoryPath) {
    wstring filePath = directoryPath + L"\\binary_file.bin";
    vector<int> numbers = { 1, 2, 3, 4, 5 };
    ofstream file(filePath, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(numbers.data()), numbers.size() * sizeof(int));
        file.close();
    }
}