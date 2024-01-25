#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

int main() {
    // initialize set with letter as key and frequency as value
    map<char, int> letterFrequencies;
    string line;
    while (getline(cin, line)) {
        for (char c : line) {
            if (isalpha(c)) {
                // case insensitive now
                c = tolower(c);
                if (letterFrequencies.find(c) != letterFrequencies.end()) {
                    // If the character exists in letterFrequencies, increment its frequency by one
                    letterFrequencies[c]++;
                } else {
                    // If the character is not in letterFrequencies, create a new key and set the value to 1
                    letterFrequencies[c] = 1;
                }
            }
        }
    }

    // Sort the letter frequencies by frequency in descending order
    vector<pair<char, int>> sortedFrequencies(letterFrequencies.begin(), letterFrequencies.end());
    std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second > rhs.second;
    });

    // Assign beauty values from 26 to 1 based on frequency
    int beautyValue = 26;
    for (auto& entry : sortedFrequencies) {
        entry.second = beautyValue;
        beautyValue--;
    }
    int sum = 0;
    for (const auto& entry : sortedFrequencies) {
        sum += entry.second
               * letterFrequencies[entry.first];  // Multiply beauty value by the frequency and add it to the sum
    }
    cout << sum << endl;

    return 0;
}
