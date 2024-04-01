#include "User.h"

int initUser(User* pUser)
{
	char buffer[10] = { 0 };
	pUser->name = (char*)malloc(USERNAME_LEN * sizeof(char));
	if (pUser->name == NULL)
		return -1;

	printf("Enter username: (max %d chars)\n", USERNAME_LEN - 1);
	fgets(pUser->name, USERNAME_LEN, stdin);
	if (strlen(pUser->name) >= USERNAME_LEN - 1)
	{
		*(pUser->name + USERNAME_LEN - 1) = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pUser->name);

	printf("Enter password: (max %d chars)\n", PW_LEN - 1);
	fgets(pUser->password, PW_LEN, stdin);
	if (strlen(pUser->password) >= PW_LEN - 1)
	{
		*(pUser->password + PW_LEN - 1) = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pUser->password);

	initMsgHistory(&pUser->msgHistory);

	printf("User registered successfully.\n");
	return 1;
}

void freeUserContents(User* pUser)
{
	free(pUser->name);
}

void cleanNewlineChar(char* str)
{
	if (*(str + strlen(str) - 1) == '\n')
	{
		*(str + strlen(str) - 1) = '\0';
	}
}

