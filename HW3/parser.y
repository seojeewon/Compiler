%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include "glob.h"


/*김서영 김효진 서지원 손효원*/
int isTypeVoid=0;
int isTypeInt=0;
int isTypeChar=0;
int isTypeConst=0;

extern void PrintError(ERRtypes err);
extern int yylex();
extern yyerror(char *s);
/*yacc source for Mini C*/
void semantic(int);
%}

%token TIDENT TCHAR TNUMBER TCONST TELSE TIF TEIF TINT TRETURN TVOID TWHILE
%token TADDASSIGN TSUBASSIGN TMULASSIGN TDIVASSIGN TMODASSIGN
%token TOR TAND TEQUAL TNOTEQU TGREATE TLESSE TINC TDEC TSMALL TBIG
%token TADD TSUB TMUL TDIV TMOD TIS TNOT 
%token TCOMMA TSEMI TLBRACE TRBRACE TLPAREN TRPAREN TLBRACKET TRBRACKET
%nonassoc TIF_ERROR TIF_CONDITION_ERROR
%nonassoc TELSE_ERROR TELSE_CONDITION_ERROR
%nonassoc UIF
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
type_qualifier 		: TCONST					{isTypeConst = 1};
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
						isTypeFloat=0;
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
						PrintError(noParam);	/* error - wrong function parameter */
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
			PrintError(emptycomma);
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
		| declarator TEQUAL TNUMBER			
		;
declarator 	: TIDENT
		{
			if(current_id->type==0)
			{
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
		;
while_st 		: TWHILE TLPAREN expression TRPAREN statement 		
		| TWHILE TLPAREN TRPAREN
		{
			yyerrok;
			PrintError(nowhilecondition);
		}
		;
return_st 	: TRETURN opt_expression TSEMI			
		;
expression 	: assignment_exp				
		;
assignment_exp 	: logical_or_exp				
		| unary_exp TEQUAL assignment_exp 		
		| unary_exp TADDASSIGN assignment_exp 	
		| unary_exp TSUBASSIGN assignment_exp 	
		| unary_exp TMULASSIGN assignment_exp 	
		| unary_exp TDIVASSIGN assignment_exp 	
		| unary_exp TMODASSIGN assignment_exp 	
		;
logical_or_exp 	: logical_and_exp				
		| logical_or_exp TOR logical_and_exp 	
		;
logical_and_exp 	: equality_exp					
		| logical_and_exp TAND equality_exp 	
		;
equality_exp 	: relational_exp				
		| equality_exp TEQUAL relational_exp 	
		| equality_exp TNOTEQU relational_exp 	
		;
relational_exp 	: additive_exp 				
		| relational_exp TBIG additive_exp 		
		| relational_exp TSMALL additive_exp 		
		| relational_exp TGREATE additive_exp 	
		| relational_exp TLESSE additive_exp 	
		;
additive_exp 	: multiplicative_exp				
		| additive_exp TADD multiplicative_exp 	
		| additive_exp TSUB multiplicative_exp 	
		;
multiplicative_exp 	: unary_exp					
		| multiplicative_exp TMUL unary_exp 		
		| multiplicative_exp TDIV unary_exp 		
		| multiplicative_exp TMOD unary_exp 		
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

