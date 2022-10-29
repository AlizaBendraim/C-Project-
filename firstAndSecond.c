
#include "firstAndSecond.h"
/*This file holds the functions used for the first scan and the second scan.*/

/*This function accepts the a_o_d variable and the index variable and will check whether a_o_d belongs to directive type sentences.
In directive-type sentences, a sentence label can appear first and must be followed by a '.' After that it will be written in lowercase letters without spaces: '.data'\'.string'\'.struct'\'.entry'\'.extern'*/
int directive(char* a_o_d, char *index){
    char* token = NULL;
    if (index != NULL) token = calloc(strlen(index) + 1, sizeof(char));
    char* i = NULL;



/*'.data': We will check that the data parameters are one or more valid integers separated by a comma character. 
   If it is indeed true - the function will convert to binary and enter the machine code according to the given requirements.
   If not, error messages will be printed as required.
   The .data sentence instructs the assembler to allocate space to the data image in which the values of the parameters will be stored and to advance the data counter accordingly.*/
    if(strcmp(a_o_d, ".data")==0) {
        while(strcmp(index,"\0")!=0){
            tempMC[DC].address=DC;
            index += jumpSpace(index, 0);
            strcpy(token, index);
            if(strchr(index, ',')){
                token = strtok(token, ",");
                if(strchr(token,' ')|| strchr(token, '\t')) {
		    char* space = strchr(token + 1, ' ');
		    char* tab = strchr(token + 1, '\t');
		    char* cut = NULL;
		    if (space == NULL && tab != NULL)
		    {
			cut = "\t";
			i = tab;
		    }
		    else if (tab == NULL && space != NULL)
		    {
			cut = " ";
			i = space;
		    }
			else if (space != NULL && tab != NULL)
		    {
			if ((int)(tab - cut) < (int)(space - cut))
			{
			    i = tab;
			    cut = "\t";
			}
			else
			{
			    i = space;
			    cut = " ";
			}
		    }
		    if (jumpSpace(i, 0) == strlen(i)) {
		    	token = strtok(token, cut);
                    }else{
                        printf("\nERROR: invalid directive %s, parameter incorrect",a_o_d);
                        free(token);
                        return -1;
                    }
                }
                if(digit(token)==0){
                    index=strchr(index, ',')+1;
                } else {
                    printf( " in directive %s\n",a_o_d);
                    free(token);
                    return -1;
                }
            }
            else{
                if(strchr(index,'\n')){
                    token = strtok(token, "\n");
                } else token = strtok(token, "\0");
                if(strchr(token,' ')|| strchr(token, '\t')) {
		char* space = strchr(token + 1, ' ');
		    char* tab = strchr(token + 1, '\t');
		    char* cut = NULL;
		    if (space == NULL && tab != NULL)
		    {
			cut = "\t";
			i = tab;
		    }
		    else if (tab == NULL && space != NULL)
		    {
			cut = " ";
			i = space;
		    }
			else if (space != NULL && tab != NULL)
		    {
			if ((int)(tab - cut) < (int)(space - cut))
			{
			    i = tab;
			    cut = "\t";
			}
			else
			{
			    i = space;
			    cut = " ";
			}
		    }
		    if (jumpSpace(i, 0) == strlen(i)) {
		    	token = strtok(token, cut);
                    }
                    else{
                        printf("\nERROR: invalid directive %s, parameter incorrect",a_o_d);
                        free(token);
                        return -1;
                    }
                }
                if(digit(token)==0){
                    index=strchr(index, '\0');
                } else {
                    printf( " in directive %s\n",a_o_d);
                    free(token);
                    return -1;
                }
            }
            /*convert token*/
            if(strncmp(token,"+",1)==0)
            {
                convertB10_B2(atoi(token+1),0,"directive");
                DC++;
            }
            else {
                convertB10_B2(atoi(token),0,"directive");
                DC++;
            }

        }
        free(token);
        return 0;
    }



/*We will check that the string parameters are indeed a string.
  The function will transfer all the received values as a string and check whether the string is correct according to the given requirements. 
  If correct - the function will convert it to binary and update the machine code in the appropriate place.
  If not, error messages will be printed as required.
  The .string statement instructs the assembler to encode the string characters into ascii characters and the assembler's data counter will advance according to the  length of the string.*/
    if(strcmp(a_o_d, ".string")==0) {
        while(strcmp(index,"\0")!=0) {
            if (strchr(index, '"') != NULL) {
                index++;
                if (strchr(index, '"') != NULL) {
                    i=index;
                    while(strchr(i, '"') != NULL){
                        i=strchr(i, '"')+1;
                    }
                } else {
                    printf("\nERROR: Invalid directive %s, has to have \" to clos the string",a_o_d);
                    free(token);
                    return -1;
                }
                if (index == i){
                    printf("\nERROR: Invalid directive %s, the string has to contain at least one character",a_o_d);
                    free(token);
                    return -1;
                } else i--;
                while (index != i) {
                    tempMC[DC].address = DC;
                    convertB10_B2(*index, 0, "directive");
                    DC++;
                    index++;
                }
                index++;
                memset(tempMC[DC].word, '\0', 10);
                tempMC[DC].address = DC;
                DC++;
                if(strchr(a_o_d,'\n')!=NULL){
                    if (jumpSpace(index, 0) == strlen(index)-1) {
                        index += jumpSpace(index, 0);
                    } else {
                        printf("\nERROR: invalid directive %s, parameter incorrect",a_o_d);
                        free(token);
                        return -1;
                    }
                }
                if(strchr(a_o_d,'\0')!=NULL){
                    if (jumpSpace(index, 0) == strlen(index)) {
                        index += jumpSpace(index, 0);
                    } else {
                        printf("\nERROR: invalid directive %s, parameter incorrect",a_o_d);
                        free(token);
                        return -1;
                    }
                }

            }else {
                printf("\nERROR: Invalid directive %s, field needs to be string", a_o_d);
                free(token);
                return -1;
            }
        }
        free(token);
        return 0;
    }


/*We will check whether a structure type record consisting of 2 fields has been assigned: the first is a number and the second is a string.
The function will check whether the values in the 2 fields are correct according to the given requirements. 
If so, the function will convert to binary and update the machine code as required and advance the data counter as required.
If not, error messages will be printed as required.*/

    if(strcmp(a_o_d, ".struct")==0) {
        tempMC[DC].address = DC;
        while(strcmp(index,"\0")!=0) {
            if(strchr(index, ',')){
                strcpy(token, index);
                token = strtok(token, ",");
                if (strchr(token, ' ')|| strchr(token, '\t')) {
		char* space = strchr(token + 1, ' ');
		    char* tab = strchr(token + 1, '\t');
		    char* cut = NULL;
		    if (space == NULL && tab != NULL)
		    {
			cut = "\t";
			i = tab;
		    }
		    else if (tab == NULL && space != NULL)
		    {
			cut = " ";
			i = space;
		    }
			else if (space != NULL && tab != NULL)
		    {
			if ((int)(tab - cut) < (int)(space - cut))
			{
			    i = tab;
			    cut = "\t";
			}
			else
			{
			    i = space;
			    cut = " ";
			}
		    }
		    if (jumpSpace(i, 0) == strlen(i)) {
		    	token = strtok(token, cut);
                    } else {
                        printf("\nERROR: invalid directive %s, first filed incorrect",a_o_d);
                        free(token);
                        return -1;
                    }
                }
                if (digit(token) == 0) {
                    index = strchr(index, ',') + 1;
                    index += jumpSpace(index, 0);
                } else {
                    printf( " in first filed directive %s\n",a_o_d);
                    free(token);
                    return -1;
                }
                /*convert number*/
                if(strncmp(token,"+",1)==0)
                {
                    convertB10_B2(atoi(token+1),0,"directive");
                    DC++;
                }
                else {
                    convertB10_B2(atoi(token), 0, "directive");
                    DC++;
                }
                while(strcmp(index,"\0")!=0){
                    if (strchr(index, '"') != NULL) {
                        index++;
                        if (strchr(index, '"') != NULL) {
                            i=index;
                            while(strchr(i, '"') != NULL){
                                i=strchr(i, '"')+1;
                            }
                        } else {
                            printf("\nERROR: Invalid directive %s, has to have \" to clos the string",a_o_d);
                            free(token);
                            return -1;
                        }
                        if (index == i){
                            printf("\nERROR: Invalid directive %s, the string has to contain at least one character",a_o_d);
                            free(token);
                            return -1;
                        } else i--;
                        while (index!=i) {
                            tempMC[DC].address = DC;
                            convertB10_B2(*index, 0, "directive");
                            DC++;
                            index++;
                        }
                        index++;
                        memset(tempMC[DC].word,'\0',10);
                        tempMC[DC].address = DC;
                        DC++;
                        if (jumpSpace(index, 0) == strlen(index)) {
                            index += jumpSpace(index, 0);
                        } else {
                            printf("\nERROR: Invalid directive %s, can only get 2 fields", a_o_d);
                            free(token);
                            return -1;
                        }
                    }else {
                        printf("\nERROR: Invalid directive %s, second field needs to be string", a_o_d);
                        free(token);
                        return -1;
                    }

                }

            }
            else{
                printf("\nERROR: Invalid directive, need 2 fields");
                free(token);
                return -1;
            }

        }
        free(token);
        return 0;
    }
    else{
        printf("\nERROR: Invalid directive, incorrect name");
        free(token);
        return -1;
    }
}

