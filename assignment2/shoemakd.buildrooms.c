#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*must create a room directory called shoemakd.rooms<PROCESS ID OF ROOMS>*/
/*Must then create 7 different room files*/

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


}


typedef struct {  /*Structure of a room.  Holds the values a room has*/
  char rName[MAX_NAME_LENGTH];
  int numCons;
  char rType; /* s for start, m for middle room, e for end room.*/
  int cons[MAX_CONS];

}roomNode;

roomNode* createRoom(){
  srand(time(0)); /*seed for random number generator function*/
  roomNode* listOfRooms = malloc(NUM_OF_ROOMS * sizeof(roomNode));  /*create a list of room nodes*/
  strcpy(listOfRooms[0].rName, "Office");
  strcpy(listOfRooms[1].rName, "Dungeon");
  strcpy(listOfRooms[2].rName, "Clinic");    /* put names of the rooms into the list of rooms, for room name*/
  strcpy(listOfRooms[3].rName, "Theatre");
  strcpy(listOfRooms[4].rName, "Puzzle");
  strcpy(listOfRooms[5].rName, "Testing");
  strcpy(listOfRooms[6].rName, "Drug");


  listOfRooms[0].numCons = 0;
  listOfRooms[1].numCons = 0;
  listOfRooms[2].numCons = 0;  /*initially each node will have no connections.  Will build up later*/
  listOfRooms[3].numCons = 0;
  listOfRooms[4].numCons = 0;
  listOfRooms[5].numCons = 0;
  listOfRooms[6].numCons = 0;

  int i; /*use for for loop later*/

  int randomRoomNumberEnd;
  int randomRoomNumberStart;

  randomRoomNumberStart = randNumGen(6,1);

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

  return listOfRooms; /*return array of rooms*/
}

roomNode* connectNodes(roomNode *listOfRooms);
int ConnectionValid(roomNode room, int rIndex);

int isGraphComplete(roomNode* listOfRooms);


/******************MAIN AREA**************/
int main(){

  createDirAndFiles();  /*create unfilled directory.  save name for creating files in that directory*/

  printf("\n");

  //createRoom();

  roomNode *roomList = createRoom();
  connectNodes(roomList);


return 0;
}
/******************END MAIN AREA**************/

/*Random number generator function*/
int randNumGen(int upperBound, int lowerBound){  /* random number generator.  used to randomize amount of connections for roomNode*/

  int num = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
  return num;
}

/*connect the nodes until they satisfty the requirement of having 3-6 connections per node*/
roomNode* connectNodes(roomNode *listOfRooms){
  while(doesGraphSatisfy(listOfRooms) == 0){
    int k;
    for(k=0;k<=MAX_CONS; k++){
      int randomIndex = randNumGen(6,0); /*while the connection isnt valid, make a new random index to find a random room to connect to*/
      while( ConnectionValid(listOfRooms[k],randomIndex) == 1 || listOfRooms[randomIndex].numCons >= MAX_CONS || randomIndex == k){


        if (listOfRooms[k].numCons >= MAX_CONS){
            break;
          }
        randomIndex = randNumGen(6,0);
      }
        printf("[%d] Connecting %s with %s\n", k, listOfRooms[k].rName, listOfRooms[randomIndex].rName);

        listOfRooms[k].cons[listOfRooms[k].numCons] = randomIndex;
        listOfRooms[randomIndex].cons[listOfRooms[randomIndex].numCons] = k;
        listOfRooms[k].numCons++;
        listOfRooms[randomIndex].numCons++;

        if(doesGraphSatisfy(listOfRooms)==0){
          printf("break here\n");
          break;
        }

    }



}
int i;
int j;
for(i=0;i<NUM_OF_ROOMS;i++){
   printf("Name: %s\nType: %c\nnoC: %d\n", listOfRooms[i].rName, listOfRooms[i].rType, listOfRooms[i].numCons);
   printf("connections: ");
   for (j = 0; j < listOfRooms[i].numCons; j++) {
          printf("%s, ", listOfRooms[listOfRooms[i].cons[j]].rName);
      }
      printf("\n\n");
  }

}

/*Helper functon to see if the nodes fill the requirements on number of connections. Returns 1 or 0.  Bool. */
int doesGraphSatisfy(roomNode* listOfRooms){

  int l; /* loop iteration */

  for(l=0;l<NUM_OF_ROOMS;l++){
    if(listOfRooms[l].numCons < 3){  /* does the number of connections for each node match the requirements */
      return 0;
    }
    return 1;
  }
}

/*check if the room we want is already connected to the random indexed room.  1 if it is and 0 if it is not. bool!*/
int ConnectionValid(roomNode room, int rIndex){
  int m;
  for(m=0; m <room.numCons; m++){
    if(room.cons[m]==rIndex){
      return 1;
    }
    return 0;
  }


}
