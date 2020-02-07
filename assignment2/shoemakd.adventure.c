#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <regex.h>

#define MAX_CONS (int)6
#define NUM_OF_ROOMS (int)7
#define MIN_CONS (int)3
#define MAX_NAME_LENGTH (int)9
const char DIRECTORYNAME[32];

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

/**********************MAIN AREA*************************************************/
int main(int argc, char *argv[]){

char *newestDir; /*latest directory we found from function */

newestDir = getNewestDir();

roomNode *roomList = initRooms();

buildStructsFromFile(newestDir, roomList);

printRoomsHelper(roomList);

return 0;
}
/**********************END MAIN AREA********************************************/



int findLine(char const* str, char const* substr){

  char* position = strstr(str, substr);
    if(position) {
        return 1;
    } else {
        return 0;
    }
}



void buildStructsFromFile(char* directory, roomNode* roomList){

  struct dirent *de;  // Pointer for directory entry

    char *dirPath[100];
    char *filePath[100];
    char *storeFileLine[100];
    strcpy(dirPath, "./");
    strcat(dirPath, directory);
    strcat(dirPath, "/");

    DIR *dir = opendir(dirPath); /*open specified path we made*/

    if (dir == NULL)  // if the directory doesn't exist, throw error
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
