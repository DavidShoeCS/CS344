#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <regex.h>

#include <time.h>
#include <pthread.h>



#define MAX_CONS (int)6
#define NUM_OF_ROOMS (int)7
#define MIN_CONS (int)3
#define MAX_NAME_LENGTH (int)9
const char DIRECTORYNAME[32];
pthread_mutex_t myMutex;

typedef struct roomNode{  /*Structure of a room.  Holds the values a room has*/
  char rName[MAX_NAME_LENGTH];
  int numCons;
  char rType; /* s for start, m for middle room, e for end room.*/
  struct roomNode *cons[MAX_CONS];

}roomNode;

roomNode* initRooms();

char *getNewestDir();
int findLine(char const* str, char const* substr);
void printRoomsHelper(roomNode *listOfRooms);
roomNode* findNode(roomNode *roomList, char *roomName);

void buildStructsFromFile(char *directory, roomNode* roomList);


void playGame(roomNode* roomList);

roomNode* findStartRoom(roomNode* roomList);

int isValidConnection(roomNode* currRoom, char *sUI);
void printVisitedNodes(roomNode *roomList, int counter);

void threadProg();

void *printTimeToFile();

void printTimeToUser();


/**********************MAIN AREA*************************************************/
int main(int argc, char *argv[]){

  char *newestDir; /*latest directory we found from function */
  newestDir = getNewestDir();

  roomNode *roomList = initRooms();

  buildStructsFromFile(newestDir, roomList);

  playGame(roomList);


  free(roomList);
return 0;
}
/**********************END MAIN AREA********************************************/


/*function to print the time into a file, which will then be read from to show the user*/
void *printTimeToFile(){
  char storage[100]; /*buffer area to be used in strftime*/
  char format[100];
  strcpy(format, "%H:%M:%S on %d, %m, %Y"); /*Make a format of time to be presented to user*/

  FILE* timeFile;
  timeFile = fopen("currentTime.txt", "w+"); /*open file to write*/

  struct tm *timeStruct; /*time structure to use strftime function*/
  time_t currTime = time(0);

  timeStruct = gmtime(&currTime);

  strftime (storage, 100, format, timeStruct);
  fputs(storage, timeFile); /*put our formatted time into the file to be read*/
  fclose(timeFile);

}

/*function to print time to the screen at user request*/
void printTimeToUser(){
  char storage[100]; /*variable to store what we read from the file*/
  FILE *timeFile;
  timeFile = fopen("currentTime.txt", "r"); /*read from this file that we created earlier*/

  fgets(storage, 100, timeFile); /*store line into storage*/
  printf("\n");
  printf("%s\n", storage); /*print contents of storage to user*/

  fclose(timeFile); /*done with file, close access*/

}


/*thread the print time program*/
void threadProg(){
  pthread_t threadV;

  pthread_mutex_init(&myMutex, NULL);
  pthread_mutex_lock(&myMutex);

  int threadID;
  threadID = pthread_create(&threadV, NULL, printTimeToFile, NULL);

  pthread_mutex_unlock(&myMutex);
  pthread_mutex_destroy(&myMutex);
  usleep(5000);

}




void playGame(roomNode* roomList){
  int buff = 100;
  int loopC;
  roomNode *currRoom;
  roomNode* myStartRoom;
  roomNode* listOfVisitedNodes = malloc(NUM_OF_ROOMS * sizeof(roomNode)); /*create a list that will hold the visited nodes in order*/
  int visitedCounter=0;
  char askPrompt='n';
  char exitPrompt='n';
  char userInput[100];
  char snippedUserInput[100];
  /***************************************************/

  myStartRoom = findStartRoom(roomList);
  currRoom = myStartRoom;
  printf("\n"); /*better readability at start of the game*/
  while(exitPrompt == 'n'){ /*while we dont want to exit*/

    while(askPrompt == 'n'){ /*keep asking the user for input until we get a correct response*/
      printf("CURRENT LOCATION: %s\n", currRoom->rName);
      printf("POSSIBLE CONNECTIONS: ");
      printf("%s", currRoom->cons[0]->rName);
      for(loopC=1;loopC<currRoom->numCons;loopC++){
        printf(", %s", currRoom->cons[loopC]->rName);
      }
      printf(".\n");
      printf("WHERE TO? >");
      fgets(userInput, buff, stdin); /*this appends a new line at the end of user's input*/
      sscanf(userInput, "%s", snippedUserInput);/*saves user input into snipped version, getting rid of the new line at the end. Found this method on google.*/


      if(strcmp(snippedUserInput, "time") == 0){
        threadProg();
        printTimeToUser();
        printf("\n");
      }
      else if(isValidConnection(currRoom, snippedUserInput)==0){
        printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        printf("\n");
      }
      else{
        printf("\n");
        askPrompt = 'y';
        currRoom = findNode(roomList, snippedUserInput);
        listOfVisitedNodes[visitedCounter] = *currRoom;
        visitedCounter++;

      }

    }
    if (currRoom->rType == 'E'){
      printf("GOOD JOB YOU MADE IT TO THE END\n");
      printVisitedNodes(listOfVisitedNodes, visitedCounter);
      exitPrompt = 'Y';

    }
    else
    {
      askPrompt = 'n';
    }


  }


}

void printVisitedNodes(roomNode *roomList, int counter){
  int i;
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", counter);
  for(i=0; i<counter;i++){
    printf("%s", roomList[i].rName);
    printf("\n");
  }

}

