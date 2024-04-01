#include "Forum.h"
#include "TimeStamp.h"
#include "Message.h"
#include "User.h"

int main(void)
{
	TimeStamp time;// = { .day = 4, .month = 11, .year = 1998, .hour = 23, .minute = 7 };
	initTime(&time);
	printf("%s\n", getTimeString(&time));

	Message msg;
	User user;
	initUser(&user);
	createMsg(&msg, user.name);
	printMsg(&msg);
}