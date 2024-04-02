#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"
#include "General.h"
#include "UserMsgHistory.h"

typedef struct
{
	char* name;
	char password[PW_LEN];
	UserMsgHistory msgHistory;
}User;

int initUser(User* pUser);
int isSamePassword(User* pUser, char* pass);
void freeUserContents(User* pUser);
