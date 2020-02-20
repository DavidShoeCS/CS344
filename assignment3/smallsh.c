#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char stop[5] = "no"; /*stop initializer for while loop*/


/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char* uIn, char myArray[512][2049], int);

void printArrayHelper(char myArray[512][2049], int);






int main(){
  char uInput[2048];
  char inputArray[512][2049];
  char userCommand[2048]; /*store the first element of user input array here.  The user's command*/
  char lastArg[2048]; /*store the last element of user input array here. could be & for background process*/
  char tempSTR[2048];/*use this to help clean new line elements from strings*/

  /*Loop until user exits shell*/
  while(strcmp("no", stop) == 0){
    int arrayCounter = 0; /*used to help tell how long my array is*/
    int i=0; /*counter for putting each word user types into an array of strings*/

    printf(": ");
    fflush(stdout); /*flush standard in buffer, as recommended from assignment*/
    fgets(uInput,512,stdin); /*get user input from stdin, store into uInput*/
    char *token = strtok(uInput, " "); /*seperate by spaces. Tokenize*/


    while (token != NULL) {
          sscanf(token, "%s", tempSTR);
          strcpy(inputArray[arrayCounter], tempSTR);
          arrayCounter++;
          token = strtok(NULL, " "); /*seperate by spaces*/
          i++;
    }

    fflush(stdin);

    strcpy(userCommand, inputArray[0]); /*get the command into its own variable*/
    strcpy(lastArg, inputArray[arrayCounter-1]);/*store the last argument the user gives into its own variable*/


    interpretUserCommand(userCommand, inputArray, arrayCounter);














  } /* end of big while loop*/





return 0;
}


/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char *uIn, char myArray[512][2049], int arrayCnt){
  char s[100];
  if(strcmp(uIn, "exit")==0){
    strcpy(stop, "yes");
    exit(0);
  }
  else if(strcmp(uIn, "cd")==0){ /*If user wants to change directory*/
    if(arrayCnt == 1){ /*if our array count is only 1 that means we did not specify an argument to use*/
      chdir(getenv("HOME")); /*change directory to home directory*/
    }
    else{
      chdir(myArray[1]); /*change directory to given argument*/
    }
  }
  else if(strcmp(uIn, "ls")==0){ /*If user wants to list elements in directory*/
    printf("list directory initiated\n");
  }
  else if(strcmp(uIn, "status")==0){ /*If user wants to view the status of a given element*/
    printf("status initiated\n");
  }
  else if(strcmp(uIn, "pwd")==0){ /*If user wants to view the status of a given element*/
    printf("pwd initiated\n");
  }
  else{
    printf("no action specified\n");
  }

}

/*helper function to print my array*/
void printArrayHelper(char myArray[512][2049], int arrayLength){

  int j = 0;
  printf("\n");
  for(j=0;j<arrayLength;j++){
    printf("%s\n", myArray[j]);
  }

}
























/**/
