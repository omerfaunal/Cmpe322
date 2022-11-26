//
// Created by Omer on 23.10.2022.
//

// You can type make run to compile and run the program.

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool inList(vector<string> list, string str) {
    // Check if a string is in a list
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == str) {
            return true;
        }
    }
    return false;
}

string trim(string str) {
    // This function finds the non whitespace characters in the string and returns the substring from the first non whitespace character to the last non whitespace character
    int i = 0;
    while (str[i] == ' ') {
        i++;
    }
    str = str.substr(i, str.length() - i);
    i = str.length() - 1;
    while (str[i] == ' ') {
        i--;
    }
    str = str.substr(0, i + 1);
    return str;
}

vector<string> split (string s, string delimiter) {
    //This function tokenizes the string s by the delimiter and returns the tokens in a vector
    //First trim the string so that there will not be empty strings in the vector
    s = trim(s);
    
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

string deleteFromString(string s, char c1, char c2){
    //This function deletes c1 and c2 from the string s and returns the new string
    for(int i = 0; i < s.size(); i++){
        //If s[i] is equal to c1 or c2, delete it
        if(s[i] == c1){
            s.erase(i, 1);
        } else if(s[i] == c2) {
            s.erase(i, 1);
        }
    }
    return s;
}

void printList(vector<string> list) {
    //This function prints the elements of the vector list
    for (int i = 0; i < list.size(); i++) {
        cout << list[i] << endl;
    }
}

void copyFile(string source, string destination) {
    //This function copies the file source to the file destination
    string command = "cp " + source + " " + destination;
    system(command.c_str());
}

void openFile(string path) {
    //This function opens the file at the path
    string command = "open " + path;
    system(command.c_str());
}

void readFile(string path) {
    //This function reads the file at the path and prints it
    ifstream file (path);
    if (!file.is_open()){
        //If the file is not open, print an error message
        cout << "File not found" << endl;
    } else {
        string line;
        while (getline(file, line)) {
            cout << line;
            //If the line is not empty, print a new line
            cin.ignore();
            //If the user presses enter, print the next line
        }
        file.close();
    }
}

int main(int argc, char const *argv[])
{
    string x = "";
    vector< string > history;
    //This vector stores the history of the commands
    string username = getenv("USER");
    //This string stores the username of the user

    while(x != "exit"){
        cout << username << " >>> "; 
        //Print the username and the prompt
        getline(cin, x);
        //Get the command from the user
        vector<string> input;
        //This vector stores the tokens of the command
        input = split(x, " ");
        //Tokenize the command
        if(history.size() > 15) {
            //If the history is greater than 15, delete the first element which is oldest element
            history.erase(history.begin());
        }
        if(input[0] != "dididothat"){
            //If the command is not dididothat, add it to the history
            history.push_back(x);
        }
        
        if(input[0] == "listdir") {
            //If the command is listdir, list the files in the current directory
            system("ls"); 
        } 
        else if(input[0] == "whatsmyip") {
            //If the command is whatsmyip, print the ip address
            system("hostname -i");
        }
        else if(input[0] == "mycomputername") {
            //If the command is mycomputername, print the computer name
            system("hostname");
        } 
        else if(input[0] == "dididothat"){
            //If the command is dididothat, check if the command is in the history
            int index = x.find("\"");
            //Find the index of the first " character
            x = x.substr(index + 1, x.length() - index - 2);
            //Get the command between the " characters
            if(inList(history, x)) {
                //If the command is in the history, print yes
                cout << "Yes" << endl;
            } else{
                //If the command is not in the history, print no
                cout << "No" << endl;
            }
        } 
        else if(input[0] == "hellotext"){
            //If the command is hellotext, open gedit
            system("gedit");
        } 
        else if(input[0] == "printfile"){ 
            //If the command is printfile
            if(input.size() == 2){
                //If the command has one argument, print the file at the path
                readFile(deleteFromString(input[1], '"', '"'));
                
            } else if(input.size() == 4) {
                //If the command has three arguments, copy the file at the first path to the second path
                if(input[2] == ">"){
                    copyFile(deleteFromString(input[1], '"', '"'), deleteFromString(input[3], '"', '"'));
                }  
            }
        } 
        else if(input[0] == "exit"){
            //If the command is exit, exit the program
            return 0;
        } 
        else{
            //If the command is not valid, print an error message
            cout << "Command not found" << endl;
        }
    }
    return 0;
    
}

