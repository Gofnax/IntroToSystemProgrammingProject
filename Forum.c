#include "Forum.h"

int initForum(Forum* pForum)
{
	NULL_CHECK(pForum, -1);
	pForum->userArr = (User*)malloc(sizeof(User) * 1);
	NULL_CHECK(pForum->userArr, -1);
	pForum->userArrSize = 0;
	pForum->currentUser = NULL;
	L_init(&pForum->subjectList);
	pForum->privateMsgBoxArr = (PrivateMsgBox*)malloc(1 * sizeof(PrivateMsgBox));
	NULL_CHECK(pForum->privateMsgBoxArr, -1);
	pForum->privateMsgBoxArrSize = 0;
	return 1;
}

int login(Forum* pForum)
{
	NULL_CHECK(pForum, -1);
	char tempUserName[MAX_STR_LEN];
	char tempPW[MAX_STR_LEN];
	User* pTmpUser = (User*)malloc(1 * sizeof(User));
	NULL_CHECK(pTmpUser, -1);

	printf("Enter your username: \n");
	USER_INPUT(tempUserName);
	pTmpUser->name = (char*)malloc(USERNAME_LEN * sizeof(char));
	NULL_CHECK(pTmpUser->name, -1);
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
		USER_INPUT(tempPW);
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
	NULL_CHECK(pForum, -1);
	User* pTmpUser = (User*)malloc(1 * sizeof(User));
	NULL_CHECK(pTmpUser, -1);

	int isNameTaken = 0;
	do
	{
		initUserName(pTmpUser);
		isNameTaken = isUserInArr(pTmpUser, pForum->userArr, pForum->userArrSize);
		if (isNameTaken != -1)
		{
			printf("Username already taken.\n");
		}
	} while (isNameTaken != -1);

	initUserPassword(pTmpUser);
	addUser(pTmpUser, pForum);
	free(pTmpUser);
	return 1;
}

void displayMsgHistory(const User* user)
{
	NULL_CHECK(user, );
	printf("Displaying message history for %s\n", user->name);
	for (int i = 0; i < user->msgHistory.numOfMsgs; i++)
	{
		printf("Message %d: %s\n", i + 1, user->msgHistory.msgHistory[i]->msgText);
	}
}

void displaySubjectList(const LIST* pSubjectList)
{
	NULL_CHECK(pSubjectList, );
	L_print(pSubjectList, printSubjectTitle);
}

