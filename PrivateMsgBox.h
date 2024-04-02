#pragma once

#include <stdio.h>
#include "User.h"
#include "Message.h"

typedef struct
{
	User* user1;
	User* user2;
	Message* messageArr;
	int numOfMsgs;
}PrivateMsgBox;

int initPrivateMsgBox(PrivateMsgBox* pPrivateBox, User* pUser1, User* pUser2);
void privateMsgBoxMenu(PrivateMsgBox* pPrivateBox, User* pUser);
int writeMessage(PrivateMsgBox* pPrivateBox, User* pUser);
void printPrivateMsgs(PrivateMsgBox* pPrivateBox);
void freePrivateMsgBoxContents(PrivateMsgBox* pPrivateBox);
void freePrivateMsgBox(PrivateMsgBox* pPrivateBox);