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

 

  roomNode *roomList = createRoom();


  roomNode *x = &roomList[0];
  roomNode *y = &roomList[1];

  printf("Node x is named: %s\n", x->rName);  /*This works!!! Now need logic to keep connecting.*/
  printf("Node y is named: %s\n", y->rName);    


  ConnectionValid(x,y); /*Works!*/




  /*int j=0;
  int m=0;
  for(j=0;j<7;j++){
    for(m=0;m<7;m++){
      printf("room %s has connections to: %s",roomList[j]->rName, roomList[j]->cons[m]->rName);
    }
  }
  */

  /*roomNode *x = &roomList[1];
  roomNode *y = &roomList[2];
  connectNodes(x, y);
*/
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
  room1->cons[room1->numCons] = room2; /*connect 2 room nodes, going both ways.*/
  room2->cons[room2->numCons] = room1;
  room1->numCons++;
  room2->numCons++;

  printf("%s now connects to %s\n ", room1->rName, room1->cons[0]->rName);


  

}

/*Helper functon to see if the nodes fill the requirements on number of connections. Returns 1 or 0.  Bool. */
int doesGraphSatisfy(roomNode* listOfRooms){

  int l; /* loop iteration */

  for(l=0;l<NUM_OF_ROOMS;l++){
    if(listOfRooms[l].numCons < MIN_CONS){  /* does the number of connections for each node match the requirements */
      return 0;
    }
  }
  return 1;
}

/*check if the 2 rooms passed in pass all tests to connect them. Return 1 if connection is valid, 0 if not*/
int ConnectionValid(roomNode* room1, roomNode* room2){

  if(room1 == room2){ /*was the same node passed in?? Don't want that*/
    printf("Room1 = Room2 problem");
    return 0;
  }
  if(isNodeFull(room1) == 0 || isNodeFull(room2) == 0){
    printf("node full problem");
    return 0;
  }
  int i;
  for(i=0;i<room1->numCons;i++){ /*check if room1 is already connected to room2*/
    
    if(room1->cons[i] == room2){
      printf("contains problem");
      return 0;
    }
}
  printf("Made it to return 1!");
   return 1;
}

/*return 1 if node can be added to, 0 if node is full*/
int isNodeFull(roomNode* room){
  int num;
  num = room->numCons;

  if (num < MAX_CONS){
    return 1;
  }
  else{
    return 0;
  }


}