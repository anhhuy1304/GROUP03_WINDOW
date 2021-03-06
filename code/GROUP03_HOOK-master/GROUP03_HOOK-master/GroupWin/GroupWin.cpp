// GroupWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GroupWin.h"
#include <CommCtrl.h>	
#include <regex>
#include <fstream>
#include<shellapi.h>
using namespace std;
#define MAX_LOADSTRING 100
#define MY_WM_NOTIFYICON WM_USER+1
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
/////////////////////////////////////////


//global pass
HFONT font;
WCHAR currentPassword[25];
bool noPassword ;
bool systrayFlag = false;
settingApp settingFile;
dataInDay dataday;
dataInWeek dataweek[7];
int action;
bool timeNoti = false;
int hour;
int startHour;
int startMin;
int EndHour;
int EndMin;
int minu;
int day; 
bool autorun;
HMODULE  hinstLib = NULL;
bool flagtimeout;

//func
void openChangePass(HWND hWnd);
void openDialogABOUT(HWND hWnd);
void openDialogHoTro(HWND hWnd);
void openDialogCaiDat(HWND hWnd);
void create(HWND hWnd);
void enableMain(HWND hDlg);
void checkEnableMain(HWND hDlg);
void afterMoKhoa(HWND hDlg);
void  readSettingFile(settingApp &result);
void readDataInDayFile(dataInDay &result);
void readDataInWeekFile(dataInWeek result[]);
void writeSettingFile(settingApp result);
void writeDataInDayFile(dataInDay result);
void writeDataInWeekFile(dataInWeek result[]);
void createSomeFile(); // test
void onCreateApp(HWND hWnd);
void updateMainbeforesave(HWND hDlg);
void onTimer(HWND hDlg);
void updateTimeEvery1Minute(HWND hDlg);
void updateTimeWhenTurnOn(HWND hDlg);
bool checkTimeLimit();
void onInitSettingDialog(HWND hDlg, dataInWeek tmp[]);
void displaysetting(HWND hDlg, dataInWeek tmp[], int disday);
void displayInDialogMain(HWND hDlg);
void displayUseTime(HWND hDlg);
typedef void(*MYPROC)();
void removeHook(HWND hWnd);
void installHook(HWND hWnd);
void HideToSysTray(HWND hWnd);
void onMainDung(HWND hWnd);
void onMainTiepTuc(HWND hWnd);
void onPhucHoiTong(HWND hWnd);
bool EnableAutoStartup();
bool DisableAutoStartup();
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
	{
		char path[500];
		GetModuleFileNameA(0, path, 500);
		std::string sPath(path);
		auto pathRegex = std::regex(R"(^((?:[A-Z]:\\)(?:[A-Za-z0-9\s\-_]+\\)*)(?:[A-Za-z0-9\s\-_]+\.exe))");
		std::smatch result;

		std::regex_match(sPath, result, pathRegex);
		std::string dir = result[1];

		SetCurrentDirectoryA(dir.c_str());
		//createSomeFile();

		onCreateApp(hWnd);
	}
		break;
	case MY_WM_NOTIFYICON:
	{
		if (lParam == WM_LBUTTONDOWN)
		{
			ShowWindow(hWnd, SW_SHOW);
			ShowWindow(hWnd, SW_RESTORE);
		}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            //Parse the menu selections:
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
		if (systrayFlag == true) HideToSysTray(hWnd);
		else {
			PostQuitMessage(0);
			DeleteObject(font);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




INT_PTR CALLBACK mainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
		if (dataweek[dataday.day].enable == true) {
		updateTimeWhenTurnOn(hDlg);
		updateTimeEvery1Minute(hDlg);
		}
		enableMain(hDlg);
		dataday.totalTime = dataweek[dataday.day].hours * 3600 + dataweek[dataday.day].minutes * 60;
		displayInDialogMain(hDlg);
		displayUseTime(hDlg);
		break;
	case WM_TIMER:
		onTimer(hDlg);

		break;
	case MY_WM_NOTIFYICON:
	{
		if (lParam == WM_LBUTTONDOWN)
		{
			ShowWindow(hDlg, SW_SHOW);
			ShowWindow(hDlg, SW_RESTORE);
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_MainMoKhoa: ///BUTTON Mở Khóa
			checkEnableMain(hDlg);
			//INFO_HANHDONG* result = new INFO_HANHDONG;

			//
			////GetDlgItemText(hDlg, IDC_EDIT1, result->hoten, 200);

			//if (SendMessage(GetDlgItem(hDlg, IDC_Mainshutdown), BM_GETCHECK, 0, 0) == BST_CHECKED)
			//{
			//	result->hanhdong = 1;
			//}

			//return (INT_PTR)TRUE;
			break;
		case IDC_MainKhoa:    ///BUTTON Khoá

			afterMoKhoa(hDlg);
			break;
		case IDC_Maincheckbat:///RADIO BUTTON CHECK BẬT CHƯƠNG TRÌNH (cho phép chạy ngầm)
			systrayFlag = systrayFlag ? false : true;
			break;
		case IDC_AUTORUN:
			autorun = autorun ? false : true;
			if (autorun) {
				EnableAutoStartup();
			}
			else DisableAutoStartup();
			break;
		case IDC_MainChangePass: ///BUTTON Đổi Mật Khẩu
			openChangePass(hDlg);
			break;
		case IDC_MainHotro:  ///BUTTON Hỗ Trợ
			openDialogHoTro(hDlg);
			break;
		case IDC_Mainthongtin:  ///BUTTON Thông Tin
			openDialogABOUT(hDlg);
			break;
		case IDC_MainSETTING:   ///BUTTON Cài Đặt
			openDialogCaiDat(hDlg);
			displayInDialogMain(hDlg);
			displayUseTime(hDlg);
			break;
		case IDC_MainDUNG: 
			onMainDung(hDlg);
			break;
		case IDC_MainTiepTuc: ///BUTTON PHỤC HỒI GIỚI  HẠN THỜI GIAN
			onMainTiepTuc(hDlg);
			break;
		case IDC_MainPHUCHOITONG:  ///BUTTONT PHỤC HỒI TỔNG THỜI GIAN
			onPhucHoiTong(hDlg);
			displayUseTime(hDlg);
			onMainDung(hDlg);
			onMainTiepTuc(hDlg);
			break;
		case IDC_Maintangthu:  ///BUTTON DÙNG ĐỂ TĂNG THỨ 
			break;
		case IDC_Maingiamthu:   ///BUTTON DÙNG ĐỂ GIẢM THỨ 
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
		//case IDC_Mainshutdown:
			//break;
		case IDC_Mainsleep:
			break;
		case IDC_MainClose:  ///BUTTON DONG
			if (systrayFlag == true) HideToSysTray(hDlg);
			else {
				EndDialog(hDlg, IDCLOSE);
				PostQuitMessage(0);
			}
			break;
		case IDC_Mainconfirm:   ///BUTTON XAC NHAN
			updateMainbeforesave(hDlg);			// cap nhat lai main

			writeDataInDayFile(dataday);
			writeDataInWeekFile(dataweek);
			writeSettingFile(settingFile);
			if (systrayFlag == true) {
				flagtimeout = false;
				HideToSysTray(hDlg);
				updateTimeWhenTurnOn(hDlg);
				updateTimeEvery1Minute(hDlg);
				afterMoKhoa(hDlg);


				int timeRemain = dataday.totalTime - dataday.timeUse;
				/*if (!((timeRemain <= 0 || checkTimeLimit() == false)))*/
			}
			else {
				EndDialog(hDlg, IDCLOSE);
				PostQuitMessage(0);
			}
			break;
		} return (INT_PTR)TRUE;
	case WM_CLOSE:
		if (systrayFlag == true) HideToSysTray(hDlg);
		else {
			EndDialog(hDlg, NULL);
			KillTimer(hDlg, IDT_TIMER3);
			KillTimer(hDlg, IDT_TIMER2);
			PostQuitMessage(0);
		}
	} return (INT_PTR)FALSE;
}




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
					// luu du lieu + chuyen sang dialog giao dien + ghi vao file
					settingApp setting;
					for (int i = 0; i < 25; ++i) {
						setting.password[i] = buffer1[i];
						currentPassword[i] = buffer1[i];
						settingFile.password[i] = buffer1[i];
					}

					setting.noPassword = false;
					setting.systrayFlag = false;
					setting.action = 1;
					setting.autorun = false;


					settingFile.action = 1;
					settingFile.systrayFlag = false;
					settingFile.noPassword = false;
					settingFile.autorun = false;


					writeSettingFile(setting);
					MessageBox(hDlg, L"Password created successfully.", L"Notice", MB_OK);
					noPassword = false;
					autorun = false;
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
						settingApp setting;
						setting.noPassword = false;
						setting.systrayFlag = systrayFlag;
						for (int i = 0; i < 25; ++i) {
							setting.password[i] = buffer1[i];
							currentPassword[i] = buffer1[i];
							settingFile.password[i] = buffer1[i];
						}
						setting.action = action;
						setting.autorun = autorun;
						writeSettingFile(setting);
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




INT_PTR CALLBACK ABOUTDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, NULL);
			break;
		case IDCANCEL:
			EndDialog(hDlg, NULL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		break;
	}
	return (INT_PTR)FALSE;
}



