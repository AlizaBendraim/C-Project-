#ifndef MACRO_H
#define MACRO_H
#include "firstAndSecond.h"
#define MAX 80

 typedef struct Macro {
    char macroName[30];
    char macroContent[256];
    struct Macro* next;
}Macro;
Macro *head1;


void create_node_with_name(char* name, char* content);

int if_macro_name_exist(char* name_to_search, char* data);

int IsMacroOrEndMacro(char* line);

Macro* get_last_node(Macro* head_to_list);

int IsMacroCall(char* name, FILE* fkr, FILE* fkw, struct Macro* tail);

char* preReadFile(char* index, FILE* fk, char* fname);

char* InsertNameOfMacro(char* line, int str);

char* InsertContent(char* line, char* name);
#endif



