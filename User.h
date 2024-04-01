#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"
#include "UserMsgHistory.h"

typedef struct
{
	char* name;
	char password[PW_LEN];
	UserMsgHistory msgHistory;
}User;

int initUser(User* pUser);
void freeUserContents(User* pUser);
void cleanNewlineChar(char* str);