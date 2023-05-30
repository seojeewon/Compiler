/*
 * main.c - token, 에러 개수 출력
 */
#include <stdio.h>
#include <stdlib.h>
#include "tn.h" 
#include "glob.h" 

extern yylex();
extern char *yytext;
extern void PrintHStable();


void main()
{
    enum tokentypes tn;
	cLine = 1;
	cErrors = 0;

	//print header
	printf("\n\n\t******MiniC parsing begins******\n");
	printf("\n\t[Error Report : Error Information]\n");
	yyparse();
	printf("\n\t******Parsing ends******\n");

	if (cErrors == 0) {
		printf("\n\t******no error detected\n\n");
		PrintHStable();
	}
	else {
		printf("\n\t******%d error(s) detected\n\n");
	}
}