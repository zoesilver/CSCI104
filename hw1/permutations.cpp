#include <iostream>
#include <string>
using namespace std;

void permutationsHelper(std::string originalLetters, size_t numLetters, std::string scrambled){
    if (scrambled.length()==numLetters){
        cout<<scrambled<<endl;
    }
    else{
        for (size_t i = 0; i<numLetters; i++){
            permutationsHelper(originalLetters, numLetters, scrambled+originalLetters[i]);
        }
    }
}

void permutations(std::string in){
    //number of letters in the original string
    size_t n = in.length();
    permutationsHelper(in, n,"");
}


