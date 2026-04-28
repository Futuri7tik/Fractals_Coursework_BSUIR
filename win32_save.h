#ifndef WIN32_SAVE_H
#define WIN32_SAVE_H

#ifdef _WIN32
char* Win32_ShowSaveDialog(const char* defaultFileName);
#endif

#endif // WIN32_SAVE_H