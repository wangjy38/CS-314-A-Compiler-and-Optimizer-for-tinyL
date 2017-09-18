/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"
void Mark(Instruction*);
//void MarkVar(Instruction* , int );
void MarkReg (Instruction* , int , int);
Instruction* Remove (Instruction* );
void Mark(Instruction *head){
	Instruction* current = head;
	while (current != NULL) {
		//if (current->opcode = READ) current->critical = 'M';
		if (current->opcode == OUTPUTAI) {
			current->critical = 'M';
			MarkReg(current->prev, current->field1, current->field2);
		}
	current = current->next;
	}
}
/*void MarkVar(Instruction* head, int var){
	Instruction* current = head;
	while (current !=NULL) {
		if (current->opcode == STOREAI && current->field2 == var){
			if (current->critical != 'M'){
				current->critical = 'M';
				MarkReg(head, current->field1);
				break;
			}
		}
		current = current->next;
	}
	
}*/
void MarkReg (Instruction* current, int reg, int offset){
	//Instruction* current = head;
	while (current != NULL){
		//if (current->field3 == reg) {
	    //current->critical = 'M'; 
		switch(current->opcode){
			case LOADI:
				if (current->field2 == reg){
				current->critical = 'M';
				//MarkVar(head,current->field2);
			    return;
			    }
				break;
			case LOADAI:
			    if (current->field3 ==reg){ 
			    	current->critical = 'M';
					MarkReg(current->prev,current->field1, current->field2);  
					return;
				}
					break;
			case STOREAI:
				if (current->field2 == reg&&current->field3 == offset){
					current->critical = 'M';
					MarkReg(current->prev,current->field1,-1); 
					MarkReg(current->prev,current->field2,-1); 
					return;
				}
				break;
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				if (current->field3 == reg){
					current->critical = 'M';
					MarkReg(current->prev,current->field1, -1);
					MarkReg(current->prev,current->field2, -1);
				    return;
				}
				break;
			default:
				break;
			}
		//}
		current= current->prev;
	}

}
Instruction* Remove (Instruction* head){
	Instruction* current = head;
	Instruction* previous = head->prev;
	Instruction* del;
	while (current != NULL) {
		//printf("%d", 1);
		if (current->critical != 'M'){
			if (current->next==NULL){ //delete from the end
			//printf("%c ",'E');	
                        if (previous!=NULL){ // More than 1 
					previous->next=NULL; 
					free(current);
					current=NULL;
				}else { //  Just One
					head=NULL; 
					free(current);
					current=NULL;
				}
			}else if (previous != NULL){ //delete from middle
                         //printf("%c ",'M');
				del=current;
				current = current->next;
				previous->next = current;
				current->prev = previous;
				free(del);
                //del=NULL;
			}else { //delete from the top
                         //printf("%c ",'B');
				del = current;
				current = current->next;
				//previous = current->prev;
				free(del);
				//del=NULL;
				previous=NULL;
				current->prev=NULL;
				head=current;
			}
		} else {
			previous=current;
			current=current->next;
			
		}
	}
    return head;
}
int main()
{
	Instruction *head;
    Instruction *i;
	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	/* YOUR CODE GOES HERE */
	Mark(head);
        //PrintInstructionList(stdout, head);	
	head=Remove(head);

	if (head) {
		PrintInstructionList(stdout, head);
		while (head) {
			i = head;
			head=head->next;
			free(i);
		}
	}	
	return EXIT_SUCCESS;
}

