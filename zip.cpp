#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
#include "huffman.h"

using namespace std;

void PrintLetters(const int Letters[]); // prints letters
void CountLetters(int Letters[], ifstream &inFile, string &s); // counts letters and returns file string
void bitRatio(int Letters[], HuffmanTree &ht); // calculates compression ratio
string toString(int &i); // convert int to string
bool checkArgs(int argc, char** argv, bool &printT); // check arguments for --help or --t, and call checkFile function to open file
bool checkFile(const char* fileName); // checks to see if the file can be opened
void printHelp(); // prints help message to user
void encode(const char* fileName, bool printT); // build tree and call on output

int main(int argc, char** argv) {
    // check validity of arguments
    bool printT;
    if(checkArgs(argc, argv, printT)) {
        string outFileName;
        encode(argc==2?argv[1]:argv[2], printT);
    }
}

// encodes string
void encode(const char* fileName, bool printT) {
    // create letter list
    const int numLetters = 256;
    int Letters[numLetters];
    string outFileName;
    
    // create file and new file name
    ifstream inFile;
    inFile.open(fileName);
    outFileName = fileName; outFileName+=".zip";

    // create huffman tree
    string input;
    CountLetters(Letters, inFile, input);
    HuffmanTree ht;
    for(int i=0; i<126; i++)  // insert every letter into vector
        if(Letters[i]>0)
            ht.insert((char)i, Letters[i]);
    
    // build tree
    ht.build();

    // encode and create output
    ofstream outFile;
    outFile.open(outFileName.c_str());
    
    string output;
    int numChars = (ht.numNodes()+1)/2; // number of characters
    output+=toString(numChars); output+='\n';

    // print out chars w/ codes
    for(int i=0; i<126; i++) {
        if(Letters[i]>0) {
            output+=toString(i);
            output+=" ";
            output+=ht.GetCode((char)i);
            output+='\n';
        }
    }

    for(int i=0; i<input.length(); i++)
        output+=ht.GetCode(input[i]);

    outFile<<output;
    inFile.close();
    outFile.close();

    // print table (if printT is true)
    if(printT) ht.PrintTable();        
    bitRatio(Letters, ht);
}

// checks if file can be opened
bool checkFile(char* fileName) {
    ifstream inFile;
    inFile.open(fileName);
    
    bool open = inFile.is_open();
    
    // file found
    if(open) {
        inFile.close();
        return true;
    }

    // file is not open (not found)
    else {
        cout<<"Error: File not found."<<endl;
        inFile.close();
        return false;
    }
}

// prints help message to user
void printHelp() {
    cout<<"~~myZip Usage~~"<<endl;
    cout<<"Syntax: ./myZip [--help] [--t] [file name]"<<endl<<endl;
    cout<<"--help: prints help menu. will override a zip command if file name is present."<<endl;
    cout<<"--t: prints the encoding table used to zip the file"<<endl;
}

// check arguments for --t/--help
bool checkArgs(int argc, char** argv, bool &printT) {

    // if --t, continue as normal
    // if --help, return FALSE to break code but print out the help message
    // handle all error messages in here

    ifstream inFile;
    printT = false;

    // no input file detected
    if(argc==1) {
        cerr<<endl<<"Error. No input file detected."<<endl;
        return false;
    }

    // 2 arguments detected
    // command + fileName OR command + --help
    else if(argc==2) {
        // flag detected
        if(argv[1][0]=='-'&&argv[1][1]=='-') {
            // --help check
            if(strcmp(argv[1], "--help")==0) {
                printHelp();
                return false;
            }

            // --t check
            else if(strcmp(argv[1], "--t")==0) {
                cerr<<"Invalid syntax. Cannot print encoding table with no file provided!"<<endl;
                return false;
            }

            // invalid flag usage
            else {
                cerr<<"Invalid flag usage. Please use ./myZip --help for more info."<<endl;
                return false;
            }
        }

        // no flag detected. search for file and compress
        // catch a .zip file and return false. 
        else {
            string file = argv[1];
            if(file.length()<4) return checkFile(argv[1]);
            else if(file.substr(file.length()-4, 4)==".zip") {
                cerr<<"Error: cannot zip a .zip file!"<<endl;
                return false;
            }
            return checkFile(argv[1]);
        }
    }

    // 3 arguments detected
    // command + flag + file
    else if(argc==3) {
        if(argv[1][0]=='-'&&argv[1][1]=='-') {
            // --help check
            if(strcmp(argv[1], "--help")==0) {
                printHelp();
                return false;
            }

            // --t check
            else if(strcmp(argv[1], "--t")==0) {
                string file = argv[2];
                if(file.length()<4) return checkFile(argv[2]);
                else if(file.substr(file.length()-4, 4)==".zip") {
                    cerr<<"Error: cannot zip a .zip file!"<<endl;
                    return false;
                }
                printT = true;
                return checkFile(argv[2]);
            }

            // invalid flag usage
            else {
                cerr<<"Invalid flag usage. Please use ./myZip --help for more info."<<endl;
                return false;
            }
        }
    }

    // if all other cases are ignored then print and error and return false
    cerr<<"Invalid syntax. Please use ./myZip --help for more info."<<endl;
    return false;
}

// function counts letters and returns string of file
// credit: Dr. Digh (slight changes from me)
void CountLetters (int Letters[], ifstream &inFile, string &out)
{
	char ch;
	for (char ch = char(0);  ch <= char(126);  ch++)
	    Letters[ch] = 0;

	inFile.get(ch);
	while (inFile) {
		Letters[ch] += 1;
        out+=ch;
		inFile.get(ch);
	}
}

// function prints letters from array
// credit: Dr. Digh
void PrintLetters (const int Letters[])
{
	for (char ch = char(0);  ch <= char(126);  ch++)
        if ( (Letters[ch] != 0) && (ch != '\n') && (ch != ' ') )
	        cout << "Char " << ch << " appearances  : " << Letters[ch] << endl;
        else if ( (Letters[ch] != 0) && (ch == '\n') )
            cout << "Char nl appearances : " << Letters[ch] << endl;
        else if ( (Letters[ch] != 0) && (ch == ' ') )
            cout << "Char sp appearances : " << Letters[ch] << endl;
}

// calculate compression ratio
void bitRatio(int Letters[], HuffmanTree &ht) {

    // calculate compression ratio by adding to sum
    float sum = 0, numChars = 0;
    for(int i=0; i<=126; i++) {
        if(Letters[i]>0) { // letter exists
            int f = ht.GetFrequency((char)i);
            int c = ht.GetCode((char)i).length();
            sum+=(f*c);
            numChars+=Letters[i];
        }
    }
    cout<<"\tFile Compressed into "<<sum<<" Bits. (%";
    printf("%.2f", (1-(sum/(numChars*8)))*100); // compression ratio equation
    cout<<" Less)"<<endl;
}

string toString(int &i) {
    stringstream s;
    s<<i;
    return s.str();
}
