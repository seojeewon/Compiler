%{
/* scanner.l - lexical analyzer for the MiniC
 * programmer - 김서영, 김효진, 서지원, 손효원
 * date - 4/24/2023
 */

#include <stdio.h>
#include "tn.h" /* token name definition */
# include "glob.h" /* global variable */
#include <stdlib.h>
enum tnumber { TIDENT, TPLUS, TSTAR, TMOD, TSLASH, TMINUS, TCONST, TELSE, TIF, TINT, TRETURN, TVOID, TWHILE, TMOD, TASSIGN,
		TADDASSIGN, TSUBASSIGN, TMULASSIGN, TDIVASSIGN, TMODASSIGN, TNOT, TAND, TOR, TEQUAL, TNOTEQU, TLESS, TGREAT,
		TLESSE, TGREATE, TINC, TDEC, TLPAREN, TRPAREN, TCOMMA, TSEMI, TLBRACE, TRBRACE, TLBRACKET, TRBRACKET, TNUMBER, TREALNUM };
%}

A	[a-zA-Z_]
D	[0-9]

%%

"+"				return(TPLUS);
"*"				return(TSTAR);
"%"				return(TMOD);
"/"				return(TSLASH);
"-"				return(TMINUS);

"const"				return(TCONST);
"else"				return(TELSE);
"if"				return(TIF);
"int"				return(TINT);
"return"				return(TRETURN);
"void"				return(TVOID);
"while"				return(TWHILE);
"%"				return(TMOD);

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

"("				return(TLPAREN);
")"				return(TRPAREN);
"."				return(TCOMMA);
";"				return(TSEMI);
"{"				return(TLBRACE);
"}"				return(TRBRACE);
"["				return(TLBRACKET);
"]"				return(TRBRACKET);

{A}({A}|{D})* 	{if(yyleng>12){
					err=illleng; PrintError(err);return(TERROR);}
				else{
					SymbolTable(yytext);
					if(err == overst){
						return(TERROR);}
					else{return(TIDENT);}}}

([1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?) 	return(TNUMBER);
{D}+({A}|{D})+			{err=illid; PrintError(err);return(TERROR);}
*				{err=illid; PrintError(err);return(TERROR);}
%%

int yywrap()
{
	return 1;
}