/*This function converts any number from a decimal base to a binary base.
The function will first check whether the number we want to transfer is positive or negative.
If the number is negative - multiply by 1 in order to get its absolute value, and only then the function will convert it to binary and if necessary - pad with 0.
Then, since the number is negative, we will turn every 0 into a 1 and every 1 into a 0 starting from the next place after the number 1 and on.
If the number is positive - the function will convert it to binary, and if necessary - pad with 0.*/
void convertB10_B2(int num, int count, char *type) {
    int sum = 0;
    int i = 9;
    int sub[10];
    int negOrPos = 0;
    if(num < 0) {
        num = num * (-1);
        negOrPos = 1;
    }
    while (num != 0) {
        sum = num % 2;
        sub[i] = sum;
        i--;
        num = num / 2;
    }
    /*pad the rest with zero*/
    if (i != -1) {
        while (i != -1) {
            sub[i] = 0;
            i--;
        }
    }
    if(negOrPos == 1){
        int in = 9;
        int flagOfFirst =0;
        while(in != -1) {
            if (sub[in] != 1 && flagOfFirst == 0) {
                in--;
            } else {
                if (flagOfFirst == 0) {
                    in--;
                    flagOfFirst = 1;
                }
                if (sub[in] == 1) {
                    sub[in] = 0;
                    in--;
                }
                if (sub[in] == 0) {
                    sub[in] = 1;
                    in--;
                }
            }
        }
    }
    /*4 bite for opcode*/
    if(strcmp(type, "op")==0){
        /*initialize MachineCode[IC]/word with zero value*/
        memset(MachineCode[IC].word, '\0', 10);
        for(i=6; i<10; i++) {
            MachineCode[IC].word[i-6]=sub[i];
        }
        return;
    }
    /*4 bite for source register*/
    if(strcmp(type, "regS")==0){
        /*initialize MachineCode[IC].word with zero value*/
        memset(MachineCode[IC+count].word, '\0', 10);
        for(i=6; i<10; i++) {
            MachineCode[IC+count].word[i-6]=sub[i];
        }
        return;
    }
    if(strcmp(type, "regDR")==0){
        /*initialize MachineCode[IC]->word with zero value*/
        for(i=6; i<10; i++) {
            MachineCode[IC+count].word[i-2]=sub[i];
        }
        return;
    }
    /*4 bite for destination register*/
    if(strcmp(type, "regD")==0){
        /*initialize MachineCode[IC]->word with zero value*/
        memset(MachineCode[IC+count].word, '\0', 10);
        for(i=6; i<10; i++) {
            MachineCode[IC+count].word[i-2]=sub[i];
        }
        return;
    }
    /*8 bite for operand number*/
    if(strcmp(type, "operand")==0){
        /*initialize MachineCode[IC].word with zero value*/
        memset(MachineCode[IC+count].word, '\0', 10);
        for(i=2; i<10; i++) {
            MachineCode[IC+count].word[i-2]=sub[i];
        }
        return;
    }
    /*10 bite for directive number*/
    if(strcmp(type, "directive")==0){
        for(i=0; i<10; i++) {
            tempMC[DC].word[i]=sub[i];
        }
        return;
    }
}

