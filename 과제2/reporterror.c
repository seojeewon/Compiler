/*reporterror.c - Check identifier error
* programmer - 김서영, 김효진, 서지원, 손효원
* date - 5/2/2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tn.h"

#define isWord(x) ((((x>='a'&&(x)<='z') || ((x)>='A'&&(x)<='Z')) || (x=='_')))
#define isNum(x) ((x) >= '0' && (x) <= '9')
extern void Symboltable(char* string);
typedef enum errorTypes ERROR;
ERROR err = noerror;

int errflag = 0;
errcnt = 0;
extern int STindex;  //the current identifier
extern int nextfree;  //the next available index of ST

/* PrintError    - 	ERRORtypes 객체 내용을 바탕으로 ERROR 정보를 출력
			overst : identifier that over 12 characters
			illid: illegal identifier
			overfl: overflow in ST. print the hashtable and abort
 */
void printerror(ERROR err, char* string)
{
	if (err == illid) {
		printf("**Error**\t\t%s is Illegal IDENT\n", string);
		errcnt++;
	}
	else if (err == overst) {
		printf("**Error**\t\t\t%s over 12 words\n", string);
		errcnt++;
	}

	else if (err == overfl) {
		printf("**Error**\t overflow occured\n");
		errcnt++;
	}
}


//reporterror 함수: overst(string 길이가 12자 이상) 에러 감지
void reporterror(char* string) {
	if (strlen(string) > 12) {
		err = overst;
		printerror(err, string);
	}

	else {
		
		Symboltable(string);
		if(err==noerror){
			printf("TIDENT\t");
			printf("%11d\t\t",STindex);
			printf("%s\n", string);
		}
	}
}