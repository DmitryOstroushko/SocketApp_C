# Socket Application

## General idea
A console client-server multi-threading application for Linux. There are two parts in the application: a server and a client.   
The server part generates a sequence of numbers consisting of subsequences (a subsequence is set by a client).
A generated sequence is sent to the client. The client processes an input subsequence and combines it with the clients' output stream.
The output stream of client is printing to a console. 

## How it works
[Makefile](https://github.com/DmitryOstroushko/SocketApplication/blob/master/Makefile) creates two programs: `server` and `client`.  
A server program and a client program interconnect using sockets technology.  
To launch a server you should input a port (usage is presented), example: `./server 4444`.  
To launch a client you should input IP address of a server and a port, divided by space (usage is presented), example: `./client 127.0.0.1 4444`.  
