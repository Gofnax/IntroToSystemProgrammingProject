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
void printThreadArr(const Thread** threadArr, int threadArrSize);
void chooseThread(Subject* pSubject, User* pCurrUser);
int  addThread(Subject* pSubject, Thread* pThread);
void printSubject(const Subject* pSubject);
void printSubjectTitle(const Subject* pSubject);
void subjectActionsMenu(Subject* pSubject, User* pCurrUser);
void printSubjectWithThread(const Subject* pSubject);
int saveSubjectToBFile(FILE* fp, const Subject* pSubject);
int readSubjectFromBFile(FILE* fp, Subject* pSubject);
int saveSubjectToTextFile(const Subject* pSubject, FILE* fp);
int loadSubjectFromTextFile(Subject* pSubject, FILE* fp);
void freeSubjectContent(Subject* pSubject);
void freeSubject(Subject* pSubject);