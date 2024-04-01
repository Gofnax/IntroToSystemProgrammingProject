#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"

typedef struct
{
	char* name;
	char password[PW_LEN];

}User;

int initUser(User* pUser);
char* getUserName(User* pUser);
void freeUserContents(User* pUser);
void cleanNewlineChar(char* str);