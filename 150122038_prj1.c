#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char toOtherDigit = 0;
int replay = 1;
char carry = 0;
char whereIsDot = 0;
char isThereDot = 'F';
struct digitNode{
	char digit;
	struct digitNode *nextPtr;
};
typedef struct digitNode digitNode;
typedef digitNode *digitNodePtr;
digitNodePtr createNode(char dgt){
	digitNodePtr newPtr = malloc(sizeof(digitNode));
	if(newPtr != NULL){
		newPtr->digit = dgt;
		newPtr->nextPtr = NULL;
	}
	return newPtr;
}
void writeToFile(digitNodePtr *reverseAnswer, char str[]){
	digitNodePtr answer = NULL;
	digitNodePtr tempRev = *reverseAnswer;
	digitNodePtr temp = NULL;
	
	while (tempRev != NULL) {
        temp = tempRev->nextPtr; 
        tempRev->nextPtr = answer; 
        answer = tempRev; 
        tempRev = temp; 
    }
    if(isThereDot == 'T'){
		digitNodePtr temp = answer;
		while((temp->nextPtr)->digit != '.'){
			if(temp->digit == '0'){
				digitNodePtr tmp = NULL;
				tmp = temp;
				temp = temp->nextPtr;
				free(tmp);
				answer = temp;
			}
			else break;
		}
		isThereDot = 'F';
	} 
	FILE *fptr;
	if((fptr = fopen("150122038_prj1_output.txt", "a")) == NULL){
		puts("File could not be found");
	}
	else{
		fprintf(fptr,"%s",str);
		digitNodePtr temp = answer;
		while(temp != NULL){
			fputc (temp->digit, fptr);
			temp = temp->nextPtr;
		}
		fputc('\n', fptr);
	}
	fclose(fptr);
}
void addDot(digitNodePtr *answer){
	digitNodePtr current = *answer;
	digitNodePtr previous = NULL;
	
	digitNodePtr newPtr = malloc(sizeof(digitNode));
	if(newPtr != NULL){
		newPtr->digit = '.';
		newPtr->nextPtr = NULL;
	}
	while(whereIsDot > 0){
		previous = current;
		current = previous->nextPtr;
		whereIsDot--;
	}
	previous->nextPtr = newPtr;
	newPtr->nextPtr = current;
}
void dotLocation(digitNodePtr *number){
	isThereDot = 'T';
	digitNodePtr tempPtr = *number;
	while(tempPtr != NULL){
		if(tempPtr->digit == '.') {
			break;
		}
		whereIsDot++;
		tempPtr = tempPtr->nextPtr;
	}
}
void SUM (digitNodePtr *sum, digitNodePtr *sumTEMP){
	int i = 1;
	digitNodePtr current = *sum;
	digitNodePtr previous = NULL;
	while(i < replay && current != NULL){
		current = current->nextPtr;
		i++;
	}
	digitNodePtr currentTEMP = *sumTEMP;
	while(currentTEMP != NULL && current != NULL){
		char dgt = (current->digit) + (currentTEMP->digit);
		dgt = dgt-'0'-'0';
		dgt += carry;
		carry = 0;
		if(dgt < 10) dgt += '0';
		else{
			carry = dgt / 10;
			dgt %= 10;
			dgt += '0';
		}
		current->digit = dgt;
		previous = current;
		current = current->nextPtr;
		currentTEMP = currentTEMP->nextPtr;
	}
	while(currentTEMP != NULL && current == NULL){
		char dgt =(currentTEMP->digit);
		dgt = dgt-'0';
		dgt += carry;
		carry = 0;
		if(dgt > 9) {
			carry = dgt / 10;
			dgt %= 10;
		}
		dgt += '0';
		digitNodePtr newPtr = createNode(dgt);
		previous->nextPtr = newPtr;
		previous = previous->nextPtr;
		currentTEMP = currentTEMP->nextPtr;
	}
}
void summationElements (digitNodePtr *sum, digitNodePtr *sumTEMP, char numb){
	if(toOtherDigit != 0)
		numb += toOtherDigit;
	toOtherDigit = numb / 10;
	if(numb >= 10)
	numb = numb % 10;
	numb += '0';
	
	if(replay == 1){
		digitNodePtr newPtr = createNode(numb);
		if(*sum == NULL){
	   		*sum = newPtr;
		}
		else{
			digitNodePtr previous = NULL;
			digitNodePtr current = *sum;
			while(current != NULL){
				previous = current;
				current = previous->nextPtr;
			}
			previous->nextPtr = newPtr;
		}
	}
	if(replay > 1){
		digitNodePtr newPtr = createNode(numb);
		if(*sumTEMP == NULL){
	   		*sumTEMP = newPtr;
		}
		else{
			digitNodePtr previous = NULL;
			digitNodePtr current = *sumTEMP;
			while(current != NULL){
				previous = current;
				current = previous->nextPtr;
			}
			previous->nextPtr = newPtr;
		}
    }
}
void Operator(digitNodePtr *multiplicand , digitNodePtr *multiplier){
	digitNodePtr sum = NULL;
	digitNodePtr sumTEMP = NULL;
	digitNodePtr tempPtr = *multiplier;
	digitNodePtr tempPtr2 = *multiplicand;
	char isThereDot = 'T';
	while(tempPtr != NULL){
		if(tempPtr->digit == '.'){
			replay--;
			dotLocation(&(*multiplier));
		} 
		else{
			while(tempPtr2 != NULL){
				if(tempPtr2->digit != '.'){
					char a = ((tempPtr->digit)-'0') * ((tempPtr2->digit)-'0');
					summationElements(&sum, &sumTEMP, a);
				}
				else{
					if(isThereDot == 'T') {
						dotLocation(&(*multiplicand));
						isThereDot = 'F';
					}
				}
				tempPtr2 = tempPtr2->nextPtr;	
			}	
		}
		if(toOtherDigit > 0){
			int temp = toOtherDigit;
			toOtherDigit = 0;
			summationElements(&sum, &sumTEMP,temp);
			}
			
		if(sumTEMP != NULL){
			SUM(&sum,&sumTEMP);
			digitNodePtr temp = sumTEMP;
			free(temp);
			sumTEMP = NULL;
		}
		replay++;
		tempPtr = tempPtr->nextPtr;
		tempPtr2 = *multiplicand;
	}
	if(whereIsDot>0){
		addDot(&sum);	
	}
	writeToFile(&sum, "result: ");
	free(sum);
} 
void createLL(digitNodePtr *head, char digit){
	digitNodePtr newPtr = createNode(digit);
	digitNodePtr tempPtr = NULL;
	if(*head == NULL){
		*head = newPtr;
	}
	else{
		tempPtr = *head;
		*head = newPtr;
		(*head)->nextPtr = tempPtr;
	}
}

