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

//Global Variables required

string savefile = "./Save.txt"; //File to store backup of the file table, to reload when required
string ip = "192.168.0.9/"; //IP address of the machine
unordered_map <string,string>hashTable;
string clientIP;

//Modules start here

int eraseOldHash(string ipandinode){    //Erases the old hash value of a modified file

    auto iterator = hashTable.begin();
    while(iterator != hashTable.end()){
        if(iterator->second == ipandinode){
            hashTable.erase(iterator);
            return 0;
        }
    }
    cout<<"Old Hash not found"<<endl;
}

int addToHashTable(string ipandinode,string hash){  //The hash table is updated with the new values

    hashTable.insert(make_pair(hash,ipandinode));

}

string checkHashTable(string ipandinode, string hash){ //Checks if there is a collision or not
    
    unordered_map<string,string>::const_iterator flag = hashTable.find(ipandinode);
    string temp;
    if(flag == hashTable.end()){
        temp = "N"+ipandinode;
        addToHashTable(ipandinode,hash);
    }
    else{
        temp = "D"+hashTable[hash];
    }
    return temp;
}

string respondToRequests(int options,string ipandinode,string hash){ //Server program that keeps actively listening to any and every requests to the coordinator
    
    string flag("");
    
    switch(options){
        case '1'    :   //Newfile, check hashtable for collision
                        flag = checkHashTable(ipandinode,hash);
                        
        break;

        case '2'    :   //When only inode value needs to be updated - in case of inode change of original file
                        eraseOldHash(ipandinode);
                        addToHashTable(ipandinode,hash);
        break;

        case '3'    :   //When content of a pre-existing file has changed : update inode table and inform of the update?
                        
        break; 

        default:    cout<<"Wrong option received"<<endl;
    }
    return flag;
}

int sendReply(){ //Send reply back to the calling system

    //Server - listen to requests and send appropriate reply
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {'\0'};;     
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        cout<<"Socket failed"<<endl;
        exit(EXIT_FAILURE);
    }
      
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        cout<<"Setsockopt failure"<<endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
        cout<<"Bind failed"<<endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        cout<<"Listen Error"<<endl;
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        cout<<"Accept error"<<endl;
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    
    //separate the ipandinode and hash value
    string temp(buffer);
    int i=0;
    
    while(temp[i]!='!')   i++;
    int option = (int)temp[0]-'0';
    string ipandinode = temp.substr(1,i);
    string hash = temp.substr(i+1);

    string data = respondToRequests(option,ipandinode,hash);
    if(option==1){
        send(new_socket , data.c_str() , data.length() , 0 );
    }
    return 0;
}

int main(){
    // sendReply();
}