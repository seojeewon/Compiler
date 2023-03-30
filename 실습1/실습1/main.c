#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "testdata.txt"
#define STsize 1000 //size of string table
#define HTsize 100 //size of hash table

#define FALSE 0
#define TRUE 1

#define isLetter(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') || ((x) == '_'))
#define isDigit(x) ((x) >= '0' && (x) <= '9')

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index; //index of identifier in ST
	HTpointer next; //pointer to next identifier
} HTentry;

enum errorTypes { noerror, illsp, illid, overst, toolong };
typedef enum errorTypes ERRORtypes;

char seperators[] = " .,;:?!\t\n";

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;	//the current identifier
int nextfree = 0;	//the next available index of ST
int hashcode;	//hash code of identifier
int sameid;	//first index of identifier

int found;	//for the previous occurrence of an identifier

ERRORtypes err;

FILE* fp; //to be a pointer to FILE
char input;
char err_input;	//store illegal seperator

void CheckHT();

//Initialize - open input file
void initialize()
{
	fp = fopen("testdata.txt", "r");
	input = fgetc(fp);
}

//지원
int isSeperator(char c) {
	int sep_len;

	sep_len = strlen(seperators);
	for (int i = 0; i < sep_len; i++) {
		if (c == seperators[i])
			return 1;
	}
	return 0;
}

//효진
void PrintHeading() {
	printf("\n\n");
	printf(" -----------            -----------\n");
	printf(" Index in ST            identifier \n");
	printf(" -----------            -----------\n");
	printf("\n");
}

/* 서영
   PrintHStable - Prints the hash table.write out the hashcode and the list of identifiers
   associated with each hashcode,but only for non-empty lists.
   Print out the number of characters used up in ST.*/
void PrintHStable()
{
	HTpointer here;

	printf("\n\n\n\n\n [[ HASH TABLE ]] \n\n");

	for (int i = 0; i < HTsize; i++) {
		if (HT[i] != NULL) {
			printf(" Hash Code %3d : ", i);
			for (here = HT[i]; here != NULL; here = here->next) {
				int j = here->index;
				while (ST[j] != '\0' && j < STsize)
					printf("%c", ST[j++]);
				printf("  ");
			}
			printf("\n");
		}
	}
	printf("\n\n <%d characters are used in the string table>\n", nextfree);
}

/* 효진
   PrintError - Print out error messages
   overst : overflow in ST
   print the hashtable and abort by calling the function "abort()".
   illid : illegal identifier
   illsp : illegal seperator
   toolong : too long identifier*/
void PrintError(ERRORtypes err, char* str)
{
	switch (err) {
	case overst:
		printf("...Error...  \t\tOVERFLOW ");
		PrintHStable();
		exit(0);
		break;
	case illsp:
		printf("...Error... \t\t%s       \t%c is not allowed \n", str, err_input);
		break;
	case illid:
		printf("...Error... \t\t");
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf(" \tstart with digit \n");
		break;
	case toolong:
		printf("...Error... \t\t");
		for (int i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
		}
		printf("       \ttoo long identifier \n");
		nextfree = nextid;
		break;
	case noerror:
		break;
	}
}

/* 지원
   Skip Seperators - skip over strings of spaces, tabs, newlines, ., ; : ? !
   if illegal seperators,print out error message. */
void SkipSeperators() {
	while (input != EOF && !(isLetter(input) || isDigit(input))) {
		if (!isSeperator(input)) {
			err_input = input;
			char* str = malloc(sizeof(char) * 50);
			int idx = 0;
			str[idx++] = input;

			while (TRUE) {
				input = fgetc(fp);
				if (isSeperator(input) || input == EOF) break;
				str[idx++] = input;
			}

			str[idx] = '\0';
			err = illsp;
			PrintError(err, str);
		}
		input = fgetc(fp);
	}
}

/* 지원
   ReadID - Read identifier from the input file the string table ST directly into
   ST(append it to the previous identifier).
   An identifier is a string of letters and digits, starting with a letter.
   If first letter is digit, print out error message. */
void ReadID()
{
	err = noerror;
	nextid = nextfree;
	if (input == EOF) {
		return;
	}
	if (isDigit(input)) {
		err = illid;
		PrintError(err, NULL);
	}
	else {
		int flag = 0;
		while (input != EOF && (!isSeperator(input))) {
			if (nextfree == STsize) {
				err = overst;
				PrintError(err, NULL);
			}
			if ((!isDigit(input) && !isLetter(input)) && !flag) {
				flag = 1;
				err_input = input;
				err = illsp;

			}
			ST[nextfree++] = input;
			input = fgetc(fp);

		}

		if (input == EOF && err != illsp) {	//ReadID내에서 input 읽다가 EOF 읽은 경우, 그 전까지 읽은 
			CheckHT();
		}

		if (err == illsp) {
			char* str = malloc(sizeof(char) * 50);
			int i;
			for (i = nextid; i < nextfree; i++) {
				str[i - nextid] = ST[i];
			}
			str[i - nextid] = '\0';
			PrintError(err, str);
			free(str);
			nextfree = nextid;
			return;
		}
		if (nextfree - nextid > 12) {
			err = toolong;
			PrintError(err, NULL);
		}
	}
}

/* 서영
   ComputeHS - Compute the hash code of identifier by summing the ordinal values of its
   characters and then taking the sum modulo the size of HT. */
void ComputeHS(int nid, int nfree) {
	int code = 0;
	char ch;
	for (int i = nid; i < nfree; i++) {
		if ((ST[i] >= 'A') && (ST[i] <= 'Z')) {
			ch = ST[i] - 'A' + 'a';

		}
		else {
			ch = ST[i];
		}
		code += (int)ch;

	}
	hashcode = (code % HTsize) + 1;
	if (hashcode == 100) hashcode = 0;
}

/* 효원
   LookupHS - For each identifier, Look it up in the hashtable for previous occurrence
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
				if (ST[i] != ST[j] && toupper(ST[i]) != toupper(ST[j]))
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

/* 효원
   ADDHT - Add a new identifier to the hash table.
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

//지원
void CheckHT() {
	if (nextfree == STsize) {
		err = overst;
		PrintError(err, NULL);
	}
	ST[nextfree++] = '\0';

	ComputeHS(nextid, nextfree);
	LookupHS(nextid, hashcode);

	if (!found) {
		printf("%6d      \t\t", nextid);
		for (int i = nextid; i < nextfree - 1; i++) {
			printf("%c", ST[i]);
		}
		printf("       \t(entered)\n");
		ADDHT(hashcode);
	}
	else {
		printf("%6d      \t\t", sameid);
		for (int i = nextid; i < nextfree; i++)
			printf("%c", ST[i]);
		printf("       \t(already existed)\n");
		nextfree = nextid;
	}
}


/* 효진
MAIN - Read the identifier from the file directly into ST.
Compute its hashcode.
Look up the idetifier in hashtable HT[hashcode]
If matched, delete the identifier from ST and print ST-index
of the matching identifier.
If not matched , add a new element to the list,pointing to new identifier.
Print the identifier,its index in ST, and whether it was entered or present.
Print out the hashtable,and number of characters used up in ST
*/
int main() {
	PrintHeading();
	initialize();

	while (input != EOF) {
		err = noerror;
		SkipSeperators();
		ReadID();

		if (input != EOF && err != illid && err != toolong && err != illsp) {
			CheckHT();
		}
	}
	PrintHStable();
}
