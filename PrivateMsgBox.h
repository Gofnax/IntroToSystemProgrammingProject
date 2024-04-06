#pragma once

#include <stdio.h>
#include "User.h"
#include "Message.h"

typedef struct
{
	char* userName1;
	char* userName2;
	User* user1;
	User* user2;
	Message* messageArr;
	int numOfMsgs;
}PrivateMsgBox;

int initPrivateMsgBox(PrivateMsgBox* pPrivateBox, User* pUser1, User* pUser2);
void privateMsgBoxMenu(PrivateMsgBox* pPrivateBox, User* pUser);
int writeMessage(PrivateMsgBox* pPrivateBox, User* pUser);
void printPrivateMsgs(PrivateMsgBox* pPrivateBox);
int savePrivateMsgBoxToBFile(FILE* fp, const PrivateMsgBox* pPrivateBox);
int readPrivateMsgBoxFromBFile(FILE* fp, PrivateMsgBox* pPrivateBox);
int savePrivateMsgBoxToTextFile(const PrivateMsgBox* privateMsgBox, FILE* fp);
int loadPrivateMsgBoxFromTextFile(PrivateMsgBox* privateMsgBox, FILE* fp);
void freePrivateMsgBoxContents(PrivateMsgBox* pPrivateBox);
void freePrivateMsgBox(PrivateMsgBox* pPrivateBox);