#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//must create a room directory called shoemakd.rooms<PROCESS ID OF ROOMS>
//Must then create 7 different room files

#define MAX_CONS (int)6
#define NUM_OF_ROOMS (int)7
#define MIN_CONS (int)3
#define MAX_NAME_LENGTH (int)9

int randNumGen(int upper, int lower);


/*
Function: CreateDirName.
create a directory with specific name.  So far it is unfilled
*/
void createDirAndFiles(){
  char directoryName[32]; /*create space for directory directoryName*/
  int PID = getpid();
  char bufferForPID[30]; /*make buffer for use on adding pid to end of string*/

  sprintf(bufferForPID, "%d", PID);
  strcpy(directoryName, "shoemakd.rooms.");
  strcat(directoryName, bufferForPID); /* concat PID with directoryName*/
  int check;
  check = mkdir(directoryName,0700);

  if(!check){                         /*check if the directory was created successfully*/
    printf("Directory Created!");
  }
  else{
    printf("Directory failed to create");
    exit(1);                          /*wasn't created successfully, throw exit error*/
  }
  printf("\n"); /* easier readability from terminal */


  /*
  char filePath[100]; /*create space for filepath*/
  /*char filePath2[100];
  char filePath3[100];
  char filePath4[100];
  char filePath5[100];
  char filePath6[100];
  char filePath7[100];

  sprintf(filePath, "./%s/Dungeon_room",directoryName);  /*create file path and use it for the directory and file name we want to create*/
  /*sprintf(filePath2, "./%s/Testing_room",directoryName);
  sprintf(filePath3, "./%s/Drug_room",directoryName);
  sprintf(filePath4, "./%s/Clinic_room",directoryName);
  sprintf(filePath5, "./%s/Theatre_room",directoryName);
  sprintf(filePath6, "./%s/Office_room",directoryName);
  sprintf(filePath7, "./%s/Puzzle_room",directoryName);

  FILE *outFile = fopen(filePath, "w");  /*create the file within directory*/
  /*fclose(outFile);
  FILE *outFile2 = fopen(filePath2, "w");
  fclose(outFile2);
  FILE *outFile3 = fopen(filePath3, "w");
  fclose(outFile3);
  FILE *outFile4 = fopen(filePath4, "w");
  fclose(outFile4);
  FILE *outFile5 = fopen(filePath5, "w");
  fclose(outFile5);
  FILE *outFile6 = fopen(filePath6, "w");
  fclose(outFile6);
  FILE *outFile7 = fopen(filePath7, "w");
  fclose(outFile7);
*/
}


typedef struct {  /*Structure of a room.  Holds the values a room has*/
  char rName[MAX_NAME_LENGTH];
  int numCons;
  char rType; /* s for start, m for middle room, e for end room.*/
  struct roomNode *cons[MAX_CONS];

}roomNode;

void createRoom(){
  roomNode listOfRooms[NUM_OF_ROOMS];  /*create a list of room nodes*/
  strcpy(listOfRooms[0].rName, "Office");
  strcpy(listOfRooms[1].rName, "Dungeon");
  strcpy(listOfRooms[2].rName, "Clinic");    /* put names of the rooms into the list of rooms, for room name*/
  strcpy(listOfRooms[3].rName, "Theatre");
  strcpy(listOfRooms[4].rName, "Puzzle");
  strcpy(listOfRooms[5].rName, "Testing");
  strcpy(listOfRooms[6].rName, "Drug");


  listOfRooms[0].numCons = randNumGen(6,3);
  listOfRooms[1].numCons = randNumGen(6,3);
  listOfRooms[2].numCons = randNumGen(6,3);  /* put random number of connections into each room's cons variable*/
  listOfRooms[3].numCons = randNumGen(6,3);
  listOfRooms[4].numCons = randNumGen(6,3);
  listOfRooms[5].numCons = randNumGen(6,3);
  listOfRooms[6].numCons = randNumGen(6,3);

  int i; /*use for for loop later*/

  int randomRoomNumberEnd;
  int randomRoomNumberStart;

  randomRoomNumberStart = randNumGen(6,0);
  listOfRooms[randomRoomNumberStart].rType = 's';  /* set start room to a random number in the range of room numbers*/
  randomRoomNumberEnd = randNumGen(6,0);

  while(randomRoomNumberStart == randomRoomNumberEnd){  /*Don't want start room to be the same as end room*/
    randomRoomNumberEnd = randNumGen(6,0);
  }
  listOfRooms[randomRoomNumberEnd].rType = 'e'; /* set a unique room to be the end*/

  for(i=0;i<7;i++){
    if(listOfRooms[i].rType != 'e' && listOfRooms[i].rType != 's'){
      listOfRooms[i].rType = 'm'; /* if the rType at this room isnt an end and not a start, put it as a mid room*/

    }
  }
  /*helper to see the rTypes.  DELETE BEFORE FINAL SUBMISSION**************************************/
  for(i=0; i<7; i++){
    printf("%c\n", listOfRooms[i].rType);
  }


}



/******************MAIN AREA**************/
int main(){
  srand(time(0)); /*seed for random number generator function*/


  createDirAndFiles();  /*create unfilled directory.  save name for creating files in that directory*/
  int number = randNumGen(6,3);
  /*printf("%d", number);*/

  printf("\n");

  createRoom();

return 0;
}
/******************END MAIN AREA**************/

/*Random number generator function*/
int randNumGen(int upperBound, int lowerBound){  /* random number generator.  used to randomize amount of connections for roomNode*/

  int num = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
  return num;
}
