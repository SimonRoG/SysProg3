#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <tlhelp32.h>
#include "Header.h"

using namespace std;

HWND hEdit;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR CLASS_NAME[] = _T("Sample Window Class");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hWnd = CreateWindowEx(0, CLASS_NAME, _T("Sample Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 565, 350, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 0;
    }
    hEdit = CreateWindowEx(0, _T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 10, 10, 200, 20, hWnd, NULL, hInstance, NULL);
    HWND hButton1 = CreateWindowEx(0, _T("BUTTON"), _T("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 40, 100, 20, hWnd, (HMENU)1, hInstance, NULL);
    HWND hButton2 = CreateWindowEx(0, _T("BUTTON"), _T("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 70, 100, 20, hWnd, (HMENU)2, hInstance, NULL);
    HWND hButton3 = CreateWindowEx(0, _T("BUTTON"), _T("3, 4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 100, 100, 20, hWnd, (HMENU)3, hInstance, NULL);
    HWND hButton4 = CreateWindowEx(0, _T("BUTTON"), _T("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 130, 100, 20, hWnd, (HMENU)4, hInstance, NULL);
    HWND hButton6 = CreateWindowEx(0, _T("BUTTON"), _T("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 160, 100, 20, hWnd, (HMENU)5, hInstance, NULL);

    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 1:
        {
            CreateDirectory(L"C:\\FILE11", NULL);
            CreateDirectory(L"C:\\FILE11\\FILE12", NULL);
            CreateDirectory(L"C:\\FILE11\\FILE12\\FILE13", NULL);
            CreateDirectory(L"C:\\FILE21", NULL);
            CreateDirectory(L"C:\\FILE21\\FILE22", NULL);
            SetWindowText(hEdit, L"Directory created");

            break;
        }
        case 2:
        {
            wstring sourceDir = L"C:\\FILE\\FILE21";
            wstring destinationDir = L"C:\\FILE21\\FILE22";
            CopyFiles(sourceDir, destinationDir);
            CreateBinaryFile(sourceDir);
            SetWindowText(hEdit, L"file copied, created");

            break;
        }
        case 3:
        {
            TCHAR currentTime[100];
            GetEnvironmentVariable(L"Time", currentTime, 100);
            SetEnvironmentVariable(L"Time", L"12:34");
            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;
            if (CreateProcess(L"C:\\Users\\Simon\\source\\repos\\SysProg2\\x64\\Debug\\SysProg2.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) { //путь к процессу надо изменить
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            SetEnvironmentVariable(L"Time", currentTime);

            break;
        }
        case 4:
        {
            wstring searchDir = L"C:\\FILE21\\FILE22\\*";
            wstring files;
            PROCESS_INFORMATION pi;
            STARTUPINFO si = { sizeof(si) };
            if (CreateProcess(L"C:\\Users\\Simon\\source\\repos\\SysProg2\\x64\\Debug\\SysProg2.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
            {
                WIN32_FIND_DATAW fd;
                HANDLE hFind = FindFirstFileW(searchDir.c_str(), &fd);
                if (hFind != INVALID_HANDLE_VALUE) {
                    do {
                        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                            wprintf(L"%ls\n", fd.cFileName);
                        }
                    } while (FindNextFileW(hFind, &fd));
                    FindClose(hFind);
                }
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }

            break;
        }
        case 5: 
        {

            printf("\nSystem information:\n");
            printf("PID of current process: %d\n", GetCurrentProcessId());
            printf("PPID of current process: %d\n", GetCurrentProcessId());

            HANDLE hProcessSnap;
            PROCESSENTRY32 pe32;
            hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hProcessSnap == INVALID_HANDLE_VALUE) {
                printf("Error: CreateToolhelp32Snapshot (of processes)\n");
                return 1;
            }
            pe32.dwSize = sizeof(PROCESSENTRY32);
            if (!Process32First(hProcessSnap, &pe32)) {
                printf("Error: Process32First\n");
                CloseHandle(hProcessSnap);
                return 1;
            }

            printf("\nProcess list:\n");

            do {
                printf("Process ID: %d, Parent Process ID: %d, Name: %ls\n", pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);
            } while (Process32Next(hProcessSnap, &pe32));
            CloseHandle(hProcessSnap);
            LPWSTR lpszVariable;
            LPTCH lpvEnv;
            lpvEnv = GetEnvironmentStringsW();
            if (lpvEnv == NULL) {
                printf("Error: GetEnvironmentStringsW\n");
                return 1;
            }

            printf("\nEnvironment variables:\n");

            lpszVariable = (LPWSTR)lpvEnv;
            while (*lpszVariable) {
                wprintf(L"%s\n", lpszVariable);
                lpszVariable += wcslen(lpszVariable) + 1;
            }
            FreeEnvironmentStringsW(lpvEnv);

            int a = 5;
            int b = 10;
            int c = a + b;
            printf("\nResults of changes:\n");
            printf("a = %d, b = %d, c = %d\n", a, b, c);

            break;
        }

        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}
