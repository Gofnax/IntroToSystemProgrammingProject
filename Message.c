#include "Message.h"

int createMsg(Message* pMsg, char* authorName)
{
	char buffer[10] = { 0 };
	pMsg->likesCounter = 0;
	int len = (int)strlen(authorName) + 1;
	pMsg->authorName = (char*)malloc(len * sizeof(char));
	if (pMsg->authorName == NULL)
		return -1;
	strncpy(pMsg->authorName, authorName, len);

	pMsg->msgText[MAX_MSG_LEN - 1] = '\0';
	printf("Enter your message: (max %d chars)\n", MAX_MSG_LEN);
	fgets(pMsg->msgText, MAX_MSG_LEN, stdin);
	if (pMsg->msgText[MAX_MSG_LEN - 1] != '\0')
	{
		pMsg->msgText[MAX_MSG_LEN - 1] = '\0';
		gets(buffer);	// buffer cleaning
	}
	cleanNewlineChar(pMsg->msgText);
	initTime(&pMsg->timeWritten);
	return 1;
}

void printMsg(const Message* pMsg)
{
	printf("----------------------------------------\n");	// 40 '-' for visuals
	printf("Author: %s\n%s\n", pMsg->authorName, getTimeString(&pMsg->timeWritten));
	printf("--------------------\n");	// 20 '-' for visuals
	printf("%s\n", pMsg->msgText);
	printf("----------------------------------------\n");	// 40 '-' for visuals
}

void freeMessageContents(Message* pMsg)
{
	if (pMsg == NULL)
		return;
	free(pMsg->authorName);
}

void freeMessage(Message* pMsg)
{
	freeMessageContents(pMsg);
	free(pMsg);
}