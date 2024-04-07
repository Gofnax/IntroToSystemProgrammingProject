#include "UserMsgHistory.h"

static const char* SortTypeStr[eNofSorts]
		= { "Not Sorted", "Sorted by Time of Writing", "Sorted by Number of Likes", "Sorted by Length of Text"};

void initMsgHistory(UserMsgHistory* pHistory)
{
	pHistory->msgHistory = (Message**)calloc(HISTORY_GROWTH_SIZE, sizeof(Message*));
	if (pHistory->msgHistory == NULL)
		return;
	pHistory->numOfMsgs = 0;
	pHistory->maxNumOfMsgs = HISTORY_GROWTH_SIZE;
	pHistory->currentSort = eNoSort;
}

int documentMsg(UserMsgHistory* pHistory, Message* pMsg)
{
	if (pHistory == NULL || pMsg == NULL)
		return -1;
	if (pHistory->numOfMsgs == pHistory->maxNumOfMsgs)
	{
		Message** tmpArr = (Message**)realloc(pHistory->msgHistory, (pHistory->numOfMsgs + HISTORY_GROWTH_SIZE) * sizeof(Message*));
		if (tmpArr == NULL)
			return -1;
		pHistory->msgHistory = tmpArr;
		pHistory->maxNumOfMsgs += HISTORY_GROWTH_SIZE;
	}
	pHistory->msgHistory[pHistory->numOfMsgs] = pMsg;
	pHistory->numOfMsgs++;
	pHistory->currentSort = eNoSort;
	return 1;
}

int documentMsgFromFile(UserMsgHistory* pHistory, Message* pMsg, int index)
{
	if (pHistory == NULL || pMsg == NULL)
		return -1;
	pHistory->msgHistory[index] = pMsg;
	return 1;
}

void printMsgHistory(const UserMsgHistory* pHistory)
{
	if (pHistory == NULL)
		return;
	for (int i = 0; i < pHistory->numOfMsgs; i++)
	{
		printMsg(pHistory->msgHistory[i]);
	}
}

int	compareByTime(const void* v1, const void* v2)
{
	const Message* pMsg1 = *(const Message**)v1;
	const Message* pMsg2 = *(const Message**)v2;
	return compareTime(&pMsg1->timeWritten, &pMsg2->timeWritten);
}

int	compareByLikes(const void* v1, const void* v2)
{
	const Message* pMsg1 = *(const Message**)v1;
	const Message* pMsg2 = *(const Message**)v2;
	return pMsg1->likesCounter - pMsg2->likesCounter;
}

int compareByLength(const void* v1, const void* v2)
{
	const Message* pMsg1 = *(const Message**)v1;
	const Message* pMsg2 = *(const Message**)v2;
	return (int)(strlen(pMsg1->msgText) - strlen(pMsg2->msgText));
}

void sortMsgs(UserMsgHistory* pHistory)
{
	if (pHistory == NULL)
		return;
	int userChoice;
	do
	{
		printf("How do you want the messgaes to be sorted:\n");
		for (int i = 1; i < eNofSorts; i++)
		{
			printf("(%d - %s) ", i, SortTypeStr[i]);
		}
		printf("\n");
		(void)scanf("%d", &userChoice);
	} while (userChoice < 1 || userChoice >= eNofSorts);
	switch (userChoice)
	{
		case eSortByTime:
			sortMessageHistory(pHistory, compareByTime);
			break;
		case eSortByLikes:
			sortMessageHistory(pHistory, compareByLikes);
			break;
		case eSortByLength:
			sortMessageHistory(pHistory, compareByLength);
			break;
	}
	pHistory->currentSort = (eSortType)userChoice;
	printf("Done sorting.\n");
}

void sortMessageHistory(UserMsgHistory* pHistory, int (*compare)(const void*, const void*))
{
	if (pHistory == NULL || compare == NULL)
		return;
	qsort(pHistory->msgHistory, pHistory->numOfMsgs, sizeof(Message*), compare);
}

Message* searchForMessage(const UserMsgHistory* pHistory)
{
	if (pHistory == NULL)
		return NULL;
	Message tmpMsg;
	Message* pTmpMsg;
	Message** res = NULL;
	switch ((int)pHistory->currentSort)
	{
		case 1:
			searchMsgByTimeHelper(&tmpMsg);
			pTmpMsg = &tmpMsg;
			res = (Message**)bsearch(&pTmpMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByTime);
			break;
		case 2:
			searchMsgByLikesHelper(&tmpMsg);
			pTmpMsg = &tmpMsg;
			res = (Message**)bsearch(&pTmpMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByLikes);
			break;
		case 3:
			searchMsgByLengthHelper(&tmpMsg);
			pTmpMsg = &tmpMsg;
			res = (Message**)bsearch(&pTmpMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByLength);
			break;
		default:
			printf("The search cannot be performed, array not sorted\n");
			return NULL;
	}
	
	if (res == NULL)
	{
		printf("Message not found\n");
		return NULL;
	}
	else
	{
		printf("Message found:\n");
		return (*res);
	}
}

