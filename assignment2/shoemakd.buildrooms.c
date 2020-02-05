#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_CONS (int)6
#define NUM_OF_ROOMS (int)7
#define MIN_CONS (int)3
#define MAX_NAME_LENGTH (int)9



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


typedef struct roomNode{  /*Structure of a room.  Holds the values a room has*/
  char rName[MAX_NAME_LENGTH];
  int numCons;
  char rType; /* s for start, m for middle room, e for end room.*/
  struct roomNode *cons[MAX_CONS];

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

void connectNodes(roomNode* room1, roomNode* room2);

int ConnectionValid(roomNode* room1, roomNode* room2 );
int isNodeFull(roomNode* room);

int doesGraphSatisfy(roomNode* listOfRooms);

int randNumGen(int upper, int lower);


/******************MAIN AREA**************/
int main(int argc, char *argv[]){

  /*createDirAndFiles();*/  /*create unfilled directory.  save name for creating files in that directory*/

  printf("\n");



  roomNode *roomList = createRoom(); /*Create a list of rooms*/


  roomNode *room1; /*initalize two rooms that we will use to verify and connect*/
  roomNode *room2;


  printf("\n------\n");


  while(doesGraphSatisfy(roomList) == 0){  /*While each node doesn't have between 3 and 6 rooms in its list*/
    int r1Index = randNumGen(6,0);
    int r2Index = randNumGen(6,0);

    while(r1Index == r2Index){
      r1Index = randNumGen(6,0);
      r2Index = randNumGen(6,0);  /* keep going through loop until the rooms will be different.*/
    }

    room1 = &roomList[r1Index];
    room2 = &roomList[r2Index];

    while(isNodeFull(room1)==0 || isNodeFull(room2)==0){ /*If one of the nodes if full, keep trying with random rooms*/
      r1Index = randNumGen(6,0);
      r2Index = randNumGen(6,0);

      room1 = &roomList[r1Index];
      room2 = &roomList[r2Index];   /*Set room1 and room2 nodes to a valid node in our list*/
    }

    if(ConnectionValid(room1, room2) == 1){ /*If the connection is valid, connect the nodes.*/
      connectNodes(room1,room2);
    }

  }



printf("\n-------------------\n");



int j;                        /*Helper area to print a node and its connections*/
int i;
for(i=0; i < NUM_OF_ROOMS; i++){

  printf("\nRoom: %s connects to: ", roomList[i].rName);
  for(j=0; j<roomList[i].numCons; j++){
    printf("%s, ", roomList[i].cons[j]->rName);
  }
printf("\n");

}


return 0;
}
/******************END MAIN AREA**************/

/*Random number generator function*/
int randNumGen(int upperBound, int lowerBound){  /* random number generator.  used to randomize amount of connections for roomNode*/

  int num = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
  return num;
}

/*connect two given nodes together*/
void connectNodes(roomNode* room1, roomNode* room2){


  /*ConnectionValid(room1, room2);*/
  room1->cons[room1->numCons++] = room2; /*connect 2 room nodes, going both ways.*/
  room2->cons[room2->numCons++] = room1;


  printf("\n%s now connects to %s\n", room1->rName, room1->cons[0]->rName);




}

/*Helper functon to see if the nodes fill the requirements on number of connections. Returns 1 or 0.  Bool. */
int doesGraphSatisfy(roomNode* listOfRooms){

  int l; /* loop iteration */

  for(l=0;l<NUM_OF_ROOMS;l++){
    if(listOfRooms[l].numCons < MIN_CONS){  /* does the number of connections for each node match the requirements */
      printf("\nDoes not satisfy");
      return 0;
    }
  }
  printf("\nDoes satisfy.");
  return 1;
}

/*check if the 2 rooms passed in pass all tests to connect them. Return 1 if connection is valid, 0 if not*/
int ConnectionValid(roomNode* room1, roomNode* room2){

  if(room1 == room2){ /*was the same node passed in?? Don't want that*/
    printf("Room1 = Room2 problem");
    return 0;
  }
  if(isNodeFull(room1) == 0 || isNodeFull(room2) == 0){
    printf("\nnode full problem\n");
    return 0;
  }
  int i;
  for(i=0;i<room1->numCons;i++){ /*check if room1 is already connected to room2*/

    if(room1->cons[i] == room2){
      printf("\ncontains problem\n");
      return 0;
    }
}
  printf("\nMade it to return 1!\n");
   return 1;
}

/*return 1 if node can be added to, 0 if node is full*/
int isNodeFull(roomNode* room){
  int num;
  num = room->numCons;

  if (num < MAX_CONS){
    return 1; /*connection is valid*/
  }
  else{
    return 0;
  }


}
