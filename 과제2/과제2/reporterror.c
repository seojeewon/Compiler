/*reporterror.c - Check identifier error
* 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tn.h"
#include "global.h"
#define isWord(x) ((((x>='a'&&(x)<='z') || ((x)>='A'&&(x)<='Z')) || (x=='_')))
#define isNum(x) ((x) >= '0' && (x) <= '9')
extern void Symboltable(char* str);
typedef enum errorTypes ERROR;
ERROR errr = noerror;

int errflag = 0;
errcnt = 0;

//PrintError �Լ�: ERRORtypes ��ü ������ �������� ERROR ������ ���
// error: illsp(������ �ʴ� ���� ����� �ĺ��� ����)
//        illid(���ڿ� �Բ� �����ϴ� �ĺ��� ����)
//        overst(�ĺ��ڱ���idlen�� �Ѵ� �ĺ��� ����)
//        overfl(ũ��� overflow �߻�)
void printerror(ERROR err, char* string)
{
	if (err == illid) {
		printf("**Error**\t\t%s Illegal IDENT", string);
		errcnt++;
	}
	else if (err == overst) {
		printf("**Error**\t\t\t%s over 12 words", string);
		errcnt++;
	}

	else if (err == overfl) {
		printf("**Error**\t overflow occured\n");
		errcnt++;
	}
}


//reporterror �Լ�: overst(string ���̰� 12�� �̻�) ���� ����
void reporterror(char* string) {
	if (strlen(string) > 12) {
		errr = overst;
		printerror(errr, string);
	}

	else {
		printf("Identifier\t");
		symtable(string);
	}
}