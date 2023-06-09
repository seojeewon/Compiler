%{
/*
 * scanner.l - lexical analyzer for the MiniC
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tn.h"
#include "glob.h"

extern int cLine;
extern int cErrors;

void PrintError(ERRORtypes err);
int SymbolTable();
%}

%%
"const"				return(TCONST);
[cC][oO][nN][sS][tT]		{ err = illid_illegal; PrintError(err);}
"else" 				return(TELSE);
[eE][lL][sS][eE]			{ err = illid_illegal; PrintError(err);}
"if" 				return(TIF);
[iI][fF]				{ err = illid_illegal; PrintError(err);}
"int"				return(TINT);
[iI][nN][tT]			{ err = illid_illegal; PrintError(err);}
"char"				return(TCHAR);
[cC][hH][aA][rR]			{ err = illid_illegal; PrintError(err);}
"return"				return(TRETURN);
[rR][eE][tT][uU][rR][nN]		{ err = illid_illegal; PrintError(err);}
"void" 				return(TVOID);
[vV][oO][iI][dD]			{ err = illid_illegal; PrintError(err);}
"while" 				return(TWHILE);
[wW][hH][iI][lL][eE]		{ err = illid_illegal; PrintError(err);}

"+" 				return(TADD);
"-" 				return(TSUB);
"*" 				return(TMUL);
"/" 				return(TDIV);
"%" 				return(TMOD);

"=" 				return(TASSIGN);
"+=" 				return(TADDASSIGN);
"-=" 				return(TSUBASSIGN);
"*=" 				return(TMULASSIGN);
"/=" 				return(TDIVASSIGN);
"%=" 				return(TMODASSIGN);

"!" 				return(TNOT);
"&&" 				return(TAND);
"||" 				return(TOR);

"==" 				return(TEQUAL);
"!="	 			return(TNOTEQU);
"<" 				return(TSMALL);
">" 				return(TBIG);
"<=" 				return(TLESSE);
">=" 				return(TGREATE);

"++" 				return(TINC);
"--" 				return(TDEC);

"(" 				return(TLPAREN);
")" 				return(TRPAREN);
"," 				return(TCOMMA);
"{"				return(TLBRACE);
"}"				return(TRBRACE);
"["				return(TLBRACKET);
"]"				return(TRBRACKET);
";" 				return(TSEMI);

[1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?	return(TNUMBER);

[A-Za-z_][A-Za-z0-9_]* 		{ 
					if(yyleng > MAX_LEN)
						{ err = illid_long; PrintError(err);}
					else
						{ SymbolTable(); return(TIDENT);}
				}



"//".*				;
"/*"([^*]|\*+[^*/])*\**"*/"		{
					for(int i=0;i<yyleng;i++)
						if(yytext[i]=='\n') cLine++;
					;

				}

[ \t]			 	;
"\n" 				{ cLine++;}

[0-9]+[A-Za-z0-9_]+ 		{ err = illid_digit; PrintError(err);}

. 				{ err = illid_illch; PrintError(err);}
%%

int yywrap()
{
	return 1;
}