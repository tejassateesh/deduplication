#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <unordered_map>
#include <ctype.h>
using namespace std;

//Global Variables required

string savefile = "./Save.txt"; //File to store backup of the file table, to reload when required
string ip = "192.168.0.9/"; //IP address of the machine
unordered_map <string,string>hashTable;
string clientIP;

//Modules start here

int sendReply(string data){ //Send reply back to the calling system

    //client : send flag data

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
    listenToRequests();
}