INT_PTR CALLBACK HoTroDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, NULL);
			break;
		case IDCANCEL:
			EndDialog(hDlg, NULL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK TimeoutDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int num;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CREATE:

		break;
	case WM_INITDIALOG:
		SetWindowPos(hDlg, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetFocus(GetDlgItem(hDlg, IDC_Timeoutpass));
		num = 0;
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Timeoutok:
		if (num < 5) {
			WCHAR buffer[100];
			ZeroMemory(buffer, sizeof(WCHAR) * 100);
			GetDlgItemTextW(hDlg, IDC_Timeoutpass, buffer, 100);
			bool check = false;
			for (int i = 0; i < 25; i++) {
				if (buffer[i] != currentPassword[i]) {
					num++;
					check = true;
					MessageBox(hDlg, L"Sai mật khẩu (nhập sai quá 5 lần sẽ tự khóa)", L"Lỗi", MB_OK);
					break;
				}
			}
			if (check == false) {
				EndDialog(hDlg, NULL);
				removeHook(hDlg);
				break;
			}
			break;
		}

		case IDC_Timeoutshutdown:
			WinExec("shutdown -s -t 5", 1);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK CaiDatDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static dataInWeek tmp[7];
	static int daynow;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		//set default
		daynow = dataday.day;
		for (int i = 0; i < 7; i++)
			tmp[i] = dataweek[i];
		displaysetting(hDlg, tmp, dataday.day);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CaiDattangthu:///BUTTON TĂNG THỨ
			daynow = (++daynow) > 6 ? 0 : daynow;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatgiamthu:///BUTTON GIẢM THỨ
			daynow = (--daynow) < 0 ? 6 : daynow;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDattangGio:///BUTTON TĂNG GIỜ
			tmp[daynow].hours=(++tmp[daynow].hours) > 23 ? 0 : tmp[daynow].hours;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatgiamGio:  ///BUTTON GIẢM GIỜ
			tmp[daynow].hours= (--tmp[daynow].hours) < 0 ? 23 : tmp[daynow].hours;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDattangPhut:   ///BUTTON TĂNG PHÚT
			tmp[daynow].minutes=(++tmp[daynow].minutes) > 59 ? 0 : tmp[daynow].minutes;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatgiamPhut:  ///BUTTON GIẢM PHÚT
			tmp[daynow].minutes=(--tmp[daynow].minutes) < 0 ? 59 : tmp[daynow].minutes;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatStartTangGio:  ///BUTTON Start TĂNG GIỜ
			tmp[daynow].start.tm_hour=(++tmp[daynow].start.tm_hour) > 23 ? 0 : tmp[daynow].start.tm_hour;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatStartGiamGio:///BUTTON Start GIẢM GIỜ
			tmp[daynow].start.tm_hour=(--tmp[daynow].start.tm_hour) < 0 ? 23 : tmp[daynow].start.tm_hour;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatStartTangPhut: //BUTTON Start TĂNG PHÚT
			tmp[daynow].start.tm_min=(++tmp[daynow].start.tm_min) > 59 ? 0 : tmp[daynow].start.tm_min;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatStartGiamPhut: ///BUTTON Start GIẢM PHÚT
			tmp[daynow].start.tm_min=(--tmp[daynow].start.tm_min) < 0 ? 59 : tmp[daynow].start.tm_min;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatEndTangGio:    ///BUTTON End TĂNG GIỜ
			tmp[daynow].end.tm_hour=(++tmp[daynow].end.tm_hour) > 23 ? 0 : tmp[daynow].end.tm_hour;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatEndGiamGio:   ///BUTTON End GIẢM GIỜ
			tmp[daynow].end.tm_hour=(--tmp[daynow].end.tm_hour) < 0 ? 23 : tmp[daynow].end.tm_hour;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatEndTangPhut:   ///BUTTON End TĂNG PHÚT
			tmp[daynow].end.tm_min=(++tmp[daynow].end.tm_min) > 59 ? 0 : tmp[daynow].end.tm_min;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_CaiDatEndGiamPhut:   ///BUTTON End GIẢM PHÚT
			tmp[daynow].end.tm_min=(--tmp[daynow].end.tm_min) < 0 ? 59 : tmp[daynow].end.tm_min;
			displaysetting(hDlg, tmp, daynow);
			break;
		case IDC_SETTINGCANCLE:
			EndDialog(hDlg, NULL);
			break;
		case IDC_SETTINGOK: {
			//luu lai vao file va update
			for (int i = 0; i < 7; i++) {
				dataweek[i] = tmp[i];

			}
			dataday.totalTime = dataweek[dataday.day].hours * 3600 + dataweek[dataday.day].minutes * 60;
			writeDataInWeekFile(dataweek);
			writeDataInDayFile(dataday);
			EndDialog(hDlg, NULL);
			break;
		}
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		break;
	}
	return (INT_PTR)FALSE;
}





