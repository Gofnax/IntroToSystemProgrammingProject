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
int initUserName(User* pUser);
int initUserPassword(User* pUser);
int isSamePassword(User* pUser, char* pass);
int saveUserToBFile(FILE* fp, const User* pUser);
int readUserFromBFile(FILE* fp, User* pUser);
int saveUserToTextFile(const User* user, FILE* fp);
int loadUserFromTextFile(User* user, FILE* fp);
void freeUserContents(User* pUser);
void freeUser(User* pUser);
