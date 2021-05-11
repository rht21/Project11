#include "symbolRecord.h"
symbolRecord::symbolRecord(string newType, string newSegment, int newOffset) {
    type = newType;
    segment = newSegment;
    offset = newOffset;
}

symbolRecord::symbolRecord()
{
    type = "";
    segment = "";
    offset = 0;
}
