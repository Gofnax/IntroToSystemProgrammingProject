#include "PrivateMsgBox.h"

int initPrivateMsgBox(PrivateMsgBox* pPrivateBox, User* pUser1, User* pUser2)
{
	if (pPrivateBox == NULL || pUser1 == NULL || pUser2 == NULL)
		return -1;
	pPrivateBox->user1 = pUser1;
	pPrivateBox->user2 = pUser2;
	pPrivateBox->numOfMsgs = 0;
	pPrivateBox->messageArr = (Message*)malloc(1 * sizeof(Message));
	if (pPrivateBox->messageArr == NULL)
		return -1;
	return 1;
}

void privateMsgBoxMenu(PrivateMsgBox* pPrivateBox, User* pUser)
{
	if (pPrivateBox == NULL)
		return;
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("Choose action: (1 - Display Messages | 2 - Write a Message | 0 - Exit Private Messages\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
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
				break;
		}
	} while (userChoice != 0);
}

int writeMessage(PrivateMsgBox* pPrivateBox, User* pUser)
{
	if (pPrivateBox == NULL || pUser == NULL)
		return -1;
	Message* tmpArr = (Message*)realloc(pPrivateBox->messageArr, (pPrivateBox->numOfMsgs + 1) * sizeof(Message));
	if (tmpArr == NULL)
		return -1;
	pPrivateBox->messageArr = tmpArr;
	int index = pPrivateBox->numOfMsgs;
	Message* destMsg = pPrivateBox->messageArr + index;
	(void)createMsg(destMsg, pUser->name);
	pPrivateBox->numOfMsgs++;
	return 1;
}

void printPrivateMsgs(PrivateMsgBox* pPrivateBox)
{
	if (pPrivateBox == NULL)
		return;
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		printMsg(&pPrivateBox->messageArr[i]);
	}
}

void freePrivateMsgBoxContents(PrivateMsgBox* pPrivateBox)
{
	if (pPrivateBox == NULL)
		return;
	for (int i = 0; i < pPrivateBox->numOfMsgs; i++)
	{
		freeMessageContents(&pPrivateBox->messageArr[i]);
		free(&pPrivateBox->messageArr[i]);
	}
}

void freePrivateMsgBox(PrivateMsgBox* pPrivateBox)
{
	freePrivateMsgBoxContents(pPrivateBox);
	free(pPrivateBox);
}