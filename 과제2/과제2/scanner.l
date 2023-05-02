%{
/* scanner.l - lexical analyzer for the MiniC
 * programmer - 김서영, 김효진, 서지원, 손효원
 * date - 4/24/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h" /* token name definition */
# include "global.h" /* global variable */

typedef enum errorTypes ERRORtypes;
ERRORtypes err;
extern reporterror(char* string);

%}

A	[a-zA-Z_]
D	[0-9]

%%

"+"				return(TPLUS);
"*"				return(TMUL);
"%"				return(TMOD);
"/"				return(TDIV);
"-"				return(TMINUS);

"const"				return(TCONST);
"else"				return(TELSE);
"if"				return(TIF);
"int"				return(TINT);
"return"				return(TRETURN);
"void"				return(TVOID);
"while"				return(TWHILE);

"="				return(TASSIGN);
"+="				return(TADDASSIGN);
"-="				return(TSUBASSIGN);
"*="				return(TMULASSIGN);
"/="				return(TDIVASSIGN);
"%="				return(TMODASSIGN);

"!"				return(TNOT);
"&&" 				return(TAND);
"||"				return(TOR);

"=="				return(TEQUAL);
"!="				return(TNOTEQU);
"<"				return(TLESS);
">"				return(TGREAT);
"<="				return(TLESSE);
">="				return(TGREATE);

"++"				return(TINC);
"--"				return(TDEC);

"\""				return(TBQUOTE);
"\'"				return(TSQUOTE);

"("				return(TLPAREN);
")"				return(TRPAREN);
","				return(TCOMMA);
";"				return(TSEMI);
"{"				return(TLBRACE);
"}"				return(TRBRACE);
"["				return(TLBRACKET);
"]"				return(TRBRACKET);

{A}({A}|{D})* 	reporterror(yytext);
([1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?) 	return(TNUMBER);
{D}+({A}|{D})+			{err=illid; return(TERROR);}


"/*"([^*]|"*"+[^*/])*"*"+"/" 		return(TCOMMENT_TEXT);
"//".* 				return(TCOMMENT_LINE);
[ \t]				;
"\n"				return(TLINE);
.				{err=illid; return(TERROR);}
%%

int yywrap()
{
	return 1;
}