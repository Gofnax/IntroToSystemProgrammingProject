#pragma once

#include "Thread.h"

typedef struct
{
	char* title;
	Thread** threadArr;
	int threadArrSize;
}Subject;

int initSubject(Subject* pSubject);
int createSubjectTitle(Subject* pSubject);
void printThreadArr(Thread** threadArr, int threadArrSize);
void chooseThread(Thread** threadArr, int threadArrSize, User* pCurrUser);
int  addThread(Subject* pSubject, Thread* pThread);
void printSubject(const Subject* pSubject);
void subjectActionsMenu(Subject* pSubject, User* pCurrUser);
void freeSubjectContent(Subject* pSubject);
void freeSubject(Subject* pSubject);