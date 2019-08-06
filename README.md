# Client-Server-Chat-App

This project implements a TCP client server network connection. Once connection is made, the client and server can send and receive messages to one another. Contents needed: Makefile, chatclient.c, chatserve


How to run programs from command line once all files are in same directory:
1. type make:
	make 
2. start the server first by typing ./chatserve and a port number, if the port number is already in use try a different number:
	./chatserve 3008
3. On the server you will be prompted to type a handle (What would you like your handle to be? ). Type a handle.
	What would you like your handle to be? Max
4. On the server a line is printed that states it's listening on the port specified earlier
	Listening on port 3008
5. Wait for client connection
6. On client window type "./chatclient <SERVER-HOSTNAME> <PORT#>", this will be the same server hostname and port# established earlier in step 2.
	./chatclient flip3.engr.oregonstate.edu 3008
7. On the client you will be prompted  to type a handle (What would you like your handle to be? ). Type a handle.
	What would you like your handle to be? Robert
8. On the client type a message and then click enter. The client will initiate contact. After initial contact, the client has to wait for the server's response before sending another message.
	Robert> Hey there I'm Robert
9. On the server type a response and click enter after you receive the client's message
	Max> Hey Robert
10. Typing \quit from the client when it's the client's turn will end the connection and end chatclient program. Typing \quit from the server when it's the server's turn will send the message to the client, the client will end the connection and the server will end the connection and start listening for new client requests. The server keeps listening until a SIGINT is received. 