//////////////Các hàm tạo dialog, enable các button trong main Dialog

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


void openDialogABOUT(HWND hWnd) {
	INT_PTR returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, ABOUTDlg);
	if (returnPtr != NULL) {
		// process data here !!!
	} return;
}


void openDialogHoTro(HWND hWnd)
{
	INT_PTR returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_HoTro), hWnd, HoTroDlg);
	if (returnPtr != NULL) {
		// process data here !!!
	} return;
}


void openDialogCaiDat(HWND hWnd)
{
	INT_PTR returnPtr = DialogBox(hInst, MAKEINTRESOURCE(IDD_CaiDat), hWnd, CaiDatDlg);
	if (returnPtr != NULL) {
		// process data here !!!
	} return;
}









void enableMain(HWND hDlg)
{
	CheckRadioButton(hDlg, 1052, 1054, 1053); // check shutdown is default tam

	//check day in week
	for (int i = 0; i <=6; i++) {
		if (dataweek[i].enable == true)
			SendMessage(GetDlgItem(hDlg, 20000 + i), BM_SETCHECK, BST_CHECKED, 0);
	}
	
	if (settingFile.systrayFlag == true)
		SendMessage(GetDlgItem(hDlg, IDC_Maincheckbat), BM_SETCHECK, BST_CHECKED, 0);
	else SendMessage(GetDlgItem(hDlg, IDC_Maincheckbat), BM_SETCHECK, BST_UNCHECKED, 0);

	if(settingFile.autorun == true)	
		SendMessage(GetDlgItem(hDlg, IDC_AUTORUN), BM_SETCHECK, BST_CHECKED, 0);
	else SendMessage(GetDlgItem(hDlg, IDC_AUTORUN), BM_SETCHECK, BST_UNCHECKED, 0);


	SendMessage(GetDlgItem(hDlg, settingFile.action + 20006), BM_SETCHECK, BST_CHECKED, 0);

	SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_MainPass), 1);/////Hàm tắt các nút hành động
	EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainHotro), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maincheckbat), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maintangthu), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maingiamthu), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK5), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK7), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK8), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainlock), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainshutdown), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainsleep), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainSETTING), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainKhoa), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_AUTORUN), FALSE);
	
}


