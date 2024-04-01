#include "Message.h"

int createMsg(Message* pMsg, char* authorName)
{
	int len = (int)strlen(authorName) + 1;
	pMsg->authorName = (char*)malloc(len * sizeof(char));
	if (pMsg->authorName == NULL)
		return -1;
	strncpy(pMsg->authorName, authorName, len);

	printf("Enter your message: (max %d chars)\n", MAX_MSG_LEN);
	fgets(pMsg->msgText, MAX_MSG_LEN, stdin);

	initTime(&pMsg->timeWritten);
}

void printMsg(Message* pMsg)
{
	printf("\nAuthor: %s\n%s\n\n", pMsg->authorName, getTimeString(&pMsg->timeWritten));
	printf("%s\n", pMsg->msgText);
	printf("----------------------------------------\n");	// 40 '-' for visuals
}

char* getMsgText(Message* pMsg)
{
	return pMsg->msgText;
}

void freeMessageContents(Message* pMsg)
{
	free(pMsg->authorName);
}
