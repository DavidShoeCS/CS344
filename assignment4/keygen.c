#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[]) {
  srand(time(0));

  char *listOfChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "; //will peak into this to get the characters we need for encoding

  int argNumber; /*store first argument here*/

  if(argc>1){
    argNumber = atoi(argv[1]); /*using atoi() function because this takes care of if a user enters non number*/

    int randInt = rand();
    int randIntMod = randInt%27; //mod by 27 so we can peak into our list of characters

    int c;
    for(c=0; c<argNumber; c++){ //loop how every many times the user specified (argNumber)
      randInt = rand();
      randIntMod = randInt%27; //mod by 27
      printf("%c",listOfChars[randIntMod]); //print off character
    }
    printf("\n"); //print new line at the end, as required by assignment
  }
  else{
    //print to standard error if user doesn't specify number for argument
    fprintf(stderr, "ERROR: this function requires an argument to use.\n");
    exit(0);
  }

  return 0;
}
