/*
 * error.c - error Ãâ·Â 
*/

#include <stdio.h>
#include <stdlib.h>
#include "glob.h"

extern char *yytext;

yyerror(s)
char* s;
{

}


void PrintError(ERRORtypes err)
{
	switch(err)
	{
	case noerror: 
		break;
	case illid_digit:
		cErrors++;
		printf("\t%6d          ERROR!!			  %-10s		start with digit\n", cLine, yytext);
		break;
	case illid_long:
		cErrors++;
		printf("\t%6d          ERROR!!			  %-13s	 too long identifier\n", cLine, yytext);
		break;
	case illid_illch:
		cErrors++;
		printf("\t%6d          ERROR!!			  %-5c		illegal symbol\n", cLine, yytext[0]);
		break;
	case illid_illegal:
		cErrors++;
		printf("\t%6d          ERROR!!			  %-5s		illegal identifier\n", cLine, yytext);
		break;
	case overst:
		cErrors++;
		printf("\t\nError : OVERFLOW in ST\n");
		printf("==================================================\n");
		exit(0);
		break;
	case wrong_stat:
		cErrors++;
		printf("\t%6d          ERROR!!			       		Wrong Statement\n", cLine);
		break;

	case wrong_funcdef:
		cErrors++;
		printf("\t%6d          ERROR!!			       		Wrong Function Definition\n", cLine);
		break;

	case noFuncHeader:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Function Header\n", cLine);
		break;

	case noParen:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Function Parenthesis\n", cLine);
		break;

	case noComma:	
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Comma\n", cLine);
		break;

	case nobrace:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Brace\n", cLine);
		break;

	case nosemi:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Semicolon\n", cLine);
		break;

	case nobracket:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Bracket\n", cLine);
		break;

	case noifcondition:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Condition for IF statement\n", cLine);
		break;

	case nowhilecondition:
		cErrors++;
		printf("\t%6d          ERROR!!			       		No Condition for WHILE statement\n", cLine);
		break;
	}
	
}