void checkEnableMain(HWND hDlg)
{
	bool checkPass = true;
	WCHAR tmp[100];
	ZeroMemory(tmp, sizeof(WCHAR) * 100);
	GetDlgItemText(hDlg, IDC_MainPass, tmp, 100);
	for (int i = 0; i < 25; ++i) {
		if (tmp[i] != currentPassword[i]) {
			checkPass = false;
			break;
		}
	};
	if (checkPass)/////Hàm bật lại nút hành động
																				 /////nếu nhập pass
	{
		EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainHotro), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maintangthu), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maingiamthu), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK5), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK7), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK8), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainlock), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainshutdown), TRUE); 
		EnableWindow(GetDlgItem(hDlg, IDC_Mainsleep), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainSETTING),TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maincheckbat), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_AUTORUN), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPass), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainKhoa), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainTiepTuc), FALSE);
	}
	else {
		EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainHotro), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maintangthu), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maingiamthu), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK5), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_AUTORUN), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK7), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK8), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainlock), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainshutdown), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Mainsleep), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainSETTING), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_Maincheckbat), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainKhoa), FALSE);
		MessageBox(hDlg, L"Wrong password!", L"Notice", MB_OK);
	}
}


void afterMoKhoa(HWND hDlg) {
	settingFile.systrayFlag = systrayFlag;
	writeDataInDayFile(dataday);
	writeDataInWeekFile(dataweek);
	writeSettingFile(settingFile);
	SetDlgItemText(hDlg, IDC_MainPass, L""); // set lai khung edit password la chuoi rong
	EnableWindow(GetDlgItem(hDlg, IDC_MainChangePass), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainHotro), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOIGIOIHAN), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainPHUCHOITONG), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainconfirm), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maincheckbat), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maintangthu), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Maingiamthu), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK5), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK7), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_CHECK8), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainlock), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainshutdown), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_Mainsleep), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainSETTING), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainKhoa), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainPass), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), TRUE);

	EnableWindow(GetDlgItem(hDlg, IDC_AUTORUN), FALSE);
}




void onCreateApp(HWND hWnd) {
	readSettingFile(settingFile);
	readDataInDayFile(dataday);
	readDataInWeekFile(dataweek);
	noPassword = settingFile.noPassword; //copy no pass
	if (noPassword)
		DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATEPASS), hWnd, createPass);
	else {
		ZeroMemory(currentPassword, sizeof(WCHAR) * 25);
		for (int i = 0; i < 25; i++) { // copy password
			currentPassword[i] = settingFile.password[i];
		}
		systrayFlag = settingFile.systrayFlag;
		action = settingFile.action;
		autorun = settingFile.autorun;
	}

	create(hWnd);


	
}

// xu ly file
void readSettingFile(settingApp &result) {
	fstream f;
	f.open("settingfile.bin", ios::in | ios::binary);
	if (f.is_open()) {
		f.read((char*)&result.noPassword, sizeof(result.noPassword));
		f.read((char*)&result.password, sizeof(WCHAR) * 25);
		f.read((char*)&result.systrayFlag, sizeof(BOOL));
		f.read((char*)&result.action, sizeof(int));
		f.read((char*)&result.autorun, sizeof(bool));
		f.close();
	}
}


