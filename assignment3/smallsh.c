
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char stop[5] = "no";

char askUserInput(char *uIn){
  printf(": ");
  scanf("%s", uIn);

  return *uIn;

}

void interpretUserInput(char *uIn){

  if(strcmp(uIn, "Exit")==0){
    strcpy(stop, "yes");
  }
  else if(strcmp(uIn, "cd")==0){
    printf("change directory initiated\n");
  }
  else{
    printf("else\n");
  }

}



int main(){
  char uInput[2048];


  while(strcmp("no", stop) == 0){
    askUserInput(uInput);

    interpretUserInput(uInput);





  }





return 0;
}
