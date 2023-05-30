%{
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>


/*김서영 김효진 서지원 손효원*/
int isTypeVoid=0;
int isTypeInt=0;
int isTypeVoid=0;
int isTypeFloat=0;

extern void PrintError(ERRPRtypes err);
extern int yylex();
extern yyerror(s);
/*yacc source for Mini C*/
void semantic(int);
%}

%token TIDENT TFLOAT TNUMBER TCONST TELSE TIF TEIF TINT TRETURN TVOID TWHILE
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
				PrintError(wrong_stat);
			};
function_def 		: function_header compound_st	
			|function_header TSEMI	
			|function_header error{
			 previous_id->type=0; //parse error로 정의
			 yyerrok;
			 PrintError(wrong_funcdef);
			}
			|error compound_st 			{yyerrok; PrintError(noFuncHeader)};
function_header 	: dcl_spec function_name formal_param	;
dcl_spec 		: dcl_specifiers				;
dcl_specifiers 		: dcl_specifier					
		 	| dcl_specifiers dcl_specifier			;
dcl_specifier 		: type_qualifier					
			| type_specifier				;
type_qualifier 		: TCONST					{isTypeConst = 1};
type_specifier 		: TINT						{isTypeInt=1;}
			| TFLOAT					{isTypeFloat=1;};
		 	| TVOID						{isTypeVoid=1;};
function_name 	: TIDENT{
					if(current_id->type==0 || current_id->type==8){
						if(isTypeInt=1){
							current_id->type=6;
						}
						else if(isTypeFloat==1){
							current_id->type=7;
						}
						else if(isTypeVoid==1){
							current_id->type=5;
						}
						isTypeInt=0;
						isTypeFloat=0;
						isTypeVoid=0;
						func_id=current_id;
					}
				};
formal_param 		: TLPAREN opt_formal_param TRPAREN
					| TLPAREN opt_formal_param error{
						yyerrok;
						PrintError(noparen);
					};
opt_formal_param 	: formal_param_list				{semantic(18);}
		   	|						{semantic(19);};
formal_param_list 	: param_dcl					{semantic(20);}
		    	| formal_param_list TCOMMA param_dcl
				| formal_param_list dcl_spec declarator{
					PrintError(nocomma);
				}
				|formal_param_list TCOMMA error{
					yyerrok;
					PrintError(emptycomma);
				};
param_dcl 		: dcl_spec declarator{
					//함수 파라미터 타입
					current_id->func_idx=func_id->index;
					isTypeInt=0;
					isTypeFloat=0;
				}
			| declarator{yyerrok; PrintError(noparam_spec);}
			;
compound_st 		: TLBRACE opt_dcl_list opt_stat_list TRBRACE
					| TLBRACE opt_dcl_list opt_stat_list error{
						yyerrok;
						PrintError(nobrace);
					};
opt_dcl_list 		: declaration_list				{semantic(24);}
			|						{semantic(25);};
declaration_list 	: declaration					{semantic(26);}
			| declaration_list declaration 			{semantic(27);};
declaration 		: dcl_spec init_dcl_list TSEMI{
						isTypeConst=0;
						isTypeFloat=0;
						isTypeInt=0;
						isTypeVoid=0;
					}
					| dcl_spec init_dcl_list error{
						previous_id->type=0;
						isTypeConst=0;
						isTypeFloat=0;
						isTypeInt=0;
						isTypeVoid=0;
						yyerrok;
						PrintError(nosemi) //세미콜론없음
					};
init_dcl_list 		: init_declarator				{semantic(29);}
			| init_dcl_list TCOMMA init_declarator 		{semantic(30);}
			| init_dcl_list TCOMMA error{
				yyerrok;
				PrintError(emptycomma);
			};
init_declarator 	: declarator						{semantic(31);}
		 	| declarator TEQUAL TNUMBER			{semantic(32);};
declarator 		: TIDENT						{semantic(33);}
	     		| TIDENT TLBRACKET opt_number TRBRACKET			{semantic(34);};
opt_number 		: TIDENT					{semantic(35);}
	     		|						{semantic(36);};
opt_stat_list 		: statement_list				{semantic(37);}
		 	|						{semantic(38);};
