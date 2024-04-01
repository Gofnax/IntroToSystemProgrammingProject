#include "TimeStamp.h"

char* getTimeString(TimeStamp* pTime)
{
	char day[3] = { 0 }, month[3] = { 0 }, year[5] = { 0 }, hour[3] = { 0 }, minute[3] = { 0 };
	if (pTime->day <= 9)
	{
		snprintf(day, 3, "0%d", pTime->day);
	}
	else
	{
		snprintf(day, 3, "%d", pTime->day);
	}
	if (pTime->month <= 9)
	{
		snprintf(month, 3, "0%d", pTime->month);
	}
	else
	{
		snprintf(month, 3, "%d", pTime->month);
	}
	snprintf(year, 5, "%d", pTime->year);
	if (pTime->hour <= 9)
	{
		snprintf(hour, 3, "0%d", pTime->hour);
	}
	else
	{
		snprintf(hour, 3, "%d", pTime->hour);
	}
	if (pTime->minute <= 9)
	{
		snprintf(minute, 3, "0%d", pTime->minute);
	}
	else
	{
		snprintf(minute, 3, "%d", pTime->minute);
	}
	int len = (int)(strlen(day) + strlen(month) + strlen(year) + strlen(hour) + strlen(minute) + strlen("// :"));
	char* timeString = (char*)calloc(len + 1, sizeof(char));
	if (timeString == NULL)
	{
		return NULL;
	}
	snprintf(timeString, len + 1, "%s/%s/%s %s:%s", day, month, year, hour, minute);
	return timeString;
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
