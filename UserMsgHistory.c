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

Message* searchForMessage(const UserMsgHistory* pHistory, Message** pMsg)
{
	if (pHistory == NULL || pMsg == NULL)
		return NULL;
	switch ((int)pHistory->currentSort)
	{
		case 1:
			return *(Message**)bsearch(pMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByTime);
			break;
		case 2:
			return *(Message**)bsearch(pMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByLikes);
			break;
		case 3:
			return *(Message**)bsearch(pMsg, pHistory->msgHistory, pHistory->numOfMsgs,
				sizeof(Message*), compareByLength);
			break;
		default:
			printf("The search cannot be performed, array not sorted\n");
	}
	return NULL;
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