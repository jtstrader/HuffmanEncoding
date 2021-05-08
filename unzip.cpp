#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<map>

using namespace std;

bool checkFile(const char* name); // checks if file is open/exists and if it ends in a .zip
void createOutput(const char* fileName, string output); // creates output file based on given output
string decode(const char* fileName, string &outFileName); // generates the decoded message from the input file

int main(int argc, char** argv) {
    // no input file
    if(argc==1) cerr<<endl<<"Error. No input file detected."<<endl;
    
    // decode
    else if(checkFile(argv[1])) {
        string outFileName;
        string output = decode(argv[1], outFileName);
        createOutput(outFileName.c_str(), output);
        cout<<"File Sucessfully Unzipped."<<endl;
    }
    //
    // no need for else/error statement here. errors handled in checkFile(const char* name); 
    //
    
    return 0;
}

// check to see if:
// 1. file ends in .zip
// 2. file can be opened
bool checkFile(const char* name) {
    string fileName = name; // conversion to string
    string extension = fileName.substr(fileName.length()-4, 4); // grabs .zip extension

    // check extension
    if(extension!=".zip") {
        cerr<<endl<<"Error. Cannot unzip a non .zip file."<<endl;
        return false;
    }

    // open file
    ifstream file;
    file.open(name);
    if(!file.is_open()) {
        cerr<<endl<<"Error. File could not be found."<<endl;
        return false;
    }

    // pass all tests
    return true;
}

// decode the message inside the input file
string decode(const char* inFileName, string &outFileName) {
    // creates output file name (removing the extension at the end)
    // create file object
    string fileName = inFileName;
    outFileName = fileName.substr(0, fileName.length()-4);
    ifstream inFile;
    inFile.open(inFileName);

    // input var and case var
    string input;
    int cases;

    // num cases                
    getline(inFile, input);
    cases = atoi(input.c_str());

    // map
    map<string, char> unzip;
    int letter; 
    string code;

    // input items into a map
    for(int i=0; i<cases; i++) {
        inFile>>letter;
        inFile>>code;
        unzip[code] = char(letter); 
    }

    // decode message
    inFile>>input;
    string output; code = "";
    for(int i=0; i<input.length(); i++) {
        code+=input[i];
        if(unzip.find(code)!=unzip.end()) {
            output+=unzip[code];
            code = "";
        }
    }
    inFile.close(); // close input file
    return output;
}

// create output file
void createOutput(const char* outFileName, string output) {
    // create new output file
    ofstream outFile;
    outFile.open(outFileName); // 
    outFile<<output;
    outFile.close(); // close output file
}