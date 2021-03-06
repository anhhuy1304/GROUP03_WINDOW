// GroupWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GroupWin.h"
#include <CommCtrl.h>	
#include <regex>
#include <fstream>
using namespace std;
#define MAX_LOADSTRING 100
#define IDT_TIMER1 50000
#pragma warning(disable:4996)
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
/////////////////////////////////////////

//global pass
WCHAR currentPassword[25];
bool noPassword ;
bool systrayFlag = false;
settingApp settingFile;
dataInDay dataday;
dataInWeek dataweek[7];
int action;
bool timeNoti = false;
////NEW VARIALES
int SetGio = 0;
int SetPhut = 0;
int SetGioBatDau = 0;
int SetPhutBatDau = 0;
int SetGioKetThuc = 0;
int SetPhutKetThuc = 0;
////
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
void onTimer();
void updateTimeEvery1Minute(HWND hDlg);
void updateTimeWhenTurnOn();
bool checkTimeLimit();
void SetNgay(HWND hWnd,int i);



////////////////////////////////////////
INT_PTR CALLBACK changePass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK createPass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



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
		
		//createSomeFile();
		onCreateApp(hWnd);
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
	switch (message) {
	case WM_INITDIALOG:
		updateTimeEvery1Minute(hDlg);
		enableMain(hDlg);
		break;
	case WM_TIMER:
		onTimer();

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
			break;
		case IDC_MainDUNG: 
			break;
		case IDC_MainPHUCHOIGIOIHAN: ///BUTTON PHỤC HỒI GIỚI  HẠN THỜI GIAN
			break;
		case IDC_MainPHUCHOITONG:  ///BUTTON PHỤC HỒI TỔNG THỜI GIAN
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
			EndDialog(hDlg, IDCLOSE);
			break;
		case IDC_Mainconfirm:   ///BUTTON XAC NHAN
			updateMainbeforesave(hDlg);			// cap nhat lai main

			writeDataInDayFile(dataday);
			writeDataInWeekFile(dataweek);
			writeSettingFile(settingFile);
			EndDialog(hDlg, IDCLOSE);
			break;
		} return (INT_PTR)TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		PostQuitMessage(0);
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
					}

					setting.noPassword = false;
					setting.systrayFlag = false;
					setting.action = 1;
					writeSettingFile(setting);
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
						settingApp setting;
						setting.noPassword = false;
						setting.systrayFlag = systrayFlag;
						for (int i = 0; i < 25; ++i) {
							setting.password[i] = buffer1[i];
							currentPassword[i] = buffer1[i];
							settingFile.password[i] = buffer1[i];
						}
						setting.action = action;
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



