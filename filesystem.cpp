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
string ip = "192.168.0.1/"; //IP address of the machine
unordered_map <string,string>hashTable;

//The Modules start here

string generateHash(string fileName){  //Calculate the MD5 hash for the file

}

int storeHash(string key, string value){ //To add the hash value and other details to the filetable

    hashTable.emplace(key,value);

    //Serialize hash table and save to file

}

string callCoordinator(string ipandinode,string hash){   //Sends request to the coordinator to check if the file is duplicate or to notify any changes made to the file

//client

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
    if(ret<0){  //on Failure
        cout<<"Error in reading file stats\n";
        return 0;
    }
    else{
        inode = temp.st_ino;
    }

    // string hash = generateHash(fileName);

    //IP address + Inode value of file - to obtain the IPandInode Pair Key to be sent to coordinator
    string ipandinode = ip+to_string(inode);

    //Returned value of Flag is in the format "{N/D}{IPandInode}": N=Not Duplicate, D=Duplicate.
    // string flag = callCoordinator(ipandinode,hash);

    // if(flag[0]=='N'){    //file content is unique and not present in any other system
        storeHash(fileName,ipandinode);
    // }
    // else if(flag[0]=='D'){   //file content exists in some system, store only address
        // ipandinode = flag.substr(1);
        // storeHash(fileName,ipandinode);
    // }
    // else{
            // cout<<"Error while obtaining reply from coordinator"<<endl;
            // return 0;
        // }
}

int modifiedFile(string fileName){ //When a file is modified or moved, this handles the events
    
    string hash =  generateHash(fileName);
    
    //Obtain old hash value
    string oldHash = hashTable[fileName];
    if(oldHash[0]=='D'){
        oldHash = requestHash();
    }
    else{
        oldHash = oldHash.substr(1);
    }

    if(oldHash==hash){  //Location of save changed, content is same;` update the inode of file in coordinator
        
    }
    else{   //Content change

        //Obtaining the INODE value of the file
        struct stat temp;
        int ret = stat(fileName.c_str(),&temp);
        ino_t inode;
        if(ret<0){
            cout<<"Error in reading file stats\n";
            return 0;
        }
        else{
            ino_t inode = temp.st_ino;
        }

        string ipandinode = ip+to_string(inode);

        string flag = callCoordinator(ipandinode,hash);
        
        if(flag[0]=='N'){    //file content is unique and not present in any other system
            storeHash(fileName,hash);
        }
        else if(flag[0]=='D'){   //file content exists in some system, store only address
            ipandinode = flag.substr(1);
            storeHash(fileName,ipandinode);
        }
        else{
            cout<<"Error while obtaining reply from coordinator"<<endl;
            return 0;
        }
    }
}

int main(int argc, char** argv){

    string fileName(argv[2]);
      
    switch(atoi(argv[1])){
        case 1  :   newFile(fileName);
        break;
        case 2  :   modifiedFile(fileName);
        break;
        case 3  :   sendFileContent(fileName);
        break;
        default :   cout<<"Wrong option, please retry!"<<endl;
    }

}