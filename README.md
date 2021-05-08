# Huffman Encoding Tree
Creates a Huffman Encoding Tree using input data to create a binary file to minimize data used in storage. Has zipping and unzipping functionality. The project's encoding is contained within huffman.cpp and it's specification file huffman.h, in which the methods are described at the bottom of this README.

# Example Test File for Input

    ZIP FILE FOR "MERCER TENNIS\n"
    10
    10 1010
    32 1011
    67 1100
    69 01
    73 1101
    77 1110
    78 001
    82 100
    83 1111
    84 000
    111001100110001100101100001001001110111111010

# Encoding Table of "MERCER TENNIS\n"

           ####  ENCODING TABLE FOR FILE  ####

        Index   Char    Weight  Parent  ChildType
        -----------------------------------------
        0       nl      1       10      0
        1       sp      1       10      1
        2       C       1       11      0
        3       E       3       16      1
        4       I       1       11      1
        5       M       1       12      0
        6       N       2       13      1
        7       R       2       14      0
        8       S       1       12      1
        9       T       1       13      0
        10      T1      2       14      1
        11      T2      2       15      0
        12      T3      2       15      1
        13      T4      3       16      0
        14      T5      4       17      0
        15      T6      4       17      1
        16      T7      6       18      0
        17      T8      8       18      1
        18      T9      14      0       -1

# Greedy Algorithm
The greedy algorithm looks for the two smallest nodes (either characters nodes or total nodes),
and adds them together to form a new total node. The paths between said character nodes and total nodes
are used to create the codes for the encoding. 

# Uncompression Process
I plan on uncompressing the file by taking in the input from the file and assigning each ASCII value to a key
in the map (the code). Then I will go through the entire coded message at the bottom, adding a new index to the end
of the check string when needed. When a match is finally found, that char in the map is printed to a file and the check
string is reset for further use. Finally, I can close the inFile and outFile objects and my uncompression is done.


# Huffman Class
===public methods===

insert(char ch, int weight): inserts a character into the tree. Sets weight char type, but does not set
parent and ChildType

inTree(char ch): checks if an item is in the tree

GetFrequency(char ch): returns the weight of a character. Calls on GetFrequency(int i) to search through vector
for the character of that given index. Uses lookUp(char ch) to find the index of that character to be used in the
int version of the GetFrequency function

build(): builds the Huffman tree using the greedy algorithm, and will be used along with GetCode to get the code
of a char to be used in the encoding.

GetCode(char ch): returns code of a given char. Uses lookUp(char ch) to grab index and use in the call for GetCode(int i)
Used after build() to grab the codes for the inputted char to encode the data.

PrintTable(): prints the encoding table for the user (flag "--t")

numNodes(): returns the number of nodes in the Huffman tree for the compression calculation (and possible for any for loops
in the algorithm)

===private methods===
vector<HNode> nodes: keeps track of all nodes. Any total node is defined as a HNode with char '\0'

numChars: the number of chars (NOT TOTAL NODES) currently in the nodes vector. Increments on every call to insert()

int lookUp(char ch): look up a given char and returns its index using a for loop through the numChars

GetFrequency(int i): takes an integer and returns the frequency of a char at the index of i in the vector of HNodes

GetCode(int i): takes an integer and returns the code of a char at index i in the vector of HNodes
