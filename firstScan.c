
#include "firstScan.h"
Boolean exist=FALSE;
/*This function gets a name of an action to convert to binary.
findes the number of the action acording to the enum opBinary_names and converts the number*/
void convertAction(char * ac){
    enum opBinary num;
    int a;
    for(a=0; a<16; a++) {
        if (strcmp(opBinary_names[a], ac)  == 0){
            num= a;
            break;
        }
    }
    convertB10_B2(num, 0, "op");
}

/*This function gets a line to read.
if there is a label puts it in the linked list and converts the rest of the line.
if the action is one of the actions so sends to the action function the name of action with the number of operands he needs to get and a pointer to the first charecter of the rest of the line after the action .
if the action is directive sends to the directive function the name of directive and a pointer to the first charecter of the rest of the line after the name of directive.
returns 0 if no errors and 1 if there is errors in the line.*/

int lineAction(char *line) {

   
    if (strchr(line, '\r'))
    {
        char* i = strchr(line, '\r');
        int j = (int)(i - line);
        line[j] = '\0';
    }
    char *index = calloc(strlen(line) + 1, sizeof(char));
    strcpy(index,line);
    index += jumpSpace(index, 0);
    if(strcmp(index,"\n")==0 || strncmp(index,";",1)==0 ||strcmp(index,"\0")==0){
        return 0;
    }
    int id=0;
    int numOfOp=0;
    char *i=NULL;
    char *a_o_d= calloc(81, sizeof(char));
    char *name= calloc(81, sizeof(char)) , *type= calloc(81, sizeof(char));//name and type of label

    /*if label*/
    if (strchr(line, ':') != NULL) {
        strcpy(name,index);
        name = strtok(name, ":");
        index = strchr(index, ':');
        index--;
        if (jumpSpace(index, 0) == 0) {
            index++;
            if (checkLabel(name) == 0) {
                index++;
                if (jumpSpace(index, 0) > 0) {
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
            else
                index = NULL;
		    if (cut != NULL)a_o_d = strtok(a_o_d, cut);
		        if (strchr(a_o_d, '.') != NULL) {
		                if (strcmp(a_o_d, ".data") == 0 || strcmp(a_o_d, ".string") == 0 ||
		                    strcmp(a_o_d, ".struct") == 0) {
		                    type = "data";
		                    if (search(name) != 0) {
		                        if(strcmp(typeLabel(name),"entry")!=0){
		                            free(a_o_d);
		                            free(name);
		                            return -1;
		                        }
		                    }
		                    exist=TRUE;
		                    id=DC;
		                }
                    } else {
                        type = "code";
                        if (search(name) != 0) {
                            if(strcmp(typeLabel(name),"entry")!=0){
				printf("\nERROR: Invalid label. label name already exists.");
                                free(a_o_d);
                            	free(name);
                                return -1;
                            }
                        }
                        exist=TRUE;
                        id=IC;
                    }
                } else {
                    printf("\nERROR: Invalid label. has to have space after \':\'");
                    free(index);
                    free(a_o_d);
                    free(name);
                    return -1;
                }
            } else {
                free(index);
                free(a_o_d);
                free(name);
                return -1;
            }

        } else {
            printf("\nERROR: Invalid label can't have space before \':\'");
            free(index);
            free(a_o_d);
            free(name);
            return -1;
        }
    }

    else{
	exist=FALSE;
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
    	if (cut != NULL )a_o_d = strtok(a_o_d, cut);
	
    }
    /*check what action or directive*/
    if(index == NULL || strlen(index)==1){
        if(strchr(a_o_d,'\n')!=NULL) {
            a_o_d = strtok(a_o_d, "\n");
        }
        if((strcmp(a_o_d,"hlt")!=0 && strcmp(a_o_d,"rts")!=0)){
            printf("\nERROR: have to have space in the line \"%s\"",a_o_d);
            free(a_o_d);
            free(name);
            return -1;
        }

    }
    else{
        index += jumpSpace(index, 0);
        if((strcmp(index,"\n")==0 || strcmp(index,"\0")==0) && (strcmp(a_o_d,"hlt")!=0 && strcmp(a_o_d,"rts")!=0)) {
            printf("\nERROR: have to have parameter after \"%s\"",a_o_d);
            free(a_o_d);
            free(index);
            free(name);
            return -1;
        }
    }



    if(strcmp(a_o_d,".extern")==0){
	type= "external";
	if (strchr(index, ' ') || strchr(index, '\t')) {
		    char* space = strchr(index + 1, ' ');
		    char* tab = strchr(index + 1, '\t');
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
		    index = strtok(index, cut);
	    } else {
	        printf("\nERROR: invalid, space in label");
		return -1;
	    }
	}
	if(strchr(index, '\n')!=NULL){
		index=strtok(index, "\n");
	}
	if(strchr(index, '\0')!=NULL){
		index=strtok(index, "\0");
	}
	if(checkLabel(index)==0){
		char* k = typeLabel(index);
		if (k != 0 &&( strcmp(k, "entry") == 0 || strcmp(k, "external") == 0)) {
			printf("\nERROR: Invalid label. label name: %s already exists.",index);
			free(a_o_d);
			return -1;
		    }
		else{
		    insert(index, -1, type);
		    free(a_o_d);
		    return 0;
		}
	} else {
	    free(a_o_d);
	    return -1;
	}

    }



    if(strcmp(a_o_d,".entry")==0){
	type= "entry";
	if (strchr(index, ' ') || strchr(index, '\t')) {
		char* space = strchr(index + 1, ' ');
		    char* tab = strchr(index + 1, '\t');
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
		    index = strtok(index, cut);
	    } else {
	        printf("\nERROR: invalid, space in label");
		return -1;
	    }
	}
	if(strchr(index, '\n')!=NULL){
		index=strtok(index, "\n");
	}
	if(strchr(index, '\0')!=NULL){
		index=strtok(index, "\0");
	}
	if(checkLabel(index)==0){
		char* k = typeLabel(index);
		if(k != 0 && (strcmp(k,"entry")==0 || strcmp(k,"external")==0))
		{
			printf("\nERROR: Invalid label. label name: %s already exists.",index);
			free(a_o_d);
			return -1;
	    	}
		else{
		    insert(index, -1, type);
		    free(a_o_d);
		    return 0;
		}
	}
	else
	{
	    free(a_o_d);
	    return -1;
	}
           
    }
    if (strchr(a_o_d, '.') != NULL) {
        int d=directive(a_o_d, index);
        if(d==0){
            if(exist==TRUE){
                insert(name,id,type);
                free(a_o_d);
                free(name);
                return 0;
            } else{
                free(a_o_d);
                free(name);
                return 0;
            }
        } else{
            free(a_o_d);
            free(name);
            return -1;
        }

    }
    else {
        numOfOp=checkAction(a_o_d);
        if(numOfOp!=-1){
            int a=action(a_o_d,numOfOp, index);
            if(a==0){
                if(exist==TRUE){
                    insert(name,id,type);
                    free(a_o_d);
                    free(name);
                    return 0;
                }
                else
                {
                    free(a_o_d);
                    free(name);
                    return 0;
                }
               
            } else{
                free(a_o_d);
                free(name);
                return -1;
            }

        } else{
            free(name);
            return -1;
        }
    }
    free(a_o_d);
    free(name);
    return 0;
}



