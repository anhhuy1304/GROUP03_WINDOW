#pragma once

#include "resource.h"
#include<time.h>
struct dataInWeek {
	bool enable;
	int day; //[0-6] sunday =0
	int hours; // >=0
	int minutes; //>=15
	tm start;
	tm end;

};

struct dataInDay {
	int day; // monday
	int timeUse; // seconds
	int totalTime;//seconds
};

struct settingApp {
	bool noPassword; // check have password
	bool systrayFlag; // check app run
	WCHAR password[25];
	int action; // 1 = shut down, 2 = lock, 3 = sleep
};

//NEW
struct Ngay {
	WCHAR TenNgay[5];
};
void SetTenNgay(Ngay NameofDay[]) {
	wsprintf(NameofDay[1].TenNgay, L"Hai");
	wsprintf(NameofDay[2].TenNgay, L"Ba");
	wsprintf(NameofDay[3].TenNgay, L"Tu");
	wsprintf(NameofDay[4].TenNgay, L"Nam");
	wsprintf(NameofDay[5].TenNgay, L"Sau");
	wsprintf(NameofDay[6].TenNgay, L"Bay");
	wsprintf(NameofDay[7].TenNgay, L"CN");
	

}
