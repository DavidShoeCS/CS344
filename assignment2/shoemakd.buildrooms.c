#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//must create a room directory called shoemakd.rooms<PROCESS ID OF ROOMS>
//Must then create 7 different room files





/*
Function: CreateDirName.
create a directory with specific name.  So far it is unfilled
*/
void createDir(){
  char directoryName[32]; //create space for directory directoryName
  int pid = getpid();
  char bufferForPID[30]; //make buffer for use on adding pid to end of string

  sprintf(bufferForPID, "%d", pid);
  strcpy(directoryName, "shoemakd.rooms.");
  strcat(directoryName, bufferForPID); //concat PID with directoryName
  int check;
  check = mkdir(directoryName,0700);

  if(!check){                         //check if the directory was created successfully
    printf("Directory Created!");
  }
  else{
    printf("Directory failed to create");
    exit(1);                          //wasn't created successfully, throw exit error
  }

  printf("\n");
}





int main(){

  createDir();     //create unfilled directory


return 0;
}
