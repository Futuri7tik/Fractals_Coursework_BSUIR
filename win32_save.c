#ifdef _WIN32
#include "win32_save.h"
#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <string.h>

static char* WideToUtf8(const wchar_t* wstr) {
    if (!wstr)
        return NULL;

    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (len <= 0) return NULL;
    char* utf8 = (char*)malloc(len);
    if (!utf8) return NULL;
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
    return utf8;
}

char* Win32_ShowSaveDialog(const char* defaultFileName) {
    wchar_t buffer[MAX_PATH] = {0};
    if (defaultFileName) {
        MultiByteToWideChar(CP_UTF8, 0, defaultFileName, -1, buffer, MAX_PATH);
    } else {
        wcscpy(buffer, L"fractal.png");
    }

    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL; // Raylib-окно по умолчанию
    ofn.lpstrFilter = L"PNG Image\0*.png\0BMP Image\0*.bmp\0JPEG Image\0*.jpg\0All Files\0*.*\0\0";
    ofn.lpstrFile   = buffer;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER;
    ofn.lpstrDefExt = L"png";
    ofn.lpstrInitialDir = L".";

    if (GetSaveFileNameW(&ofn)) {
        return WideToUtf8(buffer);
    }
    return NULL;
}

#endif