#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Message.h"
#include "User.h"


typedef struct
{
	char* title;
	Message primaryMsg;
	Message* messageArr;
	int messageArrSize;
}Thread;

int initThread(Thread* pThread, User* currentUser);
int createThreadTitle(Thread* pThread);
void printThread(const Thread* pThread);
int  addMessage(Thread* pThread, User* currentUser);
void threadActionsMenu(Thread* pThread, User* currentUser);
void likeMsg(Thread* pThread);
int saveThreadToBFile(FILE* fp, const Thread* pThread);
int readThreadFromBFile(FILE* fp, Thread* pThread);
int saveThreadToTextFile(const Thread* pThread, FILE* fp);
int loadThreadFromTextFile(Thread* pThread, FILE* fp);
void freeThreadContent(Thread* pThread);
void freeThread(Thread* pThread);