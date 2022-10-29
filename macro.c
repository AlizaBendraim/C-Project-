#include "macro.h"

Boolean flagOfMacro=FALSE;

Boolean nameInUse = FALSE;

Boolean flagOfCont = FALSE;
/*This function receives a line from the transferred file and checks whether it is the beginning of a macro definition or the end of a macro definition.  
If not, copies the line as it is to the new file.
                                                                                                    
Returns 1 and updates - if this is the start of a macro definition.
                                                                                     
Returns 2 and updates - if this is the end of a macro definition.*/
int IsMacroOrEndMacro(char* line) {
    if (strcmp(line, "\n") == 0) {/*if empty line - copy to new text*/
        return 0;
    }
    int index = 0;
    char* macron;/*pointer of macro*/
    macron = (char*)calloc(81, sizeof(char));/*Dynamic memory allocation*/
    strcpy(macron, line);/*copy to macro arry the value of line*/
    index += jumpSpace(macron, 0);
    macron = macron + index;
    if (strchr(macron, 'm') != NULL) {
        if (!isspace(macron[index]) && macron[index] != '\n') {
            strtok(macron, " ");
        }
    }
    if (strchr(macron, '\n') != NULL) {
        strtok(macron, "\n");
    }
    char* macronTemp;
    if (strchr(macron, '\t') != NULL) {
        macronTemp = macron;
        strtok(macronTemp, "\t");
        macron = macronTemp;
    }
    if (strcmp(macron, "macro") == 0) {
        return 1;
    }
    if (strcmp(macron, "endmacro") == 0) {
        flagOfMacro = TRUE;
        return 2;
    }
    if ((strchr(macron, '.') != NULL || strchr(macron, ':') != NULL || strchr(macron, ';') != NULL) &&
        flagOfMacro == FALSE) {
        return 0;
    }
    if (flagOfMacro == FALSE && if_macro_name_exist(macron, "\0") == -1) {
        nameInUse = TRUE;
        return 3;
    }
    return 0;
}

/* This function gets the head of the linked list and returns us the last non-NULL node.    
And it is for us to know where a new node can be inserted.*/
Macro* get_last_node(Macro* head_to_list) {
    Macro* current_node = head_to_list;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }
    return current_node;
}


/*This function receives the name to be searched in the linked list and the macro content of that name.                                   
The function searches in the linked list if the passed name already exists in the linked list.                                          
If it does exist, it checks if its content is updated to 0. If so, it redefines its contents according to the functions and returns -1. 
And if the content is not updated to 0, it updates it to zero and returns -1.                                                           
If the passed name does not exist in the list, it returns 0 and the intsertNameOfMacro function will insert it into the linked list.*/

/*check if name exist.*/
int if_macro_name_exist(char* name_to_search, char* data) {

    char* cont = (char*)malloc((81 * sizeof(char)));
    cont[80] = '\0';
    Macro* test = head1;
    while (test != NULL) {
        if (strcmp(test->macroName, name_to_search) == 0) {
            if (flagOfCont == FALSE) {
                strcpy(test->macroContent, data);
                flagOfCont = TRUE;
                return -1;
            }
            else {
                strcpy(cont, data);
                strcat(test->macroContent, cont);
            }
            return -1;
        }
        test = test->next;
    }
    return 0;
}

/*This function receives a name and macro content.                                                                                
This function creates a new node in the linked list when the transferred name is checked and is not found in the linked list.   
The function initializes the macro name to the passed name and the macro content to 0.                                          
The function will update the linked list properly.*/
void create_node_with_name(char* name, char* content) {

    if (if_macro_name_exist(name, content) == 0) {
        Macro* new_node = (Macro*)malloc(81 * sizeof(*new_node));
        if (new_node == NULL) {
            return;
        }
        memset(new_node->macroName, '\0', MAX);
        strcpy(new_node->macroName, name);
        memset(new_node->macroContent, '\0', MAX);
        new_node->next = NULL;
        if (head1 == NULL) {
            head1 = new_node;
        }
        else {
            get_last_node(head1)->next = new_node;
        }
    }
    else
        return;
}

/* This function receives a macro name, a file to be read, the new file to be written and a node in the linked list.           
   This function goes through the linked list and checks whether the name of the macro passed to it exists in the linked list. 
   If the macro name does exist - the contents of the macro are copied to the new file instead of the transferred macro name.  
   If it does not exist, it returns zero.*/
int IsMacroCall(char* name, FILE* fkr, FILE* fkw, struct Macro* tail) {
    struct  Macro* temp = NULL;
    temp = (struct Macro*)malloc(81 * sizeof(struct Macro));
    tail = head1;
    temp = tail;
    while (temp != NULL)
    {
        if (strcmp(temp->macroName, name) == 0)
        {
            fprintf(fkw, "%s", temp->macroContent);
        }
        temp = temp->next;
    }
    return 0;
}