void searchMsgByTimeHelper(Message* pMsg)
{
	NULL_CHECK(pMsg, );
	char buff[2] = { 0 };
	printf("Enter message year:\n");
	(void)scanf("%d", &pMsg->timeWritten.year);
	(void)gets(buff);	// buffer cleaning
	printf("Enter message month:\n");
	(void)scanf("%d", &pMsg->timeWritten.month);
	(void)gets(buff);	// buffer cleaning
	printf("Enter message day:\n");
	(void)scanf("%d", &pMsg->timeWritten.day);
	(void)gets(buff);	// buffer cleaning
	printf("Enter message hour:\n");
	(void)scanf("%d", &pMsg->timeWritten.hour);
	(void)gets(buff);	// buffer cleaning
	printf("Enter message minute:\n");
	(void)scanf("%d", &pMsg->timeWritten.minute);
	(void)gets(buff);	// buffer cleaning
}

void searchMsgByLikesHelper(Message* pMsg)
{
	NULL_CHECK(pMsg, );
	int likesInput = 0;
	char buff[2] = { 0 };
	printf("Enter the number of likes the message has:\n");
	(void)scanf("%d", &likesInput);
	(void)gets(buff);	// buffer cleaning
	pMsg->likesCounter = likesInput;
}

void searchMsgByLengthHelper(Message* pMsg)
{
	NULL_CHECK(pMsg, );
	int len = 0;
	char buff[2] = { 0 };
	printf("Enter the length of the message:\n");
	(void)scanf("%d", &len);
	(void)gets(buff);	// buffer cleaning
	if (len < 1 || len > 254)
	{
		printf("No message with such length.\n");
		return;
	}
	for (int i = 0; i < len; i++)
	{
		pMsg->msgText[i] = 'a';
	}
	pMsg->msgText[len] = '\0';
}

int saveMsgHistoryToBFile(FILE* fp, const UserMsgHistory* pHistory)
{
	if (fp == NULL || pHistory == NULL)
		return -1;
	if (fwrite(&pHistory->numOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(&pHistory->maxNumOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	return 1;
}

int readMsgHistoryFromBFile(FILE* fp, UserMsgHistory* pHistory)
{
	if (fp == NULL || pHistory == NULL)
		return -1;
	if (fread(&pHistory->numOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	if (fread(&pHistory->maxNumOfMsgs, sizeof(int), 1, fp) != 1)
		return -1;
	pHistory->msgHistory = (Message**)calloc(pHistory->maxNumOfMsgs, sizeof(Message*));
	pHistory->currentSort = eNoSort;
	return 1;
}

void msgHistoryActionMenu(UserMsgHistory* pHistory)
{
	NULL_CHECK(pHistory, );
	int userChoice = -1;
	char buff[2] = { 0 };
	do
	{
		printf("Choose action: (1 - Display Past Messages | 2 - Sort Messages | 3 - Find a Message | 0 - Exit)\n");
		(void)scanf("%d", &userChoice);
		(void)gets(buff);	// buffer cleaning
		switch (userChoice)
		{
			case 1:
				printMsgHistory(pHistory);
				break;
			case 2:
				sortMsgs(pHistory);
				break;
			case 3:
				searchForMessage(pHistory);
				break;
			case 0:
				printf("Returning to main menu.\n");
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice != 0);
}

int saveMsgHistoryToTextFile(const UserMsgHistory* pHistory, FILE* fp)
{
	if (fp == NULL || pHistory == NULL)
		return -1;
	fprintf(fp, "%d\n", pHistory->numOfMsgs);
	fprintf(fp, "%d\n", pHistory->maxNumOfMsgs);
	return 1;
}

int loadMsgHistoryFromTextFile(UserMsgHistory* pHistory, FILE* fp)
{
	if (fp == NULL || pHistory == NULL)
		return -1;
	if (fscanf(fp, "%d\n", &pHistory->numOfMsgs) != 1)
		return -1;
	if (fscanf(fp, "%d\n", &pHistory->maxNumOfMsgs) != 1)
		return -1;
	pHistory->msgHistory = (Message**)calloc(pHistory->maxNumOfMsgs, sizeof(Message*));
	pHistory->currentSort = eNoSort;
	return 1;
}

void freeMsgHistoryContents(UserMsgHistory* pHistory)
{
	if (pHistory == NULL)
		return;
	free(pHistory->msgHistory);	// the messages themselves aren't stored in UserMsgHistory, but in their threads
}

void freeMsgHistory(UserMsgHistory* pHistory)
{
	freeMsgHistoryContents(pHistory);
	free(pHistory);
}