#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Message.h"

#define HISTORY_GROWTH_SIZE 10

typedef enum {
	eNoSort, eSortByTime, eSortByLikes, eNofSorts
} eSortType;

typedef struct
{
	Message** msgHistory;
	int numOfMsgs;
	int maxNumOfMsgs;
	eSortType currentSort;
}UserMsgHistory;

void initMsgHistory(UserMsgHistory* pHistory);
int documentMsg(UserMsgHistory* pHistory, Message* pMsg);
void printMsgHistory(const UserMsgHistory* pHistory);
int	compareByTime(const void* v1, const void* v2);
int	compareByLikes(const void* v1, const void* v2);
void sortMsgs(UserMsgHistory* pHistory);
void sortMessageHistory(UserMsgHistory* pHistory, int (*compare)(const void*, const void*));
void freeMsgHistory(UserMsgHistory* pHistory);
Message* searchForMessage(const UserMsgHistory* pHistory, Message** pMsg);