int main (){
	clock_t begin, end;
	begin = clock();
	digitNodePtr multiplicand = NULL;
	digitNodePtr multiplier = NULL;
	FILE *fptrin;
	if((fptrin = fopen("150122038_prj1_input.txt", "r")) == NULL){
		puts("File could not be found");
	}
	else {
			char digit;
			char ch = '1';
			while((digit = fgetc(fptrin))){
				if(digit == '\n'){
					 ch = '2';
					 continue;
				}
				if(digit == EOF)
					break;
				switch(ch){
					case '1':createLL(&multiplicand, digit);
							 break;
					case '2':createLL(&multiplier, digit);
							 break;
				}		
			}
			FILE *fptr; 	//to clear the output file when the program starts
			if((fptr = fopen("150122038_prj1_output.txt", "w")) == NULL){
				puts("File could not be found");
			}
			if (multiplicand == NULL || multiplier == NULL){
				fprintf(fptr, "incorrect input!!!!!");
				return 1;
			}
			fclose(fptr);
			
			if(multiplicand->nextPtr == NULL){
				Operator(&multiplier, &multiplicand);
			}
			else Operator(&multiplicand, &multiplier);
			writeToFile(&multiplicand, "Multiplicand: ");
			writeToFile(&multiplier, "Multiplier: ");
			free(multiplicand);
			free(multiplier);
	}
	end = clock();
	double timeSpent = ((double)(end - begin))/ CLOCKS_PER_SEC;
	FILE *fptr;
	if((fptr = fopen("150122038_prj1_output.txt", "a")) == NULL){
		puts("File could not be found");
	}
	else{
		fprintf(fptr, "%s%lf\n","execution time: " , timeSpent);
	}
	
}
