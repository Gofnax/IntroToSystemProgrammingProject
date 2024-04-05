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
	free(pTmpUser->name);
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
	pForum->currentUser = &pForum->userArr[userIndex];
	loadMsgHistory(pForum);
	printf("Logged in successfully as %s\n", pForum->currentUser->name);
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
	addUser(pTmpUser, pForum);
	free(pTmpUser);
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

int chooseSubject(LIST* pSubjectList, User* pCurrUser)
{
	int choice;
	char buff[2] = { 0 };
	printf("Choose a subject:\n");
	displaySubjectList(pSubjectList);
	(void)scanf("%d", &choice);
	(void)gets(buff);	// buffer cleaning
	if (choice < 1 || choice > L_size(pSubjectList))
	{
		printf("Invalid choice\n");
		return -1;
	}
	Subject* pSubject = (Subject*)(L_getAt(pSubjectList, choice - 1)->key);
	printSubject(pSubject);
	subjectActionsMenu(pSubject, pCurrUser);
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
	memcpy(&pForum->userArr[pForum->userArrSize], user, sizeof(User));
	//pForum->userArr[pForum->userArrSize] = *user;
	pForum->userArrSize++;
	printf("User %s added to forum\n", user->name);
	return 1;
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
		if (((strcmp(pCurrentUser->name, privateMsgBoxArr[i].user1->name) == 0 && strcmp(pUser->name, privateMsgBoxArr[i].user2->name) == 0)) ||
			((strcmp(pCurrentUser->name, privateMsgBoxArr[i].user2->name) == 0 && strcmp(pUser->name, privateMsgBoxArr[i].user1->name) == 0)))
		{
			return i;
		}
	}
	return -1;
}

void forumMainMenu(Forum* pForum)
{
	NULL_CHECK(pForum, );
	loginRegisterMenu(pForum);
	int userChoice = 0;
	char buff[2] = { 0 };
	do
	{
		printf("Choose the desired action:\n");
		printf("1 - View Forum Subjects\n2 - Start a Private Chat\n3 - View Your Message History\n0 - Save and Exit\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				chooseSubject(&pForum->subjectList, pForum->currentUser);
				break;
			case 2:
				choosePrivateChatPartner(pForum->currentUser, pForum);
				break;
			case 3:
				msgHistoryActionMenu(&pForum->currentUser->msgHistory);
				break;
			case 0:
				// save forum to binaray and text files
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

int choosePrivateChatPartner(User* pCurrentUser, Forum* pForum)
{
	User* pTmpUser = (User*)malloc(1 * sizeof(User));
	NULL_CHECK(pTmpUser, -1);

	pTmpUser->name = getStrExactName("Enter the partner's username:\n");
	int len = (int)strlen(pTmpUser->name) + 1;
	int partnerIndex = isUserInArr(pTmpUser, pForum->userArr, pForum->userArrSize);

	if (len > USERNAME_LEN || partnerIndex == -1)
	{
		printf("No user with such name.\n");
		return -1;
	}

	startPrivateConversation(pForum->currentUser, &pForum->userArr[partnerIndex], pForum);
	return 1;
}

void loginRegisterMenu(Forum* pForum)
{
	NULL_CHECK(pForum, );
	int userConnected = -1;
	int userChoice;
	char buff[2] = { 0 };
	do
	{
		printf("To login enter 1, to register enter 2:\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				userConnected = login(pForum);
				break;
			case 2:
				userConnected = registerUser(pForum);
				printf("Please log in.\n");
				userConnected = login(pForum);
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userConnected != 1 || userChoice < 1 || userChoice > 2);
}

void loadMsgHistory(Forum* pForum)
{
	NULL_CHECK(pForum, );
	int numOfSubjects = L_size(&pForum->subjectList);
	NODE* currSubject = &pForum->subjectList.head.next;
	int numOfMsgs = pForum->currentUser->msgHistory.numOfMsgs;
	int documentedMsgsCounter = 0;
	for (int i = 0; (i < numOfSubjects) && (documentedMsgsCounter == numOfMsgs); i++)	// go through all the subjects
	{
		for (int j = 0; (j < ((Subject*)currSubject->key)->threadArrSize) && (documentedMsgsCounter == numOfMsgs); j++)	// go through all the threads
		{
			Thread* currThread = ((Subject*)currSubject->key)->threadArr[j];
			for (int k = 0; (k < currThread->messageArrSize) && (documentedMsgsCounter == numOfMsgs); k++)	// go through all the messages
			{
				Message* currMsg = &currThread->messageArr[k];
				if (strcmp(currMsg->authorName, pForum->currentUser->name) == 0)
				{
					documentMsg(&pForum->currentUser->msgHistory, currMsg);
					documentedMsgsCounter++;
				}
			}
		}
		currSubject = currSubject->next;
	}
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

