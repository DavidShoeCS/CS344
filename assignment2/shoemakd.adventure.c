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



char *getNewestDir();

void buildStructsFromFile(char *directory);


int main(int argc, char *argv[]){

char *newestDir; /*latest directory we found from function */

newestDir = getNewestDir();
/*printf("%s\n", newestDir);*/

buildStructsFromFile(newestDir);


return 0;
}


void buildStructsFromFile(char *directory){
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

    while ((de = readdir(dir)) != NULL) /*read every file in our directory*/
          if(de->d_name[0] != '.'){
            strcpy(filePath, "\0");
            strcat(filePath, "./");
            strcat(filePath, directory);
            strcat(filePath, "/");
            strcat(filePath, de->d_name); /*keep catting on filePath to get the right file directory*/
            printf("-file path: %s\n", filePath);


            fp = fopen(filePath, "r");
            while (fgets(line, 1000, fp) != NULL) {
                sscanf(line, "%*[^:]: %[^]%*[^\n]", strStorage); /* from google.  takes a line from our file, regex it to remove everthing before ":", then store the new line into strStorage.*/
                printf("%s\n", strStorage);
                /*NEXT STEP: print to a struct*/
            }

            printf("\n");
            fclose(fp);



            /*printf("%s\n", de->d_name);*/


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
