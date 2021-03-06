// GroupWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GroupWin.h"
#include <CommCtrl.h>	
#include <regex>
#include <shellapi.h>
#define MY_WM_NOTIFYICON WM_USER+1

#define MAX_LOADSTRING 100
#define IDT_TIMER1 50000
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
INT tCount = 2;
HFONT hFont = CreateFont(0, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
	TEXT("Courier New"));
RECT myRect;


//global pass
WCHAR currentPassword[25];
bool noPassword = false;
void openChangePass(HWND hWnd);
void onInput2(HWND hWnd);
void create(HWND hWnd);
bool systrayFlag = false;
INT_PTR CALLBACK changePass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK createPass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void			HideToSysTray(HWND hWnd);
void traThu(HWND hDlg, int tCount);
bool EnableAutoStartup();
bool DisableAutoStartup();


INT_PTR CALLBACK mainDlg(HWND, UINT, WPARAM, LPARAM);



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK mainDlg(HWND  , UINT , WPARAM , LPARAM );
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GROUPWIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GROUPWIN));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GROUPWIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GROUPWIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    switch (message)
    {
	case WM_CREATE:
		EnableAutoStartup();
		ZeroMemory(currentPassword, sizeof(WCHAR) * 25);
		if (noPassword)
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATEPASS), hWnd, createPass);
		create(hWnd);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




INT_PTR CALLBACK mainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char *lBuffer = new char[200];
	switch (message) {
	case WM_INITDIALOG:
		SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_MainPass), 1);/////Hàm tắt các nút hành động
		EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), FALSE);
		/*SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU HAI");
		SendMessage(GetDlgItem(hDlg, IDC_Thu), EM_GETRECT, 0, (DWORD)(LPRECT)&myRect);
		SendMessage(GetDlgItem(hDlg, IDC_Thu), EM_SETRECT, 0, (DWORD)(LPRECT)&myRect);*/
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
		{
			if (SendMessage(GetDlgItem(hDlg, IDC_MainPass), WM_GETTEXTLENGTH, 0, 0) != 0)/////Hàm bật lại nút hành động
				                                                                         /////nếu nhập pass
			{
				EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), TRUE);
			}
			else{ 
				EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), FALSE);
			}
			break;
		}
		}
		switch (LOWORD(wParam))
		{
		case IDC_MainMoKhoa:  ///BUTTON Mở Khóa
			break;
		case IDC_Maincheckbat:   ///RADIO BUTTON CHECK BẬT CHƯƠNG TRÌNH (cho phép chạy ngầm)
			{
				bool checked = IsDlgButtonChecked(hDlg, IDC_Maincheckbat);
				if (checked) {
					systrayFlag = true;
				}
				else {
					systrayFlag = false;
				}
			}
			break;
		case IDC_MainChangePass: ///BUTTON Đổi Mật Khẩu
			openChangePass(hDlg);
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_CHANGEPASS), hDlg, CHANGEPASSDlg);
			break;
		case IDC_MainHotro:  ///BUTTON Hỗ Trợ
			break;
		case IDC_Mainthongtin:  ///BUTTON Thông Tin
			onInput2(hDlg);
			break;
		case IDC_MainDUNG: 
			break;
		case IDC_MainPHUCHOIGIOIHAN: ///BUTTON PHỤC HỒI GIỚI  HẠN THỜI GIAN
			break;
		case IDC_MainPHUCHOITONG:  ///BUTTON PHỤC HỒI TỔNG THỜI GIAN
			break;
		case IDC_Maintangthu:  ///BUTTON DÙNG ĐỂ TĂNG THỨ 
			++tCount;
			if (tCount > 7) tCount = 1;
			traThu(hDlg, tCount);
			break;
		case IDC_Maingiamthu:   ///BUTTON DÙNG ĐỂ GIẢM THỨ 
			--tCount;
			if (tCount < 1) tCount = 7;
			traThu(hDlg, tCount);
			break;
		

			///CÁC RADIO BUTTON DÙNG ĐỂ TICK CÁC THỨ CẦN THAO TÁC
		case IDC_Mainthu2:
			break;
		case IDC_Mainthu3:
			break;
		case IDC_Mainthu4:
			break;
		case IDC_Mainthu5:
			break;
		case IDC_Mainthu6:
			break;
		case IDC_Mainthu7:
			break;
		case IDC_MainCN:
			break;
		///////////////////////////////////////////////////////////

			//CÁC CASE HÀNH ĐỘNG KHÓA MÁY, TẮT MÁY VÀ CHẾ ĐỘ SLEEP
		case IDC_Mainlock:
			break;
		case IDC_Mainshutdown:
			break;
		case IDC_Mainsleep:
			break;
		case IDC_MainClose:  ///BUTTON DONG
			{
				if (!systrayFlag) {
					PostQuitMessage(0);
				}
				else {
					HideToSysTray(hDlg);
				}
			}
			break;
		case IDC_Mainconfirm:   ///BUTTON XAC NHAN
			EndDialog(hDlg, IDCLOSE);
			break;
		} return (INT_PTR)TRUE;
	case WM_CLOSE:
		{
			if (!systrayFlag) {
				PostQuitMessage(0);
			}
			else {
				HideToSysTray(hDlg);
			}
		}
		break;
	case MY_WM_NOTIFYICON:
		{
			if (lParam == WM_LBUTTONDOWN)
			{
				ShowWindow(hDlg, SW_SHOW);
				ShowWindow(hDlg, SW_RESTORE);
			}
		} break;
	} return (INT_PTR)FALSE;

}


