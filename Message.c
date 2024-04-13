#include "Message.h"

int createMsg(Message* pMsg, char* authorName)
{
	NULL_CHECK(pMsg, -1);
	char buffer[10] = { 0 };
	pMsg->likesCounter = 0;
	int len = (int)strlen(authorName) + 1;
	pMsg->authorName = (char*)malloc(len * sizeof(char));
	NULL_CHECK(pMsg->authorName, NULL);
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
	NULL_CHECK(pMsg, );
	char* timeString = getTimeString(&pMsg->timeWritten);
	printf("----------------------------------------\n");	// 40 '-' for visuals
	printf("Author: %s\n%s\n", pMsg->authorName, timeString);
	printf("--------------------\n");	// 20 '-' for visuals
	printf("%s\n", pMsg->msgText);
	printf("--------------------\n");	// 20 '-' for visuals
	printf("Likes: %d\n", pMsg->likesCounter);
	printf("----------------------------------------\n");	// 40 '-' for visuals
	free(timeString);
}

void printMsgIndexed(const Message* pMsg, int index)
{
	NULL_CHECK(pMsg, );
	char* timeString = getTimeString(&pMsg->timeWritten);
	printf("----------------------------------------\n");	// 40 '-' for visuals
	printf("%d) Author: %s\n%s\n", index, pMsg->authorName, timeString);
	printf("--------------------\n");	// 20 '-' for visuals
	printf("%s\n", pMsg->msgText);
	printf("--------------------\n");	// 20 '-' for visuals
	printf("Likes: %d\n", pMsg->likesCounter);
	printf("----------------------------------------\n");	// 40 '-' for visuals
	free(timeString);
}

int saveMsgToBFile(FILE* fp, const Message* pMsg)
{
	if (fp == NULL || pMsg == NULL)
		return -1;
	int authorLen = (int)strlen(pMsg->authorName) + 1;
	if (fwrite(&authorLen, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pMsg->authorName, sizeof(char), authorLen, fp) != authorLen)
		return -1;

	int msgLen = (int)strlen(pMsg->msgText) + 1;
	if (fwrite(&msgLen, sizeof(int), 1, fp) != 1)
		return -1;
	if (fwrite(pMsg->msgText, sizeof(char), msgLen, fp) != msgLen)
		return -1;

	if (fwrite(&pMsg->likesCounter, sizeof(int), 1, fp) != 1)
		return -1;
	if (saveTimeToBFileCompressed(fp, &pMsg->timeWritten) == -1)
		return -1;
	return 1;
}

int readMsgFromBFile(FILE* fp, Message* pMsg)
{
	if (fp == NULL || pMsg == NULL)
		return -1;

	int authorLen = 0;
	if (fread(&authorLen, sizeof(int), 1, fp) != 1)
		return -1;
	pMsg->authorName = (char*)malloc(authorLen * sizeof(char));
	if (pMsg->authorName == NULL)
		return -1;
	if (fread(pMsg->authorName, sizeof(char), authorLen, fp) != authorLen)
		return -1;

	int msgLen = 0;
	if (fread(&msgLen, sizeof(int), 1, fp) != 1)
		return -1;
	char* tmpText = (char*)malloc(msgLen * sizeof(char));
	if (tmpText == NULL)
		return -1;
	if (fread(tmpText, sizeof(char), msgLen, fp) != msgLen)
		return -1;
	strncpy(pMsg->msgText, tmpText, msgLen);
	free(tmpText);

	if (fread(&pMsg->likesCounter, sizeof(int), 1, fp) != 1)
		return -1;

	if (readTimeFromBFileCompressed(fp, &pMsg->timeWritten) == -1)
		return -1;

	return 1;
}

int saveMessageToTextFile(const Message* message, FILE* fp)
{
	if (fp == NULL)
	{
		return -1;
	}
	fprintf(fp, "%s\n", message->msgText);
	fprintf(fp, "%s\n", message->authorName);
	fprintf(fp, "%d\n", message->likesCounter);
	if (saveTimeStampToTextFile(&message->timeWritten, fp) != 1)
	{
		return -1;
	}
	return 1;
}

int loadMessageFromTextFile(Message* message, FILE* fp)
{
	if (fp == NULL || message == NULL)
	{
		return -1;
	}
	(void)fscanf(fp, "%[^\n]%*c", message->msgText);
	message->authorName = (char*)malloc(sizeof(char) * USERNAME_LEN);
	if (message->authorName == NULL)
	{
		return -1;
	}
	(void)fscanf(fp, "%[^\n]%*c", message->authorName);
	(void)fscanf(fp, "%d\n", &message->likesCounter);
	if (loadTimeStampFromTextFile(&message->timeWritten, fp) != 1)
	{
		return -1;
	}
	return 1;
}

void freeMessageContents(Message* pMsg)
{
	NULL_CHECK(pMsg, );
	free(pMsg->authorName);
}

void freeMessage(Message* pMsg)
{
	NULL_CHECK(pMsg, );
	freeMessageContents(pMsg);
	free(pMsg);
}