/*This function is responsible for inserting a label into the linked list of labels.
  A label will enter the linked list only after checking whether it is a valid label according to the given guidelines.
  This function inserts the label into the linked list in the free space and updates the linked list as required.*/
void insert(char* n, int id, char *t)
{
    Label *p=head;
    Label *label = (Label *) malloc(sizeof(Label));
    strcpy(label->name, n);
    label->id = id;
    strcpy(label->type, t);
    label->next = NULL;
    if(head==NULL){
        /* if head is NULL-set label as the new head*/
        head = label;
    }
    else{
        while(p->next!=NULL){


            p = p->next;
        }
        /* if list is not empty -insert student in beginning of head*/
        p->next = label;
    }
}

/*This function accepts a line passed from text and an int value initialized to zero.
This function is responsible for "jumping" on all profits as long as it has not reached a value that is not a white profit, in an int value it saves and updates the amount of profits that has progressed.
The function returns the number of spaces it "jumped".*/
int jumpSpace(char* command, int from)
{
    char c;
    while ((c = command[from]) != '\0' && (isspace(c) || command[from] == '\t'))
        from++;
    return from;
}


/*This function accepts an operand of type char.
The function will check whether the operand is correct: it checks what type of operand was passed to it and accordingly sends the operand to the appropriate function that checks the same type of operand.
If the operand is indeed correct - the function will return the number of the addressing method according to the data.
If not - the function will return -1.*/
int checkOperand(char * op){
    char *i=NULL;
    if(op != NULL && strchr(op, '#')){
        op++;
        if(digit(op)==0){
            return 0;
        }
        else{
            printf(" in operand method 0\n");
            return -1;
        }

    }
    if(strchr(op, 'r')){
        if(checkRegister(op) == 0){
            return 3;
        }
        return -1;
    }
    if(strchr(op, '.')){
        char* str = NULL;
        if (op != NULL)
            str = calloc(strlen(op) + 1, sizeof(char));
        else
            return -1;
        i=strchr(op, '.');
        strcpy(str,op);
        char* token= strtok(str,i);
        if(checkLabel(token)==0){
            i++;
            if (strcmp(i, "1") == 0 || strcmp(i, "2") == 0)
            {
                free(str);
                return 2;
            }
                
            else{
                printf("\nERROR: Invalid operand method #2");
                free(str);
                return -1;
            }

        }
        free(str);
        return -1;

    }
    if(checkLabel(op)==0){
        return 1;
    }
    return -1;
}

