
/*************************************************************************************
*Programmer Name: Franklin Smith
*Program Name: chatclient.c
*Program Description: Implements a client in a TCP client - server network application
*Course Name: CS372
*Last Modified: 5/7/2019
*************************************************************************************/
// Works Cited: Template of client code taken from CS344 Professor Brewster client.c code winter 2019
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

// Global variables for easier access
struct sockaddr_in serverAddress;
struct hostent* serverHostInfo;
int socketFD;

/********************************************************************
* Function Name: error
* Parameters: const char *msg
* Purpose: Prints message to stderr and exits from program
* Pre and Post conditions: None, exits from program with 0
*********************************************************************/ 
void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

/********************************************************************
* Function Name: initiateContact
* Parameters: none
* Purpose: Initiates contact with server by setting up the socket and connecting
* Pre and Post conditions: Server must first be listening, makes connection
**********************************************************************/
int initiateContact(){
	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");
	return 1;
}

/*********************************************************************
* Function Name: sendMessage
* Parameters: int charsWritten, char *buffer, int size
* Purpose: Sends message to the server, calls error function if nothing is written
* Pre and Post condtions: connection made, sends message
**********************************************************************/ 
void sendMessage(int charsWritten, char *buffer, int size){
		// Send message to server
		charsWritten = send(socketFD, buffer, size, 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
}

/*********************************************************************
* Function Name: receiveMessage
* Parameters: int charsRead, char *buffer, int size
* Purpose: Receives message from the server, calls error function if problem
*          reading from socket. Returns 1 if message contains \Quit and initiates
*          ending of program since socket connection is closed
* Pre and Post conditions: connection made, receives message from server and prints it
*****************************************************************************/
int receiveMessage(int charsRead, char *buffer, int size){
		// Get return message from server
		charsRead = recv(socketFD, buffer, size - 1, 0); // Read data from the socket, leaving \0 at end
		if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	
		char* inString;
		inString = strstr(buffer, "\\quit");
		if (inString){
			return 1;
		} else {
			printf("%s\n", buffer);
			return 0;
		}	
}


int main(int argc, char *argv[])
{
	int portNumber, charsWritten, charsRead;
	char buffer[1024];
	char bufferHandle[1024];
	char handleCharacter[1024];
	char bufferSend[1024];
    
	if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Get clients handle
	printf("What would you like your handle to be? ");
	memset(bufferHandle, '\0', sizeof(bufferHandle));
	fgets(bufferHandle, sizeof(bufferHandle) - 1, stdin);
	bufferHandle[strcspn(bufferHandle, "\n")] = '\0';
	memset(handleCharacter, '\0', sizeof(handleCharacter));
	handleCharacter[0] = '>';
	handleCharacter[1] = ' ';
	strcat(bufferHandle, handleCharacter);

	// Initiate contact and make connection
	if (initiateContact())
		printf("Connected to server at %s port %d\n", argv[1], portNumber); 

	// Keep sending and receiving messages as the connection remains open
	int flag = 0;
	while (flag != 1){
		int closeConnection = 0;
		printf("%s",bufferHandle);
		memset(bufferSend, '\0', sizeof(bufferSend));
		strcat(bufferSend, bufferHandle);
		memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
		fgets(buffer, sizeof(buffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
		buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds
		
		if (strcmp(buffer,"\\quit") == 0){
			break;
		}
		
		strcat(bufferSend, buffer);
		sendMessage(charsWritten, bufferSend, strlen(bufferSend));
		memset(bufferSend, '\0', sizeof(bufferSend));
		closeConnection = receiveMessage(charsRead, bufferSend, sizeof(bufferSend));
		if (closeConnection)
			break;
                
	}

	close(socketFD); // Close the socket
	return 0;
}
