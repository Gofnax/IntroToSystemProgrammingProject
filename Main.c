#include <stdio.h>
#include "list.h"
#include "def.h"
#include "General.h"
#include "Forum.h"
#include "TimeStamp.h"
#include "Message.h"
#include "User.h"
#include "PrivateMsgBox.h"
#include "Forum.h"

int main(void);
void forumLoadMenu(Forum* pForum);
void testPrivateMsgBox();
void testThread();
void testSubject();
void testTimeStampBinaryFile();
void testMessageBinaryFile();
void testUserBinaryFile();
void testThreadBinaryFile();
void testSubjectBinaryFilr();
void testForumBinaryFile();


int main(void)
{
	TimeStamp time;
	initTime(&time);
	printf("%s\n", getTimeString(&time));

	/*Forum forum1;
	forumLoadMenu(&forum1);
	forumMainMenu(&forum1);*/

	//testPrivateMsgBox();
	//testTimeStampBinaryFile();
	//testMessageBinaryFile();
	//testUserBinaryFile();
	//testThread();
	//testSubject();
	//testThreadBinaryFile();
	//testSubjectBinaryFilr();
	testForumBinaryFile();
}

void forumLoadMenu(Forum* pForum)
{
	NULL_CHECK(pForum, );
	int userChoice = 0;
	char buff[2] = { 0 };
	FILE* fp;
	do
	{
		printf("How would you like to load the system?\n");
		printf("(1 - Using Text Files | 2 - Using Binary Files | 3 - Clean System)\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				fp = fopen(SYSTEM_TEXT_FILE, "r");
				loadForumFromTextFile(pForum, fp);
				fclose(fp);
				break;
			case 2:
				break;
			case 3:
				initForum(pForum);
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice < 1 || userChoice > 3);
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

void testThread()
{
	User user1, user2;
	printf("Register user 1:\n");
	initUser(&user1);
	printf("Register user 2:\n");
	initUser(&user2);
	Thread thread1;
	initThread(&thread1, &user1);
	threadActionsMenu(&thread1, &user2);
	threadActionsMenu(&thread1, &user1);
	threadActionsMenu(&thread1, &user2);
	printThread(&thread1);
}

void testSubject()
{
	User user1;
	printf("Register user 1:\n");
	initUser(&user1);
	Subject subject1;
	initSubject(&subject1);
	subjectActionsMenu(&subject1, &user1);
	printSubject(&subject1);
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

void testUserBinaryFile()
{
	FILE* fp = fopen("userTest.bin", "wb");
	User user1;
	printf("Register user 1:\n");
	initUser(&user1);
	saveUserToBFile(fp, &user1);
	fclose(fp);
	printf("User was saved to binary file.\n");
	fp = fopen("userTest.bin", "rb");
	User user2;
	readUserFromBFile(fp, &user2);
	fclose(fp);
	printf("The name of the user read from the file is:\n");
	printf("%s\n", user2.name);
}

void testThreadBinaryFile()
{
	FILE* fp = fopen("userTest.bin", "rb");
	User user1;
	readUserFromBFile(fp, &user1);
	fclose(fp);
	fp = fopen("threadTest.bin", "wb");
	Thread thread1;
	initThread(&thread1, &user1);
	threadActionsMenu(&thread1, &user1);
	printf("Printing thread1:\n");
	printThread(&thread1);
	printf("Saving thread to B file\n");
	saveThreadToBFile(fp, &thread1);
	fclose(fp);
	fp = fopen("threadTest.bin", "rb");
	printf("Reading thread from B file\n");
	Thread thread2;
	readThreadFromBFile(fp, &thread2);
	fclose(fp);
	printf("Printing thread2:\n");
	printThread(&thread2);
}

void testSubjectBinaryFilr()
{
	FILE* fp = fopen("userTest.bin", "rb");
	User user1;
	readUserFromBFile(fp, &user1);
	fclose(fp);
	fp = fopen("threadTest.bin", "rb");
	Thread thread1;
	readThreadFromBFile(fp, &thread1);
	fclose(fp);
	fp = fopen("subjectTest.bin", "wb");
	Subject subject1;
	initSubject(&subject1);
	addThread(&subject1, &thread1);
	subjectActionsMenu(&subject1, &user1);
	printf("\nPrinting subject1:\n");
	printSubject(&subject1);
	printf("\nSaving subject to B file\n");
	saveSubjectToBFile(fp, &subject1);
	fclose(fp);
	fp = fopen("subjectTest.bin", "rb");
	printf("\Reading subject to B file\n");
	Subject subject2;
	readSubjectFromBFile(fp, &subject2);
	fclose(fp);
	printf("\nPrinting subject2:\n");
	printSubject(&subject2);
}

void testForumBinaryFile()
{
	FILE* fp = fopen("userTest.bin", "rb");
	User user1, user2;
	user2.name = (char*)malloc(USERNAME_LEN * sizeof(char));
	NULL_CHECK(user2.name, );
	strncpy(user2.name, "Username2", 10);
	strncpy(user2.password, "123", 4);
	readUserFromBFile(fp, &user1);
	fclose(fp);
	fp = fopen("threadTest.bin", "rb");
	Thread thread1;
	readThreadFromBFile(fp, &thread1);
	fclose(fp);
	fp = fopen("subjectTest.bin", "rb");
	Subject subject1;
	readSubjectFromBFile(fp, &subject1);
	fclose(fp);
	fp = fopen("forumTest.bin", "wb");
	Forum forum1;
	initForum(&forum1);
	addUser(&user1, &forum1);
	addUser(&user2, &forum1);
	L_insert(&forum1.subjectList.head, &subject1);
	forumMainMenu(&forum1);
	printf("\nSaving forum to B file\n");
	saveForumToBFile(fp, &forum1);
	fclose(fp);
	fp = fopen("forumTest.bin", "rb");
	printf("\Reading forum from B file\n");
	Forum forum2;
	readForumFromBFile(fp, &forum2);
	fclose(fp);
	forumMainMenu(&forum2);
}