/*This function accepts a character of type char.
This function checks whether the number transferred to it is correct according to the given instructions.
If not - the function will return -1 and print error messages accordingly.*/
int digit(char *digit) {
    int i = 0;
    if (isdigit((int) (digit[i])) != 0) {
        i++;
        while (strcmp(digit + i, "\0") != 0) {
            if (isdigit((int) (digit[i])) != 0) {
                i++;
            } else {
                printf("\nERROR: Invalid number, \'%s\' is not a correct number", digit);
                return -1;
            }
        }
        return 0;
    }
    if ((int) (digit[i]) == '-' || (int) (digit[i]) == '+') {
        i++;
        while (strcmp(digit + i, "\0") != 0) {
            if (isdigit((int) (digit[i])) != 0) {
                i++;
            } else {
                printf("\nERROR: Invalid number, no digit after - or +");
                return -1;
            }
        }
        return 0;
    } else {
        printf("\nERROR: Invalid number, only can have digits");
        return -1;
    }
}

/*This function accepts a register passed as char.
This function checks whether the register transferred to it is correct according to the given instructions.
If not - the function will return -1 and print error messages accordingly.
If so, the function will return 0.*/
int checkRegister(char* r) {
        int i=0;
        if (strlen(r) > 2 || strlen(r) < 2) {
            printf("\nERROR: Invalid Register mor than tow characters");
            return -1;
        }
        while(i<strlen(r)){
            if(strchr(r+i, 'r')!=NULL){
                i++;
                if((isdigit(((int)(r[i])))!=0)  && ((atoi(r+i))>=0) && ((atoi(r+i))<=7) )
                    i++;
                else{
                    printf("\nERROR: Invalid Register, register name is only r0-r7");
                    return -1;
                }
            }
            else{
                printf("\nERROR: Invalid Register");
                return -1;
            }
        }
        return 0;
    }


