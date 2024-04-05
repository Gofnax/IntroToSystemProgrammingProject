#include "Forum.h"

int initForum(Forum* pForum)
{
	pForum->userArr = (User*)malloc(sizeof(User) * 1);
	if (pForum->userArr == NULL)
	{
		return -1;
	}
	pForum->userArrSize = 0;
	pForum->currentUser = NULL;
	L_init(&pForum->subjectList);
	pForum->privateMsgBoxArr = (PrivateMsgBox*)malloc(1 * sizeof(PrivateMsgBox));
	if (pForum->privateMsgBoxArr == NULL)
		return -1;
	pForum->privateMsgBoxArrSize = 0;
	return 1;
}

int login(Forum* pForum)
{
	char tempUserName[MAX_STR_LEN];
	char tempPW[MAX_STR_LEN];
	User* pTmpUser = (User*)malloc(1 * sizeof(User));
	if (pTmpUser == NULL)
		return -1;

	printf("Enter your username: \n");
	fgets(tempUserName, MAX_STR_LEN, stdin);
	cleanNewlineChar(tempUserName);
	pTmpUser->name = (char*)malloc(USERNAME_LEN * sizeof(char));
	if (pTmpUser->name == NULL)
		return -1;
	strncpy(pTmpUser->name, tempUserName, USERNAME_LEN);

	int userIndex = isUserInArr(pTmpUser, pForum->userArr, pForum->userArrSize);
	if (userIndex == -1)
	{
		printf("User not found\n");
		return -1;
	}
	freeUserContents(pTmpUser);	// to free the allocated space for the name we got from the user
	free(pTmpUser);
	pTmpUser = &pForum->userArr[userIndex];

	int counter = 0;
	int tries = 3;
	while (counter < 3 && tries>0)
	{
		printf("Enter your password:\n");
		fgets(tempPW, MAX_STR_LEN, stdin);
		cleanNewlineChar(tempPW);
		if (isSamePassword(pTmpUser, tempPW) != 0)
		{
			printf("Incorrect password, try again: (%d tries left)\n", tries - 1);
			counter++;
			tries--;
		}
		else break;
	}
	if (counter == 3)
	{
		printf("Too many tries, exiting program\n");
		return -1;
	}
	pForum->currentUser = pTmpUser;
	printf("Logged in successfully %s\n", pTmpUser->name);
	return 1;
}

int registerUser(Forum* pForum)
{
	User* pTmpUser = (User*)malloc(1 * sizeof(User));
	if (pTmpUser == NULL)
		return -1;

	int isNameTaken = 0;
	do
	{
		initUserName(pTmpUser);
		isNameTaken = isUserInArr(pTmpUser, pForum->userArr, pForum->userArrSize);
		if (isNameTaken != -1)
		{
			printf("Username already taken.\n");
			freeUserContents(pTmpUser);
		}
	} while (isNameTaken != -1);

	initUserPassword(pTmpUser);
	memcpy(&pForum->userArr[pForum->userArrSize - 1], pTmpUser, sizeof(User));
	pForum->userArrSize++;
	freeUserContents(pTmpUser);
	free(pTmpUser);
}

void displayMsgHistory(User* user)
{
	printf("Displaying message history for %s\n", user->name);
	for (int i = 0; i < user->msgHistory.numOfMsgs; i++)
	{
		printf("Message %d: %s\n", i + 1, user->msgHistory.msgHistory[i]->msgText);
	}
}

void displaySubjectList(LIST* pSubjectList)
{
	//printf("Displaying subject list\n");
	L_print(pSubjectList, printSubject);
}

int chooseSubject(LIST* pSubjectList)
{
	int choice;
	printf("Choose a subject:\n");
	displaySubjectList(pSubjectList);
	(void)scanf("%d", &choice);
	if (choice < 1 || choice > L_size(pSubjectList))
	{
		printf("Invalid choice\n");
		return -1;
	}
	Subject* pSubject = (Subject*)L_getAt(pSubjectList, choice - 1);
	printSubject(pSubject);
	return 1;
}

int addSubject(LIST* pSubjectList)
{
	Subject* pSubject = (Subject*)malloc(sizeof(Subject));
	if (pSubject == NULL)
	{
		return -1;
	}
	if (initSubject(pSubject) != 1)
	{
		return -1;
	}
	L_insert(&pSubjectList->head, pSubject);
	return 1;
}

int isUserInArr(User* user, User* userArr, int userArrSize)
{
	for (int i = 0; i < userArrSize; i++)
	{
		if (strcmp(user->name, userArr[i].name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int addUser(User* user, Forum* pForum)
{
	User* temp = (User*)realloc(pForum->userArr, sizeof(User) * (pForum->userArrSize + 1));
	if (temp == NULL)
	{
		return -1;
	}
	pForum->userArr = temp;
	pForum->userArr[pForum->userArrSize - 1] = *user;
	pForum->userArrSize++;
	printf("User %s added to forum\n", user->name);
	return 1;
}

void freeForumContent(Forum* pForum)
{
	if (pForum == NULL)
		return;
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		freeUserContents(&pForum->userArr[i]);
	}
	free(pForum->userArr);
	for (int i = 0; i < pForum->privateMsgBoxArrSize; i++)
	{
		freePrivateMsgBoxContents(&pForum->privateMsgBoxArr[i]);
	}
	free(pForum->privateMsgBoxArr);
	L_free(&pForum->subjectList, freeSubjectContent);
}

void freeForum(Forum* pForum)
{
	freeForumContent(pForum);
	free(pForum);
}

void startPrivateConversation(User* pCurrentUser, User* pUser, Forum* pForum)
{
	if (pCurrentUser == NULL || pUser == NULL || pForum == NULL)
		return;
	PrivateMsgBox* pPrivateBox;
	int privateBoxIndex = doesMsgBoxExist(pCurrentUser, pUser, pForum->privateMsgBoxArr, pForum->privateMsgBoxArrSize);
	if (privateBoxIndex != -1)
	{
		pPrivateBox = &pForum->privateMsgBoxArr[privateBoxIndex];
		printPrivateMsgs(pPrivateBox);
	}
	else
	{
		PrivateMsgBox* tmp = (PrivateMsgBox*)realloc(pForum->privateMsgBoxArr, (pForum->privateMsgBoxArrSize + 1) * sizeof(PrivateMsgBox));
		if (tmp == NULL)
			return;
		pForum->privateMsgBoxArr = tmp;
		pPrivateBox = &pForum->privateMsgBoxArr[pForum->privateMsgBoxArrSize - 1];
		pForum->privateMsgBoxArrSize++;
		initPrivateMsgBox(pPrivateBox, pCurrentUser, pUser);
	}
	privateMsgBoxMenu(pPrivateBox, pCurrentUser);
}

int doesMsgBoxExist(User* pCurrentUser, User* pUser, PrivateMsgBox* privateMsgBoxArr, int privateMsgBoxArrSize)
{
	for (int i = 0; i < privateMsgBoxArrSize; i++)
	{
		if ((strcmp(pCurrentUser->name, privateMsgBoxArr[i].user1->name) == 0 && strcmp(pUser->name, privateMsgBoxArr[i].user2->name) == 0) ||
			(strcmp(pCurrentUser->name, privateMsgBoxArr[i].user2->name) == 0 && strcmp(pUser->name, privateMsgBoxArr[i].user1->name) == 0))
		{
			return i;
		}
	}
	return -1;
}
