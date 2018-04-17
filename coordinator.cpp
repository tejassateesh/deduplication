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

int sendReply(string data){ //Send reply back to the calling system

    //client : send flag data
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<<"Socket creation error \n";
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        cout<<"Invalid address/ Address not supported \n";
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"Connection Failed \n";
        return -1;
    }
    send(sock , data.c_str() , data.length() , 0 );
    
    return 0;
}

int updateHashTable(string ipandinode,string hash){  //The hash table is updated with the new values

    hashTable.emplace(hash,ipandinode);

}

string checkHashTable(string hash){ //Checks if there is a collision or not
    
    unordered_map<string,string>::const_iterator flag = hashTable.find(ipandinode);
    if(flag == hashTable.end()){
        flag = "N"+ipandinode;
    }
    else{
        flag = "D"+hashTable[hash];
    }
    return flag;
}

int listenToRequests(){ //Server program that keeps actively listening to any and every requests to the coordinator
    
    int options;
    string ipandinode,hash;
    
    //server
    
    switch(options){
        case '1'    :   //newfile, check hashtable for collision
                        // string flag = checkHashTable(hash);
                        // updateHashTable(ipandinode,hash);
                        
        break;

        case '2'    :   //when only inode value needs to be updated
                        // updateHashTable(ipandinode,hash);
        break;

        default:    cout<<"Wrong option received"<<endl;
    }
}

int main(){
    for(;;){
        listenToRequests();
    }
}