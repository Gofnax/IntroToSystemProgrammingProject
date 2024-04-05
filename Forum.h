#pragma once

#include <stdio.h>
#include "list.h"
#include "def.h"
#include "General.h"
#include "TimeStamp.h"
#include "Message.h"
#include "User.h"
#include "PrivateMsgBox.h"
#include "Subject.h"

typedef struct
{
	LIST subjectList;
	User* userArr;
	int userArrSize;
	User* currentUser;
	PrivateMsgBox* privateMsgBoxArr;
	int privateMsgBoxArrSize;
}Forum;

int initForum(Forum* pForum);
int login(Forum* pForum);
int registerUser(Forum* pForum);
int chooseSubject(LIST* pSubjectList);
int addSubject(LIST* pSubjectList);
void startPrivateConversation(User* pCurrentUser, User* pUser, Forum* pForum);
void displayMsgHistory(User* user);
void displaySubjectList(LIST* pSubjectList);
int isUserInArr(User* user, User* userArr, int userArrSize);
int addUser(User* user, Forum* pForum);
void freeForumContent(Forum* pForum);
void freeForum(Forum* pForum);
int doesMsgBoxExist(User* pCurrentUser, User* pUser, PrivateMsgBox* privateMsgBoxArr, int privateMsgBoxArrSize);