/*this function gets a pointer to the name of action, number of operands the action gets and pointer to the first carecter after the name of action.
the function saves in a pointer the source operand and the destination operand acording to the number of operands the action gets and sends the name and source operand and the destination operand to a function that returns the number of Information words the action saves in the machinecode.
returns -1 if there is an error in the operands of the action and 0 if not*/
int action(char *a_o_d,int numOfOp, char *index){
    int count=0, sub=0;
    char* i = NULL; 
    char* token = NULL;
    if(index != NULL) 
        token = calloc(strlen(index) + 1, sizeof(char));
    char *sourceOp= calloc(31, sizeof(char));
    char *destinationOp= calloc(31, sizeof(char));
    if(numOfOp==2){
        /*check how many operands*/
        /*If there is a comma between them there are 2 operands*/
        if (strchr(index, ',')!=NULL) {
            strcpy(token, index);
            token=strchr(token, ',')+1;
            sub++;
            while (token != NULL) {
                if (strchr(token, ',')!=NULL){
                    sub++;
                    token = strtok(NULL, ",");
                } else break;
            }
            if (sub == 1) {
                if(strncmp(index,",",1)!=0){
                    strcpy(sourceOp,index);
                    sourceOp = strtok(sourceOp, ",");
                    if (strchr(sourceOp, ' ') || strchr(sourceOp, '\t')) {
			    char* space = strchr(sourceOp + 1, ' ');
			    char* tab = strchr(sourceOp + 1, '\t');
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
                            sourceOp = strtok(sourceOp, cut);
                        } else {
                            printf("\nERROR: invalid, space in source operand");
                            free(sourceOp);
                            free(destinationOp);
                            return -1;
                        }
                    }
                }else{
                    printf("\nERROR: Invalid action %s, no source operand", a_o_d);
                    free(token);
                    free(sourceOp);
                    free(destinationOp);
                    return -1;
                }
                index = strchr(index, ',') + 1;
                index += jumpSpace(index, 0);
                strcpy(destinationOp,index);
                if(strchr(index,'\n')!=NULL){
                    destinationOp = strtok(destinationOp, "\n");
                } else{
                    destinationOp = strtok(destinationOp, "\0");
                }
                if(destinationOp!=NULL) {
                    if (strchr(destinationOp, ' ') || strchr(destinationOp, '\t')) {
			char* space = strchr(destinationOp + 1, ' ');
			    char* tab = strchr(destinationOp + 1, '\t');
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
                            destinationOp = strtok(destinationOp, cut);
                        } else {
                            printf("\nERROR: invalid, space in destination operand");
                            free(token);
                            free(sourceOp);
                            free(destinationOp);
                            return -1;
                        }
                    }
                } else{
                    printf("\nERROR: Invalid action %s, no destination operand", a_o_d);
                    free(token);
                    free(sourceOp);
                    free(destinationOp);
                    return -1;
                }
                count = checkTwoOp(a_o_d, sourceOp, destinationOp);
                if (count != -1) {
                    IC += count + 1;

                    free(sourceOp);
                    free(destinationOp);
                    return 0;
                } else {
                    free(token);
                    free(sourceOp);
                    free(destinationOp);
                    return -1;
                }
            }
            else {
                printf("\nERROR: Invalid action %s, to many operands", a_o_d);
                free(token);
                free(sourceOp);
                free(destinationOp);
                return -1;

            }
            free(token);
            free(sourceOp);
            free(destinationOp);
            return 0;
        } else {
            printf("\nERROR: Invalid action %s, needed %d operand", a_o_d, numOfOp);
            free(token);
            free(sourceOp);
            free(destinationOp);
            return -1;
        }
    }

    if(numOfOp==1) {
        if (strchr(index, ',')==NULL) {
            strcpy(destinationOp, index);
            if(strchr(index,'\n')!=NULL){
                destinationOp = strtok(destinationOp, "\n");
            } else{
                destinationOp = strtok(destinationOp, "\0");
            }
            if(destinationOp!=NULL) {
                if (strchr(destinationOp, ' ')|| strchr(destinationOp, '\t')) {
			char* space = strchr(destinationOp + 1, ' ');
			    char* tab = strchr(destinationOp + 1, '\t');
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
                            destinationOp = strtok(destinationOp, cut);
                    } else {
                        printf("\nERROR: invalid,  space in destination operand");
                        free(token);
                        free(sourceOp);
                        free(destinationOp);
                        return -1;
                    }
                }
            }else{
                printf("\nERROR: Invalid action %s, no destination operand", a_o_d);
                free(token);
                free(sourceOp);
                free(destinationOp);
                return -1;
            }
            count = checkOneOp(a_o_d, destinationOp);
            if (count != -1) {
                IC += count + 1;
                free(token);
                free(sourceOp);
                free(destinationOp);
                return 0;
            } else {
                free(token);
                free(sourceOp);
                free(destinationOp);
                return -1;
            }
        }else {
            printf("\nERROR: invalid,  Invalid action %s, to many operands", a_o_d);
            free(token);
            free(sourceOp);
            free(destinationOp);
            return -1;
        }
        free(token);
        free(sourceOp);
        free(destinationOp);
        return 0;
    }

    if(numOfOp==0) {
        MachineCode[IC].address=IC;
        convertAction(a_o_d);
        IC++;
    } else {
        printf("\nERROR: Invalid action %s, can't get operand", a_o_d);
        free(token);
        free(sourceOp);
        free(destinationOp);
        return -1;
    }
    free(token);
    free(sourceOp);
    free(destinationOp);
    return 0;
}



