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
  char buffer[256];
  char buffer2[256];
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
  memset(buffer, '\0', 256);
  charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
  if (charsRead < 0) error("ERROR reading from socket");
  printf("\nSERVER: I received this from the client: \"%s\"\n", buffer);

  char *listOfChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  char *myMessageB = strdup(readMessageFile(buffer)); /*read from file and store into variable that we will use later*/


  // Send a Success message back to the client
  charsRead = send(establishedConnectionFD, "I am the server, and I got your file", 39, 0); // Send success back
  if (charsRead < 0) error("ERROR writing to socket");


  // Get the message from the client and display it
  memset(buffer2, '\0', 256);
  charsRead = recv(establishedConnectionFD, buffer2, 255, 0); // Read the client's message from the socket
  if (charsRead < 0) error("ERROR reading from socket");
  printf("SERVER: I received this from the client: \"%s\"\n", buffer2);


  //strdup so we dont point to something that will change
  char *myKey = strdup(readMessageFile(buffer2)); /*read from file and store into variable that we will use later*/


  // Send a Success message back to the client
  charsRead = send(establishedConnectionFD, "I am the server, and I got your key", 39, 0); // Send success back
  if (charsRead < 0) error("ERROR writing to socket");


  encryptMessage(listOfChars, myKey, myMessageB);


  close(establishedConnectionFD); // Close the existing socket which is connected to the client
  close(listenSocketFD); // Close the listening socket

  return 0;
}

char *encryptMessage(char listOfChars[], char *key, char *message){
  char *encMessage;
  int mLength = strlen(message);
  int keyLength = strlen(key);
  int messageNumArray[mLength+1];
  int keyNumArray[mLength+1];
  int encodedMessageNumArray[mLength+1];
  int i, j;

  if(keyLength < mLength){
    printf("\nERROR KEY LENGTH TOO SHORT\n");
    exit(0);
  }

  for(i=0; i<mLength; i++){
    for(j=0; j<strlen(listOfChars); j++){
      if(message[i] == listOfChars[j]){
        messageNumArray[i] = j;
      }
    }
  }
  printf("----------\n");

  for(i=0;i<mLength;i++){
    for(j=0;j<strlen(listOfChars);j++){
      if(key[i]==listOfChars[j]){
        keyNumArray[i] = j;
      }
    }
  }
  printf("\n");
  printf("test array here:\n");
  for(i=0;i<mLength;i++){
    printf("message[%d]->%d\n", i, messageNumArray[i]);
    printf("key[%d]->%d\n", i, keyNumArray[i]);
    encodedMessageNumArray[i] = (messageNumArray[i]+keyNumArray[i])%26;
  }

  printf("\n------testing encoded num array-----\n");
  for(i=0;i<mLength;i++){
    printf("encodedMessage[%d]->%d\n", i, encodedMessageNumArray[i]);
  }
  printf("\n------turning numbers into letters test-----\n");





  //strcpy(encMessage,"hi");
  return encMessage;
}



//read and put the message from a file into a variable
char *readMessageFile(char *myFile){

  char str[100000];//intitalize variable

  FILE *file = fopen(myFile, "r+"); //open the file we want

  if(file == NULL){
    perror("error opening file"); //fails
    return -1;
  }
  else{
    fgets(str, 100000, file); //read file line

  }
  fclose(file);
  return str;

}