void readDataInDayFile(dataInDay &result) {
	fstream f;
	f.open("dataInDayFile.bin", ios::in | ios::binary);
	if (f.is_open()) {
		f.read((char*)&result.day, sizeof(int));
		f.read((char*)&result.timeUse, sizeof(int));
		f.read((char*)&result.totalTime, sizeof(int));
		f.close();
	}
}


void readDataInWeekFile(dataInWeek result[]) {
	fstream f;
	f.open("dataInWeekFile.bin", ios::in | ios::binary);
	if (f.is_open()) {
		for (int i = 0; i < 7; i++) {
			f.read((char*)&result[i].enable, sizeof(bool));
			f.read((char*)&result[i].day, sizeof(int));
			f.read((char*)&result[i].end, sizeof(tm));
			f.read((char*)&result[i].hours, sizeof(int));
			f.read((char*)&result[i].minutes, sizeof(int));
			f.read((char*)&result[i].start, sizeof(tm));
		}
	}
}


void writeSettingFile(settingApp result) {
	fstream f;
	f.open("settingfile.bin", ios::out | ios::binary);
	if (f.is_open()) {
		f.write((char*)&result.noPassword, sizeof(result.noPassword));
		f.write((char*)&result.password, sizeof(WCHAR) * 25);
		f.write((char*)&result.systrayFlag, sizeof(BOOL));
		f.write((char*)&result.action, sizeof(int));
		f.write((char*)&result.autorun, sizeof(bool));
		
		f.close();
	}
}


void writeDataInDayFile(dataInDay result) {
	fstream f;
	f.open("dataInDayFile.bin", ios::out | ios::binary);
	if (f.is_open()) {
		f.write((char*)&result.day, sizeof(int));
		f.write((char*)&result.timeUse, sizeof(int));
		f.write((char*)&result.totalTime, sizeof(int));
		f.close();
	}
}


void writeDataInWeekFile(dataInWeek result[]) {
	fstream f;
	f.open("dataInWeekFile.bin", ios::out | ios::binary);
	if (f.is_open()) {
		for (int i = 0; i < 7; i++) {
			f.write((char*)&result[i].enable, sizeof(bool));
			f.write((char*)&result[i].day, sizeof(int));
			f.write((char*)&result[i].end, sizeof(tm));
			f.write((char*)&result[i].hours, sizeof(int));
			f.write((char*)&result[i].minutes, sizeof(int));
			f.write((char*)&result[i].start, sizeof(tm));
		}
	}
}


void createSomeFile() { // prepare for app
	fstream f; // create setting file
	f.open("settingfile.bin", ios::out | ios::binary);
	settingApp result;
	result.noPassword = true;
	WCHAR anhhuy[25] = L"";
	for (int i = 0; i < 25; i++)
		result.password[i] = anhhuy[i];
	result.systrayFlag = false;
	result.action = 1;
	result.autorun = false;
	f.write((char*)&result.noPassword, sizeof(bool));
	f.write((char*)&result.password, sizeof(WCHAR) * 25);
	f.write((char*)&result.systrayFlag, sizeof(bool));
	f.write((char*)&result.action, sizeof(int));
	f.write((char*)&result.autorun, sizeof(bool));
	f.close();


	//create datatinday file
	f.open("dataInDayFile.bin", ios::out | ios::binary);
	dataInDay datainday;
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	datainday.day = now.tm_wday;
	datainday.timeUse = 0;
	datainday.totalTime = 24 * 60 * 60;
	f.write((char*)&datainday.day, sizeof(int));
	f.write((char*)&datainday.timeUse, sizeof(int));
	f.write((char*)&datainday.totalTime, sizeof(int));
	f.close();

	//create datainweek file
	f.open("dataInWeekFile.bin", ios::out | ios::binary);
	dataInWeek dataweek[7];
	tm start;
	start.tm_hour = 10;
	start.tm_min = 0;
	start.tm_sec = 0;
	tm end;
	end.tm_hour = 11;
	end.tm_min = 30;
	end.tm_sec = 0;
	for (int i = 0; i < 7; i++) {
		dataweek[i].enable = false;
		dataweek[i].day = i;
		dataweek[i].end = end;
		dataweek[i].hours = 0;
		dataweek[i].minutes = 2;
		dataweek[i].start = start;
		f.write((char*)&dataweek[i].enable, sizeof(bool));
		f.write((char*)&dataweek[i].day, sizeof(int));
		f.write((char*)&dataweek[i].end, sizeof(tm));
		f.write((char*)&dataweek[i].hours, sizeof(int));
		f.write((char*)&dataweek[i].minutes, sizeof(int));
		f.write((char*)&dataweek[i].start, sizeof(tm));
	}
	f.close();
}


