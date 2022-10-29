#ifndef FIRSTANDSECOND_H
#define FIRSTANDSECOND_H
#include "global.h"
void convertB10_B2(int , int , char *) ;

void insert(char* , int , char *);

int jumpSpace(char *, int );

int checkOperand(char *);

int digit(char *);

int checkRegister(char*);

int checkLabel(char *);

int ReservedWord(char *);

int checkAction(char *);

int directive(char*, char *);

int search(char *);

char* typeLabel(char *);

enum opBinary {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, get, prn, jsr, rts, hlt};
static const char * const opBinary_names[] = {
        [mov]= "mov", [cmp]="cmp", [add]="add", [sub]="sub", [not]="not", [clr]="clr", [lea]="lea", [inc]="inc",
        [dec]="dec", [jmp]="jmp", [bne]="bne", [get]="get", [prn]="prn", [jsr]="jsr", [rts]="rts", [hlt]="hlt" };

typedef enum{FALSE,TRUE} Boolean;
#endif
