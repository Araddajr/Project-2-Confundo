#include <string>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <signal.h>

//using std::cerr; 
using namespace std;

void signal_quit(int quit);
void signal_term(int term);

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
  
}

void signal_quit(int quit) { // Function for handling SIGQUIT signal
    exit(0); // Exit with code 0
}

void signal_term(int term) {  //Function for handling SIGTERM signal
    exit(0);  // Exit with code 0
}
