#ifndef SECONDSCAN_H
#define SECONDSCAN_H
#include "firstAndSecond.h"
int labelAction(char *,int , char *,char *);
int getTwoOp(char *,char *,char *,char *);
int getOneOp(char *, char *,char *);
void convertB10_B32( int ,FILE*);
void convertB2_B32( int,FILE*);
int idLabel(char *);
void printExternal(char *,int,char *);
int line(char *,char *);
#endif
