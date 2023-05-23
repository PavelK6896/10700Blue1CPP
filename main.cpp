#include <iostream>
#include <cmath>
#include <fstream>
#include <Windows.h>

#define ICO1                101
#define ID_TRAY_APP_ICON    1001
#define ID_TRAY_EXIT        1002
#define IDT_TIMER1          1003
#define ID_TRAY_BLUE_100        100100
#define ID_TRAY_BLUE_90        10090
#define ID_TRAY_BLUE_80        10080
#define ID_TRAY_BLUE_70        10070
#define ID_TRAY_BLUE_50        10050
#define ID_TRAY_BLUE_30        10030
#define ID_TRAY_BLUE_0        10000
#define WM_SYSICON          (WM_USER + 1)


UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Blue Light Setting");
char szClassName[] = "Blue Light Setting";

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void readFileConfig(const char *string);

int __stdcall WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    WNDCLASSEX wndclassex;
    WM_TASKBAR = RegisterWindowMessageA("Blue Light Created");
    wndclassex.hInstance = hThisInstance;
    wndclassex.lpszClassName = szClassName;
    wndclassex.lpfnWndProc = WindowProcedure;
    wndclassex.style = CS_DBLCLKS;
    wndclassex.cbSize = sizeof(WNDCLASSEX);
    wndclassex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wndclassex.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclassex.lpszMenuName = NULL;
    wndclassex.cbClsExtra = 0;
    wndclassex.cbWndExtra = 0;
    wndclassex.hbrBackground = (HBRUSH) (CreateSolidBrush(RGB(255, 255, 255)));
    if (!RegisterClassEx(&wndclassex)) return 0;
    Hwnd = CreateWindowEx(0, szClassName, szClassName,
                          WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 500,
                          300, HWND_DESKTOP, NULL, hThisInstance, NULL);
    if (!Hwnd) return 0;

    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = Hwnd;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_SYSICON;
    notifyIconData.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(32512));
    strncpy(notifyIconData.szTip, szTIP, sizeof(szTIP));
    Shell_NotifyIcon(NIM_ADD, &notifyIconData);

    MSG msg;
    readFileConfig("saveFileBlue50.txt");
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }

    return msg.wParam;
}

void saveFileCurrent(const char *fileName) {
    std::ofstream out(fileName);
    WORD GammaArray[3][256];
    HDC hGa = ::GetDC(NULL);

    if (GetDeviceGammaRamp(hGa, GammaArray)) {
        if (out.is_open()) {
            int sizeOne = sizeof(GammaArray) / sizeof(GammaArray[0]);
            int sizeTwo = sizeof(GammaArray[0]) / sizeof(GammaArray[0][0]);
            for (int i = 0; i < sizeOne; ++i) {
                for (int j = 0; j < sizeTwo; ++j) {
                    out << GammaArray[i][j] << " ";
                }
                out << std::endl;
            }
        }
        out.close();
    }
}

void readFileConfig(const char *fileName) {
    std::ifstream in(fileName);
    WORD GammaArray[3][256];
    HDC hGa = ::GetDC(NULL);
    int sizeOne = sizeof(GammaArray) / sizeof(GammaArray[0]);
    int sizeTwo = sizeof(GammaArray[0]) / sizeof(GammaArray[0][0]);

    if (in.is_open()) {
        int x;
        for (int i = 0; i < sizeOne; ++i) {
            for (int j = 0; j < sizeTwo; ++j) {
                in >> x;
                GammaArray[i][j] = x;
            }
        }

        ::SetDeviceGammaRamp(hGa, GammaArray);
        Sleep(100);
        ::ReleaseDC(NULL, hGa);
    } else {
    }
    in.close();

}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            Hmenu = CreatePopupMenu();
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_100, TEXT("blue-100"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_90, TEXT("blue-90"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_80, TEXT("blue-80"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_70, TEXT("blue-70"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_50, TEXT("blue-50"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_30, TEXT("blue-30"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_BLUE_0, TEXT("blue-0"));
//            AppendMenu(Hmenu, MF_STRING, 1011, TEXT("1011-0"));
            AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));
            break;

        case WM_SYSICON: {
            if (lParam == WM_RBUTTONDOWN) {
                POINT curPoint;
                GetCursorPos(&curPoint);
                SetForegroundWindow(hwnd);
                UINT clicked = TrackPopupMenu(Hmenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x,
                                              curPoint.y, 0, hwnd, NULL);
                SendMessage(hwnd, WM_NULL, 0, 0);

                switch (clicked) {
                    case ID_TRAY_EXIT:
                        DestroyWindow(hwnd);
                        break;
                    case ID_TRAY_BLUE_100:
                        readFileConfig("saveFileBlue100.txt");
                        break;
                    case ID_TRAY_BLUE_90:
                        readFileConfig("saveFileBlue90.txt");
                        break;
                    case ID_TRAY_BLUE_80:
                        readFileConfig("saveFileBlue80.txt");
                        break;
                    case ID_TRAY_BLUE_70:
                        readFileConfig("saveFileBlue70.txt");
                        break;
                    case ID_TRAY_BLUE_50:
                        readFileConfig("saveFileBlue50.txt");
                        break;
                    case ID_TRAY_BLUE_30:
                        readFileConfig("saveFileBlue30.txt");
                        break;
                    case ID_TRAY_BLUE_0:
                        readFileConfig("saveFileBlue0.txt");
                        break;
                    case 1011:
                        saveFileCurrent("saveFileBlue80.txt");
                        break;
                    default:
                        break;
                }
            }
        }
            break;
        case WM_DESTROY:
            KillTimer(hwnd, IDT_TIMER1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


