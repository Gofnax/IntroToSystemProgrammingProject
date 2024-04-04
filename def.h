/**************************************************************************/
/* def.h                                                                  */
/* This file contains GENERAL definitions used in several data structures */
/**************************************************************************/

#ifndef _DEF_
#define _DEF_

// a value not supposed in stack and queue
#define		EMPTY	0x7FFFFFFF

// the maximum number of characters allowed in a message
#define MAX_MSG_LEN 255

// the maximum number of characters allowed in a password
#define USERNAME_LEN 17
#define PW_LEN 9

// the maximum number of characters allowed in a title
#define MAX_TITLE_NAME 41

// Macro defenition
#define NULL_CHECK(ptr, num) {\
				if(ptr == NULL)\
					return num;\
			}

// new types
typedef void* DATA;					// a type for data (easy to change)
typedef enum {False, True} BOOL;	// a boolean type

// Node
typedef struct node 
{
	DATA			key;
	struct node*	next;
}NODE;

#endif
