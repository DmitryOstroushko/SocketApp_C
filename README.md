# SocketApp_C

A console client-server multi-threading application for Linux
A server part generates a sequence of number sconsisting of subsequences (a subsequence is set by a client).
A generated sequence is sent to a client.

Makefile makes two programs: server and client:
- to launch a server you should input a port (usage is presented), example: ./server 4444
- to launch a server you should input IP address of a server and a port, divided by space (usage is presented), example: ./client 127.0.0.1 4444
