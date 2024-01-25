#include "MinHeap.h"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
using namespace std;
int main(int argc, char* argv[]) {
    int leaves = 0;
    // get length of first word
    leaves = strlen(argv[1]);
    string startWord = argv[1];
    string endWord = argv[2];
    // captialize endWord and startWord
    transform(endWord.begin(), endWord.end(), endWord.begin(), ::toupper);
    transform(startWord.begin(), startWord.end(), startWord.begin(), ::toupper);
    // create set of valid words
    set<string> validWords;
    // get valid word file
    const char* filename = argv[3];
    ifstream inputFile(filename);
    string line;
    // for each word, add to validWords set
    while (getline(inputFile, line)) {
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        validWords.insert(line);
    }
    inputFile.close();
    // number of leaves will be the length of the original string because each char can be edited in any given word
    MinHeap<myPriorityNode<string>> heap(2);
    heap.add(myPriorityNode<string>(startWord, 0), 0);
    // keep track of used words
    set<string> usedWords;
    // currentWord starts with first word
    string currentWord = startWord;
    // indicate that this word is being used
    usedWords.insert(currentWord);
    // keep track of how many times current word is changed
    int steps = 0;
    // while currentWord is not the final word
    while (currentWord != endWord) {
        // get path info from top word before removing word
        myPriorityNode<string> tempPair = heap.peek();
        // remove top word from heap
        heap.remove();
        // for all letters
        for (int i = 0; i < leaves; i++) {
            string temp = currentWord;
            // change one letter in the current word
            for (int j = 0; j < 26; j++) {
                char letter = 'A' + j;
                if (letter != temp[i]) {
                    temp[i] = letter;
                    // check that word is valid and has not been used before
                    if ((usedWords.count(temp) == 0) && (validWords.count(temp) != 0)) {
                        // get length of path to get to word
                        int gVal = tempPair.priority + 1;
                        int hVal = 0;
                        for (int j = 0; j < leaves; j++) {
                            // for every char not in end word, add one to distance
                            if (temp[j] != endWord[j]) {
                                hVal++;
                            }
                        }
                        int priority = (gVal + hVal) * (leaves + 1) + hVal;
                        // add new item to heap list
                        heap.add(myPriorityNode<string>(temp, gVal), priority);
                    }
                }
            }
        }
        usedWords.insert(currentWord);
        // if there's nothing left to search through, there is no possible way to get from start to end word
        if (heap.isEmpty()) {
            cout << "No transformation" << endl;
            cout << usedWords.size() << endl;
            exit(0);
        }
        // set new current word
        currentWord = heap.peek().item;
    }
    // steps is the word path length of end word
    steps = heap.peek().priority;
    cout << steps << endl;
    cout << usedWords.size() << endl;
    return 0;
}
