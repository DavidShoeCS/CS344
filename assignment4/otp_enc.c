#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char const *argv[]) {
  //read from keygen file
  //read from given text file, store into variable
  //send stored text file over sockets
  fflush(stdout);
  if (argc < 3) { // Check usage & args
    fprintf(stderr,"USAGE: %s textFile port\n", argv[0]);
    exit(0);
   }


  else{
    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char buffer[1000000];

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

    // Send message to server SEND PLAINTEXT NAME
  	charsWritten = send(socketFD, argv[1], strlen(argv[1]), 0); // Write to the server
  	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
  	if (charsWritten < strlen(argv[1])) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");



    FILE *fp = fopen(argv[1], "r+");
    fseek(fp, 0L, SEEK_END); //get the size of the length of message to then compare what we received through the socket vs what we need
    long int lengthOfMessageFile = ftell(fp);
    fclose(fp);


    // Get return message from server GET LENGTH OF MESSAGE
    bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
    if (charsRead < 0) error("CLIENT: ERROR reading from socket");




    // Send message to server
    charsWritten = send(socketFD, argv[2], strlen(argv[2]), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(argv[2])) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");



    // Get return message from server
    bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
    charsRead = recv(socketFD, buffer, sizeof(buffer), 0); // Read data from the socket, leaving \0 at end
    if (charsRead < 0) error("CLIENT: ERROR reading from socket");
    printf("%s", buffer);
    lengthOfMessageFile = lengthOfMessageFile - strlen(buffer); //update the length of the message we need to receive from the socket


    while(lengthOfMessageFile != 1 && strlen(buffer)>1){ //while there is still data to be read, read from socket
      bzero(buffer, sizeof(buffer)); // Clear out the buffer again for reuse
      charsRead = recv(socketFD, buffer, sizeof(buffer), 0); // Read data from the socket, leaving \0 at end
      if (charsRead < 0) error("CLIENT: ERROR reading from socket");
      lengthOfMessageFile = lengthOfMessageFile - strlen(buffer); //update the amount of data to still be read
      printf("%s", buffer);
    }
    if(strlen(buffer)!=0){ //only print new line if we dont have an empty buffer.  Fixes error of getting 1 byte in ciphertext5 test case
      printf("\n");
    }
    fflush(stdout); //flush stdout to avoid problems

    close(socketFD); // Close the socket


  }




  return 0;
}
