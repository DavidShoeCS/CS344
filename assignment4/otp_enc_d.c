#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

char *encryptMessage(char listOfChars[], char key[], char message[]);

int main(int argc, char const *argv[]) {

  int listenSocketFD, establishedConnectionFD, portNumber, charsRead;//given stuff.  Initializing
  socklen_t sizeOfClientInfo;
  int i;
  int j;
  char buffer[1000000]; //set buffers to large for sending and receiving data through sockets
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

  while(1){ //keeps the daemon available after port/socket was used

    // Accept a connection, blocking if one is not available until one connects
    sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
    establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
    if (establishedConnectionFD < 0) error("ERROR on accept");

    pid_t newPID; //create PID
    newPID = fork();
    if(newPID < 0){
      fprintf(stderr, "Fork error");
      exit(1);
    }

      if(newPID==0){ //if forking works, do stuff

    // Get the message from the client and display it
      //get message
      bzero(buffer, 100000);
      charsRead = recv(establishedConnectionFD, buffer, 99000, 0); // Read the client's message from the socket
      if (charsRead < 0) error("ERROR reading from socket");

      char *listOfChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

      char *myMessageB = strdup(buffer); /*read from file and store into variable that we will use later*/


      // Send a Success message back to the client
      charsRead = send(establishedConnectionFD, "I am the server, and I got your file", 39, 0); // Send success back
      if (charsRead < 0) error("ERROR writing to socket");

      // Get the message from the client and display it
      bzero(buffer2, 100000);
      charsRead = recv(establishedConnectionFD, buffer2, 99000, 0); // Read the client's message from the socket
      if (charsRead < 0) error("ERROR reading from socket");


      //strdup so we dont point to something that will change
      char *myKey = strdup(buffer2); /*store message into variable that we will use later*/

      // Send a Success message back to the client
      int check2;
      check2 = send(establishedConnectionFD, encryptMessage(listOfChars, myKey, myMessageB), strlen(myMessageB), 0);

    }
    close(establishedConnectionFD); // Close the existing socket which is connected to the client
  }

  close(listenSocketFD); // Close the listening socket

  return 0;
}

char *encryptMessage(char listOfChars[], char *key, char *message){
  int mLength = strlen(message); //length of the message to use
  char* encMessage;
  int keyLength = strlen(key);//get the length of our key array we passed in to use for looping
  int messageNumArray[mLength+1]; //set number arrays that we will use to store indexes for listOfChars
  int keyNumArray[mLength+1];
  int encodedMessageNumArray[mLength+1];
  int i, j, countHelp=0;

  //this loop tells us if we found bad characters in our plaintext file
  for(i=0; i<mLength; i++){
    for(j=0; j<strlen(listOfChars); j++){
      if(message[i] == listOfChars[j]){ //if the character is a good character, update countHelp
        countHelp++;
      }
    }
  }

  //if the counter doesn't match our message length, we know one or more of the characters were bad
  if(countHelp != mLength-1){ //if did not find any good characters, throw error to user
    error("ERROR: bad character found in plaintext");
  }

  encMessage = malloc(1000000 * sizeof(char)); //allocate space for encrypted message
  if(keyLength < mLength){ //if the message's length is bigger than the length of the key, give out error
    fprintf(stderr,"ERROR KEY LENGTH TOO SHORT\n");
    exit(1);
  }

  //loop through the message and see if it matches something in listOfChars
  for(i=0; i<mLength; i++){
    for(j=0; j<strlen(listOfChars); j++){
      if(message[i] == listOfChars[j]){ //if it matches, yay set save its index into a num array to be used later
        messageNumArray[i] = j;
      }
    }
  }

//loop through the key and see if it matches something in listOfChars
  for(i=0;i<mLength;i++){
    for(j=0;j<strlen(listOfChars);j++){
      if(key[i]==listOfChars[j]){//if it matches, yay save its index into a num array to be used later
        keyNumArray[i] = j;
      }
    }
  }

  for(i=0;i<mLength;i++){
    encodedMessageNumArray[i] = (messageNumArray[i]+keyNumArray[i])%27; // add our two saved indexes together and mod by 27, to get an appropriate value in our listOfChars
  }

  for(i=0; i < mLength-1; i++){
    encMessage[i] = listOfChars[encodedMessageNumArray[i]]; //save the actual character into a character array
  }
  encMessage[i] = '\0'; //add null terminator to end of our encrypted message


  return encMessage;
}
