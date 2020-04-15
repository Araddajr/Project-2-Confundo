/*
Name: Alan Radda Jr.
NUID: 54265365
Class: CSCI 3550
Assignment: Project 2: Confundo
Due Date: 4/15/2020
*/

#include <string>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>


//using std::cerr; 
using namespace std;

void signal_quit(int quit);
void signal_term(int term);

#define TOTAL 524

int main(int argc, char *argv[]) {

    if (argc != 3) {  //If the number of arguments is not correct
        cerr << "ERROR: Incorrect number of arguments. <PORT> <FILE-DIR> is required.\n";  //Print error string
        exit(EXIT_FAILURE); // Exit the program
    }

    int pnum = atoi(argv[1]);  // Get port number from command-line

    if (pnum < 1024 || pnum > 65535) {  // Check port number validity
        cerr << "ERROR: Incorrect port number. Port number must be between 1024 and 65535.\n";  //Print error string on invalid port
        exit(EXIT_FAILURE);  // Exit the program
    }

    signal(SIGQUIT, signal_quit); // Check for SIQQUIT
    signal(SIGTERM, signal_quit); // Check for SIGTERM

    int sockfd;             // Variable for socket
    struct sockaddr_in serverAddr, clientAddr;         // Structures used to store server and client addresses 

    socklen_t addrSize;  //, clientAddrSize;         // Store size of addresses

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {    // Create UDP socket
        cerr << "ERROR: Failed creating socket";            // Print error on creation fail
        exit(EXIT_FAILURE);                                 // Exit the program
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);                     // Set socket to non-blocking

    serverAddr.sin_family = AF_INET;                                        // Set address family
    serverAddr.sin_port = htons(pnum);                                      // Set port number
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);                         // Set address
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
  
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {            //Bind socket
        cerr << "ERROR: Unable to bind";
        exit(EXIT_FAILURE);
    }

    addrSize = sizeof(clientAddr);              // Save size

    int remove = 1;  // Delete / from path
    int count = 1;  // Counter
    char* directory = argv[2];  // Get directory from command-line
    string filename = to_string(count); // Set filename
    string fslash = "/";
    string path = directory + fslash + filename + ".file"; // Set path with directory and filename

    struct stat save = { 0 };
    if (stat(directory, &save) == -1) { // Check if directory exists
        mkdir(directory + 1, 0700);  // If it does not exist, make directory with user permissions of read, write, and execute
    }
   
    while (1) {
        if (stat(path.c_str() + remove, &save) == -1) {                         // Check if filename exists
            ofstream write_file(path.c_str() + remove, ios::out | ios::app | ios::binary);            // If it doesn't, write file
            break;                                                              
        }
        else {
            count++;                                                            // Update count
            filename = to_string(count);                                        // Update filename
            path = directory + fslash + filename + ".file";                     // Update path
        }
    }

    ofstream write_file(path.c_str() + remove, ios::out | ios::app | ios::binary);    // Write file
    char buffer[TOTAL];     // Total buffer size

    while (1) {

        int bytesRecv = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrSize);      // Receive data from client
        
        int i;

        for (i = 0; i < bytesRecv; i++) {      
            write_file << buffer[i];                // Write data to file
        }
    }

    write_file.close();                             // Close file
    
    return 0;
}

void signal_quit(int quit) { // Function for handling SIGQUIT signal
    exit(0); // Exit with code 0
}

void signal_term(int term) {  //Function for handling SIGTERM signal
    exit(0);  // Exit with code 0
}
