#include "User.h"

int initUser(User* pUser)
{
	pUser->name = (char*)malloc(USERNAME_LEN * sizeof(char));
	if (pUser->name == NULL)
		return -1;

	printf("Enter username: ");
	char maxCharMsg[20];
	do
	{
		snprintf(maxCharMsg, 20, "(max %d chars)\n", USERNAME_LEN - 1);
		pUser->name = getStrExactName(maxCharMsg);
	} while (strlen(pUser->name) > USERNAME_LEN - 1);

	printf("Enter password: ");
	char tmpPass[MAX_STR_LEN];
	do
	{
		printf("(max %d chars)\n", PW_LEN - 1);
		fgets(tmpPass, MAX_STR_LEN, stdin);
		cleanNewlineChar(tmpPass);
	} while (strlen(tmpPass) > PW_LEN - 1);
	strncpy(pUser->password, tmpPass, PW_LEN);

	initMsgHistory(&pUser->msgHistory);

	printf("User registered successfully.\n");
	return 1;
}

int isSamePassword(User* pUser, char* pass)
{
	if (pUser == NULL)
		return -1;
	return strcmp(pUser->password, pass);
}

void freeUserContents(User* pUser)
{
	if (pUser == NULL)
		return;
	free(pUser->name);
	freeMsgHistory(&pUser->msgHistory);
}

void freeUser(User* pUser)
{
	freeUserContents(pUser);
	free(pUser);
}


