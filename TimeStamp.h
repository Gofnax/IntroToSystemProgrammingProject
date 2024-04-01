#pragma once

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
}TimeStamp;

int initTime(TimeStamp* pTime);
char* getTimeString(TimeStamp* pTime);