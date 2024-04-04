#include "Thread.h"
#include "def.h"

int initThread(Thread* pThread, User* currentUser)
{
	pThread->title = (char*)malloc(sizeof(char) * MAX_TITLE_NAME);
	if (pThread->title == NULL)
	{
		return -1;
	}
	(void)createThreadTitle(pThread);
	pThread->messageArr = (Message*)malloc(sizeof(Message) * 1);
	if (pThread->messageArr == NULL)
	{
		return -1;
	}
	pThread->messageArrSize = 0;
	if (createMsg(&pThread->primaryMsg, currentUser->name) != 1)
	{
		return -1;
	}
	return 1;
}

int createThreadTitle(Thread* pThread)
{
	if (pThread == NULL)
		return -1;
	char buffer[10] = { 0 };
	pThread->title[MAX_TITLE_NAME - 1] = '\0';
	printf("Enter the thread's title: (max %d chars)\n", MAX_TITLE_NAME - 1);
	fgets(pThread->title, MAX_TITLE_NAME, stdin);
	if (pThread->title[MAX_TITLE_NAME - 1] != '\0')
	{
		pThread->title[MAX_MSG_LEN - 1] = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pThread->title);
	return 1;
}

void printThread(const Thread* pThread)
{
	printf("----------------------------------------\n");	// 40 '-' for visuals
	printf("Thread title: %s\n", pThread->title);
	printMsg(&pThread->primaryMsg);
	printf("\nMessages:\n");
	for (int i = 0; i < pThread->messageArrSize; i++)
	{
		printMsgIndexed(&pThread->messageArr[i], i + 1);
	}
}

int addMessage(Thread* pThread, User* pCurrentUser)
{
	Message* temp = (Message*)realloc(pThread->messageArr, sizeof(Message) * (pThread->messageArrSize + 1));
	if (temp == NULL)
	{
		return -1;
	}
	pThread->messageArr = temp;
	if (createMsg(&pThread->messageArr[pThread->messageArrSize], pCurrentUser->name) != 1)
	{
		return -1;
	}
	pThread->messageArrSize++;
	return 1;
}

void threadActionsMenu(Thread* pThread, User* pCurrentUser)
{
	if (pThread == NULL || pCurrentUser == NULL)
		return;
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("Choose action: (1 - Write a New Message | 2 - Like a Message | 3 - Display All Messages | 0 - Exit Thread)\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				addMessage(pThread, pCurrentUser);
				break;
			case 2:
				likeMsg(pThread);
				break;
			case 3:
				printThread(pThread);
				break;
			case 0:
				printf("Exiting thread.\n");
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

void likeMsg(Thread* pThread)
{
	if (pThread == NULL)
		return;
	int userChoice;
	char buff[2] = { 0 };
	printf("Enter the number of the message you like:\n");
	(void)scanf("%d", &userChoice);
	(void)gets(buff);	// buffer cleaning
	if (userChoice < 1 || userChoice > pThread->messageArrSize)
	{
		printf("No such message number\n");
	}
	else
	{
		pThread->messageArr[userChoice - 1].likesCounter++;
	}
}

void freeThreadContent(Thread* pThread)
{
	if (pThread == NULL)
		return;
	free(pThread->title);
	freeMessageContents(&pThread->primaryMsg);
	for (int i = 0; i < pThread->messageArrSize; i++)
	{
		freeMessageContents(&pThread->messageArr[i]);
	}
}

void freeThread(Thread* pThread)
{
	freeThreadContent(pThread);
	free(pThread);
}
