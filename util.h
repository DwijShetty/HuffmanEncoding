//
// STARTER CODE: util.h
//
// TODO:  Write your own header and fill in all functions below.
//

#include <iostream>
#include <fstream>
#include <map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "hashmap.h"
#include "mymap.h"

#pragma once

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode *zero;
    HuffmanNode *one;
};

class prioritize {
public:
    bool operator()(const HuffmanNode *p1, const HuffmanNode *p2) const {
//
// if you want smaller counts @ the end, return true if
// p1's count < p2's count. if you want larger counts
// @ the end, return true if p1's count > p2's count.
//
        return p1->count > p2->count;
    }
};

void _free(HuffmanNode *node) {
    if (node == nullptr) {
        return;
    }
    _free(node->zero);
    _free(node->one);
    delete node;
}

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode *node) {
    _free(node);
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
    if (isFile) {
        ifstream aFile(filename);
        if (!aFile.is_open()) {
            throw runtime_error("Error: unable to open '" + filename + "'");
        }
        char c;
        while (aFile.get(c)) {
            if (map.containsKey(c)) {
                map.put(c, map.get(c) + 1);
            } else {
                map.put(c, 1);
            }
        }
    } else {
        for (char i : filename) {
            if (map.containsKey(i)) {
                map.put(i, map.get(i) + 1);
            } else {
                map.put(i, 1);
            }
        }
    }

    map.put(PSEUDO_EOF, 1);
}

//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode *buildEncodingTree(hashmap &map) {
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, prioritize> pq;
    for (int key: map.keys()) {
        auto *hn = new HuffmanNode{.character = key,
                .count = map.get(key),
                .zero = nullptr,
                .one = nullptr};
        pq.push(hn);
    }
    while (pq.size() != 1) {
        HuffmanNode *zero = pq.top();
        pq.pop();
        HuffmanNode *one = pq.top();
        pq.pop();
        auto *nHn = new HuffmanNode{.character = NOT_A_CHAR,
                .count = zero->count + one->count,
                .zero = zero,
                .one = one};
        pq.push(nHn);
    }
    return pq.top();
}

void _buildEncodingMap(HuffmanNode *n, string &path, mymap<int, string> *eMap) {
    if (n->character != NOT_A_CHAR) {
        eMap->put(n->character, path);
        return;
    }
    string path0 = path + "0";
    string path1 = path + "1";

    _buildEncodingMap(n->zero, path0, eMap);
    _buildEncodingMap(n->one, path1, eMap);
}

//
// *This function builds the encoding map from an encoding tree.
//
mymap<int, string> buildEncodingMap(HuffmanNode *tree) {
    mymap<int, string> encodingMap;

    string path;
    _buildEncodingMap(tree, path, &encodingMap);


    return encodingMap;
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream &input, mymap<int, string> &encodingMap,
              ofbitstream &output, int &size, bool makeFile) {
    char c;
    size = 0;
    string strOut;
    string eof = encodingMap.get(PSEUDO_EOF);
    while (input.get(c)) {
        string eStr = encodingMap.get(c);
        strOut += eStr;
        if (makeFile) {
            for (char i: eStr) {
                output.writeBit(i == '0' ? 0 : 1);
                size++;
            }
        }
    }
    if (makeFile) {
        for (char i: eof) {
            output.writeBit(i == '0' ? 0 : 1);
            size++;
        }
    }
    strOut += encodingMap.get(PSEUDO_EOF);
    return strOut;
}


//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode *encodingTree, ofstream &output) {
    string strOut;
    HuffmanNode* current = encodingTree;
    while (!input.eof()) {
        int b = input.readBit();
        if (b == 1) {
            current = current -> one;
        } else if (b == 0) {
            current = current -> zero;
        }
        char dChar = current->character;
        if (current -> character != NOT_A_CHAR
        && current -> character != PSEUDO_EOF) {
            strOut += (char) dChar;
            output.put((char) dChar);
            current = encodingTree;
        } else if (current -> character == PSEUDO_EOF) {
            break;
        }
    }
    return strOut;
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string &filename) {
    hashmap map;
    buildFrequencyMap(filename, true, map);
    HuffmanNode *node = buildEncodingTree(map);
    mymap<int, string> encodingMap = buildEncodingMap(node);
    ofbitstream outFile(filename + ".huf");
    outFile << map;
    int size;
    ifstream aFile(filename);
    string result = encode(aFile, encodingMap, outFile, size, true);
    freeTree(node);
    return result;
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string &filename) {
    string change;
    for (char c : filename){
        if (c == '.'){
            break;
        }
        change += c;
    }
    ifbitstream input(filename);
    ofstream output(change += "_unc.txt");
    hashmap freqMap;
    input >> freqMap;
    HuffmanNode *encodingTree = buildEncodingTree(freqMap);
    string str = decode(input, encodingTree, output);
    freeTree(encodingTree);
    return str;
}
