#include "Forum.h"
#include "TimeStamp.h"
#include "Message.h"
#include "User.h"
#include "PrivateMsgBox.h"

int main(void)
{
	TimeStamp time;
	initTime(&time);
	printf("%s\n", getTimeString(&time));

	/*Message msg;
	User user;
	initUser(&user);
	createMsg(&msg, user.name);
	printMsg(&msg);*/

	printf("Register user 1:\n");
	User user1;
	initUser(&user1);
	printf("Register user 2:\n");
	User user2;
	initUser(&user2);
	PrivateMsgBox privateBox;
	initPrivateMsgBox(&privateBox, &user1, &user2);
	privateMsgBoxMenu(&privateBox, &user1);
	privateMsgBoxMenu(&privateBox, &user2);
	privateMsgBoxMenu(&privateBox, &user1);
	/*printf("Write a message as %s\n", user1.name);
	writeMessage(&privateBox, &user1);
	printf("Write a message as %s\n", user2.name);
	writeMessage(&privateBox, &user2);
	printPrivateMsgs(&privateBox);*/

	freePrivateMsgBoxContents(&privateBox);
	freeUserContents(&user1);
	freeUserContents(&user2);

}