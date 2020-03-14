#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

char *readMessageFile(char *myFile);
char *decryptMessage(char listOfChars[], char key[], char message[]);

int main(int argc, char const *argv[]) {

  int listenSocketFD, establishedConnectionFD, portNumber, charsRead; //set up socket numbers and ports
  socklen_t sizeOfClientInfo;
  char buffer[100000]; //initalize buffers to large for sending/reveiving data through sockets
  char buffer2[100000];
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

  while(1){ //keeps the daemon online after the port/socket was used

    // Accept a connection, blocking if one is not available until one connects
    sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
    establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
    if (establishedConnectionFD < 0) error("ERROR on accept");

    pid_t newPID; //create a pid to fork off and do sockets with child.  So we don't have to set up sockets every time
    newPID = fork();
    if(newPID < 0){
      error("error on fork");
    }
    if(newPID == 0){ //if pid successfully creates

      // Get the message from the client and display it
      //get file name
      memset(buffer, '\0', 256);
      charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
      if (charsRead < 0) error("ERROR reading from socket");

      char *listOfChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

      char *myMessageE = strdup(readMessageFile(buffer)); /*read from file and store into variable that we will use later*/

      // Send a Success message back to the client
      charsRead = send(establishedConnectionFD, "I am the server, and I got your file", 39, 0); // Send success back
      if (charsRead < 0) error("ERROR writing to socket");

      // Get the message from the client and display it
      memset(buffer2, '\0', 256);
      charsRead = recv(establishedConnectionFD, buffer2, 255, 0); // Read the client's message from the socket
      if (charsRead < 0) error("ERROR reading from socket");
      //strdup so we dont point to something that will change
      char *myKey = strdup(readMessageFile(buffer2)); /*read from file and store into variable that we will use later*/

      // Send a Success message back to the client
      charsRead = send(establishedConnectionFD, decryptMessage(listOfChars, myKey, myMessageE), strlen(myMessageE), 0); // Send success back
      if (charsRead < 0) error("ERROR writing to socket");

    }
    close(establishedConnectionFD); // Close the existing socket which is connected to the client

  }
  close(listenSocketFD); // Close the listening socket

  return 0;
}

//function to decrypt the message received through socket.  Returns unencrypted message
char *decryptMessage(char listOfChars[], char *key, char *encMessage){
  int mLength = strlen(encMessage); //get the string length of the message passed in to use in for loops
  char *decMessage; // initialize the decrypted message character array
  int keyLength = strlen(key);
  int messageNumArray[mLength+1]; // create array to store index values to subtract
  int keyNumArray[mLength+1]; // create array to store index values of key
  int decodedMessageNumArray[mLength+1]; //the array of indexes we will use to get characters in listOfChars
  int i, j;

  decMessage = malloc(1000000 * sizeof(char));//allocate space to get rid of local stack warnings.

  //loop through message and compare to listOfChars
  for(i=0; i<mLength; i++){
    for(j=0; j<strlen(listOfChars); j++){
      if(encMessage[i] == listOfChars[j]){ //if we get a match, set its index into number array to use later
        messageNumArray[i] = j;
      }
    }
  }

//loop through key and compare to listOfChars
  for(i=0;i<mLength;i++){
    for(j=0;j<strlen(listOfChars);j++){
      if(key[i]==listOfChars[j]){//same thing as above, if match set its index into number array to use later
        keyNumArray[i] = j;
      }
    }
  }

  for(i=0;i<mLength;i++){
    decodedMessageNumArray[i] = (messageNumArray[i]-keyNumArray[i])%27; //subtract the key to get the normal index of the unencrypted message.
    if(decodedMessageNumArray[i] < 0){                                  //then mod by 27 to get a number within acceptable range of listOfChars
      decodedMessageNumArray[i] += 27;//if the number we get is negative, add 27 to loop back to appropriate number
    }
  }
  for(i=0; i < mLength-1; i++){
    decMessage[i] = listOfChars[decodedMessageNumArray[i]]; // find the character that matches the number in our messagenumarray, and set it
  }

  decMessage[i] = '\0'; //add null terminator to the end of decrypted message
  return decMessage;
}

//helper to read and put the message from a file into a variable
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
