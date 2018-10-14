#ifndef CLASS_H
#define CLASS_H

#include "stdinclude.h"

//维特比算法部署
class VTB {
public:
    double traffic;
    double NODEDELAY = 0.3;
    double LINEDELAY = 1;
    double UPROBABILITY = 0;
    Two_D AdjacencyMatrix;
    Two_D NodeCapacity;
    Two_D LinkBandwidth;
    Three_D SliceReq;
    Four_D SliceReqCapacity;
    Three_D ShortestPath;
    Three_D RESULT;
    vector<int> UseCount;
    vector<double> delay;
public:
    void SplitString(const string& s, vector<string>& v, const string& c);
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SetLinkWeight();
    vector<int> VTBProcessing(int SIZE, int order, vector<int> &U, vector<double> &Ui,  int ingress, int outgress );
    void StartDeployment();
    void ComputeDelay();
    void SaveToFile(const char*);
};

//贪心算法部署
class TX {
public:
    double traffic;
    double PRODELAY = 1;
    double LINKDELAY = 1;
    double PROBABILITY =0 ;
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
    vector<double> U;
    vector<double> delay;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SetLinkWeight();
    void StartDeployment();
    void SplitString(const string& s, vector<string>& v, const string& c);
    void Update(int i);
    void ComputeDelay();
    void SaveToFile(const char*);
};

//模拟退火算法部署
class SA {
public:
    double traffic;
    double PRODELAY = 1;
    double LINKDELAY = 1;
    double PROBABILITY = 500;
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
    vector<double> U;
    vector<double> delay;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SetLinkWeight();
    void StartDeployment();
    void SplitString(const string& s, vector<string>& v, const string& c);
    void Update(int i);
    void ComputeDelay();
    void SaveToFile(const char*);
    double CheckDelay(int);
};

#endif 