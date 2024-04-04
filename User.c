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

	char tmpPass1[MAX_STR_LEN];
	char tmpPass2[MAX_STR_LEN];
	do
	{
		printf("Enter password: (max %d chars)\n", PW_LEN - 1);
		fgets(tmpPass1, MAX_STR_LEN, stdin);
		cleanNewlineChar(tmpPass1);
		printf("Enter it again for verification:\n");
		fgets(tmpPass2, MAX_STR_LEN, stdin);
		cleanNewlineChar(tmpPass2);
	} while (strlen(tmpPass1) > PW_LEN - 1 || strcmp(tmpPass1, tmpPass2));
	strncpy(pUser->password, tmpPass1, PW_LEN);

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

int saveUserToBFile(const FILE* fp, const User* pUser)
{
	if (fp == NULL || pUser == NULL)
		return -1;
	int len = (int)strlen(pUser->name) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pUser->name, sizeof(char), len, fp) != len)
		return -1;
	if (fwrite(pUser->password, sizeof(char), PW_LEN, fp) != PW_LEN)
		return -1;
	if (saveMsgHistoryToBFile(fp, &pUser->msgHistory) == -1)
		return -1;
	return 1;
}

int readUserFromBFile(const FILE* fp, User* pUser)
{
	if (fp == NULL || pUser == NULL)
		return -1;
	int len = 0;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return -1;
	pUser->name = (char*)malloc(len * sizeof(char));
	if (pUser->name == NULL)
		return -1;
	if (fread(pUser->name, sizeof(char), len, fp) != len)
		return -1;
	if (fread(pUser->password, sizeof(char), PW_LEN, fp) != PW_LEN)
		return -1;
	if (readMsgHistoryFromBFile(fp, &pUser->msgHistory) == -1)
		return -1;
	return 1;
}

void freeUserContents(User* pUser)
{
	if (pUser == NULL)
		return;
	free(pUser->name);
	freeMsgHistoryContents(&pUser->msgHistory);
}

void freeUser(User* pUser)
{
	freeUserContents(pUser);
	free(pUser);
}


