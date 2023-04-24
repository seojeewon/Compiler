/*
* main.c - 각 token에 대한 출력
* programmer-Jiwon Seo(2076200)
*/

#include <stdio.h>
#include "tn.h"
int cErrors = 0;	//number of error
void printtoken(enum tnumber tn) {
	
}

void main() {
	enum tnumber tn;
	printf("Line number\tToken type\tST-index\tToken\n");
	while ((tn = yylex()) != EOF) {
		printtoken(tn);
	}

	if (cErrors == 0)
		printf("No errors defected");
	else
		printf("%d errors detected", cErrors);
}
