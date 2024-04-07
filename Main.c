#include <stdio.h>
#include "list.h"
#include "def.h"
#include "General.h"
#include "Forum.h"
#include "TimeStamp.h"
#include "Message.h"
#include "User.h"
#include "PrivateMsgBox.h"
#include "Forum.h"

void forumLoadMenu(Forum* pForum);

int main(void)
{
	Forum forum;
	forumLoadMenu(&forum);
	forumMainMenu(&forum);
}

void forumLoadMenu(Forum* pForum)
{
	NULL_CHECK(pForum, );
	int userChoice = 0;
	char buff[2] = { 0 };
	FILE* fp;
	do
	{
		printf("How would you like to load the system?\n");
		printf("(1 - Using Text Files | 2 - Using Binary Files | 3 - Clean System)\n");
		NUM_INPUT_CLEAN_BUFF(userChoice, buff);
		switch (userChoice)
		{
			case 1:
				fp = fopen(SYSTEM_TEXT_FILE, "r");
				loadForumFromTextFile(pForum, fp);
				fclose(fp);
				break;
			case 2:
				fp = fopen(SYSTEM_BIN_FILE, "rb");
				readForumFromBFile(fp, pForum);
				fclose(fp);
				break;
			case 3:
				initForum(pForum);
				break;
			default:
				printf("Unknown option selected.\n");
		}
	} while (userChoice < 1 || userChoice > 3);
}
