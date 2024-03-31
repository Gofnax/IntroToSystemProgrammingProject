#include "Forum.h"
#include "TimeStamp.h"

int main(void)
{
	TimeStamp time = { .day = 1, .month = 1, .year = 1111, .hour = 11, .minute = 69 };
	(void)getTimeString(&time);
}