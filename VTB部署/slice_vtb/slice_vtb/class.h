#ifndef CLASS_H
#define CLASS_H

#include "stdinclude.h"

class VTB {
private:
    Two_D AdjacencyMatrix;
    Two_D NodeCapacity;
    Two_D LinkBandwidth;
    Three_D SliceReq;
    Four_D SliceReqCapacity;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
};

#endif 