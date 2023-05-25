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
	char* var_type;	//variable type
	char isIdent;	//변수인지 함수인지
	int isPara;	//함수의 parameter인지
	int index;
	HTpointer next;
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

int nextid; 
int nextfree;
int hashcode; 
int sameid;
int num_err;//number of errors
int cLine;
//int i, j, cErrors; //?

int found;

enum errorTypes { noerror, illid_digit, illid_long, illid_illch, illid_illegal, overst };
typedef enum errorTypes ERRORtypes;
ERRORtypes err;

#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z' || (x) == '_') )
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

#define MAX_LEN		12