INT_PTR CALLBACK CaiDatDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int thu = 1;
	WCHAR batdau[10];
	WCHAR ketthuc[10];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), false);
		SendMessage(GetDlgItem(hDlg, IDC_BUTTON1), EM_SETREADONLY, true, NULL);
		SetDlgItemText(hDlg, IDC_EDIT1, L"HAI");
		SetDlgItemInt(hDlg, IDC_EDIT2, SetGio, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT3, SetPhut, TRUE);
		if (SetPhut >= 15) {
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), true);
		}
		wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
		SetDlgItemText(hDlg, IDC_EDIT4, batdau);
		wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
		SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CaiDattangthu:   ///BUTTON TĂNG THỨ
			switch (thu)
			{
			case 1:
				thu++;
				SetDlgItemText(hDlg, IDC_EDIT1, L"BA");
				break;
			case 2:
				thu++;
				SetDlgItemText(hDlg, IDC_EDIT1, L"TU");
				break;
			case 3:
				thu++;
				SetDlgItemText(hDlg, IDC_EDIT1, L"NAM");
				break;
			case 4:	
				thu++;
				SetDlgItemText(hDlg, IDC_EDIT1, L"SAU");
				break;
			case 5:
				thu++;
				SetDlgItemText(hDlg, IDC_EDIT1, L"BAY");
				break;
			case 6:
				thu=0;
				SetDlgItemText(hDlg, IDC_EDIT1, L"CN");
				break;
			case 0:
				SetDlgItemText(hDlg, IDC_EDIT1, L"HAI");
				thu=1;
				break;
			}
			break;
		case IDC_CaiDatgiamthu:   ///BUTTON GIẢM THỨ
			switch (thu)
			{
			case 1:
				thu--;
				SetDlgItemText(hDlg, IDC_EDIT1, L"CN");
				break;
			case 0:
				thu=6;
				SetDlgItemText(hDlg, IDC_EDIT1, L"BAY");
				break;
			case 6:
				thu--;
				SetDlgItemText(hDlg, IDC_EDIT1, L"SAU");
				break;
			case 5:
				thu--;
				SetDlgItemText(hDlg, IDC_EDIT1, L"NAM");
				break;
			case 4:
				thu--;
				SetDlgItemText(hDlg, IDC_EDIT1, L"BON");
				break;
			case 3:
				thu--;
				SetDlgItemText(hDlg, IDC_EDIT1, L"BA");
				break;
			case 2:
				SetDlgItemText(hDlg, IDC_EDIT1, L"HAI");
				thu--;
				break;
			}
			break;
		case IDC_CaiDattangGio:   ///BUTTON TĂNG GIỜ
			SetGio++;
			if (SetGio == 24) SetGio = 0;
			SetDlgItemInt(hDlg, IDC_EDIT2, SetGio, TRUE);
			break;
		case IDC_CaiDatgiamGio:   ///BUTTON GIẢM GIỜ
			SetGio--;
			if (SetGio == -1) SetGio = 23;
			SetDlgItemInt(hDlg, IDC_EDIT2, SetGio, TRUE);
			break;
		case IDC_CaiDattangPhut:   ///BUTTON TĂNG PHÚT
			SetPhut++;
			if (SetPhut == 60) SetPhut = 0;
			SetDlgItemInt(hDlg, IDC_EDIT3, SetPhut, TRUE);
			if (SetPhut >= 15) {
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), true);
			}
			else EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), false);
			break;
		case IDC_CaiDatgiamPhut:   ///BUTTON GIẢM PHÚT
			SetPhut--;
			if (SetPhut == -1) SetPhut = 59;
			SetDlgItemInt(hDlg, IDC_EDIT3, SetPhut, TRUE);
			if (SetPhut >= 15) {
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), true);
			}
			else EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), false);
			break;
		case IDC_CaiDatStartTangGio:  ///BUTTON Start TĂNG GIỜ
			SetGioBatDau++;
			if (SetGioBatDau == 24) SetGioBatDau = 0;
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			break;
		case IDC_CaiDatStartGiamGio:   ///BUTTON Start GIẢM GIỜ
			SetGioBatDau--;
			if (SetGioBatDau == -1) SetGioBatDau = 23;
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			break;
		case IDC_CaiDatStartTangPhut:   ////BUTTON Start TĂNG PHÚT
			SetPhutBatDau++;
			if (SetPhutBatDau == 60) SetPhutBatDau = 0;
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			break;
		case IDC_CaiDatStartGiamPhut:   ///BUTTON Start GIẢM PHÚT
			SetPhutBatDau--;
			if (SetPhutBatDau == -1) SetPhutBatDau = 59;
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			break;
		case IDC_CaiDatEndTangGio:    ///BUTTON End TĂNG GIỜ
			SetGioKetThuc++;
			if (SetGioKetThuc == 24) SetGioKetThuc = 0;
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			break;
		case IDC_CaiDatEndGiamGio:   ///BUTTON End GIẢM GIỜ
			SetGioKetThuc--;
			if (SetGioKetThuc == -1) SetGioKetThuc = 23;
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			break;
		case IDC_CaiDatEndTangPhut:   ///BUTTON End TĂNG PHÚT
			SetPhutKetThuc++;
			if (SetPhutBatDau == 60) SetPhutKetThuc = 0;
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			break;
		case IDC_CaiDatEndGiamPhut:   ///BUTTON End GIẢM PHÚT
			SetPhutKetThuc--;
			if (SetPhutKetThuc == -1) SetPhutKetThuc = 59;
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			break;
		case IDC_BUTTON1:
			dataweek[thu].hours = SetGio;
			dataweek[thu].minutes = SetPhut;
			dataweek[thu].start.tm_hour = SetGioBatDau;
			dataweek[thu].start.tm_min = SetPhutBatDau;
			dataweek[thu].end.tm_hour = SetGioKetThuc;
			dataweek[thu].end.tm_min = SetPhutKetThuc;
			thu = 1;
			SetGio = 0;
			SetPhut = 0;
			SetGioBatDau = 0;
			SetPhutBatDau = 0;
			SetGioKetThuc = 0;
			SetPhutKetThuc = 0;
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), false);
			SetDlgItemText(hDlg, IDC_EDIT1, L"HAI");
			SetDlgItemInt(hDlg, IDC_EDIT2, SetGio, TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT3, SetPhut, TRUE);
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			EndDialog(hDlg, NULL);
			break;
		case IDC_BUTTON2:
			thu = 1;
			SetGio = 0;
			SetPhut = 0;
			SetGioBatDau = 0;
			SetPhutBatDau = 0;
			SetGioKetThuc = 0;
			SetPhutKetThuc = 0;
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), false);
			SetDlgItemText(hDlg, IDC_EDIT1, L"HAI");
			SetDlgItemInt(hDlg, IDC_EDIT2, SetGio, TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT3, SetPhut, TRUE);
			wsprintf(batdau, L"%d:%d", SetGioBatDau, SetPhutBatDau);
			SetDlgItemText(hDlg, IDC_EDIT4, batdau);
			wsprintf(ketthuc, L"%d:%d", SetGioKetThuc, SetPhutKetThuc);
			SetDlgItemText(hDlg, IDC_EDIT5, ketthuc);
			EndDialog(hDlg, NULL);
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
}


