#include <map>
#include <string>
#include "symbolRecord.h"
#include <exception>
#include <iostream>
using namespace std;
class symbolTable{
    map<string, symbolRecord> localMap;
    map<string, symbolRecord> globalMap;
    public:
        symbolRecord get(string name);
        void put(string name, string type, string segment, int offset);
        bool contains(string name);
        void clearLocal();
};