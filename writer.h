#include <string>
#include <fstream>
#include <iostream>
using namespace std;
static ofstream file;
class writer{
    public:
        writer(string filename);
        writer();
        void writePush(string segment, int index);
        void writePop(string segment, int index);
        void writeArtihmetic(string input);
        void writeLabel(string label);
        void writeGoTo(string label);
        void writeIfGoTo(string label);
        void writeCall(string name, int args);
        void writeFunction(string name, int localVars);
        void writeReturn();
        void closeFile();
};