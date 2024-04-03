#include "Forum.h"

int main(void)
{
	TimeStamp time;
	initTime(&time);
	printf("%s\n", getTimeString(&time));

	testPrivateMsgBox();

	//testTimeStampBinaryFile();

	//testMessageBinaryFile();
}

void testPrivateMsgBox()
{
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

	freePrivateMsgBoxContents(&privateBox);
	freeUserContents(&user1);
	freeUserContents(&user2);
}

void testTimeStampBinaryFile()
{
	FILE* fp1 = fopen("timeStampTest.bin", "wb");
	TimeStamp time;
	initTime(&time);
	printf("The created time is: %s\n", getTimeString(&time));
	printf("Saving the time to a binary file\n");
	saveTimeToBFileCompressed(fp1, &time);
	fclose(fp1);
	TimeStamp time2;
	FILE* fp2 = fopen("timeStampTest.bin", "rb");
	readTimeFromBFileCompressed(fp2, &time2);
	fclose(fp2);
	printf("The time read from the file is: %s\n", getTimeString(&time2));
}

void testMessageBinaryFile()
{
	FILE* fp = fopen("messageTest.bin", "wb");
	Message msg;
	createMsg(&msg, "Gofnax");
	saveMsgToBFile(fp, &msg);
	fclose(fp);
	printf("Got the message\n");
	printMsg(&msg);
	printf("Saved message to binary file.\n");
	fp = fopen("messageTest.bin", "rb");
	Message msg2;
	readMsgFromBFile(fp, &msg2);
	fclose(fp);
	printf("The message read from the file is:\n");
	printMsg(&msg2);
}