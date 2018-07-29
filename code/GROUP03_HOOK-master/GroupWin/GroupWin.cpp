// GroupWin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GroupWin.h"
#include <CommCtrl.h>	
#include <regex>
#include <fstream>
using namespace std;
#define MAX_LOADSTRING 100
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
int hour;
int startHour;
int startMin;
int EndHour;
int EndMin;
int minu;
int day;

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
void onInitSettingDialog(HWND hDlg);
void displaysetting(HWND hDlg);



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
		updateTimeWhenTurnOn(hDlg);
		updateTimeEvery1Minute(hDlg);
		enableMain(hDlg);
		break;
	case WM_TIMER:
		onTimer(hDlg);

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
		KillTimer(hDlg, IDT_TIMER3);
		KillTimer(hDlg, IDT_TIMER2);
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
	

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//set default


		onInitSettingDialog(hDlg);
		displaysetting(hDlg);
		
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CaiDattangthu:///BUTTON TĂNG THỨ
			day++;
			if (day == 7) day = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatgiamthu:///BUTTON GIẢM THỨ
			day--;
			if (day == -1) day = 6;
			displaysetting(hDlg);
			break;
		case IDC_CaiDattangGio:///BUTTON TĂNG GIỜ
			hour++;
			if (hour == 24) hour = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatgiamGio:  ///BUTTON GIẢM GIỜ
			hour--;
			if (hour == -1) hour = 23;
			displaysetting(hDlg);
			break;
		case IDC_CaiDattangPhut:   ///BUTTON TĂNG PHÚT
			minu++;
			if (minu == 60) minu = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatgiamPhut:  
			minu--;
			if (minu == -1) minu = 59;
			displaysetting(hDlg);///BUTTON GIẢM PHÚT
			break;
		case IDC_CaiDatStartTangGio:  ///BUTTON Start TĂNG GIỜ
			startHour++;
			if (startHour == 24) startHour = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatStartGiamGio:///BUTTON Start GIẢM GIỜ
			startHour--;
			if (startHour == -1) startHour = 23;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatStartTangPhut: //BUTTON Start TĂNG PHÚT
			startMin++;		
			if (startMin == 60) startMin = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatStartGiamPhut: 
			startMin--;
			if (startMin == -1) startMin = 59;
			displaysetting(hDlg);///BUTTON Start GIẢM PHÚT
			break;
		case IDC_CaiDatEndTangGio:    ///BUTTON End TĂNG GIỜ
			EndHour++;
			if (EndHour == 24) EndHour = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatEndGiamGio:   ///BUTTON End GIẢM GIỜ
			EndHour--;
			if (EndHour == -1) EndHour = 23;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatEndTangPhut:   ///BUTTON End TĂNG PHÚT
			EndMin++;
			if (EndMin == 60) EndMin = 0;
			displaysetting(hDlg);
			break;
		case IDC_CaiDatEndGiamPhut:   ///BUTTON End GIẢM PHÚT
			EndMin--;
			if (EndMin == -1) EndMin = 59;
			displaysetting(hDlg);
			break;
		case IDC_SETTINGCANCLE:
			EndDialog(hDlg, NULL);
			break;
		case IDC_SETTINGOK:
			//luu lai vao file va update
			dataweek[day].start.tm_hour = startHour;
			dataweek[day].start.tm_min = startMin;
			dataweek[day].end.tm_hour = EndHour;
			dataweek[day].end.tm_min = EndMin;
			dataweek[day].hours = hour;
			dataweek[day].minutes = minu;
			if (day == dataday.day) {
				dataday.totalTime = hour * 3600 + minu * 60;
				dataday.timeUse = 0;
			}
			writeDataInWeekFile(dataweek);
			writeDataInDayFile(dataday);
			break;
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
	datainday.day = -1;
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
	if (timeRemain <= 0 || checkTimeLimit() == false) {
		//install hook + open dialog 
		MessageBox(hDlg, L"CAI HOOK", L"CAI HOOK", MB_OK);
	}
	int timeNotifi = 60 * 10 * 1000; //10p nhac nho
	timeRemain = timeRemain * 1000;
	int timer;
	timer = timeRemain - timeNotifi;
	if (timer < timeNotifi && timer >0) {
		timeNoti = true;
		//open hop thoai nhac nho
		MessageBox(hDlg, L"NHAC NHO", L"NHAC NHO", MB_OK);
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
	SetTimer(hDlg, IDT_TIMER2, timeLimit, NULL);


}

void onTimer(HWND hDlg) {
	dataday.timeUse += 60;
	writeDataInDayFile(dataday);
	UINT timeRemain = dataday.totalTime - dataday.timeUse;
	if (timeRemain <= 0 || checkTimeLimit() == false) {
		MessageBox(hDlg, L"HET GIO", L"HET GIO", MB_OK);
		// neu het gio hoac ko phai thoi gian
		//install hook
	}
	UINT timeNotifi = 60 * 10 * 1000;
	UINT timer = timeRemain - timeNotifi;
	if (timer >0 &&timer < timeNotifi && timeNoti == false ) {
		timeNoti = true;
		//open hop thoai nhac nho
		MessageBox(hDlg, L"NHAC NHO", L"NHAC NHO", MB_OK);
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

void onInitSettingDialog(HWND hDlg) {

	int day = dataday.day;
	 hour		 = dataweek[day].hours;
	 minu  = dataweek[day].minutes;
	 startHour	 = dataweek[day].start.tm_hour;
	 startMin	 = dataweek[day].start.tm_min;
	 EndHour		 = dataweek[day].end.tm_hour;
	 EndMin		 = dataweek[day].end.tm_min;
}

void displaysetting(HWND hDlg) {

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


	wsprintf(strhour, L"%02d", hour);
	wsprintf(strmin, L"%02d", minu);
	wsprintf(strstart, L"%02d:%02d", startHour, startMin);
	wsprintf(strend, L"%02d:%02d", EndHour, EndMin);

	//set text
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGDAY), WM_SETTEXT, 0, (LPARAM)stringday[day]);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGHOUR), WM_SETTEXT, 0, (LPARAM)strhour);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGSTART), WM_SETTEXT, 0, (LPARAM)strstart);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGEND), WM_SETTEXT, 0, (LPARAM)strend);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGMIN), WM_SETTEXT, 0, (LPARAM)strmin);
	//set font
	HFONT font = CreateFont(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Times New Roman");
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGDAY), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGHOUR), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGSTART), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGEND), WM_SETFONT, (WPARAM)font, 0);
	SendMessage(GetDlgItem(hDlg, IDC_SETTINGMIN), WM_SETFONT, (WPARAM)font, 0);
}