void updateMainbeforesave(HWND hDlg) { // cap nhat lai enable trong week truoc khi luu
	LRESULT checkenable[7];
	for (int i = 0; i < 7; i++) {
		checkenable[i] = SendMessage(GetDlgItem(hDlg, 20000 + i), BM_GETCHECK, 0, 0);
		if (checkenable[i] == BST_CHECKED) dataweek[i].enable = true;
		if (checkenable[i] == BST_UNCHECKED) dataweek[i].enable = false;
	}
	settingFile.systrayFlag = systrayFlag;
	LRESULT checkAction[3];
	for (int i = 0; i < 3; i++) {
		checkAction[i] = SendMessage(GetDlgItem(hDlg, 20007 + i), BM_GETCHECK, 0, 0);
		if (checkAction[i] == BST_CHECKED) settingFile.action = i + 1;
	}
	LRESULT checkautorun = SendMessage(GetDlgItem(hDlg, IDC_AUTORUN), BM_GETCHECK, 0, 0);
	if (checkautorun == BST_CHECKED) settingFile.autorun = true;
	if (checkautorun == BST_UNCHECKED) settingFile.autorun = false;
}




// xu ly thoi gian
void updateTimeWhenTurnOn(HWND hDlg) {
	time_t t = time(0);
	tm now;
	localtime_s(&now,&t); // get time now
	if (now.tm_wday != dataday.day) {
		dataday.day = now.tm_wday;
		dataday.totalTime = dataweek[dataday.day].hours * 3600 + dataweek[dataday.day].minutes * 60;
		dataday.timeUse = 0;
		writeDataInDayFile(dataday);
	}
	int timeRemain = dataday.totalTime - dataday.timeUse;
	if ((timeRemain <= 0 || checkTimeLimit() == false) && flagtimeout == false && systrayFlag == true && dataweek[dataday.day].enable == true) {
		flagtimeout = true;
		ShowWindow(hDlg, SW_SHOW);
		SetForegroundWindow(hDlg);
		SetActiveWindow(hDlg);
		SetFocus(hDlg);
		ShowWindow(hDlg, SW_RESTORE);
		//SetForegroundWindow(hDlg);
		installHook(hDlg);
		KillTimer(hDlg, IDT_TIMER2);
		KillTimer(hDlg, IDT_TIMER3);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_TimeOut), hDlg, TimeoutDialog);		
		//install hook + open dialog 
		
	}
	else if ( systrayFlag == true && dataweek[dataday.day].enable == true) {
		int timeNotifi = 60 * 10 * 1000; //10p nhac nho
		timeRemain = timeRemain * 1000;
		int timer;
		timer = timeRemain - timeNotifi;
		if (timeRemain < timeNotifi) {
			timeNoti = true;
			//open hop thoai nhac nho
			WCHAR str[50];
			int phut = timeRemain / 1000 / 60;
			wsprintf(str, L"Còn %d phút", phut );
			MessageBox(hDlg, str, L"Nhắc nhở", MB_OK);
		}
	}

}

void updateTimeEvery1Minute(HWND hDlg) {
	SetTimer(hDlg, IDT_TIMER3, 60000, NULL);
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t); // get time now
	int minutesLimit = 0;
	int Hours = (dataweek[dataday.day].end.tm_hour - now.tm_hour);
	if (Hours == 0)  minutesLimit = dataweek[dataday.day].end.tm_min - now.tm_min;
	else minutesLimit = 60 - now.tm_min + (Hours - 1) * 60 + dataweek[dataday.day].end.tm_min;
	UINT timeLimit = minutesLimit * 60 * 1000;
	if(timeLimit> 0)
	SetTimer(hDlg, IDT_TIMER2, timeLimit, NULL);


}

void onTimer(HWND hDlg) {
	dataday.timeUse += 60;
	writeDataInDayFile(dataday);
	displayUseTime(hDlg);
	int timeRemain = dataday.totalTime - dataday.timeUse;
	if ((timeRemain <= 0 || checkTimeLimit() == false)&&  flagtimeout == false && systrayFlag == true && dataweek[dataday.day].enable == true) {
		flagtimeout = true;
		ShowWindow(hDlg, SW_SHOW);
		SetForegroundWindow(hDlg);
		SetActiveWindow(hDlg);
		SetFocus(hDlg);
		ShowWindow(hDlg, SW_RESTORE);
		//SetForegroundWindow(hDlg);
		installHook(hDlg);
		KillTimer(hDlg, IDT_TIMER2);
		KillTimer(hDlg, IDT_TIMER3);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_TimeOut), hDlg, TimeoutDialog);		
		// neu het gio hoac ko phai thoi gian
		//install hook
	}
	else  if (systrayFlag == true && dataweek[dataday.day].enable == true) {
		int timeNotifi = 60 * 10 * 1000;
		int timer = timeRemain - timeNotifi;
		if (timeRemain < timeNotifi && timeNoti == false) {
			timeNoti = true;
			//open hop thoai nhac nho
			WCHAR str[50];
			int phut = timeRemain / 1000 / 60;
			wsprintf(str, L"Còn %d phút", phut);
			MessageBox(hDlg, str, L"Nhắc nhở", MB_OK);
		}
	}

}

