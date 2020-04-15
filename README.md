# CS118 Project 2

Template for for [CSCI3550 Spring 2020 Project 2](https://uno-csci3550.github.io/Project2-Confundo-Description/)

## Makefile

This provides a couple make targets for things.
By default (all target), it makes the `server` and `client` executables.

It provides a `clean` target, and `tarball` target to create the submission file as well.

You will need to modify the `Makefile` to add your userid for the `.tar.gz` turn-in at the top of the file.

## Provided Files

`server.cpp` and `client.cpp` are the entry points for the server and client part of the project.

## Wireshark dissector

For debugging purposes, you can use the wireshark dissector from `tcp.lua`. The dissector requires
at least version 1.12.6 of Wireshark with LUA support enabled.

To enable the dissector for Wireshark session, use `-X` command line option, specifying the full
path to the `tcp.lua` script:

    wireshark -X lua_script:./confundo.lua

To dissect tcpdump-recorded file, you can use `-r <pcapfile>` option. For example:

    wireshark -X lua_script:./confundo.lua -r confundo.pcap



Name: Alan Radda Jr.
NUID: 54265365
Class: CSCI 3550
Assignment: Project 2: Confundo
Due Date: 4/15/2020


High Level Design:

server.cpp is a UDP server program. It starts by verifying the command-line arguments and the port number.
Next, it creates a UDP socket, and sets up the addressing. Then it binds the connection to the socket. 
After that, it creates a directory and saves files in the newly made directory using the format "1.file, 2.file, ..."
Finally, the server recieves data from the client and writes it to the file it created. 

client.cpp is a UDP client program. It starts by verifying the command-line arguments and the port number. 
Next, it retrieves and verifies the hostname. Then, it creates a UDP socket and sets up the addressing.
Then, it sets up the buffer to send data and reading from a file. Finally, it sends the file data to the server.

Problems:

I could not figure out how to implement the packet. The way I understood it was you needed a packet header file and a 
packet.cpp file. In the packet header file, you would declare the funcions and set up the header. In the packet.cpp file, 
you would then implement the functions in the packet header file. This would allow you to create an packet object, that would
essentially encapsulate the data you were sending into segments that were 512 bytes long, and you would use the 12 byte header
info for identification and proper sending of the packet. I spent hours trying to figure out how to implement the packet, but
unfortunately, I was unable to make anything work so kept it out of my code. 

Acknowledgements:

https://stackoverflow.com/questions/16409851/client-server-text-file-transfer-using-udp-sockets-in-c-server-creates-a-bl
https://www.codeproject.com/Questions/646310/Server-client-file-transfer-using-UDP-in-C
https://stackoverflow.com/questions/13619412/c-constructing-a-packet-with-headers-and-sending-it-through-a-udp-socket
https://www.geeksforgeeks.org/udp-server-client-implementation-c/