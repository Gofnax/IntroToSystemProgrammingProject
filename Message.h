#pragma once

#include <stdio.h>
#include <string.h>
#include "def.h"
#include "TimeStamp.h"

typedef struct
{
	char msgText[MAX_MSG_LEN];
	char* authorName;
	TimeStamp timeWritten;
}Message;

int createMsg(Message* pMsg, char* authorName);
void printMsg(Message* pMsg);
char* getMsgText(Message* pMsg);
void freeMessageContents(Message* pMsg);