int isValidConnection(roomNode* currRoom, char *sUserInput){
  int i;
  for(i=0; i<currRoom->numCons;i++){
      if(strcmp(currRoom->cons[i]->rName, sUserInput)==0){
        return 1; /*If the user input is indeed a connected node to the room we are currently at, return 1.*/
      }
  }
  return 0;

}

 /*helper function to get the  start room*/
roomNode* findStartRoom(roomNode* roomList){

  int j;
  for(j=0; j < NUM_OF_ROOMS; j++){ /*loop through my list of rooms until we find the start room, then return it for later use.*/
    if(roomList[j].rType == 'S'){
      return &roomList[j];
    }

  }

}


int findLine(char const* str, char const* substr){

  char* position = strstr(str, substr);
    if(position) {
        return 1;
    } else {
        return 0;
    }
}



void buildStructsFromFile(char* directory, roomNode* roomList){

  struct dirent *de;  /*Pointer for directory entry*/

    char *dirPath[100];
    char *filePath[100];
    char *storeFileLine[100];
    strcpy(dirPath, "./");
    strcat(dirPath, directory);
    strcat(dirPath, "/");

    DIR *dir = opendir(dirPath); /*open specified path we made*/

    if (dir == NULL)  /* if the directory doesn't exist, throw error*/
    {
        printf("Could not open current directory" );
    }

    FILE *fp;
    char line[1000];
    char strStorage[100];
    int i=0;
    int conCounter=0;
    while ((de = readdir(dir)) != NULL) /*read every file in our directory*/
          if(de->d_name[0] != '.'){
            strcpy(filePath, "\0");
            strcat(filePath, "./");
            strcat(filePath, directory);
            strcat(filePath, "/");
            strcat(filePath, de->d_name); /*keep catting on filePath to get the right file directory*/

            fp = fopen(filePath, "r");
            conCounter=0;
            while (fgets(line, 1000, fp) != NULL) {

                sscanf(line, "%*[^:]: %[^]%*[^\n]", strStorage); /* from google.  takes a line from our file, regex it to remove everthing before ":", then store the new line into strStorage.*/

                if(findLine(line, "ROOM NAME") == 1){
                  strcpy(roomList[i].rName, strStorage);
                }
                if(findLine(line, "ROOM TYPE") == 1){
                  roomList[i].rType = strStorage[0];
                }
                if(findLine(line, "CONNECTION") == 1){ /*set the amount of connections of each room*/
                  conCounter++;
                  roomList[i].numCons = conCounter;
                }
            }
            i++;
            fclose(fp);
          }

    closedir(dir);


    /*************************************************************************************/
    dir = opendir(dirPath); /*This section will re open all the files, and fill in connections*/
    if (dir == NULL)  /* if the directory doesn't exist, throw error*/
    {
        printf("Could not open current directory" );
    }


    i=0;
    conCounter=0;
    while ((de = readdir(dir)) != NULL) /*read every file in our directory*/
          if(de->d_name[0] != '.'){
            strcpy(filePath, "\0");
            strcat(filePath, "./");
            strcat(filePath, directory);
            strcat(filePath, "/");
            strcat(filePath, de->d_name); /*keep catting on filePath to get the right file directory*/

            fp = fopen(filePath, "r");
            conCounter=0;
            while (fgets(line, 1000, fp) != NULL) {

                sscanf(line, "%*[^:]: %[^]%*[^\n]", strStorage); /* from google.  takes a line from our file, regex it to remove everthing before ":", then store the new line into strStorage.*/
                if(findLine(line, "CONNECTION") == 1){

                  roomList[i].cons[conCounter] = findNode(roomList, strStorage);

                  conCounter++; /*Keep track of what room we are looking at*/
          }

        }
        i++;
        fclose(fp);
        }
      closedir(dir);
}


/*Function to get the newest directory (shoemakd.rooms.*) made in current directory.*/
char *getNewestDir(){
  struct dirent *de;  /* struct dirent found from geeksforgeeks*/
  struct stat buffRoom;
  char *latestDir;

  DIR *dir = opendir("."); /*open current directory and peek inside.*/
  long latestModTime = 0; /*m_time returns long, so use long here*/
  while ((de = readdir(dir)) != NULL){
    stat(de->d_name, &buffRoom);
    if(de->d_name[14] == '.' && de->d_name[20] != 'c' && buffRoom.st_mtime >= latestModTime){
        latestModTime = buffRoom.st_mtime;
        latestDir = de->d_name;  /*when the if statement passes, set the newest directory to latestDir*/
        }
      }
      /*printf("%s\n", latestDir);*/
      return latestDir;

  closedir(dir);
}

roomNode* initRooms(){
  roomNode* listOfRooms = malloc(NUM_OF_ROOMS * sizeof(roomNode));  /*create a list of room nodes*/
  int i;
  for(i=0; i< NUM_OF_ROOMS; i++){
    listOfRooms[i].numCons=0;
  }

  return listOfRooms;

}

void printRoomsHelper(roomNode *listOfRooms){

  int j;
  int i;
  for(i=0; i < NUM_OF_ROOMS; i++){

  printf("\nRoom: %s's number of Cons: %d", listOfRooms[i].rName, listOfRooms[i].numCons);
  printf("\nRoom: %s(%c) connects to: ", listOfRooms[i].rName, listOfRooms[i].rType);
  for(j=0; j<listOfRooms[i].numCons; j++){
    printf("%s, ", listOfRooms[i].cons[j]->rName);
  }
  printf("\n");

  }

}

roomNode* findNode(roomNode *roomList, char *roomName){
  int m;
  for(m=0; m<NUM_OF_ROOMS; m++){
    if(strcmp(roomList[m].rName, roomName)==0){
      return &roomList[m];
  }
  }
  return NULL;

}
