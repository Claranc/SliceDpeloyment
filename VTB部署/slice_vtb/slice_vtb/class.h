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
    Three_D ShortestPath;
    Three_D RESULT;
    vector<int> UseCount;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SliceDeployment();
    void SetLinkWeight();
    vector<int> VTBProcessing(int SIZE, int order, vector<int> &U, vector<double> &Ui,  int ingress, int outgress );
    void StartDeployment();
    void ComputeDelay();
};

#endif 