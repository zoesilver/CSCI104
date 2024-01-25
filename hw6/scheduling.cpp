#include "avlbst.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template<typename Key, typename Value>
bool assignNextCourse(
        AVLTree<Key, Value>& existingAs,
        std::vector<int>& courses,
        int slots,
        const std::vector<std::vector<int>>& data);

template<typename Key, typename Value>
bool possSoFar(AVLTree<Key, Value>& existingAs, const std::vector<std::vector<int>>& data);

int main(int argc, char* argv[]) {
    std::ifstream inputFile(argv[1]);
    std::string firstLine;
    std::getline(inputFile, firstLine);
    std::istringstream iss(firstLine);
    int coursesNum, students, slots;
    iss >> coursesNum >> students >> slots;

    // Parse through all info
    std::vector<std::vector<int>> data;
    std::vector<int> coursesTA;
    std::vector<int> coursesTotal;

    string line;
    while (getline(inputFile, line)) {
        vector<int> registered;
        istringstream iss(line);
        string studentName;
        iss >> studentName;
        int code;
        while (iss >> code) {
            registered.push_back(code);
            bool exists = false;
            for (int i = 0; i < coursesTA.size(); i++) {
                if (coursesTA[i] == code) {
                    exists = true;
                    break;
                }
            }
            if (exists == false) {
                // Courses contains all unique courses
                coursesTA.push_back(code);
                coursesTotal.push_back(code);
            }
        }
        // Data contains every course each student is taking
        data.push_back(registered);
    }
    // All data is now read in

    // Create AVL tree
    AVLTree<int, int> existingAs;

    if (assignNextCourse(existingAs, coursesTA, slots, data)) {
        sort(coursesTotal.begin(), coursesTotal.end());

        for (int i = 0; i < coursesTotal.size(); i++) {
            cout << existingAs.find(coursesTotal[i])->first << ' ' << existingAs.find(coursesTotal[i])->second << endl;
        }
    } else {
        cout << "No valid solution" << endl;
    }

    return 0;
}

template<typename Key, typename Value>
bool assignNextCourse(
        AVLTree<Key, Value>& existingAs,
        std::vector<int>& coursesTA,
        int slots,
        const std::vector<std::vector<int>>& data) {
    if (coursesTA.size() == 0) {
        return true;
    } else {
        for (int i = 1; i <= slots; i++) {
            if (possSoFar(existingAs, data)) {
                int examinedCourse = coursesTA.front();
                coursesTA.erase(coursesTA.begin());
                existingAs.insert(std::make_pair(examinedCourse, i));
                bool possible = assignNextCourse(existingAs, coursesTA, slots, data);
                if (possible) {

                    return true;
                }
                coursesTA.insert(coursesTA.begin(), examinedCourse);
                existingAs.remove(examinedCourse);
            }
        }
        return false;
    }
}

template<typename Key, typename Value>
bool possSoFar(AVLTree<Key, Value>& existingAs, const std::vector<std::vector<int>>& data) {
    for (int i = 0; i < data.size(); i++) {
        vector<int> filled;
        // Go through all courses a student is taking
        for (int j = 0; j < data[i].size(); j++) {
            auto result = existingAs.find(data[i][j]);
            // if course is in the tree
            if (result != existingAs.end()) {
                // get the time it's scheduled for
                int time = result->second;
                for (int k = 0; k < filled.size(); k++) {

                    // if time is already in filled, time has already been filled, so this is not possible
                    if (time == filled[k]) {
                        return false;
                    }
                }
                filled.push_back(time);
            }
        }
    }
    return true;
}
