#include "Forum.h"
#include "TimeStamp.h"

int main(void)
{
	TimeStamp time;// = { .day = 4, .month = 11, .year = 1998, .hour = 23, .minute = 7 };
	(void)initTime(&time);
	printf("%s\n", getTimeString(&time));
	LIST testList;
	L_init(&testList);
	L_insert(&testList.head, 3);

}