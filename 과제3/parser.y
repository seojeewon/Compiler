%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include "glob.h"



int isTypeVoid = 0;
int isTypeInt = 0;
int isTypeChar = 0;
int isTypeConst = 0;

extern int yylex();
extern yyerror(char *s);
/*yacc source for Mini C*/
/*서지원 김서영 손효원 김효진*/
%}

%token TIDENT TCHAR TNUMBER TCONST TELSE TIF TEIF TINT TRETURN TVOID TWHILE
%token TADDASSIGN TSUBASSIGN TMULASSIGN TDIVASSIGN TMODASSIGN
%token TOR TAND TEQUAL TNOTEQU TGREATE TLESSE TINC TDEC TSMALL TBIG
%token TADD TSUB TMUL TDIV TMOD TIS TNOT TASSIGN
%token TCOMMA TSEMI TLBRACE TRBRACE TLPAREN TRPAREN TLBRACKET TRBRACKET

%left '+' '-'
%left '*' '/'
%right '='
%nonassoc LOWER_THAN_ELSE
%nonassoc TELSE

%%
mini_c 			: translation_unit			;
translation_unit 	: external_dcl					
			| translation_unit external_dcl		;
external_dcl 		: function_def					
		  	| declaration					
			| TIDENT TSEMI
			| TIDENT error{
				yyerrok;
				PrintError(wrong_stat);	/* error - wrong statement */
			};
function_def 		: function_header compound_st	
			|function_header TSEMI	
			|function_header error{
			 	current_tmp->type=0;
			 	yyerrok;
			 	PrintError(wrong_funcdef);	/* error - wrong function definition */
			}
			|error compound_st 			{yyerrok; PrintError(noFuncHeader)};
function_header 	: dcl_spec function_name formal_param	;
dcl_spec 		: dcl_specifiers				;
dcl_specifiers 		: dcl_specifier					
		 	| dcl_specifiers dcl_specifier			;
dcl_specifier 		: type_qualifier					
			| type_specifier				;
type_qualifier 		: TCONST					{isTypeConst = 1;};
type_specifier 		: TINT						{isTypeInt=1; isTypeVoid=0; isTypeChar=0;}
		 	| TVOID						{isTypeInt=0; isTypeVoid=1; isTypeChar=0;}
			| TCHAR						{isTypeInt=0; isTypeVoid=0; isTypeChar=1;}

;
function_name 	: TIDENT{
					if(current_id->type==0 || current_id->type==9){
						if(isTypeInt==1){	/*return int function name*/
							current_id->type=5;
						}									
						else if(isTypeVoid==1){	/*return void function name*/
							current_id->type=6;
						}
						else{current_id->type=7;} 	/*return char function name*/
						isTypeInt=0;
						isTypeChar=0;
						isTypeVoid=0;
						current_tmp=current_id;
					}
		}
		|
		{
			yyerrok;
			PrintError(wrong_funcdef);
		}
		;
formal_param 		: TLPAREN opt_formal_param TRPAREN
					| TLPAREN opt_formal_param error{
						yyerrok;
						PrintError(noParen);	/* error - no paranthesis */
					};
opt_formal_param 	: formal_param_list				
		|						
		;
formal_param_list 	: param_dcl					
		| formal_param_list TCOMMA param_dcl
		| formal_param_list dcl_spec declarator{
			yyerrok;	
			PrintError(noComma);	/* error - No Comma */
		}
		|formal_param_list TCOMMA error{
			yyerrok;
			PrintError(noComma);
		};
param_dcl 	: dcl_spec declarator{
			if(current_id->type == 1){
				isTypeInt=0;
				isTypeVoid=0;
				isTypeChar=0;
				current_id->type=8;	/* function parameter */
			}								
		}
		;
compound_st 	: TLBRACE opt_dcl_list opt_stat_list TRBRACE
		| TLBRACE opt_dcl_list opt_stat_list error{
			yyerrok;
			PrintError(nobrace);
		};
opt_dcl_list 	: declaration_list				
		|						
		;
declaration_list 	: declaration					
		| declaration_list declaration 			
		;
declaration 	: dcl_spec init_dcl_list TSEMI
		{
			isTypeConst=0;
			isTypeChar=0;
			isTypeInt=0;
			isTypeVoid=0;
		}
		| dcl_spec init_dcl_list error{
			yyerrok;
			//previous_id->type=0;
			isTypeConst=0;
			isTypeChar=0;
			isTypeInt=0;
			isTypeVoid=0;
			PrintError(nosemi); 
		}
		;
init_dcl_list 		: init_declarator				
			| init_dcl_list TCOMMA init_declarator 		
			| init_dcl_list TCOMMA error{
				yyerrok;
				PrintError(noComma);
			};
init_declarator 	: declarator						
		| declarator TASSIGN TNUMBER			
		;
declarator 	: TIDENT
		{
			if(current_id->type==0)
			{
				if(isTypeConst == 1){
					current_id->isConst=1;	
				}
				else{
					current_id->isConst=0;
				}
				if(isTypeInt == 1) current_id->type=1;		/*integer scalar variable*/
				if(isTypeChar == 1) current_id->type=2;		/*char scalar variable*/
				
			}
			current_tmp=current_id;
		}						
	     	| TIDENT TLBRACKET opt_number TRBRACKET
		{
			if(current_id->type==0)
			{
				if(isTypeInt == 1) current_id->type=3;		/*integer array variable*/
				if(isTypeChar == 1) current_id->type=4;		/*char array variable*/
			}
			current_tmp=current_id;
		}
		| TIDENT TLBRACKET opt_number error
		{
			yyerrok;
			PrintError(nobracket);	/* error - No Bracket */
		}			
		;
