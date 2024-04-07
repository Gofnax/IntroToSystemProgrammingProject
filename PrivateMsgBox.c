#include "PrivateMsgBox.h"

int initPrivateMsgBox(PrivateMsgBox* pPrivateBox, User* pUser1, User* pUser2)
{
	if (pPrivateBox == NULL || pUser1 == NULL || pUser2 == NULL)
		return -1;
	pPrivateBox->user1 = pUser1;
	int len = (int)strlen(pUser1->name) + 1;
	pPrivateBox->userName1 = (char*)malloc(len * sizeof(char));
	if (pPrivateBox->userName1 == NULL)
		return -1;
	strncpy(pPrivateBox->userName1, pUser1->name, len);
	pPrivateBox->user2 = pUser2;
	len = (int)strlen(pUser2->name) + 1;
	pPrivateBox->userName2 = (char*)malloc(len * sizeof(char));
	if (pPrivateBox->userName2 == NULL)
		return -1;
	strncpy(pPrivateBox->userName2, pUser2->name, len);	pPrivateBox->numOfMsgs = 0;
	pPrivateBox->messageArr = (Message*)malloc(1 * sizeof(Message));
	if (pPrivateBox->messageArr == NULL)
		return -1;
	return 1;
}

void privateMsgBoxMenu(PrivateMsgBox* pPrivateBox, User* pUser)
{
	NULL_CHECK(pPrivateBox, );
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("Choose action: (1 - Display Messages | 2 - Write a Message | 0 - Exit Private Messages)\n");
		NUM_INPUT_CLEAN_BUFF(userChoice, buff);
		switch (userChoice)
		{
			case 1:
				printPrivateMsgs(pPrivateBox);
				break;
			case 2:
				writeMessage(pPrivateBox, pUser);
				break;
			case 0:
				printf("Returning to main menu.\n");
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

int writeMessage(PrivateMsgBox* pPrivateBox, User* pUser)
{
	if (pPrivateBox == NULL || pUser == NULL)
		return -1;
	Message* tmpArr = (Message*)realloc(pPrivateBox->messageArr, (pPrivateBox->numOfMsgs + 1) * sizeof(Message));
	NULL_CHECK(tmpArr, -1);
	pPrivateBox->messageArr = tmpArr;
	int index = pPrivateBox->numOfMsgs;
	Message* destMsg = pPrivateBox->messageArr + index;
	(void)createMsg(destMsg, pUser->name);
	pPrivateBox->numOfMsgs++;
	return 1;
}

void printPrivateMsgs(PrivateMsgBox* pPrivateBox)
{
	NULL_CHECK(pPrivateBox, );
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		printMsg(&pPrivateBox->messageArr[i]);
	}
}

int savePrivateMsgBoxToBFile(FILE* fp, const PrivateMsgBox* pPrivateBox)
{
	if (fp == NULL || pPrivateBox == NULL)
		return -1;
	int len = (int)strlen(pPrivateBox->userName1) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pPrivateBox->userName1, sizeof(char), len, fp) != len)
		return -1;
	len = (int)strlen(pPrivateBox->userName2) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pPrivateBox->userName2, sizeof(char), len, fp) != len)
		return -1;
	if (fwrite(&pPrivateBox->numOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		if (saveMsgToBFile(fp, &pPrivateBox->messageArr[i]) == -1)
			return -1;
	}
	return 1;
}

int readPrivateMsgBoxFromBFile(FILE* fp, PrivateMsgBox* pPrivateBox)
{
	if (fp == NULL || pPrivateBox == NULL)
		return -1;
	int len = 0;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return -1;
	pPrivateBox->userName1 = (char*)malloc(len * sizeof(char));
	NULL_CHECK(pPrivateBox->userName1, -1);
	if (fread(pPrivateBox->userName1, sizeof(char), len, fp) != len)
		return -1;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return -1;
	pPrivateBox->userName2 = (char*)malloc(len * sizeof(char));
	NULL_CHECK(pPrivateBox->userName2, -1);
	if (fread(pPrivateBox->userName2, sizeof(char), len, fp) != len)
		return -1;
	if (fread(&pPrivateBox->numOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	pPrivateBox->messageArr = (Message*)malloc((pPrivateBox->numOfMsgs) * sizeof(Message));
	NULL_CHECK(pPrivateBox->messageArr, -1);
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		if(readMsgFromBFile(fp, &pPrivateBox->messageArr[i]) == -1)
			return -1;
	}
	pPrivateBox->user1 = NULL;
	pPrivateBox->user2 = NULL;
	return 1;
}

int savePrivateMsgBoxToTextFile(const PrivateMsgBox* privateMsgBox, FILE* fp)
{
	if (fp == NULL)
	{
		return -1;
	}
	fprintf(fp, "%s\n", privateMsgBox->user1->name);
	fprintf(fp, "%s\n", privateMsgBox->user2->name);
	fprintf(fp, "%d\n", privateMsgBox->numOfMsgs);
	for (int i = 0; i < privateMsgBox->numOfMsgs; i++)
	{
		saveMessageToTextFile(&privateMsgBox->messageArr[i], fp);
	}
	return 1;
}

int loadPrivateMsgBoxFromTextFile(PrivateMsgBox* privateMsgBox, FILE* fp)
{
	if (fp == NULL)
	{
		return -1;
	}
	(void)fscanf(fp, "%s\n", privateMsgBox->user1->name);
	(void)fscanf(fp, "%s\n", privateMsgBox->user2->name);
	(void)fscanf(fp, "%d\n", &privateMsgBox->numOfMsgs);
	for (int i = 0; i < privateMsgBox->numOfMsgs; i++)
	{
		Message* message = (Message*)malloc(sizeof(Message));
		if (message == NULL)
		{
			return -1;
		}
		if (loadMessageFromTextFile(message, fp) != 1)
		{
			return -1;
		}
		privateMsgBox->messageArr[i] = *message;
	}
	return 1;
}

void freePrivateMsgBoxContents(PrivateMsgBox* pPrivateBox)
{
	NULL_CHECK(pPrivateBox, );
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		freeMessageContents(&pPrivateBox->messageArr[i]);
	}
	free(pPrivateBox->userName1);
	free(pPrivateBox->userName2);
}

void freePrivateMsgBox(PrivateMsgBox* pPrivateBox)
{
	NULL_CHECK(pPrivateBox, );
	freePrivateMsgBoxContents(pPrivateBox);
	free(pPrivateBox);
}