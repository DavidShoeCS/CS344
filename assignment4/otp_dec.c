#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues
char *readMessageFile(char *myFile);


int main(int argc, char *argv[]) {

  //read from keygen file
  //read from given text file, store into variable
  //send stored text file over sockets

  if (argc < 3) { // Check usage & args
    fprintf(stderr,"USAGE: %s textFile port\n", argv[0]);
    exit(0);
   }
  else{
    int socketFD, portNumber, charsWritten, charsRead; //given stuff.  Initialize
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char buffer[1000000]; //initialize buffer to something large for sending and receiving through sockets

    // Set up the server address struct
  	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
  	serverAddress.sin_family = AF_INET; // Create a network-capable socket
  	serverAddress.sin_port = htons(portNumber); // Store the port number
  	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
  	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
  	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) error("CLIENT: ERROR opening socket");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
      error("CLIENT: ERROR connecting");


    //store the message to a variable, which we will send to the server (Daemon)
    char *originalMessage = strdup(readMessageFile(argv[1]));

    // Send message to server
    //send file name to daemon
  	charsWritten = send(socketFD, originalMessage, strlen(originalMessage), 0); // Write to the server
  	if (charsWritten < 0) error("CLIENT: ERROR writing to socket->echo");
  	if (charsWritten < strlen(originalMessage)) printf("CLIENT: WARNING: Not all data written to socket!\n");

    FILE *fp = fopen(argv[1], "r+");//get the length of our message file to know we are getting the right amount of data received
    fseek(fp, 0L, SEEK_END);
    long int lengthOfMessageFile = ftell(fp);
    fclose(fp);






    // Get return message from server
    //if sending file was a success, get message
    bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
    if (charsRead < 0) error("CLIENT: ERROR reading from socket->1");

    //save the contents from file specified by user, then send through socket to server(daemon)
    char *theKey = strdup(readMessageFile(argv[2]));

    // Send MESSAGE to server
    charsWritten = send(socketFD, theKey, strlen(theKey), 0); // Write to the server
    if (charsWritten < strlen(theKey)) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");


    // Get return message from server
    bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
    charsRead = recv(socketFD, buffer, sizeof(buffer), 0); // Read data from the socket, leaving \0 at end
    if (charsRead < 0) error("CLIENT: ERROR reading from socket burk");
    printf("%s", buffer);
    lengthOfMessageFile = lengthOfMessageFile - strlen(buffer);

    while(lengthOfMessageFile != 1){ //until the data left to read is 1, keep reading from the socket
      bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
      charsRead = recv(socketFD, buffer, sizeof(buffer), 0); // Read data from the socket, leaving \0 at end
      lengthOfMessageFile = lengthOfMessageFile - strlen(buffer);
      printf("%s", buffer); //print off buffer before it resets
    }

    printf("\n");//print new line at the end for '>' into files
    fflush(stdout);

    close(socketFD); // Close the socket

  }

  return 0;
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
