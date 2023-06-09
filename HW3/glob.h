/*
 * glob.h - global variable for the project
 */
#define STsize 1000
#define HTsize 100
#define FALSE 0
#define TRUE 1

typedef struct HTentry *HTpointer;
typedef struct HTentry {	//Hash Table의 구조
	int line;	//line number
	int type;	//variable type
	int isConst;	//상수인지
	int index;	//string table의 index
	HTpointer next;
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

HTpointer current_id;	//current id
HTpointer current_tmp;	//pointer

int nextid; 
int nextfree;
int hashcode; 
int sameid;
int cLine;
int cErrors;
//int i, j, cErrors; //?

int found;

enum errorTypes { noerror, illid_digit, illid_long, illid_illch, illid_illegal, overst, wrong_stat, wrong_funcdef, noFuncHeader, noParen, noComma, nobrace, nosemi, nobracket, noifcondition, nowhilecondition};
typedef enum errorTypes ERRORtypes;
ERRORtypes err;

void PrintError(ERRORtypes err);

#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z' || (x) == '_') )
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

#define MAX_LEN		12