bool checkTimeLimit() {
	time_t t = time(0);
	tm now;
	localtime_s(&now,&t); // get time now
	int day = now.tm_wday; // get week day

	// check limit
	if (now.tm_hour > dataweek[day].end.tm_hour || now.tm_hour < dataweek[day].start.tm_hour) return false;
	if (now.tm_hour == dataweek[day].end.tm_hour)
		if (now.tm_min > dataweek[day].end.tm_min) return false;
	if (now.tm_hour == dataweek[day].start.tm_hour)
		if (now.tm_min < dataweek[day].start.tm_min) return false;
	return true;
}

void onInitSettingDialog(HWND hDlg, dataInWeek tmp[])
{

	tmp = dataweek;
	day = dataday.day;
	hour		 = dataweek[day].hours;
	minu  = dataweek[day].minutes;
	startHour	 = dataweek[day].start.tm_hour;
	startMin	 = dataweek[day].start.tm_min;
	EndHour		 = dataweek[day].end.tm_hour;
	EndMin		 = dataweek[day].end.tm_min;

}

void displaysetting(HWND hDlg, dataInWeek tmp[], int disday) {

	WCHAR Sun[] = L"Chủ Nhật";
	WCHAR Mon[] = L"Hai";
	WCHAR Tue[] = L"Ba";
	WCHAR Web[] = L"Tư";
	WCHAR Thu[] = L"Năm";
	WCHAR Fri[] = L"Sáu";
	WCHAR Sat[] = L"Bảy";

	vector<WCHAR*> stringday;
	stringday.push_back(Sun);
	stringday.push_back(Mon);
	stringday.push_back(Tue);
	stringday.push_back(Web);
	stringday.push_back(Thu);
	stringday.push_back(Fri);
	stringday.push_back(Sat);

	WCHAR strhour[5];
	WCHAR strmin[5];
	WCHAR strstart[10];
	WCHAR strend[10];


	wsprintf(strhour, L"%02d", tmp[disday].hours);
	wsprintf(strmin, L"%02d", tmp[disday].minutes);
	wsprintf(strstart, L"%02d:%02d", tmp[disday].start.tm_hour, tmp[disday].start.tm_min);
	wsprintf(strend, L"%02d:%02d", tmp[disday].end.tm_hour, tmp[disday].end.tm_min);

	//set text
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGDAY), WM_SETTEXT, 0, (LPARAM)stringday[disday]);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGHOUR), WM_SETTEXT, 0, (LPARAM)strhour);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGSTART), WM_SETTEXT, 0, (LPARAM)strstart);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGEND), WM_SETTEXT, 0, (LPARAM)strend);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGMIN), WM_SETTEXT, 0, (LPARAM)strmin);
	//set font
	font = CreateFont(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Times New Roman");
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGDAY), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGHOUR), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGSTART), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGEND), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGMIN), WM_SETFONT, (WPARAM)font, 0);
}




void displayInDialogMain(HWND hDlg) {
	
	WCHAR Sun[] = L"Chủ Nhật";
	WCHAR Mon[] = L"Hai";
	WCHAR Tue[] = L"Ba";
	WCHAR Web[] = L"Tư";
	WCHAR Thu[] = L"Năm";
	WCHAR Fri[] = L"Sáu";
	WCHAR Sat[] = L"Bảy";

	vector<WCHAR*> stringday;
	stringday.push_back(Sun);
	stringday.push_back(Mon);
	stringday.push_back(Tue);
	stringday.push_back(Web);
	stringday.push_back(Thu);
	stringday.push_back(Fri);
	stringday.push_back(Sat);

	WCHAR strhour[20];
	WCHAR strmin[20];
	WCHAR strstart[20];
	WCHAR strend[20];


	wsprintf(strhour, L"%02d", dataweek[dataday.day].hours);
	wsprintf(strmin, L"%02d", dataweek[dataday.day].minutes);
	wsprintf(strstart, L"%02d:%02d", dataweek[dataday.day].start.tm_hour, dataweek[dataday.day].start.tm_min);
	wsprintf(strend, L"%02d:%02d", dataweek[dataday.day].end.tm_hour, dataweek[dataday.day].end.tm_min);

	SendMessage(GetDlgItem(hDlg, IDC_Mainthu), WM_SETTEXT, 0, (LPARAM)stringday[dataday.day]);
	SendMessage(GetDlgItem(hDlg, IDC_Maingio), WM_SETTEXT, 0, (LPARAM)strhour);
	SendMessage(GetDlgItem(hDlg, IDC_Mainstart), WM_SETTEXT, 0, (LPARAM)strstart);
	SendMessage(GetDlgItem(hDlg, IDC_Mainend), WM_SETTEXT, 0, (LPARAM)strend);
	SendMessage(GetDlgItem(hDlg, IDC_Mainphut), WM_SETTEXT, 0, (LPARAM)strmin);


	font = CreateFont(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Times New Roman");
	SendMessage(GetDlgItem(hDlg, IDC_Mainthu), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_Maingio), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_Mainphut), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_Mainstart), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_Mainend), WM_SETFONT, (WPARAM)font, 0);
}

