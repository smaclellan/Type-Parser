#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 50


void intRealloc(int** intArray,int *numOfElements,int newNum);
void douRealloc(double **dblArray,int *numOfElements,double newNum);
void charRealloc(char **charArray, int *numOfElements, char newChar);
void freeArray(int **myArr);
int determineType(char *input);

int main(int argc, char *argv[]){



  int *intArr;
  char *charArr;
  double *douArr;
  char *stArr;
  int numint = 0;
  int numdou = 0;
  int numchar = 0;
  int numstr = 0;
  char *ptr;
  int arraysize;
  int newint;
  int newdbl;
  int newchar;
  int type;




  intArr = malloc(sizeof(int));
  charArr = malloc(sizeof(char));
  douArr = malloc(sizeof(double));
  stArr = malloc(sizeof(char));

  char input[BUFFER_SIZE];


  for(int i=0; i < 25; i++){

  printf("Please enter a string, char, int, or double\n");
    fgets(input, BUFFER_SIZE, stdin);

    type = determineType(input);
    if(type==1){
        newint = strtol(input,&ptr,0);
        intRealloc(&intArr,&numint,newint);
    }
    if(type==2){
        newdbl = strtod(input,&ptr);
        douRealloc(&douArr,&numdou,newdbl);
    }
    if(type==3){
        newchar = input[0];
        charRealloc(&charArr,&numchar,newchar);
    }
    if(type==4){
        for(int i =0;i<(sizeof(input)/sizeof(char));i++){
            if(input[i]=='\n'){
                break;
            }
            charRealloc(&stArr,&numstr,input[i]);
        }
        char emptySpace = 0x20 ;
        charRealloc(&stArr,&numstr,emptySpace);



    }


        arraysize = numint;
        printf("Integer List: ");
        for(int j=0;j<arraysize;j++){
            printf("%d ",intArr[j]);
        }
        printf("\n");
        arraysize = numdou;
        printf("Double List: ");
        for(int j=0;j<arraysize;j++){
            printf("%f ",douArr[j]);
        }
        printf("\n");
        arraysize = numchar;
        printf("Character List: ");
        for(int j=0;j<arraysize;j++){
            printf("%c ",charArr[j]);
        }
        printf("\n");
        arraysize = numstr;
        printf("String List: ");
        for(int j=0;j<arraysize;j++){
            printf("%c",stArr[j]);
        }

        printf("\n");

        }

    free(intArr);
    free(douArr);
    free(charArr);
    free(stArr);


  return 0;

}


void intRealloc(int** intArray,int *numOfElements,int newNum){
    *numOfElements+=1;
    int temp = *numOfElements-1;
    *intArray = realloc(*intArray,*numOfElements*sizeof(int));
    (*intArray)[temp] = newNum;
}
void douRealloc(double **dblArray,int *numOfElements,double newNum){
    *numOfElements+=1;
    int temp = *numOfElements-1;
    *dblArray = realloc(*dblArray,*numOfElements*sizeof(double));
    (*dblArray)[temp] = newNum;
}
void charRealloc(char **charArray, int *numOfElements, char newChar){
    *numOfElements+=1;
    int temp = *numOfElements-1;
    *charArray = realloc(*charArray,*numOfElements*sizeof(char));
    (*charArray)[temp] = newChar;
}
int determineType(char *input){
    //Going to use bootleg booleans, 1 equals true. 0 equals false
    int aChar =1;
    int anInt = 1;
    int aDbl =1;
    //If there is a period the entry could be a double. So lets keep track
    int periods =0;
    //checking to see if 2nd character is a new line.
    if(input[1]=='\n'){
        //if so we can check to see if the 1st character is a digit
        if(isdigit(input[0])==0){
        //if not it has to be a character
            anInt =0;
            aDbl =0;
        }else{
        //otherwise its an integer
            aDbl =0;
            aChar=0;
        }
    }else{
        //otherwise iterate until \n is found, can't be a character
        aChar =0;
        for(int i=0;input[i]!='\n';i++){
            //check to see if there is a period
            if(input[i]==46){
                periods++;
            }
            if(isdigit(input[i])==0){
                //cant be an integer
                anInt=0;
            }

        }
        if(periods==1){
            //Its a double or a string!
            anInt = 0;
        }
        if(periods>1 || periods <1){
            //It has to be a string at this point
            aDbl = 0;
        }
    }
    if(anInt==1){
        return 1;
    }
    if(aDbl==1){
        return 2;
    }
    if(aChar==1){
        return 3;
    }
    else{
        //if nothing else it has to be a string
        return 4;
    }
}


