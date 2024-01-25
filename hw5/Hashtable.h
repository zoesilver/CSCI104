#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <utility>
#include <vector>
#include <ctime>

template<typename T>

class Hashtable {
public:
    Hashtable(bool debug = false, unsigned int size = 11) : debug(debug) {
        m_size = size;
        // dynamically allocate
        table = new std::pair<std::string, T>[m_size];
        // set default values in table
        for (int i = 0; i < m_size; ++i) {
            table[i] = std::make_pair(" ", T());
        }

        //  Initialize r1, r2, r3, r4, r5 based on the debug flag
        if (debug) {
            r1 = 983132572;
            r2 = 1468777056;
            r3 = 552714139;
            r4 = 984953261;
            r5 = 261934300;
        } else {
            // seed random numbers
            srand(time(NULL));
            // Generate random numbers between 0 and m-1
            r1 = rand() % size;
            r2 = rand() % size;
            r3 = rand() % size;
            r4 = rand() % size;
            r5 = rand() % size;
        }
    }

    ~Hashtable() { delete[] table; 
    }
    int add(std::string k, const T& val) {
        //reset variables
        noResize = true;
        noCollision = true;
        // update number of used spots
        used++;
        //  if load factor exceeds .05, resize
        if (used * 2 >= m_size) {
            noResize = false;
            resize();
        }

        number = 0;
        // find initial psuedo-random index
        int spot = hash(k);
        int loc = spot;
        //if there's a collision, indicate this in the noCollision variable
        if (table[loc].first != " "){
            noCollision = false;

        }
        // use quad probing to find an empty space
        // while the spot on the table is filled, increase the value that is being checked
        while (table[loc].first != " ") {
            number++;
            //track number of collisions
            collisions++;
            // make sure loc is between 0 and m_size-1
            loc = static_cast<int>(spot + pow(number, 2)) % static_cast<int>(m_size);
        }
        //  set empty spot to pair that's being added
        table[loc].first = k;
        table[loc].second = val;

                return number;

    }
    const T& lookup(const std::string& k) const {
        // iterate through all items in table, seeing if any match the string
        for (int i = 0; i < m_size; i++) {
            if (k == table[i].first) {
                return table[i].second;
            }
        }
        int check = m_size - 1;
        // if no matches, find last value and return
        while (table[check].first == " ") {
            check--;
        }
        return table[check].second;
    }