void checkEnableMain(HWND hDlg)
{
	bool checkPass = true;
	WCHAR tmp[100];
	ZeroMemory(tmp, sizeof(WCHAR) * 100);
	GetDlgItemText(hDlg, IDC_MainPass, tmp, 100);
	for (int i = 0; i < 25; ++i) {
		if (tmp[i] != currentPassword[i]) {
			checkPass = true;
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
		EnableWindow(GetDlgItem(hDlg, IDC_MainPass), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainKhoa), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_MainMoKhoa), FALSE);
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
	}
	updateTimeWhenTurnOn();
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
	f.write((char*)&result.noPassword, sizeof(bool));
	f.write((char*)&result.password, sizeof(WCHAR) * 25);
	f.write((char*)&result.systrayFlag, sizeof(bool));
	f.write((char*)&result.action, sizeof(int));
	f.close();


	//create datatinday file
	f.open("dataInDayFile.bin", ios::out | ios::binary);
	dataInDay datainday;
	datainday.day = 0;
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
	start.tm_hour = 0;
	start.tm_min = 0;
	start.tm_sec = 0;
	tm end;
	end.tm_hour = 23;
	end.tm_min = 59;
	end.tm_sec = 59;
	for (int i = 0; i < 7; i++) {
		dataweek[i].enable = false;
		dataweek[i].day = i;
		dataweek[i].end = end;
		dataweek[i].hours = 24;
		dataweek[i].minutes = 0;
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
}




// xu ly thoi gian
void updateTimeWhenTurnOn() {
	time_t t = time(0);
	tm* now = localtime(&t); // get time now
	if (now->tm_wday != dataday.day) {
		dataday.day = now->tm_wday;
		dataday.totalTime = dataweek[dataday.day].hours * 3600 + dataweek[dataday.day].minutes * 60;
		dataday.timeUse = 0;
	}
	UINT timeRemain = dataday.totalTime - dataday.timeUse;
	if (timeRemain <= 0 || checkTimeLimit() == false) {
		//install hook + open dialog 
	}
	UINT timeNotifi = 60 * 10 * 1000; //10p nhac nho
	UINT timer = timeRemain - timeNotifi;
	if (timer < timeNotifi) {
		timeNoti = true;
		//open hop thoai nhac nho
	}
}

void updateTimeEvery1Minute(HWND hDlg) {
	SetTimer(hDlg, IDT_TIMER3, 60000, NULL);
}

void onTimer() {
	dataday.timeUse += 60;
	writeDataInDayFile(dataday);
	UINT timeRemain = dataday.totalTime - dataday.timeUse;
	if (timeRemain <= 0 || checkTimeLimit() == false) { // neu het gio hoac ko phai thoi gian
		//install hook
	}
	UINT timeNotifi = 60 * 10 * 1000;
	UINT timer = timeRemain - timeNotifi;
	if (timer < timeNotifi && timeNoti == false ) {
		timeNoti = true;
		//open hop thoai nhac nho
	}


}

bool checkTimeLimit() {
	time_t t = time(0);
	tm* now = localtime(&t); // get time now
	int day = now->tm_wday; // get week day

	// check limit
	if (now->tm_hour > dataweek[day].end.tm_hour || now->tm_hour < dataweek[day].start.tm_hour) return false;
	if (now->tm_hour == dataweek[day].end.tm_hour)
		if (now->tm_min > dataweek[day].end.tm_min) return false;
	if (now->tm_hour == dataweek[day].start.tm_hour)
		if (now->tm_min < dataweek[day].start.tm_min) return false;
	return true;

	
}

//Xuly Dialog Caidat
