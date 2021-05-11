#include <string>
using namespace std;
class symbolRecord{
    public:
    string type = "";
    string segment = "";
    int offset = 0;
    symbolRecord(string newType, string newSegment, int newOffset);
    symbolRecord();
};