#include "main.h"
/*This project was writen by:
Rivki Chait id: 333820298 and Aliza bendraim id: 212573844 */


/*this function gets an address of a label and checks if it exists in the linked list.
returns 0 if the address exists in the linked list and -1 if not*/
int searchId(int i){
    Label * temp = head;
    while(temp!=NULL){
        if(temp->id==i){
            return 0;
        } else temp= temp->next;
    }
    return -1;
}
/*this function updates the address of the label that was saved in the temp machine code*/
void updateId(int i, int newId){
    struct Label * temp = head;
    while(temp!=NULL){
        if(temp->id==i){
            temp->id=newId;
            return;
        }
        temp = temp->next;
    }
}


/*this main function gets the files to reed. Performs the macro layout step, if there are no errors, creates a new file with the layout, goes to the first pass, and if there are no errors, goes to the second pass, at the end, outputs output files of the file.
returns 0 at the end of the function.*/
int main(int argc, char* argv[]) {
    if (argc < 1) {
        printf("\nERROR: no file were entered\n");
        return 0;
    }
    int j = 0;
    for (j = 1; j < argc; j++)
    {
        int i = 0;
        FILE* ptr=NULL;
        char ch[256];
        /* Opening file in reading mode*/
        char* nameOfFile = argv[j];
        ptr = fopen(nameOfFile, "r");
        if (NULL == ptr) {
	    flag=TRUE;
            printf("\nfile %s can't be opened \n",nameOfFile);
        }
        /*macro*/
	if(flag == FALSE) { 
		char* nameFile = preReadFile(0, ptr, nameOfFile);
		if(nameFile==0){
			flag = TRUE;
			printf("\n--failed macro retirement in file %s--\n",nameOfFile);
		}

		/*first scan*/
		if (flag == FALSE) {            
		    IC = 100;
		    DC = 0;
		 /*read first line*/
		    ptr = fopen(nameFile, "r");
		    if (NULL == ptr) {
		        printf("\nfile %s can't be opened \n", nameFile);
		        flag = TRUE;
		    }


		    /* Printing what is written in file character by character using loop.*/
		    char* f = calloc(257, sizeof(char));
		    f = fgets(ch, sizeof(ch), ptr);
		    while (f != NULL) {
		        if (strlen(f) - 1 < 80) {
		            int line = lineAction(f);
		            if (line == -1) {
		                flag = TRUE;
		            }
		        }
		        else {
		            printf("\nERROR: length of line can't be mor than 80\n");
		            flag = TRUE;
		        }
		
		        f = fgets(ch, sizeof(ch), ptr);
		    }


		    int k = 0;
		    while (k < DC) {
		        if ((IC + k) < MAX_MEMORY) {
		            if (tempMC[k].address == k) {
		                if (searchId(k) == 0) {
		                    updateId(k, k + IC);
		                }
		                MachineCode[IC + k].address = IC + k;
		                for (i = 0; i < 10; i++) {
		                    MachineCode[IC + k].word[i] = tempMC[k].word[i];
		                }
		            }
		            k++;
		        }
		        else printf("ERROR: no place in memory.");
		    }
		    /*second Scan.*/
		    if (flag == FALSE) {
		        IC = 100;
		        DC = 0;
		        FILE* p;
		        p = fopen(nameFile, "r");
		        if (NULL == p) {
		            printf("\nfile can't be opened \n");
		        }
		        f = fgets(ch, sizeof(ch), p);
		        while (f != NULL) {
		            if (strlen(f) - 1 < 80) {
		                int l = line(f,nameOfFile);
		                if (l == -1) {
		                    flag = TRUE;
		                }
		            }
		            else {
		                printf("\nERROR: length of line can't be mor than 80\n");
		                flag = TRUE;
		            }
		            f = fgets(ch, sizeof(ch), p);
		        }         
		        if (flag == FALSE) {
			    char* fname = calloc(sizeof(fname) + 20, sizeof(char));
	    		    sprintf(fname, "%s.ob", nameOfFile);
		            FILE* ps;
		            ps = fopen(fname, "w");
		            int id = 100;
		            while (MachineCode[id].address != 0) {
		                fprintf(ps, "\n");
		                convertB10_B32(MachineCode[id].address, ps);
		                fprintf(ps, "\t");
		                convertB2_B32(id, ps);
		                id++;
		            }
		            fclose(ps);
		        }
			else{
				printf("\n--failed second scan in file %s--\n",nameOfFile);
			}

		    }
			else{
				printf("\n--failed first scan in file %s--\n",nameOfFile);
			}
		
		    fclose(ptr);
		    free(f);
		    printf("\n\ncomplete\n\n");
		    flag = FALSE;
		}
        }

    }
    return 0;
}
