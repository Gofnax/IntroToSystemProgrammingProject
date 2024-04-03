#include "TimeStamp.h"

char* getTimeString(const TimeStamp* pTime)
{
	char day[3] = { 0 }, month[3] = { 0 }, year[5] = { 0 }, hour[3] = { 0 }, minute[3] = { 0 };
	formatTimeElement(day, 3, pTime->day);
	formatTimeElement(month, 3, pTime->month);
	snprintf(year, 5, "%d", pTime->year);
	formatTimeElement(hour, 3, pTime->hour);
	formatTimeElement(minute, 3, pTime->minute);
	int len = (int)(strlen(day) + strlen(month) + strlen(year) + strlen(hour) + strlen(minute) + strlen("// :"));
	char* timeString = (char*)calloc(len + 1, sizeof(char));
	if (timeString == NULL)
	{
		return NULL;
	}
	snprintf(timeString, len + 1, "%s/%s/%s %s:%s", day, month, year, hour, minute);
	return timeString;
}

void formatTimeElement(char* charElement, int len, int numElement)
{
	if (numElement <= 9)
	{
		snprintf(charElement, len, "0%d", numElement);
	}
	else
	{
		snprintf(charElement, 3, "%d", numElement);
	}
}

int	initTime(TimeStamp* pTime)
{
	time_t currTimeTmp = time(&currTimeTmp);
	struct tm* currTime = localtime(&currTimeTmp);
	pTime->day = currTime->tm_mday;
	pTime->month = currTime->tm_mon + 1;	//tm_mon is between 0 and 11
	pTime->year = currTime->tm_year + 1900;	//tm_year counts years since 1900
	pTime->hour = currTime->tm_hour;
	pTime->minute = currTime->tm_min;
	return 1;
}

int compareTime(const TimeStamp* pTime1, const TimeStamp* pTime2)
{
	if (pTime1->year != pTime2->year)
		return pTime1->year - pTime2->year;
	if (pTime1->month != pTime2->month)
		return pTime1->month - pTime2->month;
	if (pTime1->day != pTime2->day)
		return pTime1->day - pTime2->day;
	if (pTime1->hour != pTime2->hour)
		return pTime1->hour - pTime2->hour;
	if (pTime1->minute != pTime2->minute)
		return pTime1->minute - pTime2->minute;
	return 0;
}

int saveTimeToBFileCompressed(const FILE* fp, const TimeStamp* pTime)
{
	if (fp == NULL || pTime == NULL)
		return -1;
	BYTE data[3] = { 0 };
	data[0] = (pTime->month << 5) | pTime->day;
	data[1] = (pTime->hour << 5) | ((pTime->year - 2020) << 1) | (pTime->month >> 3);
	data[2] = (pTime->minute << 2) | (pTime->hour >> 3);

	if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
		return -1;

	return 1;
}

int readTimeFromBFileCompressed(const FILE* fp, TimeStamp* pTime)
{
	if (fp == NULL || pTime == NULL)
		return -1;

	BYTE data[3] = { 0 };
	if (fread(data, sizeof(BYTE), 3, fp) != 3)
		return -1;

	int mask = 0x1F;
	pTime->day = data[0] & mask;
	mask = 1;
	pTime->month = ((data[1] & mask) << 3) | (data[0] >> 5);
	mask = 0xF;
	pTime->year = 2020 + ((data[1] >> 1) & mask);
	mask = 3;
	pTime->hour = ((data[2] & mask) << 3) | (data[1] >> 5);
	pTime->minute = data[2] >> 2;

	return 1;
}
