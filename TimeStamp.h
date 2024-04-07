#pragma once

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "def.h"

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
int saveTimeToBFileCompressed(FILE* fp, const TimeStamp* pTime);
int readTimeFromBFileCompressed(FILE* fp, TimeStamp* pTime);
int saveTimeStampToTextFile(const TimeStamp* timeStamp, FILE* fp);
int loadTimeStampFromTextFile(TimeStamp* timeStamp, FILE* fp);