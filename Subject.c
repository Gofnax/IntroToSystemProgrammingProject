#include "Subject.h"

int initSubject(Subject* pSubject)
{
	NULL_CHECK(pSubject, -1);
	pSubject->title = (char*)malloc(sizeof(char) * MAX_TITLE_NAME);
	NULL_CHECK(pSubject->title, -1);
	createSubjectTitle(pSubject);
	pSubject->threadArr = (Thread**)malloc(sizeof(Thread*) * 1);
	NULL_CHECK(pSubject->threadArr, -1);
	pSubject->threadArrSize = 0;
	return 1;
}

int createSubjectTitle(Subject* pSubject)
{
	NULL_CHECK(pSubject, -1);
	char buffer[10] = { 0 };
	pSubject->title[MAX_TITLE_NAME - 1] = '\0';
	printf("Enter the subject: (max %d chars)\n", MAX_TITLE_NAME - 1);
	fgets(pSubject->title, MAX_TITLE_NAME, stdin);
	if (pSubject->title[MAX_TITLE_NAME - 1] != '\0')
	{
		pSubject->title[MAX_MSG_LEN - 1] = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pSubject->title);
	return 1;
}

void printThreadArr(const Thread** threadArr, int threadArrSize)
{
	NULL_CHECK(threadArr, );
	for (int i = 0; i < threadArrSize; i++)
	{
		printf("%d. %s\n", i + 1, threadArr[i]->title);
	}
}

void chooseThread(Subject* pSubject, User* pCurrUser)
{
	if (pSubject == NULL || pCurrUser == NULL)
		return;
	int choice = 0;
	char buff[2] = { 0 };
	printThreadArr(pSubject->threadArr, pSubject->threadArrSize);
	do
	{
		printf("Choose a thread:\n");
		NUM_INPUT_CLEAN_BUFF(choice, buff);
		if (choice < 1 || choice > pSubject->threadArrSize)
		{
			printf("Invalid choice.\n");
		}
	} while (choice < 1 || choice > pSubject->threadArrSize);
	printThread(pSubject->threadArr[choice - 1]);
	threadActionsMenu(pSubject->threadArr[choice - 1], pCurrUser);
}

int  addThread(Subject* pSubject, Thread* pThread)
{
	Thread** temp = (Thread**)realloc(pSubject->threadArr, sizeof(Thread*) * (pSubject->threadArrSize + 1));
	NULL_CHECK(temp, -1);
	pSubject->threadArr = temp;
	pSubject->threadArr[pSubject->threadArrSize] = pThread;
	pSubject->threadArrSize++;
	return 1;
}

void printSubject(const Subject* pSubject)
{
	NULL_CHECK(pSubject, );
	printf("Subject: %s\n", pSubject->title);
	printf("Threads:\n");
	printThreadArr(pSubject->threadArr, pSubject->threadArrSize);
}

void printSubjectTitle(const Subject* pSubject)
{
	NULL_CHECK(pSubject, );
	printf("%s\n", pSubject->title);
}

void subjectActionsMenu(Subject* pSubject, User* pCurrUser)
{
	if (pSubject == NULL || pCurrUser == NULL)
		return;
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("Current subject: %s\n", pSubject->title);
		printf("Choose action: (1 - View Thread | 2 - Add a New Thread | 3 - Display Thread List | 0 - Exit Subject)\n");
		NUM_INPUT_CLEAN_BUFF(userChoice, buff);
		switch (userChoice)
		{
			case 1:
				if (pSubject->threadArrSize == 0)
					printf("There are no existing threads.\n");
				else
					chooseThread(pSubject, pCurrUser);
				break;
			case 2:
				{
					Thread* pNewThread = (Thread*)malloc(1 * sizeof(Thread));
					NULL_CHECK(pNewThread, );
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

void printSubjectWithThread(const Subject* pSubject)
{
	NULL_CHECK(pSubject, );
	printf("Subject: %s\n", pSubject->title);
	printf("Threads:\n");
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		printThread(pSubject->threadArr[i]);
	}
}

int saveSubjectToBFile(FILE* fp, const Subject* pSubject)
{
	if (fp == NULL || pSubject == NULL)
		return -1;
	int len = (int)strlen(pSubject->title) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pSubject->title, sizeof(char), len, fp) != len)
		return -1;
	if (fwrite(&pSubject->threadArrSize, sizeof(int), 1, fp) != 1)
		return -1;
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		if (saveThreadToBFile(fp, pSubject->threadArr[i]) == -1)
			return -1;
	}
	return 1;
}

int readSubjectFromBFile(FILE* fp, Subject* pSubject)
{
	if (fp == NULL || pSubject == NULL)
		return -1;
	pSubject->title = (char*)malloc(sizeof(char) * MAX_TITLE_NAME);
	NULL_CHECK(pSubject->title, -1);
	int len = 0;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return -1;
	if (len > MAX_TITLE_NAME)
		return -1;
	if (fread(pSubject->title, sizeof(char), len, fp) != len)
		return -1;
	int numOfThreads = 0;
	if (fread(&numOfThreads, sizeof(int), 1, fp) != 1)
		return -1;
	pSubject->threadArrSize = numOfThreads;
	pSubject->threadArr = (Thread**)malloc(sizeof(Thread*) * numOfThreads);
	NULL_CHECK(pSubject->threadArr, -1);
	for (int i = 0; i < numOfThreads; i++)
	{
		pSubject->threadArr[i] = (Thread*)malloc(1 * sizeof(Thread));
		NULL_CHECK(pSubject->threadArr[i], -1);
		if (readThreadFromBFile(fp, pSubject->threadArr[i]) == -1)
			return -1;
	}
	return 1;
}

int saveSubjectToTextFile(const Subject* pSubject, FILE* fp)
{
	if (fp == NULL || pSubject == NULL)
	{
		return -1;
	}
	fprintf(fp, "%s\n", pSubject->title);
	fprintf(fp, "%d\n", pSubject->threadArrSize);
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		saveThreadToTextFile(pSubject->threadArr[i], fp);
	}
	return 1;
}

int loadSubjectFromTextFile(Subject* pSubject, FILE* fp)
{
	if (fp == NULL || pSubject == NULL)
	{
		return -1;
	}
	pSubject->title = (char*)malloc(sizeof(char) * MAX_TITLE_NAME);
	(void)fscanf(fp, "%[^\n]%*c", pSubject->title);
	(void)fscanf(fp, "%d\n", &pSubject->threadArrSize);
	pSubject->threadArr = (Thread**)malloc(sizeof(Thread*) * pSubject->threadArrSize);
	NULL_CHECK(pSubject->threadArr, -1);
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		pSubject->threadArr[i] = (Thread*)malloc(sizeof(Thread));
		NULL_CHECK(pSubject->threadArr[i], -1);
		loadThreadFromTextFile(pSubject->threadArr[i], fp);
	}
	return 1;
}

void freeSubjectContent(Subject* pSubject)
{
	NULL_CHECK(pSubject, );
	free(pSubject->title);
	for (int i = 0; i < pSubject->threadArrSize; i++)
	{
		freeThread(pSubject->threadArr[i]);
	}
	free(pSubject->threadArr);
}

void freeSubject(Subject* pSubject)
{
	NULL_CHECK(pSubject, );
	freeSubjectContent(pSubject);
	free(pSubject);
}
