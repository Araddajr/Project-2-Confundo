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

    char *directory = argv[2];  // Get directory from command-line

    struct stat save = { 0 }; 
    if (stat(directory, &save) == -1) { // Check if directory exists
        mkdir(directory + 1, 0700);  // If it does not exist, make directory with user permissions of read, write, and execute
    }

    signal(SIGQUIT, signal_quit); // Check for SIQQUIT
    signal(SIGTERM, signal_quit); // Check for SIGTERM

    int sockfd;             // Variable for socket
    char buffer[TOTAL];     // Total buffer size
    struct sockaddr_in serverAddr, clientAddr;         // Structures used to store server and client addresses 

    socklen_t addrSize, clientAddrSize;         // Store size of addresses

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0) < 0)) {    // Create UDP socket
        cerr << "ERROR: Failed creating socket";            // Print error on creation fail
        exit(EXIT_FAILURE);                                 // Exit the program
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);                     // Set socket to non-blocking

    serverAddr.sin_family = AF_INET;                                        // Set address family
    serverAddr.sin_port = htons(pnum);                                      // Set port number
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);                         // Set address
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
  
    if (bind(sockfd, (struct sockaddr*) & serverAddr, sizeof(serverAddr)) < 0) {            //Bind socket
        cerr << "ERROR: Unable to bind";
        exit(EXIT_FAILURE);
    }

    fd_set readfds;                             // File decriptors to check
    FD_ZERO(&readfds);                          // Set file descriptors to zero
    FD_SET(sockfd, &readfds);                   // Set the file descriptor 

    addrSize = sizeof(clientAddr);              // Save size
}

void signal_quit(int quit) { // Function for handling SIGQUIT signal
    exit(0); // Exit with code 0
}

void signal_term(int term) {  //Function for handling SIGTERM signal
    exit(0);  // Exit with code 0
}