    void reportAll(std::ostream& out) const {
        // if there is an item in that index, print it out
        for (int i = 0; i < m_size; i++) {
            if (table[i].first != " ") {
                out << table[i].first << ' ' << table[i].second << std::endl;
            }
        }
    }
    int hash(const std::string& k) const {
        // as will hold each set of 6 chars
        std::vector<int> as;
        // ws will hold up to 5 sets of 6 processed digits
        std::vector<int> ws;
        // all spots will be set as 0 until filled
        for (int i = 0; i < 5; i++) {
            ws.push_back(0);
        }
        int a = 0;
        // figure out how many slots in ws will be filled (vs leading 0s)
        int toFill = k.length() % 6;
        if (toFill != 0) {
            toFill = k.length() / 6 + 1;
        } else {
            toFill = k.length() / 6;
        }
        // keep track how many terms have been added to ws
        int alreadyFilled = 0;
        // get chunks of 6 chars at a time starting at the end of the word
        for (int x = k.length() - 6; x >= -5; x = x - 6) {
            int start;
            int end;
            // if there are more than 6 chars left, parse 6 chars
            if (x >= 0) {
                start = x;
                end = 6;
            } else {
                // if less than 6 chars left, parse through only remaining ones
                end = x + 6;
                start = 0;
            }
            // get rid of leftover items
            as.clear();
            // parse through 6 chars
            for (int j = start; j < start + end && j < int(k.length()); j++) {
                // find integer value of each char and add to as array;
                for (int i = 0; i < 26; i++) {
                    if (k[j] == 'a' + i) {
                        char letter = 'a' + i;
                        int integer = letter - a - 96;
                        // add integer to as
                        as.push_back(integer);
                    }
                }
            }

            // if there are not 6 digits to fill as, add leading 0s
            while (as.size() != 6) {

                as.insert(as.begin(), 0);
            }

            // calculate 27^5 a1 + 27^4 a2 + 27^3 a3 + 27^2 a4 + 27 a5 + a6
            long long sum = 0;
            for (int z = 0; z < 6; z++) {
                sum = sum + pow(27, 5 - z) * as[z];
            }
            // add sum to ws
            ws[4 - alreadyFilled] = sum;
            alreadyFilled++;
        }

        // add leading 0s if ws does not have 5 items
        while (ws.size() != 5) {
            ws.insert(ws.begin(), 0);
        }
        // calculate (r1 w1 + r2 w2 + r3 w3 + r4 w4 + r5 w5) % m
        long long bigSum = 0;
        for (int i = 0; i < int(ws.size()); i++) {
            if (i == 0) {
                bigSum += r1 * static_cast<long long>(ws[i]);
            } else if (i == 1) {
                bigSum += r2 * static_cast<long long>(ws[i]);
            } else if (i == 2) {
                bigSum += r3 * static_cast<long long>(ws[i]);
            } else if (i == 3) {
                bigSum += r4 * static_cast<long long>(ws[i]);
            } else {
                bigSum += r5 * static_cast<long long>(ws[i]);
            }
        }
        long long hashVal = bigSum % m_size;
        return static_cast<int>(hashVal);
    }
    std::string randomStringGen() {
    static const char choices[] = "abcdefghijklmnopqrstuvwxyz";
    static bool seeded = false;

    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    std::string random_string;

    for (int i = 0; i < 28; ++i) {
        int letter = rand() % 26;
        random_string += choices[letter];
    }


    return random_string;
}
    int getUnder23(){
        return under23;
    }
        int getOver23(){
        return over23;
    }
    bool isNoResize(){
        return noResize;
    }
        bool isNoCollision(){

        return noCollision;
    }
    void setNoResize(){
        noResize = true;
    }
    void setNoCollision(){
        noCollision = true;
    }
    int getNumCollisions(){
        return collisions;
    }
    int getSize(){
        return m_size;
    }
    int getNumber(){
        return number;
    }

private:
    void resize() {
        //  sequence of resizing
        std::vector<int> growth
                = {0,
                   11,
                   23,
                   47,
                   97,
                   197,
                   397,
                   797,
                   1597,
                   3203,
                   6421,
                   12853,
                   25717,
                   51437,
                   102877,
                   205759,
                   411527,
                   823117};
        // increase size to next number in the sequence
        for (int i = 0; i < int(growth.size()); i++) {
            if (m_size == growth[i]) {
                newSize = growth[i + 1];
                break;
            }
            // if size is between numbers in the growth array, set it to the next biggest number in the array
            else if (m_size < growth[i] && m_size > growth[i - 1]) {
                newSize = growth[i];
                break;
            }
        }
        //  store old size
        int oldSize = m_size;
        // update m_size
        m_size = newSize;
        // create new table with updated size
        std::pair<std::string, T>* updatedTable;
        updatedTable = new std::pair<std::string, T>[newSize];
        // set all terms in updatedTable to default
        for (int i = 0; i < newSize; i++) {
            updatedTable[i] = std::make_pair(" ", T());
        }
        // search through old table
        for (int i = 0; i < oldSize; i++) {
            if (table[i].first != " ") {
                //  rehash strings
                int loc = hash(table[i].first);
                int number2 = 0;
                int loc2 = loc;
                // check for collision
                while (updatedTable[loc2].first != " ") {
                    //collisions++;
                    number2++;
                    // make sure index is within range 0 to m_size-1
                    loc2 = static_cast<int>(loc + pow(number2, 2)) % m_size;
                }
                //  set empty spot to pair that's being added
                updatedTable[loc2].first = table[i].first;
                updatedTable[loc2].second = table[i].second;
            }
        }

        // delete table
        delete[] table;
        // new table is now called table
        table = updatedTable;

        // Initialize r1, r2, r3, r4, r5 based on the debug flag
        if (debug) {
            r1 = 983132572;
            r2 = 1468777056;
            r3 = 552714139;
            r4 = 984953261;
            r5 = 261934300;
        } else {
            // seed random numbers
            srand(time(NULL));
            // Generate random numbers between 0 and m-1
            r1 = rand() % m_size;
            r2 = rand() % m_size;
            r3 = rand() % m_size;
            r4 = rand() % m_size;
            r5 = rand() % m_size;
        }

    }

    std::pair<std::string, T>* table;

    long long r1, r2, r3, r4, r5, m_size, newSize;
    int used = 0;
    int resizings = 0;
    bool debug;
    bool noResize = true;
    bool noCollision =true;
    int additions =0;
    int under23 =0;
    int over23 =0;
    int collisions =0;
    int number;
    std::vector<int> randomNums;
};
#endif
