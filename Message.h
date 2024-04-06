#pragma once

#include <stdio.h>
#include <string.h>
#include "def.h"
#include "General.h"
#include "TimeStamp.h"

typedef struct
{
	char msgText[MAX_MSG_LEN];
	char* authorName;
	int likesCounter;
	TimeStamp timeWritten;
}Message;

int createMsg(Message* pMsg, char* authorName);
void printMsg(const Message* pMsg);
void printMsgIndexed(const Message* pMsg, int index);
int saveMsgToBFile(FILE* fp, const Message* pMsg);
int readMsgFromBFile(FILE* fp, Message* pMsg);
int saveMessageToTextFile(const Message* message, FILE* fp);
int loadMessageFromTextFile(Message* message, FILE* fp);
void freeMessageContents(Message* pMsg);
void freeMessage(Message* pMsg);