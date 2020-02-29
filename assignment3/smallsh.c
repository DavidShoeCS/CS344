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
int backGroundCheck = 0;
int allowBG = 1;

int testCommand = 1;

/*take user input and do actions based on what the user typed*/
void interpretUserCommand(char* uIn, char *myArray[2049], int);

void printArrayHelper(char* myArray[2049], int);

void getStatus(int st); /*idea from lecture*/

void catchCTRLC(int signo);
void catchCTRLZ(int sign);


char *iFile = NULL;
char *oFile = NULL;

int fileWriteStatus = -1;
int fileReadStatus = -1;

char *replaceDollarSigns(const char *string, const char *oldString, int PID){
  char newString[100];
  sprintf(newString, "%d", PID);
  char *res;
  int i=0;
  int count = 0;
  int newStringLen = strlen(newString);
  int oldStringLen = strlen(oldString);

  for(i=0; string[i] != '\0'; i++){
    if(strstr(&string[i], oldString) == &string[i]){
      count++;
      i+=oldStringLen - 1;
    }
  }

  res = (char *)malloc(i+ count * (newStringLen - oldStringLen) +1);
  i=0;
  while(*string){
    if(strstr(string, oldString) == string){
      strcpy(&res[i], newString);
      i+=newStringLen;
      string+=oldStringLen;
    }
    else{
      res[i++] = *string++;
    }
    res[i] = "\0";
    return res;
  }

}
//-------------
char *replaceWord(const char *s, const char *oldW, int PID) {
    char newW[100];
    sprintf(newW, "%d", PID);
    char *result;
    int i, count = 0, newWlen = strlen(newW) , oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            count++;
            i += oldWlen - 1;
        }
    }

    result = (char *)malloc(i + count * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}


int main(){
  char uInput[2048];
  char *inputArray[2049];
  char userCommand[2048]; /*store the first element of user input array here.  The user's command*/
  char lastArg[2048]; /*store the last element of user input array here. could be & for background process*/
  char tempSTR[2048];/*use this to help clean new line elements from strings*/
  char *testArray[2049];




  backGroundCheck = 0; /*initalize to not want to run stuff in background.  User can change by specifying '&'*/

  struct sigaction SIGCTRLC_action = {0}, SIGCTRLZ_action = {0};

/*SIG STUFF TAKEN FROM LECTURE--------------------------------*/
  SIGCTRLC_action.sa_handler = SIG_IGN;
  sigfillset(&SIGCTRLC_action.sa_mask);
  SIGCTRLC_action.sa_flags = 0;

  SIGCTRLZ_action.sa_handler = catchCTRLZ;
  sigfillset(&SIGCTRLZ_action.sa_mask);
  SIGCTRLZ_action.sa_flags = 0;

  sigaction(SIGINT, &SIGCTRLC_action, NULL);
  sigaction(SIGTSTP, &SIGCTRLZ_action, NULL);
/*END SIG STUFF-----------------------------------------------*/

  /*Loop until user exits shell*/
  while(strcmp("no", stop) == 0){
    backGroundCheck = 0;
    int arrayCounter = 0; /*used to help tell how long my array is*/
    iFile = NULL;
    oFile = NULL;

    printf(": ");
    fflush(stdout); /*flush standard in buffer, as recommended from assignment*/
    fgets(uInput,512,stdin); /*get user input from stdin, store into uInput*/
    char *token = strtok(uInput, " "); /*seperate by spaces. Tokenize*/
    strcpy(tempSTR, "");
    while (token != NULL) {
          sscanf(token, "%s", tempSTR); /*turn the word from bottom of while loop into tempSTR*/

          if(strcmp(tempSTR, "<")==0){
            token = strtok(NULL, " ");
            sscanf(token, "%s", tempSTR);
            iFile = strdup(tempSTR);
          }
          else if(strcmp(tempSTR, ">")==0){
            token = strtok(NULL, " ");
            sscanf(token, "%s", tempSTR);
            oFile = strdup(tempSTR);
          }
          else if(strcmp(tempSTR, "$$")==0){
            sprintf(tempSTR, "%d", getpid());
            inputArray[arrayCounter] = strdup(replaceDollarSigns(tempSTR, "$$", getpid()));
            arrayCounter++;
          }
          else{
            inputArray[arrayCounter] = strdup(tempSTR);
            arrayCounter++;
          }

          token = strtok(NULL, " "); /*get next word from input*/
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
      backGroundCheck = 1;
    }




    interpretUserCommand(userCommand, inputArray, arrayCounter);






  } /* end of big while loop*/





return 0;
}

