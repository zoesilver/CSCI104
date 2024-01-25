#include "Hashtable.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

int main() {
    int under23 = 0;
    int over23 = 0;
    for (int i = 0; i < 100000; i++) {
        // Create instance of class containing pair of string and int

        Hashtable<int> birthdayTable(true, 365);
        int adds = 0;
        // reset variables
        birthdayTable.setNoResize();
        birthdayTable.setNoCollision();

        while (birthdayTable.isNoResize() && birthdayTable.isNoCollision()) {

            std::string random = birthdayTable.randomStringGen();
            birthdayTable.add(random, adds);
            adds++;
        }
        if (adds <= 23) {
            under23++;
        } else {
            over23++;
        }
    }
    cout << "Percentage under 23: " << static_cast<long long>(under23) * 100.0 / (under23 + over23) << endl;
    double bigSum = 0.0;
    int runs = 1000;
    double expected;
    int bigProbes = 0;

    for (int k = 0; k < runs; k++) {
        int totalCollisions = 0;
        Hashtable<std::string> probeTable(true, 11);
        int numToAdd = 100;
        int biggestProbe = 0;

        for (int i = 0; i < numToAdd; i++) {
            std::string random = probeTable.randomStringGen();
            probeTable.add(random, random);
            if (probeTable.getNumber() > biggestProbe) {
                biggestProbe = probeTable.getNumber();
            }
        }

        totalCollisions = probeTable.getNumCollisions();
        bigProbes += biggestProbe;
        double average = static_cast<double>(totalCollisions) / numToAdd;

        expected = 2.0 / 3.0;
        bigSum += average;
    }
    cout << "Actual overall value: " << std::fixed << std::setprecision(5) << static_cast<double>(bigSum) << endl;
    cout << "Expected overall value: " << std::fixed << std::setprecision(5) << expected * runs << " or less" << endl;
    cout << "Average biggest probe: " << static_cast<double>(bigProbes) / runs << endl;

    return 0;
}
