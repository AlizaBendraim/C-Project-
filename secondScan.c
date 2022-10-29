
#include "secondScan.h"


/*This function gets a number in base 10 and converts to the unique base 32.
Saves the conversion in the p file of the output in the place of the IC*/
void convertB10_B32( int num,FILE* p){
    char base_32 [32]= {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
    int sum=0,i=0;
    int sub[2];
    while(num!=0){
        sum=num%32;
        sub[i]=sum;
        i++;
        num/=32;
    }
    i--;
    for(i; i>-1;i--){
        fprintf(p,"%c",base_32[sub[i]]);
    }
}

/*This function gets the IC number and converts the 10 digit word in base 2 in the machine code memory to the unique base 32.
Saves the conversion in the p file of the output in the place of the IC*/
void convertB2_B32( int num ,FILE* p){
    char base_32 [32]= {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
    int sum=0,i=0,j=16;
    while(i<5){
        if(MachineCode[num].word[i]==1){
            sum+=j;
            i++;
            j/=2;
        } else{
            j/=2;
            i++;
        }
    }
    fprintf(p,"%c",base_32[sum]);
    i=5;
    j=16;
    sum=0;
    while(i<10){
        if(MachineCode[num].word[i]==1){
            sum+=j;
            i++;
            j/=2;
        } else{
            j/=2;
            i++;
        }
    }
    fprintf(p,"%c",base_32[sum]);
}

/*This function gets a name of a label and searches for the name in the linked list of the labels.
if finds the name in the linked list returns the address of the label and 0 if not */
int idLabel(char *name){
    struct Label * temp = head;
    while(temp!=NULL){
        if(strcmp(temp->name, name)==0 && temp->id!=-1){
            return temp->id;
        }
        temp = temp->next;
    }
    return 0;
}

/*This function gets a name of an external label ,the address of the label and the name of the input file. saves the name in the output of the file of the external type labels and converts the address from base 10 to base 32 and saves the convertion in the output of the file of the external labels*/
void printExternal(char *name,int i,char *fname){
    FILE *ps;
    char* filename = calloc(sizeof(fname) + 20, sizeof(char));
    sprintf(filename, "%s.ext", fname);
    ps= fopen(filename,"a");
    fprintf(ps,"\n%s\t",name);
    convertB10_B32(i,ps);

}

/*this function gets a pointer to line to reed and a pointer to the name of the input file.
if there is a label checks if the type of the label is "entry". if it's saves the name in the output of the file of the entry type labels and converts the address from base 10 to base 32 and saves the convertion in the output of the file of the external labels. saves the name of the action,If it's a declaration of an entry label or an external label advances to the next line and advances IC to +1. if it's a directive action so advances the variable IC to plus the number of words of information that the instruction word line stores in memory. and if this is one of the action names then it will update the addresses of the actions according to the addresses that are updated in a linked list in the machine code memory.
returns -1 if there is an error in the line and 0 if not.*/
int line(char *line,char* fname) {
    char *index= calloc(81, sizeof(char));
    strcpy(index,line);
    index += jumpSpace(index, 0);
     if (strchr(index, '\r'))
    {
        char* i = strchr(index, '\r');
        int indx = (int)(i - index);
        index[indx]= '\0';
    }
    /*if empty line just \n or a note sentence*/
    if(strcmp(index,"\n")==0 || strncmp(index,";",1)==0||strcmp(index,"\0")==0){
        return 0;
    }
    int numOfOp=0;
    char *a_o_d= calloc(81, sizeof(char));
    /*name and type of label*/
    char *name= calloc(81, sizeof(char));
    char *type= calloc(81, sizeof(char));
    /*if label*/
    if (strchr(index, ':') != NULL) {
        strcpy(name, index);
        name = strtok(name, ":");
        char* typeL=typeLabel(name);
        if(typeL != 0 && strcmp(typeL,"entry")==0){
            FILE *ps;
	    char* filename = calloc(sizeof(fname) + 20, sizeof(char));
            sprintf(filename, "%s.ent", fname);
            ps= fopen(filename,"a");
            fprintf(ps,"\n%s\t",name);
            convertB10_B32(IC,ps);
        }

        index = strchr(index, ':')+1;
        index += jumpSpace(index, 0);
        strcpy(a_o_d, index);
            char* space = strchr(index + 1, ' ');
            char* tab = strchr(index + 1, '\t');
            char* cut = NULL;
            if (space == NULL && tab != NULL)
            {
                cut = "\t";
                index = tab;
            }
            else if (tab == NULL && space != NULL)
            {
                cut = " ";
                index = space;
            }
            else if (space != NULL && tab != NULL)
            {
                if ((int)(tab - cut) < (int)(space - cut))
                {
                    index = tab;
                    cut = "\t";
                }
                else
                {
                    index = space;
                    cut = " ";
                }
            }
		else{
			index = NULL;
		    }
		if (cut != NULL )
			a_o_d = strtok(a_o_d, cut);
    }

    else{
        strcpy(a_o_d, index);
            char* space = strchr(index + 1, ' ');
            char* tab = strchr(index + 1, '\t');
            char* cut = NULL;
            if (space == NULL && tab != NULL)
            {
                cut = "\t";
                index = tab;
            }
            else if (tab == NULL && space != NULL)
            {
                cut = " ";
                index = space;
            }
            else if (space != NULL && tab != NULL)
            {
                if ((int)(tab - cut) < (int)(space - cut))
                {
                    index = tab;
                    cut = "\t";
                }
                else
                {
                    index = space;
                    cut = " ";
                }
            }
		else{
			index = NULL;
		    }
		if (cut != NULL )
			a_o_d = strtok(a_o_d, cut);
    }
    
    
    if(index==NULL){
        if(strchr(a_o_d,'\n')!=NULL) {
            a_o_d = strtok(a_o_d, "\n");
        }
    }
    else{
        index += jumpSpace(index, 0);
    }
    /*check if action or directive*/
    if(strchr(a_o_d,'.')!=NULL){
        if(strcmp(a_o_d,".extern")==0||strcmp(a_o_d,".entry")==0){
            free(a_o_d);
            free(name);
            free(type);
            return 0;
        } else{
            int d= directive(a_o_d,index);
            if(d!=-1){
                IC+=d+1;
                free(a_o_d);
                free(name);
                free(type);
                return 0;
            }
        }

    }

    numOfOp=checkAction(a_o_d);
    if(numOfOp!=-1){
        labelAction(a_o_d,numOfOp, index,fname);
        free(a_o_d);
        free(name);
        free(type);
        return 0;

    } else{
        IC++;
        free(a_o_d);
        free(name);
        free(type);
        return 0;
    }
    free(a_o_d);
    free(name);
    free(type);
    return 0;
}

/*This action gets a pointer to the name of the action and the number of operands he needs to get, pointer to the first charecter after the name of action and a pointer to the name of the input file.
The function saves the source and destination operand from the index pointer and passes them to the function that converts to binary according to the number of operands.
returns 0 if there is an error in one of the operands and 0 if not*/
int labelAction(char *a_o_d,int numOfOp, char *index,char *fname){
    char* sourceOp = NULL;
    char* destinationOp = NULL;
    if (index != NULL)
    {
        sourceOp = calloc(strlen(index) + 1, sizeof(char));
        destinationOp = calloc(strlen(index) + 1, sizeof(char));
    }
    if(numOfOp==2){
        /*check how many operands*/
        strcpy(sourceOp,index);
        sourceOp = strtok(sourceOp, ",");
        if (strchr(sourceOp, ' ')|| strchr(sourceOp, '\t')) {
		char* space = strchr(sourceOp + 1, ' ');
	    char* tab = strchr(sourceOp + 1, '\t');
	    char* cut = NULL;
	    if (space == NULL && tab != NULL)
	    {
		cut = "\t";
	    }
	    else if (tab == NULL && space != NULL)
	    {
		cut = " ";
	    }
		else if (space != NULL && tab != NULL)
	    {
		if ((int)(tab - cut) < (int)(space - cut))
		{
		    cut = "\t";
		}
		else
		{
		    cut = " ";
		}
	    }
            sourceOp = strtok(sourceOp, cut);
        }
        index = strchr(index, ',') + 1;
        index += jumpSpace(index, 0);
        strcpy(destinationOp, index);
        if(strchr(index,'\n')!=NULL){
            destinationOp = strtok(destinationOp, "\n");
        } else{
            destinationOp = strtok(destinationOp, "\0");
        }
        if (strchr(destinationOp, ' ')|| strchr(destinationOp, '\t')) {
	    char* space = strchr(destinationOp + 1, ' ');
	    char* tab = strchr(destinationOp + 1, '\t');
	    char* cut = NULL;
	    if (space == NULL && tab != NULL)
	    {
		cut = "\t";
	    }
	    else if (tab == NULL && space != NULL)
	    {
		cut = " ";
	    }
		else if (space != NULL && tab != NULL)
	    {
		if ((int)(tab - cut) < (int)(space - cut))
		{
		    cut = "\t";
		}
		else
		{
		    cut = " ";
		}
	    }
            destinationOp = strtok(destinationOp, cut);
        }
        IC += getTwoOp(a_o_d, sourceOp, destinationOp,fname) + 1;
        free(sourceOp);
        free(destinationOp);
        return 0;
    }

    if(numOfOp==1) {
        strcpy(destinationOp, index);
        if(strchr(index,'\n')!=NULL){
            destinationOp = strtok(destinationOp, "\n");
        } else{
            destinationOp = strtok(destinationOp, "\0");
        }
        if (strchr(destinationOp, ' ')|| strchr(destinationOp, '\t')) {
	    char* space = strchr(destinationOp + 1, ' ');
	    char* tab = strchr(destinationOp + 1, '\t');
	    char* cut = NULL;
	    if (space == NULL && tab != NULL)
	    {
		cut = "\t";
	    }
	    else if (tab == NULL && space != NULL)
	    {
		cut = " ";
	    }
		else if (space != NULL && tab != NULL)
	    {
		if ((int)(tab - cut) < (int)(space - cut))
		{
		    cut = "\t";
		}
		else
		{
		    cut = " ";
		}
	    }
            destinationOp = strtok(destinationOp, cut);
        }
        IC += getOneOp(a_o_d, destinationOp,fname) + 1;
        free(sourceOp);
        free(destinationOp);
        return 0;
    }

    if(numOfOp==0) {
        IC++;
        free(sourceOp);
        free(destinationOp);
        return 0;
    }
    free(sourceOp);
    free(destinationOp);
    return 0;
}
/* This function gets a pointer to the name of action, a pointer to the source operand ,a pointer to the destenation operand and the name of file.
the function checks what addressing method is each operand and converts only the operands that are labels acording to the address of the label in the label linked list in to the place in the struct macine code according to the IC.
if the label is not one of the labels in the linkes list from the first scan so returns -1 otherwise 0*/
int getTwoOp(char *action,char *sourceOp,char *destinationOp,char* fname){
    int count=0;
    int s=checkOperand(sourceOp);
    if(s==-1){
        return -1;
    }
    int d=checkOperand(destinationOp);
    if(d==-1){
        return -1;
    }
    if(strcmp(action,"mov")==0 ||strcmp(action,"add")==0 || strcmp(action,"sub")==0 || strcmp(action,"cmp")==0||strcmp(action,"lea")==0) {
        if(s==0){
            count++;
        }
        if( s == 1 ) {
            count+=1;

            if(search(sourceOp)==-1){
		char* typeL=typeLabel(sourceOp);
                if(typeL != 0 && strcmp(typeL,"external")==0){
                    memset(MachineCode[IC+count].word,'\0',8);
                    MachineCode[IC+count].word[8]=0;
                    MachineCode[IC+count].word[9]=1;
                    printExternal(sourceOp,IC+count,fname);
                } else{
                    convertB10_B2(idLabel(sourceOp),count,"operand");
                    MachineCode[IC+count].word[8]=1;
                    MachineCode[IC+count].word[9]=0;
                }
            } else {
			printf("\nERROR: Label name does not exists %s",sourceOp);
			return -1;
		}
        }
        if(s == 2 ) {
            count++;
            sourceOp= strtok(sourceOp,".");
            if(search(sourceOp)==-1){
		char* typel=typeLabel(sourceOp);
                if(typel!=0 && strcmp(typel,"external")==0){
                    MachineCode[IC+count].word[8]=0;
                    MachineCode[IC+count].word[9]=1;
                    printExternal(sourceOp,IC+count,fname);
                } else{
                    convertB10_B2(idLabel(sourceOp),count,"operand");
                    MachineCode[IC+count].word[8]=1;
                    MachineCode[IC+count].word[9]=0;
                }
            } else  {
			printf("\nERROR: Label name does not exists %s",sourceOp);
			return -1;
		}
            count++;
        }
        if(s==3){
            count++;
        }
        if(d==0){
            count++;
        }
        if( d == 1 ) {
            count += 1;
            if (search(destinationOp) == -1) {
		char* typel=typeLabel(destinationOp);
                if (typel !=0 && strcmp(typel, "external") == 0) {
                    MachineCode[IC + count].word[8] = 0;
                    MachineCode[IC + count].word[9] = 1;
                    printExternal(destinationOp,IC+count,fname);
                } else {
                    convertB10_B2(idLabel(destinationOp), count, "operand");
                    MachineCode[IC + count].word[8] = 1;
                    MachineCode[IC + count].word[9] = 0;
                }
            }
        }
        if( d == 2){
            count++;
            destinationOp= strtok(destinationOp,".");
            if(search(destinationOp)==-1){
		char* typel=typeLabel(destinationOp);
                if(typel!=0 && strcmp(typel,"external")==0){
                    MachineCode[IC+count].word[8]=0;
                    MachineCode[IC+count].word[9]=1;
                    printExternal(destinationOp,IC+count,fname);
                } else{
                    convertB10_B2(idLabel(destinationOp),count,"operand");
                    MachineCode[IC+count].word[8]=1;
                    MachineCode[IC+count].word[9]=0;
                }
            } else  {
			printf("\nERROR: Label name does not exists %s",destinationOp);
			return -1;
		}
            count++;
        }
        if(d==3){
            count++;
        }
        return count;
    }
    else {
        count++;
        return count;
    }
}
/* This function gets a pointer to the name of action,a pointer to the destenation operand and the name of file.
the function checks what addressing method is the operand and converts the operand only if it's a label acording to the address of the label in the label linked list in to the place in the struct macine code according to the IC.
if the label is not one of the labels in the linkes list from the first scan so returns -1 otherwise 0*/

int getOneOp(char *action, char *destinationOp,char* fname){
    int count=0;
    int d=checkOperand(destinationOp);
    if(d==-1){
        return -1;
    }
    if(strcmp(action,"not")==0 || strcmp(action,"clr")==0 || strcmp(action,"inc")==0 || strcmp(action,"dec")==0 || strcmp(action,"jmp")==0
       || strcmp(action,"bne")==0 || strcmp(action,"get")==0 || strcmp(action,"jsr")==0 || strcmp(action,"prn")==0){
        if(d == 1 ){
            count++;
            if (search(destinationOp) == -1) {
		char* typel=typeLabel(destinationOp);
                if (typel!=0 && strcmp(typel, "external") == 0) {
                    MachineCode[IC + count].word[8] = 0;
                    MachineCode[IC + count].word[9] = 1;
                    printExternal(destinationOp,IC+count,fname);
                } else {
                    convertB10_B2(idLabel(destinationOp), count, "operand");
                    MachineCode[IC + count].word[8] = 1;
                    MachineCode[IC + count].word[9] = 0;
                }
            }
        }

        if(d == 2){
            count++;
            destinationOp= strtok(destinationOp,".");
            if(search(destinationOp)==-1){
		char* typel=typeLabel(destinationOp);
                if(typel!=0 && strcmp(typel,"external")==0){
                    MachineCode[IC+count].word[8]=0;
                    MachineCode[IC+count].word[9]=1;
                    printExternal(destinationOp,IC+count,fname);
                } else{
                    convertB10_B2(idLabel(destinationOp),count,"operand");
                    MachineCode[IC+count].word[8]=1;
                    MachineCode[IC+count].word[9]=0;
                }
            } else  {
			printf("\nERROR: Label name does not exists %s ",destinationOp);
			return -1;
		}
            count++;
        }
        return count;
    }

    else {
        count++;
        return count;
    }
}


