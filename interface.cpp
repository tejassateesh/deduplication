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

#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"
#define MY_IP "127.0.0.1"

string sendFileContent(string fileInode){   //Sending the file content when other systems request it
    
    string command;
    char buffer[128];
    string result;

    //Obtaining filename
    command = "find -inum "+fileInode;
    
    FILE *fp = popen(command.c_str(), "r");
    if(!fp){
        cout<<"Command execution failed"<<endl;
    }
    while(fgets(buffer,128,fp) != NULL){
        result += buffer;
    }
    pclose(fp);
    cout<<result<<endl;
    string fileName = result;
    
    result = "";

    command = "cat "+fileName;
    
    fp = popen(command.c_str(), "r");
    if(!fp){
        cout<<"Command execution failed"<<endl;
    }
    while(fgets(buffer,128,fp) != NULL){
        result += buffer;
    }
    pclose(fp);
    return result;
}

int listenToRequests(){ //Socket that always listens to any request from any system in the network

    //Server that always listens to requests
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        cerr<<"Socket failed";
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        cerr<<"setsockopt";
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
        cerr<<"bind failed";
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
        cerr<<"listen";
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        cerr<<"accept";
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket , buffer, 1024);

    string dataReceived(buffer);

    string dataToSend = sendFileContent(dataReceived);
    cout<<dataToSend<<endl;
    send(new_socket , dataToSend.c_str() , dataToSend.length() , 0 );
    return 0;

//??CREATE OPTION WHEN A FILE IS DELETED TO CHECK IF THAT FILE EXISTS IN THIS SYSTEM AS A POINTER AND GET A COPY SO THAT IT CAN BE SAVED HERE AND UPDATE THE IP AND INODE IN THE COORDINATOR TABLE??

}

int main(){
    for(;;)
        listenToRequests();
}