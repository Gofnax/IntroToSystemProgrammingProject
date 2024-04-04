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
	return 1;
}

int login(User* user, Forum* pForum)
{
	char tempPW[MAX_STR_LEN];
	if (isUserInArr(user, pForum->userArr, pForum->userArrSize) != 1)
	{
		printf("User not found\n");
		return -1;
	}
	int counter = 0;
	int tries = 3;
	while (counter < 3 && tries>0)
	{
		printf("Enter your Password max %d characters:\n", PW_LEN - 1);
		fgets(tempPW, MAX_STR_LEN, stdin);
		cleanNewlineChar(tempPW);
		if (isSamePassword(user, tempPW) != 0)
		{
			printf("Incorrect password try again, you have %d tries left\n", tries - 1);
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
	pForum->currentUser = user;
	printf("Logged in successfully %s\n", user->name);
	//addUser(user, pForum);
	return 1;
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
			return 1;
		}
	}
	return 0;
}

int addUser(User* user, Forum* pForum)
{
	User* temp = (User*)realloc(pForum->userArr, sizeof(User) * (pForum->userArrSize + 1));
	if (temp == NULL)
	{
		return -1;
	}
	pForum->userArr = temp;
	pForum->userArr[pForum->userArrSize] = *user;
	pForum->userArrSize++;
	printf("User %s added to forum\n", user->name);
	return 1;
}

void freeForumContent(Forum* pForum)
{
	free(pForum->userArr);
}

void freeForum(Forum* pForum)
{
	freeForumContent(pForum);
	free(pForum);
}

void startPrivateConversation(User* currentUser, User* user, Forum* pForum)
{
	PrivateMsgBox privateBox;
	if (doesMsgBoxExist(currentUser, user, pForum->privateMsgBoxArr, pForum->privateMsgBoxArrSize) == 1)
	{
		printPrivateMsgs(&privateBox);
		return;
	}
	else
	{
		initPrivateMsgBox(&privateBox, currentUser, user);
		printPrivateMsgs(&privateBox);
		return;
	}

}

int doesMsgBoxExist(User* currentUser, User* user, PrivateMsgBox* privateMsgBoxArr, int privateMsgBoxArrSize)
{
	for (int i = 0; i < privateMsgBoxArrSize; i++)
	{
		if ((strcmp(currentUser->name, privateMsgBoxArr[i].user1->name) == 0 && strcmp(user->name, privateMsgBoxArr[i].user2->name) == 0) ||
			(strcmp(currentUser->name, privateMsgBoxArr[i].user2->name) == 0 && strcmp(user->name, privateMsgBoxArr[i].user1->name) == 0))
		{
			return 1;
		}
	}
	return 0;
}
