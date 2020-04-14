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
#include <netdb.h>
#include <fstream>
#include <unistd.h>

using namespace std;

#define TOTAL 524

int main(int argc, char *argv[]) {
    
        if (argc != 4) {  // Check number of command-line arguments
            cerr << "ERROR: Incorrect number of arguments. <HOSTNAME> <PORT> <FILENAME> is required.\n";  //Print error if incorrect number of arguments
            exit(EXIT_FAILURE); // Exit the program
        }

        char* host = argv[1];           // Get host from command-line
        int pnum = atoi(argv[2]);       // Get port number from command-line
        string file = argv[3];          // Get file name from command-line

        if (pnum < 1024 || pnum > 65535) {  // Check port number validity
            cerr << "ERROR: Incorrect port number. Port number must be between 1024 and 65535.\n";  //Print error string on invalid port
            exit(EXIT_FAILURE);  // Exit the program
        }

        struct addrinfo hostAddr, *hostInfo, *p; // Create structure to hold host address information

        memset(&hostAddr, 0, sizeof(hostAddr));     // Clear hostAddr

        hostAddr.ai_socktype = SOCK_DGRAM;          // Set socket type
        hostAddr.ai_family = AF_INET;               // Set address family
        int check = getaddrinfo(host, NULL, &hostAddr, &hostInfo);

        if (check)  // Check hostname
        {
            cerr << "ERROR: Hostname is not correct\n";     // Print error if hostname not correct
            exit(EXIT_FAILURE);     // Exit the program
        }

        char hostName[256];
        for (p = hostInfo; p != NULL; p = p->ai_next) {
            getnameinfo(p->ai_addr, p->ai_addrlen, hostName, sizeof(hostName), NULL, 0, NI_NUMERICHOST);
        }

        freeaddrinfo(hostInfo);

        int sockfd;     //Variable for socket
        struct sockaddr_in clientAddr;

        if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {    // Create UDP socket
            cerr << "ERROR: Failed creating socket";            // Print error on creation fail
            exit(EXIT_FAILURE);                                 // Exit the program
        }

        fcntl(sockfd, F_SETFL, O_NONBLOCK);                     // Set socket to non-blocking

        clientAddr.sin_family = AF_INET;
        clientAddr.sin_port = htons(pnum);
        clientAddr.sin_addr.s_addr = inet_addr(hostName);
        memset(clientAddr.sin_zero, '\0', sizeof(clientAddr.sin_zero));

        socklen_t clientSize;

        fd_set readfds;                             // File decriptors to check
        FD_ZERO(&readfds);                          // Set file descriptors to zero
        FD_SET(sockfd, &readfds);                   // Set the file descriptor 

        clientSize = sizeof(clientAddr);              // Save size

        char buffer[TOTAL];
        ifstream read_file(file, ios::binary);

        while (1) {
            memset(buffer, '\0', sizeof(buffer));

            int bytes_sent = read_file.read(buffer, sizeof(buffer)).gcount();

            fprintf(stderr, "%s", file.c_str());

            if (bytes_sent == 0) {
                break;
            }

            if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) & clientAddr, clientSize) == -1) {
                cerr << "ERROR: Unable to send";
                exit(EXIT_FAILURE);
            }
        }

        //close(sockfd);

        return 0;
}
