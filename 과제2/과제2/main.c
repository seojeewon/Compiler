/*
* main.c - �� token�� ���� ���
* programmer-�輭��, ��ȿ��, ������, ��ȿ��
*/

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"	//token name define
//#include "global.h"

//extern void SkipSeperators();
extern void reporterror(char* string);
extern yylex();
extern int yywrap();
extern char* yytext;
int errcnt;
int line=1;


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
		printf("%-11d\t", line);
		switch (tn)
		{
		case TIDENT: reporterror(yytext); break;
			break;
		case TPLUS: printf("TPLUS\t\t\t\t%s\n", yytext);
			break;
		case TMUL: printf("TMUL\t\t\t\t%s\n", yytext);
			break;
		case TMOD: printf("TMOD\t\t\t\t%s\n", yytext);
			break;
		case TMINUS: printf("TMINUS\t\t\t\t%s\n", yytext);
			break;
		case TDIV: printf("TDIV\t\t\t\t%s\n", yytext);
			break;
		case TCONST: printf("TCONST\t\t\t\t%s\n", yytext);
			break;
		case TELSE: printf("TELSE\t\t\t\t%s\n", yytext);
			break;
		case TIF: printf("TIF\t\t\t\t%s\n", yytext);
			break;
		case TINT: printf("TINT\t\t\t\t%s\n", yytext);
			break;
		case TRETURN: printf("TRETURN\t\t\t\t%s\n", yytext);
			break;
		case TVOID: printf("TVOID\t\t\t\t%s\n", yytext);
			break;
		case TWHILE: printf("TWHILE\t\t\t\t%s\n", yytext);
			break;
		case TASSIGN: printf("TASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TADDASSIGN: printf("TADDASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TSUBASSIGN: printf("TSUBASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TMULASSIGN: printf("TMULASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TDIVASSIGN: printf("TDIVASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TMODASSIGN: printf("TMODASSIGN\t\t\t\t%s\n", yytext);
			break;
		case TNOT: printf("TNOT\t\t\t\t%s\n", yytext);
			break;
		case TAND: printf("TAND\t\t\t\t%s\n", yytext);
			break;
		case TOR: printf("TOR\t\t\t\t%s\n", yytext);
			break;
		case TEQUAL: printf("TEQUAL\t\t\t\t%s\n", yytext);
			break;
		case TNOTEQU: printf("TNOTEQU\t\t\t\t%s\n", yytext);
			break;
		case TLESS: printf("TLESS\t\t\t\t%s\n", yytext);
			break;
		case TGREAT: printf("TGREAT\t\t\t\t%s\n", yytext);
			break;
		case TLESSE: printf("TLESSE\t\t\t\t%s\n", yytext);
			break;
		case TGREATE: printf("TGREATE\t\t\t\t%s\n", yytext);
			break;
		case TINC: printf("TINC\t\t\t\t%s\n", yytext);
			break;
		case TDEC: printf("TDEC\t\t\t\t%s\n", yytext);
			break;
		case TLPAREN: printf("TLPAREN\t\t\t\t%s\n", yytext);
			break;
		case TRPAREN: printf("TRPAREN\t\t\t\t%s\n", yytext);
			break;
		case TCOMMA: printf("TCOMMA\t\t\t\t%s\n", yytext);
			break;
		case TSEMI: printf("TSEMI\t\t\t\t%s\n", yytext);
			break;
		case TLBRACE: printf("TLBRACE\t\t\t\t%s\n", yytext);
			break;
		case TRBRACE: printf("TRBRACE\t\t\t\t%s\n", yytext);
			break;
		case TLBRACKET: printf("TLBRACKET\t\t\t\t%s\n", yytext);
			break;
		case TRBRACKET: printf("TRBRACKET\t\t\t\t%s\n", yytext);
			break;
		case TNUMBER: printf("TNUMBER\t\t\t\t%s\n", yytext);
			break;
		case TCOMMENT_LINE: printf("TCOMMENT_LINE\t\t\t\t%s\n", yytext);
			break;
		case TCOMMENT_TEXT: printf("TCOMMENT_TEXT\t\t\t\t%s\n", yytext); countline(yytext);
			break;
		case TBQUOTE: printf("TBQUOTE\t\t\t\t%s\n", yytext);
			break;
		case TSQUOTE: printf("TSQUOTE\t\t\t\t%s\n", yytext);
			break;
		default:
			break;
		}
	}
	return;
}

void main() {
	enum tokentypes tn;
	enum errorTypes err;

	//print header
	printf("============================================================\n");
	printf(" Line number\tToken type\tST-index\tToken\n");
	printf("============================================================\n");

	//scan tokens using the Flex lexical analyzer
	while ((tn = yylex()) != EOF) {
		if(yywrap==1){
			break;
		}
		printtoken(tn);
	}

	if (errcnt == 0)
		printf("\n\n*******************No errors defected");
	else
		printf("\n\n*******************%d errors detected\n\n", errcnt);
	printf("============================================================\n");
}