void displayUseTime(HWND hDlg) {
	SetDlgItemInt(hDlg, IDC_MainShowtotal, dataday.totalTime/60 , TRUE);
	SetDlgItemInt(hDlg, IDC_Mainshowused, dataday.timeUse/60 , TRUE);
	SetDlgItemInt(hDlg, IDC_Mainshowtimeremaining, (dataday.totalTime - dataday.timeUse)/60 , TRUE);
}


void installHook(HWND hWnd) {
	MYPROC ProcAddr1;
	MYPROC ProcAddr2;
	hinstLib = LoadLibrary(L"Dlllock.dll");
	if (hinstLib != NULL) {
		ProcAddr1 = (MYPROC)GetProcAddress(hinstLib, "installLockMouseHook");
		if (ProcAddr1 != NULL) {
			ProcAddr1();
		}
		else MessageBox(hWnd, L"error", L"không mở đươc file dll", MB_OK);
		ProcAddr2 = (MYPROC)GetProcAddress(hinstLib, "installLockSomeKey");
		if (ProcAddr2 != NULL) {
			ProcAddr2();
		}
		else MessageBox(hWnd, L"error", L"không mở đươc file dll", MB_OK);
	}
}

void removeHook(HWND hWnd) {
	MYPROC ProcAddr1;
	MYPROC ProcAddr2;
	hinstLib = LoadLibrary(L"Dlllock.dll");
	if (hinstLib != NULL) {
		ProcAddr1 = (MYPROC)GetProcAddress(hinstLib, "RemoveLockKey");
		if (ProcAddr1 != NULL) {
			ProcAddr1();
		}
		else MessageBox(hWnd, L"error", L"không mở đươc file dll", MB_OK);
		ProcAddr2 = (MYPROC)GetProcAddress(hinstLib, "RemoveLockMouse");
		if (ProcAddr2 != NULL) {
			ProcAddr2();
		}
		else MessageBox(hWnd, L"error", L"không mở đươc file dll", MB_OK);
	}
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

void onMainDung(HWND hDlg) {
	KillTimer(hDlg, IDT_TIMER3);
	KillTimer(hDlg, IDT_TIMER2);
	EnableWindow(GetDlgItem(hDlg, IDC_MainTiepTuc), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), FALSE);
}

void onMainTiepTuc(HWND hDlg) {
	updateTimeWhenTurnOn(hDlg);
	updateTimeEvery1Minute(hDlg);
	EnableWindow(GetDlgItem(hDlg, IDC_MainDUNG), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_MainTiepTuc), FALSE);
}

void onPhucHoiTong(HWND hDlg) {
	dataday.timeUse = 0;
}



bool EnableAutoStartup() {
	
	char path[500];
	GetModuleFileNameA(0, path, 500);	
	LPBYTE pPath = (LPBYTE) path;
	std::wstring ws(szTitle);
	std::string AppName(ws.begin(), ws.end());

	std::string sPath(path);
	auto pathRegex = std::regex(R"(^((?:[A-Z]:\\)(?:[A-Za-z0-9\s\-_]+\\)*)(?:[A-Za-z0-9\s\-_]+\.exe))");
	std::smatch result;

	std::regex_match(sPath, result, pathRegex);
	std::string dir = result[1];
	LPBYTE pDir = (LPBYTE)dir.c_str();

	//SetCurrentDirectoryA(dir.c_str());

	HKEY hAppPath;
	HKEY hTmp;
	RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\GroupWin.exe", &hTmp);
	RegCloseKey(hTmp);
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Groupwin.exe",
		0, KEY_ALL_ACCESS, &hAppPath);
	if (RegSetValueExA(hAppPath, "PATH", 0, REG_SZ, (LPBYTE)dir.c_str(), sizeof(dir.c_str())*dir.length()) != ERROR_SUCCESS) {
		return false;
	}
	if (RegSetValueExA(hAppPath, NULL, 0, REG_SZ, pPath, sizeof(path)) != ERROR_SUCCESS) {
		return false;
	}

	std::string sP(path);
	sP = "\"" + sP + "\"" + " --auto-launch-at-startup";
	pPath = (LPBYTE)sP.c_str();

	HKEY hOpened;
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_ALL_ACCESS, &hOpened);
	if (RegSetValueExA(hOpened, AppName.c_str(), 0, REG_SZ,
		pPath, sizeof(sP.c_str())*sP.length()) != ERROR_SUCCESS)
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
