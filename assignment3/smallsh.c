
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char stop[5] = "no"; /*stop initializer for while loop*/

/*Function to ask for user input and wait until enter is pressed*/
char *askUserInput(char *uIn);

/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char* uIn);






int main(){
  char uInput[2048];
  char inputArray[512][2049];
  char userCommand[2048];

  /*Loop until user exits shell*/
  while(strcmp("no", stop) == 0){
    int arrayCounter = 0; /*used to help tell how long my array is*/
    int i=0; /*counter for putting each word user types into an array of strings*/

    printf(": ");
    fgets(uInput,512,stdin); /*get user input from stdin, store into uInput*/
    char *token = strtok(uInput, " "); /*seperate by spaces*/

    while (token != NULL) {
          strcpy(inputArray[i], token);
          arrayCounter++;
          token = strtok(NULL, " "); /*seperate by spaces*/
          i++;
      }
    fflush(stdin);

    for (i=0; i < arrayCounter; i++ ){
      printf("%s\n", inputArray[i]);
    }




  }





return 0;
}


/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char *uIn){

  if(strcmp(uIn, "exit")==0){
    strcpy(stop, "yes");
  }
  else if(strcmp(uIn, "cd")==0){ /*If user wants to change directory*/
    printf("change directory initiated\n");
  }
  else if(strcmp(uIn, "ls")==0){ /*If user wants to list elements in directory*/
    printf("list directory initiated\n");
  }
  else if(strcmp(uIn, "status")==0){ /*If user wants to view the status of a given element*/
    printf("status initiated\n");
  }
  else{
    printf("no action specified\n");
  }

}

























/**/
