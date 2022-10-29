#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#define MAX_MEMORY 256

int IC, DC;

struct machineCode{
    int address;
    int word[10];
};
struct machineCode MachineCode[MAX_MEMORY];

struct machineCode tempMC[MAX_MEMORY];

/*linked line of the labels*/
typedef struct Label{
    char name[30];
    int id;
    char type[10];
    struct Label *next;
}Label;
Label *head;



#endif


