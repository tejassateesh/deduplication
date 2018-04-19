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
string hashsavefile = "./SaveHash.txt"; //File to store backup of the hash table, to reload when required
string inodesavefile = "./SaveInode.txt"; //File to store backup of the inode table, to reload when required
string ip = "192.168.0.1/"; //IP address of the machine
unordered_map <string,string>hashTable;
unordered_map <string,string>inodeTable;

//The Modules start here

int listenToRequests(){ //Server Program to listen to incoming requests

}

string generateHash(string fileName){  //Calculate the MD5 hash for the file

}

int storeHash(string key, string value){ //To add the hash value and other details to the filetable

    hashTable.insert(make_pair(key,value));

    //Serialize hash table and save to file

}

int storeInode(string key, string value){ //To add the hash value and other details to the filetable

    inodeTable.insert(make_pair(key,value));

    //Serialize hash table and save to file

}

string callCoordinator(string ipandinode,string hashVal){   //Sends request to the coordinator to check if the file is duplicate or to notify any changes made to the file

//client
string dataToSend = ipandinode + "-" + hashVal;
}

int requestHash(){  //Request hash value for duplicate files

//client

}

int requestFileContent()    {   //When the file is duplicate, this function requests the system with the file to send the contents when requested to display

//client

}

int sendFileContent(string fileName){   //Sending the file content when other systems request it

//server

}

int newFile(string fileName){ //When a new file is found, this module handles the events to be done
    
    //Obtaining the INODE value of the file
    struct stat temp;
    int ret = stat(fileName.c_str(),&temp);
    ino_t inode;
    if(ret<0){  //On Failure
        cout<<"Error in reading file stats\n";
        return 0;
    }
    else{
        inode = temp.st_ino;
    }

    //IP address + Inode value of file - to obtain the IPandInode Pair Key to be sent to coordinator
    string ipandinode = ip+to_string(inode);

    //Obtain the MD5 checksum
    string hashVal = generateHash(fileName);
    
    //Returned value of Flag is in the format "{N/D}{IPandInode}": N=Not Duplicate, D=Duplicate.
    // string flag = callCoordinator(ipandinode,hashVal);

    // if(flag[0]=='N'){    //File content is unique and not present in any other system
        storeInode(fileName,ipandinode);
        storeHash(fileName,hashVal);
    // }
    // else if(flag[0]=='D'){   //File content exists in some system, store only address
        // ipandinode = flag.substr(1);
        // storeInode(fileName,ipandinode);
        // storeHash(fileName,hashVal);
        //Delete file from the system
    // }
    // else{
            // cout<<"Error while obtaining reply from coordinator"<<endl;
            // return 0;
        // }
}

int modifiedFile(string fileName){ //When a file is modified or moved, this handles the events
        
    //Obtain old hash value
    string oldHash = hashTable[fileName];
    oldHash = oldHash.substr(1);

    string hashVal = generateHash(fileName);
    
    if(oldHash==hashVal){  //Location of save changed, content is same: update the inode of file in coordinator if original else only in local
        
    }
    else{   //Content change

        //Obtaining the INODE value of the file
        string ipandinode = inodeTable[fileName];

        string flag = callCoordinator(ipandinode,hashVal);
        
        if(flag[0]=='N'){    //File content is unique and not present in any other system
            storeHash(fileName,hashVal);
            storeInode(fileName,ipandinode);
        }
        else if(flag[0]=='D'){   //File content exists in some system, store only address
            ipandinode = flag.substr(1);
            storeHash(fileName,hashVal);
            storeInode(fileName,ipandinode);
        }
        else{
            cout<<"Error while obtaining reply from coordinator"<<endl;
            return 0;
        }
    }
}

int main(int argc, char** argv){

    //Reload the hash and inode table

    string fileName(argv[2]);
      
    switch(atoi(argv[1])){
        case 1  :   newFile(fileName);
        break;
        case 2  :   modifiedFile(fileName);
        break;
        case 3  :   sendFileContent(fileName);
        break;
        // case 4  :   changedName(oldFileName,NewFileName);
        // break;
        default :   cout<<"Wrong option, please retry!"<<endl;
    }

}