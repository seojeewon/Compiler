/*
 * Symbol.c - Symbol table management
 */

#include <stdio.h>
#include <stdlib.h>
#include "glob.h"

void PrintError(ERRORtypes err);
extern char *yytext;
extern int yyleng;

char print_ST[STsize];	//ST for printing the results
int p_nextfree = 0;		//nextfree of print_ST
int str_length;			//count length of string to print the results nicely


/*
 * computeHS() - Compute the hash code of identifier by summing the ordinal values of 
 *             its charactors an then taking the sum modulo the size of HT
 */
void ComputeHS(int nid,int nfree)
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

/*
 * LookupHS() - For each identifier, Look it up in the hashtable for previous occurrence
 *              of the identifier. If fine a match, set the found flag as true. Otherwise flase.
 *              If fine a match, save the starting index of ST in same id
 */
void LookupHS(int nid,int hscode)
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

/*
 * ADDHT() - Add a new identifier to the hash table.
 *           If list head ht[hashcode] is null, simply add a list element with
 *           starting index of the identifier in ST.
 *           If list head is not a null, it adds a new identifier to the head of the chain
 */
void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	// 맨 처음에 들어온 identifier
	if (HT[hscode] == NULL) {
		ptr->type = 0;
		ptr->index = nextid;
		ptr->line = cLine;
		ptr->next = NULL;
		HT[hscode] = ptr;		
		ptr->isConst = 0;

	}
	else {
		ptr->type = 0;
		ptr->index = nextid;
		ptr->line = cLine;
		ptr->next = HT[hscode];
		HT[hscode] = ptr;
	}
	current_id = ptr;
	
}

/*
 * SymbolTable() - If read the identifier, symbol table management 
 */
int SymbolTable()
{
	err = noerror;
	if((nextfree == STsize) || ((nextfree+yyleng) > STsize)) {
		err = overst;
		PrintError(err);
	}

	//READ identifier
	for (int i = 0; i<yyleng; i++) {
		ST[nextfree++] = yytext[i];
	}
	ST[nextfree++] = '\0';

	ComputeHS(nextid, nextfree);
	LookupHS(nextid, hashcode);
	if (!found) {
		ADDHT(hashcode);
		nextid = nextfree;
	}
	else {
		nextfree = nextid;
	}
	
	return 1;
}

void PrintHStable() {
	HTpointer here;
	int i, j;
	printf("\n\t[Hash Table : Identifier Information]\n\n");
	for (i = 0; i < HTsize; i++) {
		if (HT[i] != NULL) {
			here = HT[i];
			do {
				printf("\tHash Code %4d : (", i);
				for (j = here->index; ST[j] != '\0'; j++)
					printf("%c", ST[j]);
				printf(" : ");
				if (here->isConst == 1)
					printf("const ");
				switch (here->type)
				{
				case 1:
					printf("integer scalar variable, line: %d)\n", here->line);
					break;
				case 2:
					printf("char scalar variable, line: %d)\n", here->line);
					break;
				case 3:
					printf("integer array variable, line: %d)\n", here->line);
					break;
				case 4:
					printf("char array variable, line: %d)\n", here->line);
					break;
				case 5:
					printf("function name, return type = int, line: %d)\n", here->line);
					break;
				case 6:
					printf("function name, return type = void, line: %d)\n", here->line);
					break;
				case 7:
					printf("function name, return type = char, line: %d)\n", here->line);
					break;
				case 8:
					printf("function parameter, line: %d)\n", here->line);
					break;
				case 9:
					printf("not defined identifier or function, line: %d)\n", here->line);
					break;
				}
				here = here->next;
			} while (here != NULL);
		}
	}

}