/*
* symboltable.c - Hashtable Implementation (STsize  = 1000)
* programmer - 김서영, 김효진, 서지원, 손효원
* date - 5/2/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tn.h"


#define STsize 1000  //size of string table
#define HTsize 100	//size of hash table

#define FALSE 0
#define TRUE 1

#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z' || (x) == '_') )
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

#define MAX_LEN	12

extern void printerror(enum errorTypes err, char* str);

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;  //index of identifier in ST
	HTpointer next;  //pointer to next identifier
} HTentry;

enum errorTypes err;

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;  //the current identifier
int nextfree = 0;  //the next available index of ST
int hashcode;  //hash code of identifier
int sameid;  //first index of identifier
int STindex;

int found;  //for the previous occurrence of an identifie

char input;
char* string;	// input string

/* PrintError    - 	Print out error messages
			overst : identifier that over 12 characters
			illid: illegal identifier
			overfl: overflow in ST. print the hashtable and abort
 */
void PrintError(enum errorTypes err)
{
	switch (err) {
	case overst:
		nextfree = nextid;
		printerror(err, string);
		break;
	case illid:
	printerror(err, string);
		break;
	case overfl:
	printerror(err, string);
		break;
	}
}

/*ReadIO 	- 	Read identifier from the input file the string table ST directly into
			ST(append it to the previous identifier).
			An identifier is a string of letters and digits, starting with a letter.
			If first letter is digit, print out error message. */
void ReadID(char* str)
{
	int count = 0;
	nextid = nextfree;
	if (isDigit(input)) {
		err = illid;
		PrintError(err);
	}
	else {
		while (input != '\0') {
			if (nextfree == STsize) {
				err = overfl;
				PrintError(err);
			}
			ST[nextfree++] = input;
			input = *++str;
			count++;

			if (!(isLetter(input) || isDigit(input)) && input != '\0') {
				err = illid;
			}
		}
		if (count >= MAX_LEN) {
			err = overst;
			ST[nextfree] = '\0';
			PrintError(err);
			nextfree = nextid;
		}
		if (err == illid) {
			ST[nextfree] = '\0';
			PrintError(err);
			nextfree = nextid;
		}
	}
}

/* ComputeHS 	- 	Compute the hash code of identifier by summing the ordinal values of its
					characters and then taking the sum modulo the size of HT. */
void ComputeHS(int nid, int nfree)
{
	int code, i;
	code = 0;
	for (i = nid; i < nfree - 1; i++) {
		int current = (int)ST[i];
		//If current is lowercase, convert it to uppercase
		current = (current >= 'A' && current <= 'Z' ? current - 'A' + 'a' : current);
		code += current;
	}
	hashcode = (code % HTsize) + 1;
	hashcode = (hashcode == HTsize ? 0 : hashcode);
}

/*LookupHS 	-	For each identifier,Look it up in the hashtable for previous occurrence
				of the identifier.If find a match, set the found flag as true.
				Otherwise flase.
				If find a match, save the starting index of ST in same id. */
void LookupHS(int nid, int hscode)
{
	HTpointer here;
	int i, j;

	found = FALSE;
	if (HT[hscode] != NULL) {
		here = HT[hscode];
		while (here != NULL && found == FALSE) {
			found = TRUE;
			i = here->index;
			j = nid;
			sameid = i;

			while (ST[i] != '\0' && ST[j] != '\0' && found == TRUE) {
				int left = ST[i] >= 'A' && ST[i] <= 'Z' ? ST[i] - 'A' + 'a' : ST[i];
				int right = ST[j] >= 'A' && ST[j] <= 'Z' ? ST[j] - 'A' + 'a' : ST[j];
				if (left != right)
					found = FALSE;
				else {
					i++;
					j++;
				}
			}
			if (found == TRUE) {
				STindex = sameid;
			}
			here = here->next;
		}
	}
}


/* ADDHT	-	Add a new identifier to the hash table.
			If list head ht[hashcode] is null, simply add a list element with
			starting index of the identifier in ST.
			IF list head is not a null , it adds a new identifier to the head of the chain */
void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	ptr->index = nextid;
	ptr->next = HT[hscode];
	HT[hscode] = ptr;
}

/* Symboltable		-	Read the identifier from the file directly into ST.
			Compute its hashcode.
			Look up the idetifier in hashtable HT[hashcode]
			If matched, delete the identifier from ST and print ST-index
			of the matching identifier.
			If not matched , add a new element to the list,pointing to new identifier.
			Print the identifier,its index in ST, and whether it was entered or present.
			Print out the hashtable,and number of characters used up in ST
*/
void Symboltable(char* str)
{
	string = str;
	input = *str;
	err = noerror;
	ReadID(str);
	if (err == noerror) {
		if (nextfree == STsize) {
			err = overfl;
			PrintError(err);
		}
		ST[nextfree++] = '\0';

		ComputeHS(nextid, nextfree);
		LookupHS(nextid, hashcode);

		if (!found) {
			ADDHT(hashcode);
			STindex = nextid;
		}
		else {	// already existed
			nextfree = nextid;			
		}
	}
}