//////////////Các hàm khởi tạo Dialog
void create(HWND hWnd)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_main), hWnd, mainDlg);
}
void openChangePass(HWND hWnd) {
	INT_PTR returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_CHANGEPASS), hWnd, changePass);
	if (returnPtr != NULL) {
		// process data here !!!
	} return;
}


void onInput2(HWND hWnd) {
	//INT_PTR returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, ABOUTDlg);
	//if (returnPtr != NULL) {
	//	// process data here !!!
	//} return;
}
//////////////////////////////////


INT_PTR CALLBACK createPass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	WCHAR buffer1[100], buffer2[100];
	ZeroMemory(buffer1, sizeof(WCHAR) * 100);
	ZeroMemory(buffer2, sizeof(WCHAR) * 100);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hDlg, IDC_NEWPASS));
	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CREATEPASSCANCEL || LOWORD(wParam) == IDCANCEL)
		{
			PostQuitMessage(0);
		}
		if (LOWORD(wParam) == IDC_TOOLTIP) {
			LPWSTR rules = (LPWSTR)L"7-24 characters\nCase sensitive\nNo special characters";
			MessageBox(hDlg, rules, L"Rules", MB_OK);
		}
		if (LOWORD(wParam) == IDC_CREATEPASSOK) {
			GetDlgItemTextW(hDlg, IDC_NEWPASS, buffer1, 100);
			GetDlgItemTextW(hDlg, IDC_REENTERPASS, buffer2, 100);

			auto passwordRegex = std::regex(R"(^\w{7,24}$)");
			std::smatch result;
			std::wstring ws(buffer1);
			std::string str(ws.begin(), ws.end());
			if (!std::regex_match(str, result, passwordRegex)) {
				MessageBox(hDlg, L"Invalid password. Click the [?] button for more details.", L"Error", MB_OK);
			}
			else {
				bool flag = true;
				for (int i = 0; i < 25; ++i) {
					if (buffer1[i] != buffer2[i]) {
						MessageBox(hDlg, L"Password must be the same.", L"Error", MB_OK);
						flag = false;
						break;
					}
				}
				if (flag) {
					// luu du lieu + chuyen sang dialog giao dien
					for (int i = 0; i < 25; ++i) {
						currentPassword[i] = buffer1[i];
					}
					MessageBox(hDlg, L"Password created successfully.", L"Notice", MB_OK);
					noPassword = false;
					EndDialog(hDlg, LOWORD(wParam));
				}
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK changePass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	WCHAR buffer[100], buffer1[100], buffer2[100];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{

	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ChangePassCancel || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_TOOLTIP) {
			LPWSTR rules = (LPWSTR)L"7-24 characters\nCase sensitive\nNo special characters";
			MessageBox(hDlg, rules, L"Rules", MB_OK);
		}
		if (LOWORD(wParam) == IDC_ChangePassOK) {
			ZeroMemory(buffer, sizeof(WCHAR) * 100);
			ZeroMemory(buffer1, sizeof(WCHAR) * 100);
			ZeroMemory(buffer2, sizeof(WCHAR) * 100);
			GetDlgItemTextW(hDlg, IDC_ChangePassNew1, buffer1, 100);
			GetDlgItemTextW(hDlg, IDC_ChangePassNew2, buffer2, 100);
			GetDlgItemTextW(hDlg, IDC_ChangePassOld, buffer, 100);
			// Check xem password cu co dung khong
			bool correctPass = true;
			for (int i = 0; i < 25; ++i) {
				if (buffer[i] != currentPassword[i]) {
					MessageBox(hDlg, L"Wrong  password. Try again!", L"Error", MB_OK);
					correctPass = false;
					break;
				}
			}

			// dung pass + tao pass moi
			if (correctPass) {
				auto passwordRegex = std::regex(R"(^\w{7,24}$)");
				std::smatch result;
				std::wstring ws(buffer1);
				std::string str(ws.begin(), ws.end());
				if (!std::regex_match(str, result, passwordRegex)) {
					MessageBox(hDlg, L"Invalid password. Click the [?] button for more details.", L"Error", MB_OK);
				}
				else {
					bool flag = true;
					for (int i = 0; i < 25; ++i) {
						if (buffer1[i] != buffer2[i]) {
							MessageBox(hDlg, L"Password must be the same.", L"Error", MB_OK);
							flag = false;
							break;
						}
					}
					if (flag) {
						// luu du lieu + chuyen sang dialog giao dien
						for (int i = 0; i < 25; ++i) {
							currentPassword[i] = buffer1[i];
						}
						MessageBox(hDlg, L"Password changed successfully.", L"Notice", MB_OK);
						EndDialog(hDlg, LOWORD(wParam));
					}
				}
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void HideToSysTray(HWND hWnd)
{
	ShowWindow(hWnd, SW_HIDE);

	HICON myAppIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GROUPWIN));

	NOTIFYICONDATA tnid;
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = IDI_GROUPWIN;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnid.hIcon = myAppIcon;
	tnid.uCallbackMessage = MY_WM_NOTIFYICON;
	lstrcpyn(tnid.szTip, L"Ten chuong trinh", sizeof(tnid.szTip));

	Shell_NotifyIcon(NIM_ADD, &tnid);

	if (myAppIcon)
		DestroyIcon(myAppIcon);
}

// TRA VE THU
void traThu(HWND hDlg, int tCount) {
	switch (tCount) { 
	case 1: {
		
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETFONT, (WPARAM)hFont, MAKELPARAM(true, 0));
		
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"CHU NHAT");
	} break;
	case 2: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU HAI");
	} break;
	case 3: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU BA");
	} break;
	case 4: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU TU");
	} break;
	case 5: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU NAM");
	} break;
	case 6: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU SAU");
	} break;
	case 7: {
		SendMessage(GetDlgItem(hDlg, IDC_Thu), WM_SETTEXT, 0, (LPARAM)L"THU BAY");
	} break;
	}
}

bool EnableAutoStartup() {
	HKEY hOpened;
	char path[500];
	GetModuleFileNameA(0, path, 500);
	LPBYTE pPath = (LPBYTE)path;
	std::wstring ws(szTitle);	std::string AppName(ws.begin(), ws.end());
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_ALL_ACCESS, &hOpened);
	if (RegSetValueExA(hOpened, AppName.c_str(), 0, REG_SZ,
		pPath, sizeof(path)) != ERROR_SUCCESS)
		return false;
	RegCloseKey(hOpened);
	return true;
}

bool DisableAutoStartup() {
	HKEY hOpened;
	char path[500];
	GetModuleFileNameA(0, path, 500);
	std::wstring ws(szTitle);	std::string AppName(ws.begin(), ws.end());
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_ALL_ACCESS, &hOpened);
	long lRet = RegDeleteValueA(hOpened, AppName.c_str());
	/*if (RegDeleteKeyExW(hOpened, szTitle, KEY_WOW64_64KEY, 0) != ERROR_SUCCESS)
	return false;*/
	RegCloseKey(hOpened);
	return true;
}
