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

//Modules start here

int updateHashTable(){  //The hash table is updated with the new values

}

int listenToRequests(){ //Server program that keeps actively listening to any and every requests to the coordinator

}

int checkHashTable(string hash){ //Checks if there is a collision or not

}

int main(){

}