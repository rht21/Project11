#include "symbolTable.h"
using namespace std;

symbolRecord symbolTable::get(string name){
    if(localMap.find(name)!= localMap.end()){
        return localMap.at(name);
    } 
    else if(globalMap.find(name)!= globalMap.end()){
        return globalMap.at(name);
    }
    else{
        cerr << "invalid get" << endl;
        throw "invalid get" ;
        return symbolRecord("lol", "rekt", 69);
    }
}  
void symbolTable::put(string name, string type, string segment, int offset){
    if(segment == "static" || segment == "this"){
        globalMap[name] = symbolRecord(type, segment, offset);
    }
    if(segment == "local" || segment == "argument"){
        localMap[name] = symbolRecord(type, segment, offset);
    }
} 
bool symbolTable::contains(string name){
    if(globalMap.find(name) != globalMap.end() || localMap.find(name) != localMap.end()){
        return true;
    }
    return false;
}
void symbolTable::clearLocal(){
    localMap.empty();
}