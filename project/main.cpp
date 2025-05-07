/*
 * Windows Notification Reposition
 * Based on 'vanlocvo/top-right-noti' on GitHub
 * Link: https://github.com/vanlocvo/top-right-noti
 */

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// Define default values
#define DEFAULT_Y_POS       100
#define DEFAULT_WAIT_TIME   10


// Entry point
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    // New Y-position 
    int nNewPosY = 0;

    // Get position from command line
    if (szCmdLine != NULL) {
        nNewPosY = atoi(szCmdLine);
	}
    // Otherwise, use default
    else {
        nNewPosY = DEFAULT_Y_POS;
	}

    while (true) {
        
        // Get Windows System Notifications
        HWND hNotifyWnd = ::FindWindow(L"Windows.UI.Core.CoreWindow", L"New notification");

        // Get the current position of the notification window
        RECT rcNotifyRect;
        GetWindowRect(hNotifyWnd, &rcNotifyRect);

        // Re-calculate notification window rectangle
        rcNotifyRect.right -= rcNotifyRect.left;
        rcNotifyRect.bottom -= rcNotifyRect.top;
        
        // Reposition notification window
        int nNewXPos = GetSystemMetrics(SM_CXSCREEN) - rcNotifyRect.right;
        SetWindowPos(hNotifyWnd, NULL, nNewXPos, nNewPosY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);

        // Wait for a blink
        Sleep(DEFAULT_WAIT_TIME);
    }

    return 0;
}