char replaceDollarSignsHelper(char *string, char *dollarSigns){
  int pid = getpid();
  char pidStringForm[2048];
  sprintf(pidStringForm, "%d", pid);

  char *outputResult;
  int i=0;
  int count = 0;
  int pidStringFormLen = strlen(pidStringForm);
  int oldPidLen = strlen(dollarSigns);

  for(i=0; string[i]!='\0'; i++){
    if(strstr(&string[i], dollarSigns) == &string[i]){
      count++;
      i += oldPidLen - 1;
    }
  }

  outputResult = (char *)malloc(i+count * (pidStringFormLen - oldPidLen)+1);
  i=0;

  while(*string){
    if(strstr(string, oldPidLen) == string){
      strcpy(&outputResult[i], pidStringForm);
      i+=pidStringFormLen;
      string+=oldPidLen;

    }
    else{
      outputResult[i++] = *string++;
    }
  }
  outputResult[i]= "\0";

  return outputResult;

}

void catchCTRLC(int signo){
  //char *message = " Caught SIGINT/CTRLC!\n";
  //write(STDOUT_FILENO, message, 38);
  fflush(stdout);

}

void catchCTRLZ(int sign){
  if(allowBG == 1){
    allowBG = 0;
    write(1, "\nentering foreground-only mode (& is now ignored)\n", 50);
    testCommand = 0;
  }
  else{
    allowBG = 1;
    write(1, "\nExiting foreground-only mode\n", 30);
    testCommand = 0;
  }

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
  else if(strcmp(uIn, "#") == 0 || uIn[0] == '#'){
      ; /*Do nothing*/
  }
  else if(strcmp(uIn, "&") == 0){
      ; /*Do nothing*/
  }
  else if(strcmp(uIn, "") == 0){
      ;
  }
  else if(testCommand == 0){
    testCommand = 1;
  }
  // else if(strcmp(uIn, "echo")==0 && myArray[1] != NULL && strcmp(myArray[1], "$$")== 0){
  //   printf("%d\n", getpid());
  // }



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
        if(backGroundCheck == 0 || allowBG == 0){          //Foreground can be interrupted if need be
            struct sigaction SIGCTRLC_action = {0};
            SIGCTRLC_action.sa_handler = SIG_DFL;          //give power back to ctrl-c signal
            sigaction(SIGINT, &SIGCTRLC_action, NULL);

        }
        if (iFile != NULL){
          fileReadStatus = open(iFile, O_RDONLY);
          if(fileReadStatus == -1){
            printf("Cannot open %s for input\n", iFile);
            fflush(stdout);
            _exit(1);
          }

          if(dup2(fileReadStatus, 0) == -1){
              perror("error in read dup");
              _exit(1);
          }
          close(fileReadStatus);
        }

        if(oFile != NULL){
          fileWriteStatus = open(oFile, O_WRONLY | O_CREAT | O_TRUNC, 0744); /*Taken from stack overflow*/
          if(fileWriteStatus == -1){
            printf("Output fail.  File cannot be opened.\n");
            fflush(stdout);
            _exit(1);
          }

          if(dup2(fileWriteStatus, 1) == -1){
            perror("error in write dup");
            _exit(1);
           }
           close(fileWriteStatus);
        }

        fflush(stdout);
        if(execvp(uIn, myArray)){
          printf("%s: no such file or directory\n", uIn);
          fflush(stdout);

          _Exit(1); /*abort if child fails*/
        }
        break;
      }
      default: {

        if(backGroundCheck == 0 || allowBG == 0){
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


    usleep(100000);
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
