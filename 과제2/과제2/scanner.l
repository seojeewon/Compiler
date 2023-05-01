%{
/* scanner.l - lexical analyzer for the MiniC
 * programmer - 김서영, 김효진, 서지원, 손효원
 * date - 4/24/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h" /* token name definition */
# include "global.h" /* global variable */

extern ERRORtypes err;
extern SymbolTable(string yytext);
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
"return"			return(TRETURN);
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

"\""				return(TBQUOTE);
"\'"				return(TSQUOTE);

"("				return(TLPAREN);
")"				return(TRPAREN);
"."				return(TCOMMA);
";"				return(TSEMI);
"{"				return(TLBRACE);
"}"				return(TRBRACE);
"["				return(TLBRACKET);
"]"				return(TRBRACKET);

{A}({A}|{D})* 	{if(yyleng>12){
					err=illleng; return(TERROR);}
				else{
					SymbolTable(yytext); 
					//SymbolTable(identifier)넣으면 처리해줘야함
					if(err == overst){
						return(TERROR);}
					else{return(TIDENT);}}}
([1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?) 	return(TNUMBER);
{D}+({A}|{D})+			{err=illid_digit; return(TERROR);}


"/*"([^*]|"*"+[^*/])*"*"+"/" return(TCOMMENT);
"//".* 						return(TCOMMENT);
[ \t]				;
"\n"				return(TLINE);
.				{err=illid; return(TERROR);}
%%

int yywrap()
{
	return 1;
}