opt_number 		: TNUMBER					
	     		|						
			;
opt_stat_list 		: statement_list				
		 	|						
			;
statement_list 		: statement					
                           	| statement_list statement 
		 	| statement_list declaration 			
			;
statement 		: compound_st				
	   		| expression_st				
	   		| if_st						
	   		| while_st					
	   		| return_st					
	   		;
expression_st 	: opt_expression TSEMI				
		| expression error
		{
			yyerrok;
			PrintError(nosemi);
		}
		;
opt_expression 	: expression					
		|						
		;
if_st 		: TIF TLPAREN expression TRPAREN statement	%prec LOWER_THAN_ELSE 
		| TIF TLPAREN expression TRPAREN statement TELSE statement 	
		| TIF TLPAREN TRPAREN
		{
			yyerrok;
			PrintError(noifcondition);	/* error - no condition */
		}
		| TIF TLPAREN expression error
		{
			yyerrok;
			PrintError(noParen);
		}
		;
while_st 		: TWHILE TLPAREN expression TRPAREN statement 		
		| TWHILE TLPAREN TRPAREN
		{
			yyerrok;
			PrintError(nowhilecondition);
		}
		| TWHILE TLPAREN expression error
		{
			yyerrok;
			PrintError(noParen);
		}
		;
return_st 	: TRETURN opt_expression TSEMI
		| TRETURN expression error
		{
			yyerrok;
			PrintError(nosemi);		/* error - no semicolon */
		}		
		;
expression 	: assignment_exp				
		;
assignment_exp 	: logical_or_exp				
		| unary_exp TASSIGN assignment_exp 	%prec TEQUAL	
		| unary_exp TADDASSIGN assignment_exp 	
		| unary_exp TSUBASSIGN assignment_exp 	
		| unary_exp TMULASSIGN assignment_exp 	
		| unary_exp TDIVASSIGN assignment_exp 	
		| unary_exp TMODASSIGN assignment_exp
		| unary_exp TASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		}
		| unary_exp TADDASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		}
		| unary_exp TSUBASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		}
		| unary_exp TMULASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		}
		| unary_exp TDIVASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		}
		| unary_exp TMODASSIGN
		{
			yyerrok;
			PrintError(wrong_stat);	
		} 	
		;
logical_or_exp 	: logical_and_exp				
		| logical_or_exp TOR logical_and_exp
		| logical_or_exp TOR
		{
			yyerrok;
			PrintError(wrong_stat);
		} 	
		;
logical_and_exp 	: equality_exp					
		| logical_and_exp TAND equality_exp
		| logical_and_exp TAND
		{
			yyerrok;
			PrintError(wrong_stat);
		}  	
		;
equality_exp 	: relational_exp				
		| equality_exp TEQUAL relational_exp 	
		| equality_exp TNOTEQU relational_exp
		| equality_exp TEQUAL
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| equality_exp TNOTEQU
		{
			yyerrok;
			PrintError(wrong_stat);
		}  	
		;
relational_exp 	: additive_exp 				
		| relational_exp TBIG additive_exp 		
		| relational_exp TSMALL additive_exp 		
		| relational_exp TGREATE additive_exp 	
		| relational_exp TLESSE additive_exp
		| relational_exp TBIG
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| relational_exp TSMALL
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| relational_exp TGREATE
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| relational_exp TLESSE 
		{
			yyerrok;
			PrintError(wrong_stat);
		}	
		;
additive_exp 	: multiplicative_exp				
		| additive_exp TADD multiplicative_exp 	
		| additive_exp TSUB multiplicative_exp
		| additive_exp TADD error
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| additive_exp TSUB error
		{
			yyerrok;
			PrintError(wrong_stat);
		} 	
		;
multiplicative_exp 	: unary_exp					
		| multiplicative_exp TMUL unary_exp 		
		| multiplicative_exp TDIV unary_exp 		
		| multiplicative_exp TMOD unary_exp
		| multiplicative_exp TMUL error
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| multiplicative_exp TDIV error
		{
			yyerrok;
			PrintError(wrong_stat);
		}
		| multiplicative_exp TMOD error
		{
			yyerrok;
			PrintError(wrong_stat);
		} 		
		;
unary_exp 	: postfix_exp					
	   	| TSUB unary_exp				
	   	| TNOT unary_exp				
	   	| TINC unary_exp				
	   	| TDEC unary_exp				
		;
postfix_exp 	: primary_exp					
	      	| postfix_exp TLBRACKET expression TRBRACKET 		
	      	| postfix_exp TLPAREN opt_actual_param TRPAREN 	
	      	| postfix_exp TINC				
	      	| postfix_exp TDEC
		| postfix_exp TLBRACKET expression error
		{
			yyerrok;
			PrintError(nobracket);
		}
		| postfix_exp TLPAREN opt_actual_param error
		{
			yyerrok;
			PrintError(noParen);
		}				
		;
opt_actual_param 	: actual_param				
		|						
		;
actual_param 	: actual_param_list				
		;
actual_param_list 	: assignment_exp				
		| actual_param_list TCOMMA assignment_exp 	
		;
primary_exp 	: TIDENT
		{
			if(current_id->type==0) current_id->type=9;	 /* not defined identifier or function */
		}						
	     	| TNUMBER					
	     	| TLPAREN expression TRPAREN				
		;
%%

