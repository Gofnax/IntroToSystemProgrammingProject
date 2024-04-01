#pragma once

#include <stdio.h>
#include <string.h>
#include "def.h"
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
void freeMessageContents(Message* pMsg);