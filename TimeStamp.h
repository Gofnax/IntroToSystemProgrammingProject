#pragma once

#include <stdio.h>

typedef struct
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
}TimeStamp;

char* getTimeString(TimeStamp* pTime);