/*
  This function receives a file passed to it from main, and creates a new file- This file will be the new file after the macros are retired.
  The function checks whether the file passed to it exists, if not it returns 0.
  The function reads line by line from the transferred file as long as it is not equal to NULL.
  And every time you check whether:
  This is an empty line - copy it as it is to the new file.
  Is this the start or end of a macro definition - sends to the appropriate functions to update the linked list.
  Is this a macro call? If so, copy the contents of the macro to the new file using the appropriate functions.
  If not - copies the line as it is to the new file.
 */
char* preReadFile(char* index, FILE* fk, char* fname) {
    int num;
    char* name;
    char* temp = calloc(31, sizeof(char));
    FILE* fkw;
    FILE* fkr;
    char ch[256];
    fkr = fk;
    char* filename = calloc(sizeof(fname) + 20, sizeof(char));
    sprintf(filename, "%s.am", fname);

    fkw = fopen(filename, "w");
    if (fkr == NULL)
    {
        printf("ERROR: can't open this file!");
        return 0;
    }
    char* f = fgets(ch, sizeof(ch), fkr);
     if (strchr(f, '\r'))
    {
        char* i = strchr(f, '\r');
        int indx = (int)(i - f);
        f[indx]= '\0';
    }
    /*if empty line - copy to new text*/
    if (strcmp(f, "\n") == 0 || strcmp(f, "\0") == 0) {
        fprintf(fkw, "%s", "\n");
        f = fgets(ch, sizeof(ch), fkr);
    }
    while (f != NULL) {
	if (strchr(f, '\r'))
    	{
		char* i = strchr(f, '\r');
		int indx = (int)(i - f);
		f[indx]= '\0';
    	}
        /*if empty line - copy to new text*/
        if (strcmp(f, "\n") == 0 || strcmp(f, "\0") == 0) {
            fprintf(fkw, "%s", "\n");
            f = fgets(ch, sizeof(ch), fkr);
        }
        num = IsMacroOrEndMacro(f);
        if (num == 1) {
            name = InsertNameOfMacro(f, num);
            if (name == 0) {
                fclose(fkw);
                fclose(fkr);
                return 0;
            }
            if (nameInUse == TRUE) {
                temp = strcpy(temp, name);
            }
            f = fgets(ch, sizeof(ch), fkr);
            while (IsMacroOrEndMacro(f) != 2 && flagOfMacro == FALSE) {
                InsertContent(f, temp);
                if (nameInUse == TRUE) {
                    temp = strcpy(temp, name);
                }
                f = fgets(ch, sizeof(ch), fkr);
                if (strchr(f, '\r'))
                {
                    char* k = strchr(f, '\r');
                    int j = (int)(k - f);
                    f[j] = '\0';
                }
            }
            if (flagOfMacro == TRUE) {
                f = fgets(ch, sizeof(ch), fkr);
            }
            flagOfMacro = FALSE;
        }
        if ((IsMacroOrEndMacro(f)) == 3) {
            IsMacroCall(temp, fkr, fkw, head1);
            f = fgets(ch, sizeof(ch), fkr);
        }
        else {
            fprintf(fkw, "%s", f);
            f = fgets(ch, sizeof(ch), fkr);
        }
    }
    fclose(fkw);
    fclose(fkr);
    free(temp);
    return filename;
}

/*This function receives a line from the transferred file and an int type value transferred from the function IsMacroOrEndMacro
 The function checks whether the name is written legally:
 Was the macro name really defined in the macro definition line?
 Is the name of the macro not one of the reserved names of a prompt/instruction?
 Has the name not already been defined before?
 If the name is indeed correct - the function will return it to the if_macro_name_exist function
 that it will pass it to the create_node_with_name function which will create a new node with it in the linked list.
 */
char* InsertNameOfMacro(char* line, int str) {
    if ((str) == 1) {
        int index = 0;
        char* name = (char*)malloc((31 * sizeof(char)));
        name[30] = '\0';
        name = line;
        index += jumpSpace(name, 0);
        /*move after "macro"*/
        name = strchr(name, ' ');
        index += jumpSpace(name, 0);
        name = name + index;
        if (strcmp(name, "\0") == 0 || strcmp(name, "\n") == 0) {
            printf("ERROR: The macro name is not defined");
            return 0;
        }
        if (strchr(name, '\n') != NULL)
        {
            if (!isspace(name[0]) && name[0] != '\n') {
                strtok(name, "\n");
            }
        }
        name = strtok(name, " ");
        if (ReservedWord(name)==0) {
            printf("ERROR: macro name - \"%s\" cannot be a Reserved Word.", name);
            return 0;
        }
        create_node_with_name(name, NULL);
        nameInUse = TRUE;
        return name;
    }
    return 0;
}


/*
  This function receives a line from the transferred file and a macro name.
  The function concatenates into the contents all the contents of the macro that belonged to the same definition of the passed macro name.
  The function sends the content and the name to the appropriate function.
  and returns the contents.
 */
char* InsertContent(char* line, char* name) {

    char* content = (char*)calloc(81, sizeof(char));
    memset(content, '\0', MAX);

    /*Chains more content*/
    strncat(content, line, MAX);
    create_node_with_name(name, content);
    nameInUse = FALSE;
    free(content);
    return 0;
}


