#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
// Function to calculate GCD between two numbers
int calculateGCD(int a, int b);

// Function to find the decryption key value
long long findKey(int p, int q);

// Function to perform encryption
void encrypt(string command);

// Function to perform decryption
void decrypt(string command, long long key, int p, int q);

// Function to calculate GCD between two numbers
int calculateGCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find the decryption key value
long long findKey(int p, int q) {
    long long pm1 = p - 1;
    long long qm1 = q - 1;
    int gcdpm1qm1 = calculateGCD(pm1, qm1);
    long long L = pm1 * qm1 / gcdpm1qm1;

    if (L <= 65537) {
        cout << "Invalid command. Goodbye." << endl;
        exit(0);
    }
    // find d using extended Euclidean Algorithm
    long long t = 1, old_t = 0, r = 65537, old_r = L;
    while (r != 0) {
        long long quotient = old_r / r, temp = r;
        r = old_r - quotient * r;
        old_r = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }

    long long d = old_t;

    while (d < 0) {
        d += 65537 * L;
    }

    int e = 65537;
    // Calculate GCD between e and L
    int gcd = calculateGCD(e, L);

    if (gcd != 1) {
        cout << "Bad p and q choices. Goodbye.";
        exit(0);
    }

    return d;
}

// Function to perform encryption
void encrypt(string command) {
    istringstream iss(command);
    string encryptCall, filename, nVal, message;
    iss >> encryptCall >> filename >> nVal;
    // read rest of the line as message
    getline(iss, message);
    // remove first char
    message.erase(0, 1);
    // Cannot be properly read in
    if (iss.fail()) {
        cout << "Invalid ENCRYPT command format." << endl;
        return;
    }

    int n = stoi(nVal);
    if (n < 27) {
        cout << "Invalid 'n' value." << endl;
        return;
    }
    // Calculate how long each chunk will be
    int x = 1 + floor(log10(n / 27.0) / log10(100.0));
    string answer;
    int messageLength = message.length();
    // Calculate how many iterations will be needed
    int iterations = (messageLength + x - 1) / x;
    // Final will have all encrypted chunks appended to it
    string final;
    // chunk  up message into x-length iterations
    for (int i = 0; i < iterations; i++) {
        int start = i * x;
        string chunk = message.substr(start, x);
        answer = "";
        // Check if chunk is shorter than x and pad with spaces
        while (chunk.length() < x) {
            chunk += ' ';
        }
        // Convert each character in the chunk into integer
        for (char character : chunk) {
            int integer;
            if (character == ' ') {
                integer = 0;
            } else {
                integer = character - 'a' + 1;
            }

            if (integer < 10) {
                answer += '0';
            }
            // append integer to answer
            answer += to_string(integer);
        }
        // Use Modular Exponentiation Algorithm to calculate C
        long long C = 1;
        long long M = stoll(answer);
        long long e = 65537;
        long long nLL = n;
        M = M % n;

        while (e > 0) {
            if (e % 2 == 1) {
                C = (C * M) % nLL;
            }
            e = e / 2;
            M = (M * M) % n;
        }

        // add space between chunks if there are other chunks already in final
        if (final.length() != 0) {
            final += ' ';
        }
        // add this chunk to the final encrypted word
        final = final + to_string(C);

        // Clear C for the next chunk
        C = 1;
    }
    // Print the entire message
    ofstream ofile(filename);
    ofile << final;
    ofile.close();
}

void decrypt(string command, long long key, int p, int q) {
    // read in information
    istringstream iss(command);
    string decryptCall, infile, outfile;
    iss >> decryptCall >> infile >> outfile;

    if (iss.fail()) {
        cout << "Invalid DECRYPT command format." << endl;
        return;
    }

    string fileContents;
    string line;
    ifstream ifile(infile);
    while (getline(ifile, line)) {
        fileContents += line + "\n";
    }

    // Use a stringstream to read words from fileContents
    istringstream iss2(fileContents);
    istringstream iss3(fileContents);
    string word;
    string decryptedMessage;
    string final;
    string longestWord;

    // Find longest word
    while (iss2 >> word) {
        // Check if the current word is longer than the longestWord
        if (word.length() > longestWord.length()) {
            longestWord = word;
        }
    }
    long long n = p * q;
    // Calculate how long each chunk will be
    int x = 1 + floor(log10(n / 27.0) / log10(100.0));
    while (iss3 >> word) {
        // Use Modular Exponentiation Algorithm to calculate M
        long long C = stoll(word);
        long long M = 1;
        long long d = key;

        while (d > 0) {
            if (d % 2 == 1) {
                M = (M * C) % n;
            }
            C = (C * C) % n;
            d = d / 2;
        }
        for (int i = 0; i < x; i++) {
            // if chunk is shorter than longest chunk, add leading zeros
            string MStr = to_string(M);
            int l = word.length();

            while (l < longestWord.length()) {
                MStr = '0' + MStr;
                l++;
            }

            int digit = stoi(MStr) % 100;
            char character = ' ';
            // if it is a non-zero, convert and add, if it is zero, default is already a space so don't do anything more
            if (digit >= 1 && digit <= 26) {
                character = static_cast<char>('a' + digit - 1);
            }

            decryptedMessage = character + decryptedMessage;

            M = M / 100;
        }
        // Append the fully constructed decryptedMessage to final
        final = final + decryptedMessage;
        // Clears the decryptedMessage string
        decryptedMessage.clear();
    }
    // Print the entire message
    ofstream ofile(outfile);
    ofile << final;
    ofile.close();
}

int main(int argc, char* argv[]) {
    int p = stoi(argv[1]);
    int q = stoi(argv[2]);
    long long key = findKey(p, q);
    string command;
    while (command != "EXIT") {
        cout << "Enter a command: ";
        getline(cin >> ws, command);
        if (command.substr(0, 7) == "DECRYPT") {
            decrypt(command, key, p, q);
        } else if (command.substr(0, 7) == "ENCRYPT") {
            encrypt(command);
        } else if (command.substr(0, 4) == "EXIT") {
            cout << "Goodbye. Have a lovely day." << endl;
            return 0;
        } else {
            cout << "Invalid command. Try again." << endl;
            exit(0);
        }
    }
}