statement_list 		: statement					{semantic(39);}
                             | statement_list error 
		 	| statement_list statement 			{semantic(40);};
statement 		: compound_st				{semantic(41);}
	   		| expression_st				{semantic(42);}
	   		| if_st						{semantic(43);}
	   		| while_st					{semantic(44);}
	   		| return_st					{semantic(45);}
	   		;
expression_st 	: opt_expression TSEMI				{semantic(46);};
opt_expression 	: expression					{semantic(47);}
		 	|						{semantic(48);};
if_st 			: TIF TLPAREN expression TRPAREN statement	%prec LOWER_THAN_ELSE {semantic(49);}
			| TIF TLPAREN expression TRPAREN statement TELSE statement 	{semantic(50);};
while_st 		: TWHILE TLPAREN expression TRPAREN statement 		{semantic(51);};
return_st 		: TRETURN opt_expression TSEMI			{semantic(52);};
expression 		: assignment_exp				{semantic(53);};
assignment_exp 	: logical_or_exp				{semantic(54);}
			| unary_exp '=' assignment_exp 		{semantic(55);}
			| unary_exp TADDASSIGN assignment_exp 	{semantic(56);}
			| unary_exp TSUBASSIGN assignment_exp 	{semantic(57);}
			| unary_exp TMULASSIGN assignment_exp 	{semantic(58);}
			| unary_exp TDIVASSIGN assignment_exp 	{semantic(59);}
			| unary_exp TMODASSIGN assignment_exp 	{semantic(60);}
			;
logical_or_exp 	: logical_and_exp				{semantic(61);}
			| logical_or_exp TOR logical_and_exp 	{semantic(62);};
logical_and_exp 	: equality_exp					{semantic(63);}
		 	| logical_and_exp TAND equality_exp 	{semantic(64);};
equality_exp 		: relational_exp				{semantic(65);}
			| equality_exp TEQUAL relational_exp 	{semantic(66);}
			| equality_exp TNOTEQU relational_exp 	{semantic(67);};
relational_exp 	: additive_exp 				{semantic(68);}
			| relational_exp TBIG additive_exp 		{semantic(69);}
			| relational_exp TSMALL additive_exp 		{semantic(70);}
			| relational_exp TGREATE additive_exp 	{semantic(71);}
			| relational_exp TLESSE additive_exp 	{semantic(72);};
additive_exp 		: multiplicative_exp				{semantic(73);}
			| additive_exp TADD multiplicative_exp 	{semantic(74);}
			| additive_exp TSUB multiplicative_exp 	{semantic(75);};
multiplicative_exp 	: unary_exp					{semantic(76);}
		    	| multiplicative_exp TMUL unary_exp 		{semantic(77);}
		    	| multiplicative_exp TDIV unary_exp 		{semantic(78);}
		    	| multiplicative_exp TMOD unary_exp 		{semantic(79);};
unary_exp 		: postfix_exp					{semantic(80);}
	   		| TSUB unary_exp				{semantic(81);}
	   		| TNOT unary_exp				{semantic(82);}
	   		| TINC unary_exp				{semantic(83);}
	   		| TDEC unary_exp				{semantic(84);};
postfix_exp 		: primary_exp					{semantic(85);}
	      		| postfix_exp TLBRACKET expression TRBRACKET 		{semantic(86);}
	      		| postfix_exp TLPAREN opt_actual_param TRPAREN 	{semantic(87);}
	      		| postfix_exp TINC				{semantic(88);}
	      		| postfix_exp TDEC				{semantic(89);};
opt_actual_param 	: actual_param				{semantic(90);}
		  	|						{semantic(91);};
actual_param 		: actual_param_list				{semantic(92);};
actual_param_list 	: assignment_exp				{semantic(93);}
		   	| actual_param_list TCOMMA assignment_exp 	{semantic(94);};
primary_exp 		: TIDENT						{semantic(95);}
	     		| TNUMBER					{semantic(96);}
	     		| TLPAREN expression TRPAREN				{semantic(97);};
%%

void semantic(int n)
{	
	printf("reduced rule number = %d\n",n);
}
