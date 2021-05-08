#include "huffman.h"
#include <iostream>
using namespace std;

HuffmanTree:: HuffmanTree()
	: numChars(0), built(false) {}

HuffmanTree:: ~HuffmanTree() {
    nodes.clear();
}

void HuffmanTree:: insert(char ch, int weight) {
    HNode newNode = {ch, weight, -1, -1}; 
    built = false;
    nodes.push_back(newNode);
    numChars++;

}

bool HuffmanTree:: inTree(char ch) {
    
    // search through numChars, NOT nodes.size() as that
    // would include total nodes. return true if found,
    // false if not
    for(int i=0; i<numChars; i++)
        if(nodes[i].ch==ch)
            return true;
    return false;
}

int HuffmanTree:: GetFrequency(char ch) {

    // call look up to gather integer value to be passed into
    // private GetFrequency function
    return GetFrequency(lookUp(ch));
}

int HuffmanTree:: GetFrequency(int i) {

    // return weight of an item in the node list
    if(i==-1)  // if item not found
        return 0;
    return nodes[i].weight;
}

int HuffmanTree:: lookUp(char ch) {

    // search through numChars, NOT nodes.size() as that
    // would include total nodes. return positive int if found,
    // -1 if not found
    for(int i=0; i<numChars; i++)
        if(nodes[i].ch==ch)
            return i;
    return -1;
}

string HuffmanTree:: GetCode(char ch) {

    // call lookUp() to gather integer value to be passed into
    // private GetFrequency function
    return GetCode(lookUp(ch));
}


string HuffmanTree:: GetCode(int i) {
    
    // recursively go through vector to find path
    // of parents back to root for character code
    // returns empty string if item not found
    if(nodes[i].parent==0)
        return "";
    else
        return (GetCode(nodes[i].parent)+(char)(nodes[i].childType+'0'));
}

void HuffmanTree:: PrintTable() {
    
    int totalNode = 1; // track num of total nodes
    cout<<endl;
    cout<<"\t   ####  ENCODING TABLE FOR FILE  ####"<<endl<<endl;
    // CHARACTER | WEIGHT | PARENT | CHILD TYPE //

    cout<<"\tIndex\tChar\tWeight\tParent\tChildType"<<endl;
    cout<<"\t-----------------------------------------"<<endl;
    for(int i=0; i<nodes.size(); i++) {
        if(nodes[i].ch==' ')  // space
            cout<<'\t'<<(i)<<'\t'<<"sp"<<'\t'<<nodes[i].weight<<'\t'<<nodes[i].parent<<'\t'<<nodes[i].childType<<endl;
        else if(nodes[i].ch=='\n')  // new line
            cout<<'\t'<<(i)<<'\t'<<"nl"<<'\t'<<nodes[i].weight<<'\t'<<nodes[i].parent<<'\t'<<nodes[i].childType<<endl;
        else if(nodes[i].ch=='\0') {  // total node
            cout<<'\t'<<(i)<<'\t'<<"T"<<(char)(totalNode+'0')<<'\t'<<nodes[i].weight<<'\t'<<nodes[i].parent<<'\t'<<nodes[i].childType<<endl;
            totalNode++;
        }
        else  // regular char
            cout<<'\t'<<(i)<<'\t'<<nodes[i].ch<<'\t'<<nodes[i].weight<<'\t'<<nodes[i].parent<<'\t'<<nodes[i].childType<<endl;
    }
}

int HuffmanTree:: numNodes() {
    return nodes.size();
}

void HuffmanTree:: build() {

    if(nodes.size()==0) return;
    // define index vars:
    //    - i1: first lowest value
    //    - i2: second lowest value
    for(int i=0, i1=0, i2, low; i<2*numChars-2; i++) {
        // only run if the curr node does NOT have a parent (total node)
        if(nodes[i1].parent==-1) {
            // look for first lowest values to combine
            for(int r=1; r<nodes.size(); r++) {
                low = nodes[i1].weight;
                if(low>nodes[r].weight&&nodes[r].parent==-1)
                    i1 = r;   // new lowest value
            }

            // i2 cannot be the same as i1
            i2 = i1!=0?0:1;
            while(nodes[i2].parent!=-1||i2==i1) {
                i2++;
            }
            // look for second lowest value to combine
            for(int r=1; r<nodes.size(); r++) {
                low = nodes[i2].weight;
                if(low>nodes[r].weight&&nodes[r].parent==-1&&i1!=r) {
                    i2 = r;   // new lowest value
                }
            }

            // special case creation: root total node if last run (i = 2*numChars-4)
            /*
                Note to self: reasoning behind i = 2*numChars-4. There are 2C-1 items in vector at the end, 
                which implies 2C-2 iterations w/o the root  as there is a root note that is NOT combined. 
		        Therefore we run the for loop 2*numChars-2 times (technically I believe this could also work as 2C-3)

                When setting parent of the root to 0, we use 2C-4 because when using 2C-2, there is still
                a final check done where i = next to last potition in the list. We need to catch the run
                BEFORE that because that is the run where the final total node, the root, is created.
                Well the run before the final check of 2C-2 is 2C-4, as the final check of 2C-2 is 2C-3:

                2C-3 - 1 = 2C-4
            */
            HNode newNode = {'\0', nodes[i1].weight+nodes[i2].weight, -1, -1};
            nodes.push_back(newNode);
            nodes[i1].parent = nodes.size()-1; nodes[i1].childType = 0;
            nodes[i2].parent = nodes.size()-1; nodes[i2].childType = 1;
        }
        // reset i1
        i1 = i+1;
    }

    // root node: set parent to 0
    nodes[nodes.size()-1].parent = 0;
}