int chooseSubject(LIST* pSubjectList, User* pCurrUser)
{
	if (pSubjectList == NULL || pCurrUser == NULL)
		return -1;
	int choice, subjectChoice;
	char buff[2] = { 0 };
	Subject* pSubject;
	do
	{
		if (L_size(pSubjectList) > 0)
		{
			printf("The subjects are:\n");
			displaySubjectList(pSubjectList);
			printf("Choose action: (1 - Open Subject | 2 - Add a New Subject | 0 - Exit)\n");
			NUM_INPUT_CLEAN_BUFF(choice, buff);
		}
		else
		{
			printf("There are no existing subjects. Creating a new one:\n");
			choice = 2;
		}
		switch (choice)
		{
			case 1:
				printf("Choose the subject:\n");
				NUM_INPUT_CLEAN_BUFF(subjectChoice, buff);
				if (subjectChoice < 1 || subjectChoice > L_size(pSubjectList))
				{
					printf("Invalid choice.\n");
				}
				pSubject = (Subject*)(L_getAt(pSubjectList, subjectChoice - 1)->key);
				printSubject(pSubject);
				subjectActionsMenu(pSubject, pCurrUser);
				choice = 1;
				break;
			case 2:
				addSubject(pSubjectList);
				pSubject = (Subject*)(pSubjectList->head.next->key);
				subjectActionsMenu(pSubject, pCurrUser);
				break;
			case 0:
				printf("Returning to main menu.\n");
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (choice != 0);
	
	return 1;
}

int addSubject(LIST* pSubjectList)
{
	NULL_CHECK(pSubjectList, -1);
	Subject* pSubject = (Subject*)malloc(sizeof(Subject));
	NULL_CHECK(pSubject, -1);
	if (initSubject(pSubject) != 1)
	{
		return -1;
	}
	L_insert(&pSubjectList->head, pSubject);
	return 1;
}

int isUserInArr(const User* user, const User* userArr, int userArrSize)
{
	if (user == NULL || userArr == NULL)
		return -1;
	for (int i = 0; i < userArrSize; i++)
	{
		if (strcmp(user->name, userArr[i].name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int addUser(const User* user, Forum* pForum)
{
	if (user == NULL || pForum == NULL)
		return -1;
	User* temp = (User*)realloc(pForum->userArr, sizeof(User) * (pForum->userArrSize + 1));
	NULL_CHECK(temp, -1);
	pForum->userArr = temp;
	memcpy(&pForum->userArr[pForum->userArrSize], user, sizeof(User));
	pForum->userArrSize++;
	return 1;
}

void startPrivateConversation(const User* pCurrentUser, const User* pUser, Forum* pForum)
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
		pPrivateBox = &pForum->privateMsgBoxArr[pForum->privateMsgBoxArrSize];
		pForum->privateMsgBoxArrSize++;
		initPrivateMsgBox(pPrivateBox, pCurrentUser, pUser);
	}
	privateMsgBoxMenu(pPrivateBox, pCurrentUser);
}

int doesMsgBoxExist(const User* pCurrentUser, const User* pUser, const PrivateMsgBox* privateMsgBoxArr, int privateMsgBoxArrSize)
{
	if (pCurrentUser == NULL || pUser == NULL || privateMsgBoxArr == NULL)
		return -1;
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
	FILE* fp;
	do
	{
		printf("Choose the desired action:\n");
		printf("1 - View Forum Subjects\n2 - Start a Private Chat\n3 - View Your Message History\n");
		printf("4 - Display User with Most Messages\n5 - Print the WHOLE Forum\n0 - Save and Exit\n");
		NUM_INPUT_CLEAN_BUFF(userChoice, buff);
		int check;
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
			case 4:
				displayMostActiveUser(pForum);
				break;
			case 5:
				printForum(pForum);
				break;
			case 0:
				fp = fopen(SYSTEM_TEXT_FILE, "w");
				saveForumToTextFile(pForum, fp);
				fclose(fp);
				fp = fopen(SYSTEM_BIN_FILE, "wb");
				saveForumToBFile(fp, pForum);
				fclose(fp);
				freeForumContent(pForum);
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

void displayMostActiveUser(const Forum* pForum)
{
	NULL_CHECK(pForum, );
	if (pForum->userArrSize == 0)
	{
		printf("No users in the system.\n");
		return;
	}
	User* mostActive = &pForum->userArr[0];
	for (int i = 1; i < pForum->userArrSize; i++)
	{
		if (pForum->userArr[i].msgHistory.numOfMsgs > mostActive->msgHistory.numOfMsgs)
			mostActive = &pForum->userArr[i];
	}
	printf("The user with the most messages is: %s\n", mostActive->name);
	printf("They have posted %d messages!\n", mostActive->msgHistory.numOfMsgs);
}

int choosePrivateChatPartner(User* pCurrentUser, Forum* pForum)
{
	if (pCurrentUser == NULL || pForum == NULL)
		return -1;
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
	else if (strcmp(pCurrentUser->name, pTmpUser->name) == 0)
	{
		printf("Can't chat with yourself.\n");
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
		NUM_INPUT_CLEAN_BUFF(userChoice, buff);
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
	NODE* currSubjectNode = pForum->subjectList.head.next;
	int numOfMsgs = pForum->currentUser->msgHistory.numOfMsgs;
	int documentedMsgsCounter = 0;
	for (int i = 0; (i < numOfSubjects) && (documentedMsgsCounter < numOfMsgs); i++)	// go through all the subjects
	{
		Subject* pCurrSubject = (Subject*)currSubjectNode->key;
		for (int j = 0; (j < pCurrSubject->threadArrSize) && (documentedMsgsCounter < numOfMsgs); j++)	// go through all the threads
		{
			Thread* currThread = pCurrSubject->threadArr[j];
			for (int k = 0; (k < currThread->messageArrSize) && (documentedMsgsCounter < numOfMsgs); k++)	// go through all the messages
			{
				Message* currMsg = &currThread->messageArr[k];
				if (strcmp(currMsg->authorName, pForum->currentUser->name) == 0)
				{
					documentMsgFromFile(&pForum->currentUser->msgHistory, currMsg, documentedMsgsCounter);
					documentedMsgsCounter++;
				}
			}
		}
		currSubjectNode = currSubjectNode->next;
	}
}

void printForum(const Forum* pForum)
{
	NULL_CHECK(pForum, );
	printf("Forum:\n");
	printf("Subjects:\n");
	printSubjectList(&pForum->subjectList);
	printf("Users:\n");
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		printUser(&pForum->userArr[i]);
	}
	printf("Private message boxes:\n");
	for (int i = 0; i < pForum->privateMsgBoxArrSize; i++)
	{
		printPrivateMsgBox(&pForum->privateMsgBoxArr[i]);
	}

}

void printSubjectList(const LIST* pSubjectList)
{
	NULL_CHECK(pSubjectList, );
	L_print(pSubjectList, printSubjectWithThread);

}

int saveForumToBFile(FILE* fp, const Forum* pForum)
{
	if(fp == NULL || pForum == NULL)
		return -1;
	int numOfSubjects = L_size(&pForum->subjectList);
	if (fwrite(&numOfSubjects, sizeof(int), 1, fp) != 1)
		return -1;
	NODE* currSubject = pForum->subjectList.head.next;
	for (int i = 0; i < numOfSubjects && currSubject != NULL; i++)
	{
		if (saveSubjectToBFile(fp, (Subject*)(currSubject->key)) == -1)
			return -1;
		currSubject = currSubject->next;
	}
	if (fwrite(&pForum->userArrSize, sizeof(int), 1, fp) != 1)
		return -1;
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		if (saveUserToBFile(fp, &pForum->userArr[i]) == -1)
			return -1;
	}
	if (fwrite(&pForum->privateMsgBoxArrSize, sizeof(int), 1, fp) != 1)
		return -1;
	for (int i = 0; i < pForum->privateMsgBoxArrSize; i++)
	{
		if (savePrivateMsgBoxToBFile(fp, &pForum->privateMsgBoxArr[i]) == -1)
			return -1;
	}
	return 1;
}

int readForumFromBFile(FILE* fp, Forum* pForum)
{
	if (fp == NULL || pForum == NULL)
		return -1;
	L_init(&pForum->subjectList);
	int numOfSubjects = 0;
	if (fread(&numOfSubjects, sizeof(int), 1, fp) != 1)
		return -1;
	NODE* lastSubjectNode = &pForum->subjectList.head;
	for (int i = 0; i < numOfSubjects; i++)
	{
		Subject* currSubject = (Subject*)malloc(1 * sizeof(Subject));
		NULL_CHECK(currSubject, -1);
		if (readSubjectFromBFile(fp, currSubject) == -1)
			return -1;
		L_insert(lastSubjectNode, currSubject);
		lastSubjectNode = lastSubjectNode->next;
	}

	if (fread(&pForum->userArrSize, sizeof(int), 1, fp) != 1)
		return -1;
	pForum->userArr = (User*)malloc((pForum->userArrSize) * sizeof(User));
	NULL_CHECK(pForum->userArr, -1);
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		if (readUserFromBFile(fp, &pForum->userArr[i]) == -1)
			return -1;
	}

	if (fread(&pForum->privateMsgBoxArrSize, sizeof(int), 1, fp) != 1)
		return -1;
	pForum->privateMsgBoxArr = (PrivateMsgBox*)malloc((pForum->privateMsgBoxArrSize) * sizeof(PrivateMsgBox));
	NULL_CHECK(pForum->privateMsgBoxArr, -1);
	for (int i = 0; i < pForum->privateMsgBoxArrSize; i++)
	{
		if (readPrivateMsgBoxFromBFile(fp, &pForum->privateMsgBoxArr[i]) == -1)
			return -1;
		for (int j = 0; j < pForum->userArrSize; j++)
		{
			if (strcmp(pForum->userArr[j].name, pForum->privateMsgBoxArr[i].userName1) == 0)
				pForum->privateMsgBoxArr[i].user1 = &pForum->userArr[j];
			else if(strcmp(pForum->userArr[j].name, pForum->privateMsgBoxArr[i].userName2) == 0)
				pForum->privateMsgBoxArr[i].user2 = &pForum->userArr[j];
		}
	}

	pForum->currentUser = NULL;
	return 1;
}

int saveForumToTextFile(const Forum* pForum, FILE* fp)
{
	if (fp == NULL || pForum == NULL)
	{
		return -1;
	}
	if (saveSubjectListToTextFile(pForum->subjectList, fp) != 1)
	{
		return -1;
	}
	fprintf(fp, "%d\n", pForum->userArrSize);
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		saveUserToTextFile(&pForum->userArr[i], fp);
	}
	if (savePrivateMsgBoxArrToTextFile(pForum->privateMsgBoxArr, pForum->privateMsgBoxArrSize, fp) != 1)
	{
		return -1;
	}
	return 1;
}

int saveSubjectListToTextFile(const LIST subjectList, FILE* fp)
{
	if (fp == NULL)
	{
		return -1;
	}
	fprintf(fp, "%d\n", L_size(&subjectList));
	int size = L_size(&subjectList);
	NODE* tmp = subjectList.head.next;
	for (int i = 0; i < size; i++)
	{
		saveSubjectToTextFile((Subject*)tmp->key, fp);
		tmp = tmp->next;
	}
	return 1;
}

int savePrivateMsgBoxArrToTextFile(const PrivateMsgBox* privateMsgBoxArr, const int privateMsgBoxArrSize, FILE* fp)
{
	if (fp == NULL || privateMsgBoxArr == NULL)
	{
		return -1;
	}
	fprintf(fp, "%d\n", privateMsgBoxArrSize);
	for (int i = 0; i < privateMsgBoxArrSize; i++)
	{
		savePrivateMsgBoxToTextFile(&privateMsgBoxArr[i], fp);
	}
	return 1;
}

int loadForumFromTextFile(Forum* pForum, FILE* fp)
{
	if (fp == NULL || pForum == NULL)
	{
		return -1;
	}
	L_init(&pForum->subjectList);
	if (loadSubjectListFromTextFile(&pForum->subjectList, fp) != 1)
	{
		return -1;
	}
	(void)fscanf(fp, "%d\n", &pForum->userArrSize);
	pForum->userArr = (User*)malloc(sizeof(User)*pForum->userArrSize);
	NULL_CHECK(pForum->userArr, -1);
	for (int i = 0; i < pForum->userArrSize; i++)
	{
		User* user = (User*)malloc(sizeof(User));
		NULL_CHECK(user, -1);
		if (loadUserFromTextFile(user, fp) != 1)
		{
			return -1;
		}
		pForum->userArr[i] = *user;
		pForum->userArr[i].msgHistory.msgHistory = (Message**)malloc(sizeof(Message*) * pForum->userArr[i].msgHistory.maxNumOfMsgs);
	}
	pForum->currentUser=(User*)malloc(sizeof(User));
	pForum->currentUser = NULL;
	pForum->privateMsgBoxArr = (PrivateMsgBox*)malloc(sizeof(PrivateMsgBox));
	NULL_CHECK(pForum->privateMsgBoxArr, -1);
	if (loadPrivateMsgBoxArrFromTextFile(pForum->privateMsgBoxArr, pForum, fp) != 1)
	{
		return -1;
	}
	return 1;
}

int loadSubjectListFromTextFile(LIST* subjectList, FILE* fp)
{
	if (fp == NULL || subjectList == NULL)
	{
		return -1;
	}
	int size;
	(void)fscanf(fp, "%d\n", &size);
	for (int i = 0; i < size; i++)
	{
		Subject* pSubject = (Subject*)malloc(sizeof(Subject));
		NULL_CHECK(pSubject, -1);
		if (loadSubjectFromTextFile(pSubject, fp) != 1)
		{
			return -1;
		}
		L_insert(&subjectList->head, pSubject);
	}
	return 1;
}

int loadPrivateMsgBoxArrFromTextFile(PrivateMsgBox* privateMsgBoxArr, Forum* pForum, FILE* fp)
{
	if (fp == NULL || privateMsgBoxArr == NULL || pForum == NULL)
	{
		return -1;
	}
	(void)fscanf(fp, "%d\n", &pForum->privateMsgBoxArrSize);
	for (int i = 0; i < pForum->privateMsgBoxArrSize; i++)
	{
		PrivateMsgBox* privateMsgBox = (PrivateMsgBox*)malloc(sizeof(PrivateMsgBox));
		NULL_CHECK(privateMsgBox, -1);
		if (loadPrivateMsgBoxFromTextFile(privateMsgBox, fp) != 1)
		{
			return -1;
		}
		privateMsgBoxArr[i] = *privateMsgBox;
		for (int j = 0; j < pForum->userArrSize; j++)
		{
			if (strcmp(pForum->userArr[j].name, pForum->privateMsgBoxArr[i].userName1) == 0)
				pForum->privateMsgBoxArr[i].user1 = &pForum->userArr[j];
			else if (strcmp(pForum->userArr[j].name, pForum->privateMsgBoxArr[i].userName2) == 0)
				pForum->privateMsgBoxArr[i].user2 = &pForum->userArr[j];
		}
	}
	return 1;
}

void freeForumContent(Forum* pForum)
{
	NULL_CHECK(pForum, );
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
	L_free(&pForum->subjectList, freeSubject);
}

void freeForum(Forum* pForum)
{
	NULL_CHECK(pForum, );
	freeForumContent(pForum);
	free(pForum);
}

