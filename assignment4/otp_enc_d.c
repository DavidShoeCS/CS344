#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues


char *readMessageFile(char *myFile);
char *encryptMessage(char listOfChars[], char key[], char message[]);

int main(int argc, char const *argv[]) {

  int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
  socklen_t sizeOfClientInfo;
  char buffer[1000000];
  char buffer2[1000000];
  struct sockaddr_in serverAddress, clientAddress;

  if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

  // Set up the address struct for this process (the server)
  memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
  serverAddress.sin_family = AF_INET; // Create a network-capable socket
  serverAddress.sin_port = htons(portNumber); // Store the port number
  serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

  // Set up the socket
  listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
  if (listenSocketFD < 0) error("ERROR opening socket");

  // Enable the socket to begin listening
  if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
    error("ERROR on binding");
  listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

  // Accept a connection, blocking if one is not available until one connects
  sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
  establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
  if (establishedConnectionFD < 0) error("ERROR on accept");

  // Get the message from the client and display it
  //get file name
  bzero(buffer, 100000);
  charsRead = recv(establishedConnectionFD, buffer, 99000, 0); // Read the client's message from the socket
  if (charsRead < 0) error("ERROR reading from socket");

  char *listOfChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  char *myMessageB = strdup(readMessageFile(buffer)); /*read from file and store into variable that we will use later*/


  // Send a Success message back to the client
  charsRead = send(establishedConnectionFD, "I am the server, and I got your file", 39, 0); // Send success back
  if (charsRead < 0) error("ERROR writing to socket");


  // Get the message from the client and display it
  bzero(buffer2, 100000);
  charsRead = recv(establishedConnectionFD, buffer2, 99000, 0); // Read the client's message from the socket
  if (charsRead < 0) error("ERROR reading from socket");


  //strdup so we dont point to something that will change
  char *myKey = strdup(readMessageFile(buffer2)); /*read from file and store into variable that we will use later*/


  printf("length of encrypted message before sending->%d\n", strlen(encryptMessage(listOfChars, myKey, myMessageB)));

  //encryptedMessageToSend = encryptMessage(listOfChars, myKey, myMessageB);

  // Send a Success message back to the client
  // charsRead = send(establishedConnectionFD, encryptMessage(listOfChars, myKey, myMessageB), strlen(encryptMessage(listOfChars, myKey, myMessageB)), 0); // Send success back
  // if (charsRead < 0) error("ERROR writing to socket");
  int check2;
  check2 = send(establishedConnectionFD, encryptMessage(listOfChars, myKey, myMessageB), strlen(myMessageB), 0);
  printf( "CHECK DAEMON HERE->%d\n ", check2);
  close(establishedConnectionFD); // Close the existing socket which is connected to the client
  close(listenSocketFD); // Close the listening socket

  return 0;
}

char *encryptMessage(char listOfChars[], char *key, char *message){
  int mLength = strlen(message);
  char* encMessage;
  int keyLength = strlen(key);
  int messageNumArray[mLength+1];
  int keyNumArray[mLength+1];
  int encodedMessageNumArray[mLength+1];
  int i, j;


  encMessage = malloc(1000000 * sizeof(char));
  if(keyLength < mLength){
    fprintf(stderr,"\nERROR KEY LENGTH TOO SHORT\n");
    exit(0);
  }

  for(i=0; i<mLength; i++){
    for(j=0; j<strlen(listOfChars); j++){
      if(message[i] == listOfChars[j]){
        messageNumArray[i] = j;
      }
    }
  }


  for(i=0;i<mLength;i++){
    for(j=0;j<strlen(listOfChars);j++){
      if(key[i]==listOfChars[j]){
        keyNumArray[i] = j;
      }
    }
  }

  for(i=0;i<mLength;i++){
    encodedMessageNumArray[i] = (messageNumArray[i]+keyNumArray[i])%27;
  }

  for(i=0; i < mLength-1; i++){
    encMessage[i] = listOfChars[encodedMessageNumArray[i]];
  }
  encMessage[i] = '\0';


  return encMessage;
}



//read and put the message from a file into a variable
char *readMessageFile(char *myFile){

  char* str;//intitalize variable

  str = malloc(100000 * sizeof(char));

  FILE *file = fopen(myFile, "r+"); //open the file we want

  if(file == NULL){
    perror("error opening file"); //fails
    exit(0);
  }
  else{
    fgets(str, 100000, file); //read file line

  }
  fclose(file);
  return str;

}
