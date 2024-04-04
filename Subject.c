#include "Subject.h"

int initSubject(Subject* pSubject)
{
	pSubject->title = (char*)malloc(sizeof(char) * MAX_TITLE_NAME);
	if (pSubject->title == NULL)
	{
		return -1;
	}
	createSubjectTitle(pSubject);
	pSubject->threadArr = (Thread**)malloc(sizeof(Thread*) * 1);
	if (pSubject->threadArr == NULL)
	{
		return -1;
	}
	pSubject->threadArrSize = 0;
	return 1;
}

int createSubjectTitle(Subject* pSubject)
{
	if (pSubject == NULL)
		return -1;
	char buffer[10] = { 0 };
	pSubject->title[MAX_TITLE_NAME - 1] = '\0';
	printf("Enter the subject's title: (max %d chars)\n", MAX_TITLE_NAME - 1);
	fgets(pSubject->title, MAX_TITLE_NAME, stdin);
	if (pSubject->title[MAX_TITLE_NAME - 1] != '\0')
	{
		pSubject->title[MAX_MSG_LEN - 1] = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pSubject->title);
	return 1;
}

void printThreadArr(Thread** threadArr, int threadArrSize)
{
	for (int i = 0; i < threadArrSize; i++)
	{
		printf("%d. %s\n", i + 1, threadArr[i]->title);
	}
}

void chooseThread(Thread** threadArr, int threadArrSize, User* pCurrUser)
{
	if (threadArr == NULL)
		return;
	int choice;
	char buff[2] = { 0 };
	printThreadArr(threadArr, threadArrSize);
	do
	{
		printf("Choose a thread:\n");
		(void)scanf("%d", &choice);
		(void)gets(buff);	// buffer cleaning
		if (choice < 1 || choice > threadArrSize)
		{
			printf("Invalid choice.\n");
		}
	} while (choice < 1 || choice > threadArrSize);
	printThread(threadArr[choice - 1]);
	threadActionsMenu(threadArr[choice - 1], pCurrUser);
}

int  addThread(Subject* pSubject, Thread* pThread)
{
	Thread** temp = (Thread**)realloc(pSubject->threadArr, sizeof(Thread*) * (pSubject->threadArrSize + 1));
	if (temp == NULL)
	{
		return -1;
	}
	pSubject->threadArr = temp;
	pSubject->threadArr[pSubject->threadArrSize] = pThread;
	pSubject->threadArrSize++;
	return 1;
}

void printSubject(const Subject* pSubject)
{
	printf("Subject title: %s\n", pSubject->title);
	printf("Threads:\n");
	printThreadArr(pSubject->threadArr, pSubject->threadArrSize);
}

void subjectActionsMenu(Subject* pSubject, User* pCurrUser)
{
	if (pSubject == NULL || pCurrUser == NULL)
		return;
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("Choose action: (1 - View Thread | 2 - Add a New Thread | 3 - Display Thread List | 0 - Exit Subject)\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				chooseThread(pSubject->threadArr, pSubject->threadArrSize, pCurrUser);
				break;
			case 2:
				{
					Thread* pNewThread = (Thread*)malloc(1 * sizeof(Thread));
					if (pNewThread == NULL)
						return;
					initThread(pNewThread, pCurrUser);
					addThread(pSubject, pNewThread);
				}
				break;
			case 3:
				printSubject(pSubject);
				break;
			case 0:
				printf("Returning to main menu.\n");
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

void freeSubjectContent(Subject* pSubject)
{
	free(pSubject->title);
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		freeThread(pSubject->threadArr[i]);
	}
	free(pSubject->threadArr);
}

void freeSubject(Subject* pSubject)
{
	freeSubjectContent(pSubject);
	free(pSubject);
}
