/*
 * Windows Notification Reposition
 * Based on 'vanlocvo/top-right-noti' on GitHub
 * Link: https://github.com/vanlocvo/top-right-noti
 */

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <windows.h>

using namespace std;

// Window positions
enum WndPosition {
    onTopLeft, onTopRight,
    onBottomLeft, onBottomRight
};

// Define default values
#define DEFAULT_WND_POSITION            onTopRight
#define DEFAULT_OVERRIDEN_H_MARGIN      100         /* pixel */
#define DEFAULT_OVERRIDEN_V_MARGIN      100         /* pixel */
#define DEFAULT_WAIT_INTERVAL           10          /* milisecs */

// Global variables
SIZE screenSize = {0};
FILE* fp = nullptr;
bool continueRunning = true;
unsigned char wndPosition = DEFAULT_WND_POSITION;
unsigned char horizontalMargin = DEFAULT_OVERRIDEN_H_MARGIN;
unsigned char verticalMargin = DEFAULT_OVERRIDEN_V_MARGIN;

// Display help info
void DisplayHelpInfo()
{
    // Redirect stdout to the command prompt
    // Create a console window if it's not already available
    HWND hConsoleWnd = GetConsoleWindow();
    if (!hConsoleWnd) {
        AllocConsole();
        errno_t err = freopen_s(&fp, "CONOUT$", "w", stdout);
        if (err != 0) {
            wchar_t buffer[1024];
            swprintf_s(buffer, sizeof(buffer) / sizeof (wchar_t), L"Failed to redirect stdout to the console. (Code: 0x%08X)", err);
            MessageBoxW(NULL, buffer, L"Windows Notification Reposition - Error", MB_OK | MB_ICONERROR);
        }
    }

    // Print help info to console screen
    cout << "\n[Windows Notification Reposition]\n";
    cout << "Usage: \n";
    cout << "  win-notif-reposition.exe [-help] [-position=$p] [-hmargin=$h] [-vmargin=$v]\n\n";
    cout << "  Arguments:\n";
    cout << "    -position\t: Set window position (0: top-left, 1: top-right, 2: bottom-left, 3: bottom-right)\n";
    cout << "    -hmargin\t: Set horizontal margin (range: 50-200 px)\n";
    cout << "    -vmargin\t: Set vertical margin (range: 50-200 px)\n";
    cout << "    -help or /?\t: Display this help message\n";
}

// Parse arguments from command-line
void ParseArgument(const wchar_t* arg)
{
    // -position=$p (0: top-left, 1: top-right, 2: bottom-left, 3: bottom-right)
    if (!wcsncmp(arg, L"-position=", 10)) {
        int val = _wtoi(arg + 10);
        if (val >= 0 && val <= 3) {
            wndPosition = val;
        }
    }

    // -hmargin=$h : horizontal margin (50 - 200px)
    else if (!wcsncmp(arg, L"-hmargin=", 9)) {
        int val = _wtoi(arg + 9);
        if (val >= 50 && val <= 200) {
            horizontalMargin = val;
        }
    }
    
    // -vmargin=$v : vertical margin (50 - 200px)
    else if (!wcsncmp(arg, L"-vmargin=", 9)) {
        int val = _wtoi(arg + 9);
        if (val >= 50 && val <= 200) {
            verticalMargin = val;
        }
    }
}

// Calculate notification window rectangle
void CalculatePosition(RECT rc, int& retXPos, int& retYPos)
{
    // Get window size
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    // Default position
    retXPos = rc.left; retYPos = rc.top;

	// Calculate new top-left position based on display position
    switch (wndPosition)
    {
        case onTopLeft:
            retXPos = horizontalMargin;
            retYPos = verticalMargin;
            break;
        
        case onTopRight:
            retYPos = verticalMargin;
            retXPos = screenSize.cx - width - horizontalMargin;
            break;

        case onBottomLeft:
            retXPos = horizontalMargin;
            retYPos = screenSize.cy - height - verticalMargin;
            break;

        case onBottomRight:
            retXPos = screenSize.cx - width - horizontalMargin;
            retYPos = screenSize.cy - height - verticalMargin;
            break;

        default:
            return;
    }
}

// Entry point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int iCmdShow)
{
    // Redirect stdout to the command prompt
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        errno_t err = freopen_s(&fp, "CONOUT$", "w", stdout);
        if (err != 0) {
            wchar_t buffer[1024];
            swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"Failed to redirect stdout to the console. (Code: 0x%08X)", err);
            MessageBoxW(NULL, buffer, L"Windows Notification Reposition - Error", MB_OK | MB_ICONERROR);
        }
    }

    // Process command line
    if (cmdLine != NULL) {
        // Convert the command line from ANSI to wide-char (UTF-16) for compatibility
        int cmdLineLength = lstrlenA(cmdLine);
        WCHAR* cmdLineW = (WCHAR*)malloc((cmdLineLength + 1) * sizeof(WCHAR));
        MultiByteToWideChar(CP_ACP, 0, cmdLine, -1, cmdLineW, cmdLineLength + 1);

        // Command-line: -help or /?
        if (!wcscmp(cmdLineW, L"-help") || !wcscmp(cmdLineW, L"/?")) {

            // Display help info
            DisplayHelpInfo();

            // Trigger exitting the application
            continueRunning = false;
        }
        else {
            // Get arguments from command-line
            int argc = 0;
            wchar_t** argv = CommandLineToArgvW(cmdLineW, &argc);

            // Parse each arguments
            for (int i = 0; i < argc; i++) {
                ParseArgument(argv[i]);
            }

            // Clean up
            LocalFree(argv);
            free(cmdLineW);
        }
    }

    // Get desktop screen size
    screenSize.cx = GetSystemMetrics(SM_CXSCREEN);
    screenSize.cy = GetSystemMetrics(SM_CYSCREEN);

    // Main loop
    while (continueRunning) {
        
        // Get Windows System Notifications
        HWND hNotifyWnd = ::FindWindow(L"Windows.UI.Core.CoreWindow", L"New notification");

        if (hNotifyWnd) {
            // Get the current position of the notification window
            RECT rcNotifyRect;
            GetWindowRect(hNotifyWnd, &rcNotifyRect);

            // Re-calculate notification window rectangle
            int newXPos = 0, newYPos = 0;
            CalculatePosition(rcNotifyRect, newXPos, newYPos);
            
            // Reposition notification window
            SetWindowPos(hNotifyWnd, NULL, newXPos, newYPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
        }

        // Wait for a blink
        Sleep(DEFAULT_WAIT_INTERVAL);
    }

    // Close the file stream
    if (fp != nullptr)
        fclose(fp);

    return 0;
}
