/*
* main.c - �� token�� ���� ���
* programmer-�輭��, ��ȿ��, ������, ��ȿ��
*/

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"	//token name define
#include "global.h"

//extern void SkipSeperators();
extern void reporterror(char* string);
extern yylex();
extern char* yytext;

void countline(char* string) {
	int i = 0;
	while (string[i] != '\0') {
		if (string[i] == '\n' || string[i] == '\r') line++;
		i++;
	}
}


void printtoken(enum tokentypes tn) {
	if (tn == TLINE) {
		line++;
		return;
	}
	else {
		printf("%11d\t", line);
		switch (tn)
		{
		case TIDENT: reporterror(yytext); break;
			break;
		case TPLUS: printf("Plus\t");
			break;
		case TMUL: printf("Multiply\t");
			break;
		case TMOD: printf("Mod\t");
			break;
		case TMINUS: printf("Minus\t");
			break;
		case TDIV: printf("Divide\t");
			break;
		case TCONST: printf("Constant\t");
			break;
		case TELSE: printf("Else\t");
			break;
		case TIF: printf("If\t");
			break;
		case TINT: printf("Integer\t");
			break;
		case TRETURN: printf("Return\t");
			break;
		case TVOID: printf("Void\t");
			break;
		case TWHILE: printf("While\t");
			break;
		case TASSIGN: printf("Assign\t");
			break;
		case TADDASSIGN: printf("Add and Assign\t");
			break;
		case TSUBASSIGN: printf("Subtract and Assign\t");
			break;
		case TMULASSIGN: printf("Multiply and Assign\t");
			break;
		case TDIVASSIGN: printf("Divide and Assign\t");
			break;
		case TMODASSIGN: printf("Mod and Assign\t");
			break;
		case TNOT: printf("Not\t");
			break;
		case TAND: printf("And\t");
			break;
		case TOR: printf("Or\t");
			break;
		case TEQUAL: printf("Equal\t");
			break;
		case TNOTEQU: printf("Not Equal\t");
			break;
		case TLESS: printf("Less\t");
			break;
		case TGREAT: printf("Great\t");
			break;
		case TLESSE: printf("Less Equal\t");
			break;
		case TGREATE: printf("Great Equal\t");
			break;
		case TINC: printf("Increase\t");
			break;
		case TDEC: printf("Decrease\t");
			break;
		case TLPAREN: printf("Left small bracket\t");
			break;
		case TRPAREN: printf("Right small bracket\t");
			break;
		case TCOMMA: printf("Comma\t");
			break;
		case TSEMI: printf("Semicolon\t");
			break;
		case TLBRACE: printf("Left Medium Bracket\t");
			break;
		case TRBRACE: printf("Right Medium Bracket\t");
			break;
		case TLBRACKET: printf("Left Large Bracket\t");
			break;
		case TRBRACKET: printf("Right Large Bracket\t");
			break;
		case TNUMBER: printf("Number: %d\t", atoi(yytext));
			break;
		case TCOMMENT_LINE: printf("Comment line\t");
			break;
		case TCOMMENT_TEXT: printf("Comment line\t"); countline(yytext);
			break;		
		case TBQUOTE: printf("Big Quote");
			break;
		case TSQUOTE: printf("Small Quote");
			break;
		default:
			break;
		}
	}
}

void main() {
	enum tokentypes tn;
	enum errorTypes err;
	line = 1;
	errcnt = 0;
	//print header
	printf("============================================================\n");
	printf(" Line number\tToken type\tST-index\tToken\n");
	printf("============================================================\n");

	//scan tokens using the Flex lexical analyzer
	while ((tn = yylex()) != EOF) {
		printtoken(tn);
	}

	if (errcnt == 0)
		printf("\n\n*******************No errors defected");
	else
		printf("\n\n*******************%d errors detected\n\n", errcnt);
	printf("============================================================\n");
}
