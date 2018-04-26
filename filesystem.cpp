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

//Global Variables required

string hashsavefile = "./SaveHash.txt"; //File to store backup of the hash table, to reload when required
string inodesavefile = "./SaveInode.txt"; //File to store backup of the inode table, to reload when required
string my_ip = "127.0.0.1/";
unordered_map <string,string>hashTable;
unordered_map <string,string>inodeTable;

//The Modules start here

string generateHash(string fileName){  //Calculate the MD5 hash for the file
    
    string command = "md5sum "+fileName;
    char buffer[128];
    string result;

    FILE *fp = popen(command.c_str(), "r");
    if(!fp){
        cout<<"Command execution failed"<<endl;
    }
    while(fgets(buffer,128,fp) != NULL){
        result += buffer;
    }
    pclose(fp);

    result = result.substr(0,result.find(" "));
    return result;
}

int storeHash(string key, string value){ //To add the hash value and other details to the filetable

    hashTable.insert(make_pair(key,value));

    //Serialize hash table and save to file

}

int storeInode(string key, string value){ //To add the hash value and other details to the filetable

    inodeTable.insert(make_pair(key,value));

    //Serialize hash table and save to file

}

int deletedFile(string fileName){   //What happens when a file is deleted in this system

}

string callCoordinator(string ipandinode,string hashVal,string option){   //Sends request to the coordinator to check if the file is duplicate or to notify any changes made to the file

    //Client calling the Coordinator

    struct sockaddr_in address;
    int sock = 0, valread, sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<<"\n Socket creation error \n";
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MY_IP);
    address.sin_port = 8080;
    bind(sockfd, (struct sockaddr *)&address, sizeof(address));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serv_addr.sin_port = htons(PORT);
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"\nConnection to server failed \n";
    }
    
    string dataToSend = option + ipandinode + "!" + hashVal;

    send(sock , dataToSend.c_str() , dataToSend.length() , 0 );
    valread = read(sock , buffer, 1024);

    string retVal(buffer);
    return retVal;
}

int requestFileContent(string fileName)    {   //When the file is duplicate, this function requests the system with the file to send the contents when requested to display

    int i = 0;

    string ipandinode = inodeTable[fileName];
    
    while(ipandinode[i] != '/') i++;
    //MYIP == ip?
    
    string ipToCall = ipandinode.substr(0,i);
    string inodeToAccess = ipandinode.substr(i+1);
    cout<<"IPtoCall = "<<ipToCall<<"\ninodeToAccess = "<<inodeToAccess<<endl;
    //Client calling the system that has the file
    struct sockaddr_in address;
    int sock = 0, valread, sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<<"\n Socket creation error \n";
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MY_IP);
    address.sin_port = 8080;
    bind(sockfd, (struct sockaddr *)&address, sizeof(address));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ipToCall.c_str());
    serv_addr.sin_port = htons(PORT);
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"\nConnection to server failed \n";
    }
    
    send(sock , inodeToAccess.c_str() , inodeToAccess.length() , 0 );
    valread = read(sock , buffer, 1024);
    cout<<buffer<<endl;

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
    string ipandinode = my_ip + to_string(inode);
    //Obtain the MD5 checksum
    string hashVal = generateHash(fileName);
    
    //Returned value of Flag is in the format "{N/D}{IPandInode}": N=Not Duplicate, D=Duplicate.
    cout<<"Calling Coordinator\n";
    string flag = callCoordinator(ipandinode,hashVal,"1");

    if(flag[0]=='N'){    //File content is unique and not present in any other system
        storeInode(fileName,ipandinode);
        storeHash(fileName,hashVal);
        cout<<"Not a duplicate File\n";
    }
    else if(flag[0]=='D'){   //File content exists in some system, store only address
        ipandinode = flag.substr(1);
        storeInode(fileName,ipandinode);
        storeHash(fileName,hashVal);
        cout<<"Duplicate file, located in "<<ipandinode<<endl;
        //Delete file from the system
    }
    else{
            cout<<"Error while obtaining reply from coordinator"<<endl;
            return 0;
        }
}

int modifiedFile(string fileName){ //When a file is modified or moved, this handles the events
        
    //Obtain old hash value
    string oldHash = hashTable[fileName];
    cout<<"old hash = "<<oldHash<<endl;
    string option;

    string hashVal = generateHash(fileName);
    cout<<"New hash = "<<hashVal<<endl;
    
    if(oldHash==hashVal){  //Location of save changed, content is same: update the inode of file in coordinator if original else only in local
        option = "2";
        cout<<"Same hash\n";
    }
    else{   //Content change
        option = "1";
        
        //Obtaining the INODE value of the file
        string ipandinode = inodeTable[fileName];

        string flag = callCoordinator(ipandinode,hashVal,option);
        
        if(flag[0]=='N'){    //File content is unique and not present in any other system
            storeHash(fileName,hashVal);
            storeInode(fileName,ipandinode);
            cout<<"Not a duplicate File\n";
        }
        else if(flag[0]=='D'){   //File content exists in some system, store only address
            ipandinode = flag.substr(1);
            storeHash(fileName,hashVal);
            storeInode(fileName,ipandinode);
            cout<<"Duplicate File\n";
        }
        else{
            cout<<"Error while obtaining reply from coordinator"<<endl;
            return 0;
        }
    }
}

int main(){

    int opt;
    string fileName;


    for(;;){
        cout<<"Enter file name and option\n";
        cin>>fileName>>opt;

        switch(opt){
            case 1  :   newFile(fileName);
            break;
            case 2  :   modifiedFile(fileName);
            break;
            // case 3  :   changedName(oldFileName,NewFileName);
            // break;
            // case 4  :   deletedFile(fileName);
            // break;
            case 5  :   requestFileContent(fileName);
            break;   
            default :   cout<<"Wrong option, please retry!"<<endl;
        }
    }
}