/* checks if the label is correct.
This function accepts a label passed as char.
This function checks whether the label transferred to it is correct according to the given instructions.
If not - the function will return -1 and print error messages accordingly.
If so, the function will return 0.*/
int checkLabel(char *token){
        if(strlen(token) > 30){
            printf("\nERROR: Invalid label, length mor than 30");
            return -1;
        }
        if(ReservedWord(token)==0){
            printf("\nERROR: Invalid name of label, label name can't be a Reserved word");
            return -1;
        }
        int i = 0;
        if(isalpha((int)(token[i]))==0) {
            printf("\nERROR: Invalid name of label, need to start with alphabetical letter");
            return -1;
        }
        while(i<strlen(token)){
            if((int)(token[i])== '-' || (int)(token[i])== '+'){
                i++;
                if((isdigit((int)(token[i])))==0){
                    printf("\nERROR: Invalid name of label, no digit after - or +");
                    return -1;
                }
                else i++;
            }
            else {
                
                if (token[i] == '\r' || token[i] == '\n' || isalnum(token[i])) {
                    i++;
                }
                else{
                    printf("\nERROR: Invalid name of label -%s-, has to contain digits or alphabetical letters",token);
                    return -1;
                }
            }
        }

        return 0;
    }

/*This function holds all the reserved words.
And with its help we will check whether a label name or a register name or an operand name is a reserved word. If so - the function will return -1 and print error messages accordingly.
If not - the names are correct.*/
int ReservedWord(char * token){
        /*Checks if this is a directive type word*/
        if(strcmp(token, ".data")==0 || strcmp(token, ".string")==0 || strcmp(token, ".struct")==0 || strcmp(token, ".entry")==0 || strcmp(token, ".extern")==0
           ||strcmp(token, "data")==0 || strcmp(token, "string")==0 || strcmp(token, "struct")==0 || strcmp(token, "entry")==0 || strcmp(token, "extern")==0)
            return 0;
        /*Checks if it is a register type word*/
        int i=0;
        if(strchr(token+i, 'r')!=NULL) {
            if (strlen(token) > 2 || strlen(token) < 2) {
                return -1;
            }
            while (i < strlen(token)) {
                if (strchr(token + i, 'r') != NULL) {
                    i++;
                    if ((isdigit(atoi(token + i)) != 0) && (isdigit(atoi(token + i)) != 0) && atoi(token + i) >= 0 &&
                        atoi(token + i) <= 7)
                        i++;
                    else {
                        return -1;
                    }
                } else {
                    return -1;
                }
            }
            return 0;
        }
        /*Checks if it's an action word*/
        else{
            int a;
            for(a=0; a<16; a++) {
                if (strcmp(opBinary_names[a], token)  == 0){

                    return 0;
                }
            }
            if(a==16){
                return -1;
            }
        }
        return -1;
    }

/*This function accepts an action word passed as a char.
The function checks which operation word it is and how many operands it should receive and returns the number of operands that operation word receives.
If the action word is not one of the given action words - the function will return -1 and print an error message as required.*/
int checkAction(char * line_cp){
    char* ac = NULL;
    if(line_cp != NULL) ac = calloc(strlen(line_cp) + 1, sizeof(char));
    strcpy(ac, line_cp);
        int a;
        for(a=0; a<16; a++) {
            if (strcmp(opBinary_names[a], ac)  == 0){
                if(a<4 || a==6){
                    free(ac);
                    return 2;
                }
                if((a>3 && a<6)|| (a>6 && a<14)){
                    free(ac);
                    return 1;
                }
                free(ac);
                return 0;
            }
        }
        if(a==16){
            printf("\nERROR: Invalid action, incorrect action name %s",ac);
            free(ac);
            return -1;
        }
	return -1;
    }

/*This function accepts a label name passed to it as a char.
The function will search if the label name already appears in the linked list.
If not - enter the name of the label in the linked list in the space available.
If so - the function will return -1.*/
int search(char *n)
{
    Label* temp = head;
    while(temp!=NULL){
        if(strcmp(temp->name, n)!=0){
            temp = temp->next;
        }
        else{
            return -1;
        }
    }
    return 0;
}

/*This function gets a name of a label and searches for the name in the linked list of the labels.
if finds the name in the linked list returns the type of the label and 0 if not */
char* typeLabel(char *name){
    Label * temp = head;
    while(temp!=NULL){
	if(strcmp(temp->name, name)==0 && temp->id==-1 ){
    		return temp->type;
	}
        else temp = temp->next;
    }
    return 0;
}


