/****************************************************************************************************************

Hashtable Implementation (STsize  = 1000)
=========================================


Programmer  :   Byoungju Choi, Jihyun Park

Date		:	2023/03/30

Description :

Input	:		A file consisting of identifiers seperated by spaces,tab characters,newlines and punctuation marks.
				An identifier is a string of letters and digits,starting with a letter.

Output	:	The identifier,its index in the stringtable and whether entered or present.
			Prints error message for illegal identifier(starting with a digit, too long identifier, containing illegal characters) and overflow.
			Prints the hashtable before terminating. Simply write out hashcode and the list of identifiers
			associated with each hashcode, but only for non-empty lists.
			Finally,print out the number of characters used up in ST.

Grobal variations :
				ST - Array of string table
				HT - Array of list head of hashtable
				letters - Set of letters A..Z, a..z
				digits - Set of digits 0..9
				seperators - null , . ; : ? ! \t \n

*****************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "testdata.txt"

#define STsize 10  //size of string table
#define HTsize 100	//size of hash table

#define FALSE 0
#define TRUE 1

#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z' || (x) == '_') )
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

#define MAX_LEN		12

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;  //index of identifier in ST
	HTpointer next;  //pointer to next identifier
} HTentry;

enum errorTypes { noerror, illid, illleng, overst,illid_digit };;
typedef enum errorTypes ERRORtypes;

char seperators[] = " .,;:?!\t\n";

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;  //the current identifier
int nextfree = 0;  //the next available index of ST
int hashcode;  //hash code of identifier
int sameid;  //first index of identifier

int found;  //for the previous occurrence of an identifie

ERRORtypes err;

FILE* fp;   //to be a pointer to FILE 
char input;

//Initialize - open input file
void initialize()
{
	fp = fopen(FILE_NAME, "r");
	input = fgetc(fp);
}

//isSerperator  -  distinguish the seperator
//Returns 1 if seperator, otherwise returns 0
int isSeperator(char c)
{
	int i;
	int sep_len;

	sep_len = strlen(seperators);
	for (i = 0; i < sep_len; i++) {
		if (c == seperators[i])
			return 1;
	}
	return 0;
}

//printHeading	 -		Print the heading
void PrintHeading()
{
	printf("\n\n");
	printf("  -----------      ------------ \n");
	printf("  Index in ST       identifier  \n");
	printf("  -----------      ------------ \n");
	printf("\n");
}

/*PrintHStable     -   	Prints the hash table.write out the hashcode and the list of identifiers
						associated with each hashcode,but only for non-empty lists.
						Print out the number of characters used up in ST. */
void PrintHStable()
{
	int i, j;
	HTpointer here;

	printf("\n\n\n\n\n [[  HASH TABLE ]] \n\n");

	for (i = 0; i < HTsize; i++)
		if (HT[i] != NULL) { // non-empty 
			printf("  Hash Code %3d : ", i);
			for (here = HT[i]; here != NULL; here = here->next) {
				j = here->index;
				while (ST[j] != '\0' && j < STsize)
					printf("%c", ST[j++]);
				printf("\t");
			}
			printf("\n");
		}

	printf("\n\n\n < %5d characters are used in the string table >\n", nextfree);
}

/* PrintError    - 	Print out error messages
			overst :  overflow in ST. print the hashtable and abort
			illid_digit    : illegal identifier (start with digit)
			illid_long	: illegal identifier (too long identifier)
			illid_illch	: illegal identifier (containing illegal characters) */
void PrintError(ERRORtypes err)
{
	switch (err) {
	case overst:
		nextfree = nextid;
		printf("...Error...   OVERFLOW ");
		PrintHStable();
		exit(0);
		break;
	case illleng:
		printf("...Error... ");
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf(" too long identifier \n");
		break;
	case illid:
		printf("...Error... ");
		int index = nextid;
		while (ST[index] != '\0') {
			printf("%c", ST[index++]);
		}
		printf(" identifier containing illegal character\n");
		break;
	case illid_digit:
		printf("...Error... ");
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf(" start with digit \n");
		break;
	}
}

/* Skip Seperators -   	skip over strings of spaces,tabs,newlines, . , ; : ? !
						if illegal seperators,print out error message.*/

void SkipSeperators()
{
	while (input != EOF && !(isLetter(input) || isDigit(input))) {
		if (!isSeperator(input)) {
			err = illid;
			PrintError(err);
		}
		input = fgetc(fp);
	}
}

/*ReadIO 	- 	Read identifier from the input file the string table ST directly into
			ST(append it to the previous identifier).
			An identifier is a string of letters and digits, starting with a letter.
			If first letter is digit, print out error message. */
void ReadID()
{
	int count = 0;
	nextid = nextfree;
	if (isDigit(input)) {
		err = illid_digit;
		PrintError(err);
	}
	else {
		while (input != EOF && !isSeperator(input)) {
			if (nextfree == STsize) {
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = input;
			input = fgetc(fp);
			count++;

			if (!(isLetter(input) || isDigit(input) || isSeperator(input)) && input != EOF) {
				err = illid;
			}
		}
		if (count >= MAX_LEN) {
			err = illleng;
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

/* MAIN		-	Read the identifier from the file directly into ST.
			Compute its hashcode.
			Look up the idetifier in hashtable HT[hashcode]
			If matched, delete the identifier from ST and print ST-index
			of the matching identifier.
			If not matched , add a new element to the list,pointing to new identifier.
			Print the identifier,its index in ST, and whether it was entered or present.
			Print out the hashtable,and number of characters used up in ST
*/
int main()
{
	int i;
	PrintHeading();
	initialize();

	while (input != EOF) {
		err = noerror;
		SkipSeperators();
		ReadID();
		if (err == noerror) {
			if (nextfree == STsize) {
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = '\0';

			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);

			if (!found) {
				printf("%6d         ", nextid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("          (entered)\n");
				ADDHT(hashcode);
			}
			else {
				printf("%6d         ", sameid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("          (already existed)\n");
				nextfree = nextid;
			}
		}
	}
	PrintHStable();
}
