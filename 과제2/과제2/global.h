/*global.h-global variable for the project
* programmer - 김서영, 김효진, 서지원, 손효원
* date -
*/

#define STsize 1000

int line;
char ST[STsize];
int nid = 0;	//the current identifier
int nfree = 0;	//the next available index of ST
int sameid = 0;	//identifier의 첫번째 index
int errcnt;