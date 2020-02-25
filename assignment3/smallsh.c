#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

char stop[5] = "no"; /*stop initializer for while loop*/
pid_t forkPID;
int status = 0;
char backGroundCheck = 'n';

/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char* uIn, char *myArray[2049], int);

void printArrayHelper(char* myArray[2049], int);

void getStatus(int st); /*idea from lecture*/

void catchCTRLC(int signo);
void catchCTRLZ(int signo);




int main(){
  char uInput[2048];
  char *inputArray[2049];
  char userCommand[2048]; /*store the first element of user input array here.  The user's command*/
  char lastArg[2048]; /*store the last element of user input array here. could be & for background process*/
  char tempSTR[2048];/*use this to help clean new line elements from strings*/
  char *testArray[2049];

  backGroundCheck = 'n'; /*initalize to not want to run stuff in background.  User can change by specifying '&'*/

  struct sigaction SIGCTRLC_action = {0}, SIGCTRLZ_action = {0};

/*SIG STUFF TAKEN FROM LECTURE--------------------------------*/
  SIGCTRLC_action.sa_handler = catchCTRLC;
  sigfillset(&SIGCTRLC_action.sa_mask);
  SIGCTRLZ_action.sa_flags = 0;

  SIGCTRLZ_action.sa_handler = catchCTRLZ;
  sigfillset(&SIGCTRLZ_action.sa_mask);
  SIGCTRLZ_action.sa_flags = 0;

  sigaction(SIGINT, &SIGCTRLC_action, NULL);
  sigaction(SIGUSR2, &SIGCTRLZ_action, NULL);
/*END SIG STUFF-----------------------------------------------*/

  printf("pid in main->%d\n",getppid());
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
          inputArray[arrayCounter] = strdup(tempSTR);
          arrayCounter++;
          token = strtok(NULL, " "); /*seperate by spaces*/
          i++;
    }



    fflush(stdin);

    strcpy(userCommand, inputArray[0]); /*get the command into its own variable*/
    strcpy(lastArg, inputArray[arrayCounter-1]);/*store the last argument the user gives into its own variable*/

    if(strcmp(inputArray[arrayCounter-1], "&") == 0){
      inputArray[arrayCounter-1] = NULL; /*If the last argument is an &, then save it to lastArg then set that spot in array to null for execvp readability*/
    }
    else{
      inputArray[arrayCounter] = NULL; /*set the element after our last argument in the array to null, so execvp knows when to stop*/
    }

    if(strcmp(lastArg, "&")==0){ /*If user wants to run command in background*/
      backGroundCheck = 'y';
    }

    interpretUserCommand(userCommand, inputArray, arrayCounter);

















  } /* end of big while loop*/





return 0;
}


void catchCTRLC(int signo){
  char *message = " Caught SIGINT/CTRLC!\n";
  write(STDOUT_FILENO, message, 38);
  fflush(stdout);

}

void catchCTRLZ(int signo){
  char *message = "Caught! BYEE!\n";
  write(STDOUT_FILENO, message, 25);
  exit(0);
}

/*get the status.  This idea was from the lecture videos*/
void getStatus(int st){
   int eSt = -10;
   if(WIFEXITED(st)){
     eSt = WEXITSTATUS(st);
     printf("exit value is %i\n", eSt);
     fflush(stdout);
   }
   else{
     printf("Terminated by signal %i\n", st);
     fflush(stdout);
   }
}


/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char *uIn, char *myArray[2049], int arrayCnt){
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
  else if(strcmp(uIn, "status")==0){ /*If user wants to view the status of a given element*/
    getStatus(status);
  }
  else if(strcmp(uIn, "#") == 0){
      ; /*Do nothing*/
  }
  else if(strcmp(uIn, "&") == 0){
      ; /*Do nothing*/
  }


  else{
    forkPID = fork();

    switch(forkPID){
      case -1: {
        perror("Hull Breach!\n");
        fflush(stdout);
        status = 1;
        break;
      }
      case 0:{
        fflush(stdout);
        if(execvp(uIn, myArray)){
          printf("Command Not Found\n");
          fflush(stdout);

          _Exit(1); /*abort if child fails*/
        }
        break;
      }
      default: {

        if(backGroundCheck == 'n'){
          waitpid(forkPID, &status, 0);
        }
        else{
          printf("BackgroundPID -> %d\n", forkPID);
          fflush(stdout);
          usleep(10000); /*help the colon be presented to the user in the right spot. stops the program from getting ahead of itself*/
          break;
        }

      }

    }

  }
    forkPID = waitpid(-1, &status, WNOHANG);           //Check if any process has completed; Returns 0 if no terminated processes
    while(forkPID > 0){
        printf("background process, %i, is done: ", forkPID); /*FIX MEEEEEEEE*/
        fflush(stdout);
        getStatus(status);
        forkPID = waitpid(-1, &status, WNOHANG);
    }

}

/*helper function to print my array*/
void printArrayHelper(char *myArray[2049], int arrayLength){

  int j = 0;
  printf("\n");
  for(j=0;j<arrayLength;j++){
    printf("%s\n", myArray[j]);
  }

}
























/**/
