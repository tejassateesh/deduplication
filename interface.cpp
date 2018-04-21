/*This program activates the file program by always listening to other VMs with a socket and also sends file contents when requested*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <unordered_map>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

using namespace std;

int sendFileContent(string fileName){   //Sending the file content when other systems request it

//server

}

int listenToRequests(){ //Socket that always listens to any request from any system in the network

//server

//Call the fileHandler program with appropriate options

//??CREATE OPTION WHEN A FILE IS DELETED TO CHECK IF THAT FILE EXISTS IN THIS SYSTEM AS A POINTER AND GET A COPY SO THAT IT CAN BE SAVED HERE AND UPDATE THE IP AND INODE IN THE COORDINATOR TABLE??

}

int main(){
    for(;;)
        listenToRequests();
}