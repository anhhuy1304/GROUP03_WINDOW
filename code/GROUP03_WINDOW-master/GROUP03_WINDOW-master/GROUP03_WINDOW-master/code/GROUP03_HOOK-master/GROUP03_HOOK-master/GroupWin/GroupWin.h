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
	int action; // 1 = shut down, 2 = lock, 3 = sleep
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
};
