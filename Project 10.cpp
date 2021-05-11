// Project 10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <filesystem>
#include "Parser.h"
#include "Token.h"
#include "Scanner.h"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Error:Incorrect number of command line arguments. Expected 2 got " << argc << "." << endl;
        return -1;
    }

    // Convert the file name from a c-string into a std::string
    // I am currently only doing this so I can substr it to see if it is the correct file type
    string fileName(argv[1]);

    // Check for correct file type
    // This implementation should make sure that the affix is at the end of the name and not somewhere weird
    Token tok("dab");
    if (filesystem::is_directory(fileName)) {
        for (auto& p : filesystem::directory_iterator(fileName)) {
            if (p.path().generic_string().substr(p.path().generic_string().size() - 5) != ".jack") {
                continue;
            }
            tok.openOutputFile(p.path().generic_string().substr(0, p.path().generic_string().size() - 5) + "T.xml");
            Parser parser(p.path().generic_string(), p.path().generic_string().substr(0, p.path().generic_string().size() - 5) + ".vm");
            parser.run();
            tok.closeOutputFile();
        }
    }
    else {
        if (fileName.substr(fileName.size() - 5) != ".jack") {
            cerr << "Error: Target was of incorrect type.(Expected .jack) " << endl;
            return -1;
        }
        tok.openOutputFile(fileName.substr(0, fileName.size() - 5) + "T.xml");
        Parser parser(fileName, fileName.substr(0, fileName.size() - 5) + ".xml");
        parser.run();
        tok.closeOutputFile();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
