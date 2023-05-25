/*
 * main.c - token, ���� ���� ���
 */
#include <stdio.h>
#include <stdlib.h>
#include "tn.h" 
#include "glob.h" 

extern yylex();
extern char *yytext;
void printtoken(tn);

void main()
{
    enum tokentypes tn;
	cLine = 1;

	//print header
	printf("=======================================================\n");
	printf(" Line number   Token type    ST-index    token\n");
	printf("=======================================================\n");
		
	//scan tokens using the flex lexical analyzer 
    while((tn = yylex()) != TEOF) 
	{
		printtoken(tn);
	}

	if (num_err == 0)
		printf("\n\n*************** No errors detected\n");
	else
		printf("\n\n*************** Number of errors: %d\n\n", num_err);

	printf("=======================================================\n");
}

/*
 * printtoken() - token ���
 */
void printtoken(enum tokentypes tn){
	switch (tn) {
	        case TCONST : printf("%6d          TCONST  %22s\n", cLine, yytext); break;
	        case TELSE : printf("%6d          TELSE  %22s\n", cLine, yytext); break;
	        case TIF : printf("%6d          TIF  %22s\n", cLine, yytext); break;
	        case TINT : printf("%6d          TINT  %22s\n", cLine, yytext); break;
	        case TRETURN : printf("%6d          TRETURN  %22s\n", cLine, yytext); break;
	        case TVOID : printf("%6d          TVOID  %22s\n", cLine, yytext); break;
	        case TWHILE : printf("%6d          TWHILE  %22s\n", cLine, yytext); break;
	        case TPLUS : printf("%6d          TPLUS  %20s\n", cLine, yytext); break;
	        case TMINUS : printf("%6d          TMINUS  %19s\n", cLine, yytext); break;
	        case TSTAR : printf("%6d          TSTAR  %20s\n", cLine, yytext); break;
	        case TSLASH : printf("%6d          TSLASH  %19s\n", cLine, yytext); break;
	        case TMOD : printf("%6d          TMOD  %21s\n", cLine, yytext); break;
	        case TEQUAL : printf("%6d          TEQUAL   %19s\n", cLine, yytext); break;
	        case TNOTEQU : printf("%6d          TNOTEQU  %19s\n", cLine, yytext); break;
	        case TLESS : printf("%6d          TLESS   %19s\n", cLine, yytext); break;
	        case TGREAT : printf("%6d          TGREAT   %19s\n", cLine, yytext); break;
	        case TLESSE : printf("%6d          TLESSE   %19s\n", cLine, yytext); break;
	        case TGREATE : printf("%6d          TGREATE   %19s\n", cLine, yytext); break;
	        case TNOT : printf("%6d          TNOT  %21s\n", cLine, yytext); break;
	        case TAND : printf("%6d          TAND  %21s\n", cLine, yytext); break;
	        case TOR : printf("%6d          TOR   %21s\n", cLine, yytext); break;
	        case TINC : printf("%6d          TINC   %21s\n", cLine, yytext); break;
	        case TDEC : printf("%6d          TDEC   %21s\n", cLine, yytext); break;
	        case TADDASSIGN : printf("%6d          TADDASSIGN  %16s\n", cLine, yytext); break;
	        case TSUBASSIGN : printf("%6d          TSUBASSIGN  %16s\n", cLine, yytext); break;
	        case TMULASSIGN : printf("%6d          TMULASSIGN  %16s\n", cLine, yytext); break;
	        case TDIVASSIGN : printf("%6d          TDIVASSIGN  %16s\n", cLine, yytext); break;
	        case TMODASSIGN : printf("%6d          TMODASSIGN  %16s\n", cLine, yytext); break;
			case TCOMMA: printf("%6d          TCOMMA  %18s\n", cLine, yytext); break;
			case TLBRACE: printf("%6d          TLBRACE  %18s\n", cLine, yytext); break;
			case TRBRACE: printf("%6d          TRBRACE  %18s\n", cLine, yytext); break;
			case TLBRACKET: printf("%6d          TLBRACKET  %16s\n", cLine, yytext); break;
			case TRBRACKET: printf("%6d          TRBRACKET  %16s\n", cLine, yytext); break;
			case TCOMMENT: printf("%6d          TCOMMENT\n", cLine); break;
			case TIDENT : break;
			case THEX: printf("%6d          THEX			  %d\n", cLine, strtol(yytext, NULL, 16));  break;
			case TOCT: printf("%6d          TOCT			  %d\n", cLine, strtol(yytext, NULL, 8));  break;
	        case TNUMBER : printf("%6d          TNUMBER   %18d\n", cLine, atoi(yytext)); break;
	        case TERROR : break;
	        case TSEMI : printf("%6d          TSEMI   %19s\n", cLine, yytext); break;
	        case TASSIGN : printf("%6d          TASSIGN   %17s\n", cLine, yytext); break;
	        case TLPAREN : printf("%6d          TLPAREN  %18s\n", cLine, yytext); break;
            case TRPAREN : printf("%6d          TRPAREN  %18s\n", cLine, yytext); break;
	        case TCARR :  break; 
		}
}