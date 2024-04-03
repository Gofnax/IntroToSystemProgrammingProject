#pragma once

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char BYTE;

typedef struct
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
}TimeStamp;

int initTime(TimeStamp* pTime);
char* getTimeString(const TimeStamp* pTime);
void formatTimeElement(char* charElement, int len, int numElement);
int compareTime(const TimeStamp* pTime1, const TimeStamp* pTime2);
int saveTimeToBFileCompressed(const FILE* fp, const TimeStamp* pTime);
int readTimeFromBFileCompressed(const FILE* fp, TimeStamp* pTime);