/*This function gets the name of action, source operand and destenation operand when there are two operands in the action line. checks if the operands are valid and converts them to binary acording to the addressing methods and saves it in the stract of the machine code.
returns how many words of information the action and the operands saves.*/
int checkTwoOp(char *action,char *sourceOp,char *destinationOp) {
    if (strchr(destinationOp, '\r'))
    {
        char* i = strchr(destinationOp, '\r');
        int index = (int)(i - destinationOp);
        destinationOp[index]= '\0';
    }
    int count=0;
    int s=checkOperand(sourceOp);
    if(s==-1){
        return -1;
    }
    int d = 0;
    if (destinationOp != NULL) d=checkOperand(destinationOp);
    if(d==-1){
        return -1;
    }
    convertAction(action);
    MachineCode[IC].address=IC;
    if(strcmp(action,"mov")==0 ||strcmp(action,"add")==0 || strcmp(action,"sub")==0 || strcmp(action,"cmp")==0) {
        if (s == 3 && d == 3){
            MachineCode[IC].word[4]=1;
            MachineCode[IC].word[5]=1;
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=1;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert 2 of the registers(check register)*/
            convertB10_B2(atoi(sourceOp+1) ,count,"regS");
            convertB10_B2(atoi(destinationOp+1),count,"regDR");
        }

        if (s == 0 ) {
            MachineCode[IC].word[4]=0;
            MachineCode[IC].word[5]=0;
            count+=1;
            MachineCode[IC+count].address=IC+count;
            /*convert number to binary(check number)*/
            if(strncmp(sourceOp,"#+",2)==0)
            {
                convertB10_B2(atoi(sourceOp+2),0,"directive");
            }
            else convertB10_B2(atoi(sourceOp+1),count,"operand");
        }
        if( s == 1 ) {
            MachineCode[IC].word[4]=0;
            MachineCode[IC].word[5]=0;
            count+=1;
            MachineCode[IC+count].address=IC+count;
        }
        if(s == 2 ) {
            MachineCode[IC].word[4]=1;
            MachineCode[IC].word[5]=0;
            count++;
            MachineCode[IC+count].address=IC+count;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert number*/
            convertB10_B2(atoi(strchr(sourceOp, '.')+1),count,"operand");
            
        }
        if( s == 3 && d != 3) {
            MachineCode[IC].word[4]=1;
            MachineCode[IC].word[5]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
            /*convert register*/
            convertB10_B2(atoi(sourceOp+1),count,"regS");
        }
        if( d == 1 ) {
            MachineCode[IC].word[6]=0;
            MachineCode[IC].word[7]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
        }
        if( d == 2){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=0;
            count++;
            MachineCode[IC+count].address=IC+count;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert number*/
            convertB10_B2(atoi(strchr(destinationOp, '.')+1),count,"operand");
        }
        if( d == 3 && s != 3){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
            /*convert register*/
            convertB10_B2(atoi(destinationOp+1),count,"regD");
        }
        return count;
    }
    if( strcmp(action,"cmp")==0 && d == 0){
        MachineCode[IC].word[6]=0;
        MachineCode[IC].word[7]=0;
        count+=1;
        MachineCode[IC+count].address=IC+count;
        /*convert number*/
        convertB10_B2(atoi(destinationOp+1),count,"operand");
        return count;
    }
    if(strcmp(action,"lea")==0){
        if( s == 1 ) {
            MachineCode[IC].word[4]=0;
            MachineCode[IC].word[5]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
        }
        if( d == 1 ) {
            MachineCode[IC].word[6]=0;
            MachineCode[IC].word[7]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
        }
        if( s == 2 ) {
            MachineCode[IC].word[4]=1;
            MachineCode[IC].word[5]=0;
            count++;
            MachineCode[IC+count].address=IC+count;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert number*/
            convertB10_B2(atoi(strchr(sourceOp, '.')+1),count,"operand");
        }
        if( d == 2){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=0;
            count++;
            MachineCode[IC+count].address=IC+count;
            count++;
            MachineCode[IC+count].address=IC+count;
            convertB10_B2(atoi(strchr(destinationOp, '.')+1),count,"operand");
        }
        if( d == 3 ){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
            /*convert register*/
            convertB10_B2(atoi(destinationOp+1),count,"regD");

        }
        return count;
    }
    else {
        printf("\nERROR: Invalid action %s- can't get tow operands", action);
        return -1;
    }
}

