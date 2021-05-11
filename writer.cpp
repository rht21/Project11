#include "writer.h"
using namespace std;
writer::writer(string filename){
    file.open(filename, ios::out);
}
writer::writer(){
    
}
void writer::writePush(string segment, int index){
    file << "push " << segment <<  " " << index << endl;
}
void writer::writePop(string segment, int index){
    file << "pop " << segment << " " << index << endl;
}
void writer::writeArtihmetic(string input){
    file << input << endl;
}
void writer::writeLabel(string label){
    file << "label "<< label << endl;
}
void writer::writeGoTo(string label){
    file << "goto " << label  << endl;
}
void writer::writeIfGoTo(string label){
    file << "if-goto " << label << endl;
}
void writer::writeCall(string name, int args){
    file << "call " << name << " " << args << endl;
}
void writer::writeFunction(string name, int localVars){
    string out = "function " + name + " " + to_string(localVars) + "\n";
    file << out;
}
void writer::writeReturn(){
    file << "return" << endl;
}
void writer::closeFile() {
    file.close();
}