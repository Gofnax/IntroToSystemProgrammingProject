#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Message.h"

#define HISTORY_GROWTH_SIZE 10

typedef enum {
	eNoSort, eSortByTime, eSortByLikes, eSortByLength, eNofSorts
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
int documentMsgFromFile(UserMsgHistory* pHistory, Message* pMsg, int index);
void printMsgHistory(const UserMsgHistory* pHistory);
int	compareByTime(const void* v1, const void* v2);
int	compareByLikes(const void* v1, const void* v2);
int compareByLength(const void* v1, const void* v2);
void sortMsgs(UserMsgHistory* pHistory);
void sortMessageHistory(UserMsgHistory* pHistory, int (*compare)(const void*, const void*));
Message* searchForMessage(const UserMsgHistory* pHistory);
void searchMsgByTimeHelper(Message* pMsg);
void searchMsgByLikesHelper(Message* pMsg);
void searchMsgByLengthHelper(Message* pMsg);
int saveMsgHistoryToBFile(FILE* fp, const UserMsgHistory* pHistory);
int readMsgHistoryFromBFile(FILE* fp, UserMsgHistory* pHistory);
void msgHistoryActionMenu(UserMsgHistory* pHistory);
int saveMsgHistoryToTextFile(const UserMsgHistory* pHistory, FILE* fp);
int loadMsgHistoryFromTextFile(UserMsgHistory* pHistory, FILE* fp);
void freeMsgHistoryContents(UserMsgHistory* pHistory);
void freeMsgHistory(UserMsgHistory* pHistory);