/*This function gets the name of actionand destenation operand when there is only one operands in the action line. checks if the operand is valid and converts it to binary acording to the addressing methods and saves it in the stract of the machine code.
returns how many words of information the action and the operand saves.*/
int checkOneOp(char *action, char *destinationOp){
    int count=0;
    int d=checkOperand(destinationOp);
    if(d==-1){
        return -1;
    }
    convertAction(action);
    MachineCode[IC].address=IC;
    if(strcmp(action,"not")==0 || strcmp(action,"clr")==0 || strcmp(action,"inc")==0 || strcmp(action,"dec")==0 || strcmp(action,"jmp")==0
       || strcmp(action,"bne")==0 || strcmp(action,"get")==0 || strcmp(action,"jsr")==0 || (strcmp(action,"prn")==0 && d!=0)){
        if(d == 1 ){
            MachineCode[IC].word[6]=0;
            MachineCode[IC].word[7]=1;
            count+=1;
            MachineCode[IC+count].address=IC+count;
        }

        if(d == 2){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=0;
            count++;
            MachineCode[IC+count].address=IC+count;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert number*/
            convertB10_B2(atoi(strchr(destinationOp, '.')+1),count,"operand");
        }
        if ( d == 3){
            MachineCode[IC].word[6]=1;
            MachineCode[IC].word[7]=1;
            count++;
            MachineCode[IC+count].address=IC+count;
            /*convert register*/
            convertB10_B2(atoi(destinationOp+1),count,"regD");
        }
        return count;
    }
    if(strcmp(action,"prn")==0 && d==0){
        MachineCode[IC].word[6]=0;
        MachineCode[IC].word[7]=0;
        count+=1;
        MachineCode[IC+count].address=IC+count;
        /*convert number*/
        if(strncmp(destinationOp,"#+",2)==0)
        {
            convertB10_B2(atoi(destinationOp+2),0,"directive");
        }
        else convertB10_B2(atoi(destinationOp+1),count,"operand");
        return count;
    }
    else {
        printf("\nERROR: Invalid action %s- can't get tow operands", action);
        return -1;
    }
}





