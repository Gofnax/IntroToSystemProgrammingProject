#include "User.h"

int initUser(User* pUser)
{
	NULL_CHECK(pUser, -1);
	initUserName(pUser);
	initUserPassword(pUser);
	return 1;
}

int initUserName(User* pUser)
{
	NULL_CHECK(pUser, -1);

	printf("Enter username: ");
	char maxCharMsg[20];
	do
	{
		snprintf(maxCharMsg, 20, "(max %d chars)\n", USERNAME_LEN - 1);
		pUser->name = getStrExactName(maxCharMsg);
	} while (strlen(pUser->name) > USERNAME_LEN - 1);
	return 1;
}

int initUserPassword(User* pUser)
{
	NULL_CHECK(pUser, -1);
	char tmpPass1[MAX_STR_LEN];
	char tmpPass2[MAX_STR_LEN];
	int doPasswordsMatch = -1;
	do
	{
		printf("Enter password: (max %d chars)\n", PW_LEN - 1);
		fgets(tmpPass1, MAX_STR_LEN, stdin);
		cleanNewlineChar(tmpPass1);
		printf("Enter it again for verification:\n");
		fgets(tmpPass2, MAX_STR_LEN, stdin);
		cleanNewlineChar(tmpPass2);
		doPasswordsMatch = strcmp(tmpPass1, tmpPass2);
		if (doPasswordsMatch)
			printf("Passwords entered don't match. Try again\n");
	} while (strlen(tmpPass1) > PW_LEN - 1 || doPasswordsMatch);
	strncpy(pUser->password, tmpPass1, PW_LEN);

	initMsgHistory(&pUser->msgHistory);

	printf("User registered successfully.\n");
	return 1;
}

int isSamePassword(const User* pUser, char* pass)
{
	NULL_CHECK(pUser, -1);
	return strcmp(pUser->password, pass);
}

void printUser(const User* pUser)
{
	NULL_CHECK(pUser, );
	printf("Username: %s\n", pUser->name);
	printf("Number of Messages: %d\n", pUser->msgHistory.numOfMsgs);
}

int saveUserToBFile(FILE* fp, const User* pUser)
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

int readUserFromBFile(FILE* fp, User* pUser)
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

int saveUserToTextFile(const User* user, FILE* fp)
{
	if (fp == NULL || user == NULL)
	{
		return -1;
	}
	fprintf(fp, "%s\n", user->name);
	fprintf(fp, "%s\n", user->password);
	fprintf(fp, "%d\n", user->msgHistory.numOfMsgs);
	fprintf(fp, "%d\n", user->msgHistory.maxNumOfMsgs);
	return 1;
}

int loadUserFromTextFile(User* user, FILE* fp)
{
	if (fp == NULL || user == NULL)
	{
		return -1;
	}
	char tmpName[USERNAME_LEN] = { 0 };
	user->name = (char*)malloc(USERNAME_LEN * sizeof(char));
	NULL_CHECK(user->name, -1);
	if (fscanf(fp, "%[^\n]%*c", tmpName) != 1)
	{
		return -1;
	}
	strncpy(user->name, tmpName, USERNAME_LEN);
	if (fscanf(fp, "%s\n", user->password) != 1)
	{
		return -1;
	}
	if (fscanf(fp, "%d\n", &user->msgHistory.numOfMsgs) != 1)
	{
		return -1;
	}
	if (fscanf(fp, "%d\n", &user->msgHistory.maxNumOfMsgs) != 1)
	{
		return -1;
	}
	return 1;
}

void freeUserContents(User* pUser)
{
	NULL_CHECK(pUser, );
	free(pUser->name);
	freeMsgHistoryContents(&pUser->msgHistory);
}

void freeUser(User* pUser)
{
	NULL_CHECK(pUser, );
	freeUserContents(pUser);
	free(pUser);
}


