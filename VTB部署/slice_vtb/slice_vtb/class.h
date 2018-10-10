#ifndef CLASS_H
#define CLASS_H

#include "stdinclude.h"

//维特比算法部署
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
    void SplitString(const string& s, vector<string>& v, const string& c);
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SliceDeployment();
    void SetLinkWeight();
    vector<int> VTBProcessing(int SIZE, int order, vector<int> &U, vector<double> &Ui,  int ingress, int outgress );
    void StartDeployment();
    void ComputeDelay();
};

//贪心算法部署
class TX {
private:
    Two_D AdjacencyMatrix;
    Two_D NodeCapacity;
    Two_D LinkBandwidth;
    Three_D SliceReq;
    Four_D SliceReqCapacity;
    Three_D ShortestPath;
    Two_D D;
    Three_D RESULT;
    vector<vector<double>> LineWeight;
    vector<int> UseCount;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SetLinkWeight();
    void StartDeployment();
    void SplitString(const string& s, vector<string>& v, const string& c);
    void Update(int i);
    void ComputeDelay();
};

#endif 