#pragma once

#include <stdio.h>
#include <string.h>
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
void displayMsgHistory(const User* user);
void displaySubjectList(const LIST* pSubjectList);
int chooseSubject(LIST* pSubjectList, User* pCurrUser);
int addSubject(LIST* pSubjectList);
int isUserInArr(const User* user, const User* userArr, int userArrSize);
int addUser(const User* user, Forum* pForum);
void startPrivateConversation(const User* pCurrentUser, const User* pUser, Forum* pForum);
int doesMsgBoxExist(const User* pCurrentUser, const User* pUser, const PrivateMsgBox* privateMsgBoxArr, int privateMsgBoxArrSize);
void forumMainMenu(Forum* pForum);
void displayMostActiveUser(const Forum* pForum);
int choosePrivateChatPartner(User* pCurrentUser, Forum* pForum);
void loginRegisterMenu(Forum* pForum);
void loadMsgHistory(Forum* pForum);
void printForum(const Forum* pForum);
void printSubjectList(const LIST* pSubjectList);
int saveForumToBFile(FILE* fp, const Forum* pForum);
int readForumFromBFile(FILE* fp, Forum* pForum);
int saveForumToTextFile(const Forum* pForum, FILE* fp);
int saveSubjectListToTextFile(const LIST subjectList, FILE* fp);
int savePrivateMsgBoxArrToTextFile(const PrivateMsgBox* privateMsgBoxArr, const int privateMsgBoxArrSize, FILE* fp);
int loadForumFromTextFile(Forum* pForum, FILE* fp);
int loadSubjectListFromTextFile(LIST* subjectList, FILE* fp);
int loadPrivateMsgBoxArrFromTextFile(PrivateMsgBox* privateMsgBoxArr, Forum* pForum, FILE* fp);
void freeForumContent(Forum* pForum);
void freeForum(Forum* pForum);
