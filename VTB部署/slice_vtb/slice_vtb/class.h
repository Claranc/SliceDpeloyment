#ifndef CLASS_H
#define CLASS_H

#include "stdinclude.h"

//维特比算法部署
class VTB {
public:
    double traffic = 1.0;
    double NODEDELAY = 0.05;
    double LINEDELAY = 0.3;
    double UPROBABILITY = 200;
    Two_D AdjacencyMatrix;
    Two_D NodeCapacity;
    Two_D LinkBandwidth;
    Three_D SliceReq;
    Four_D SliceReqCapacity;
    Three_D ShortestPath;
    Three_D RESULT;
    vector<int> UseCount;
    vector<double> delay;
    vector<int> max_delay;
public:
    void SplitString(const string& s, vector<string>& v, const string& c);
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void SetLinkWeight();
    vector<int> VTBProcessing(int SIZE, int order, vector<int> &U, vector<double> &Ui,  int ingress, int outgress );
    void StartDeployment();
    void ComputeDelay();
    void SaveToFile(const char*);
    void ComputeRatio(const char*);
};

//贪心算法部署
class TX {
public:
    double traffic = 1.0;
    double PRODELAY = 0.05;
    double LINKDELAY = 0.3;
    double PROBABILITY = 200 ;
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
    vector<int> max_delay;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void ComputeRatio(const char*);
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
    double traffic=1.0;
    double PRODELAY = 0.5;
    double LINKDELAY = 0.3;
    double PROBABILITY = 200;
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
    vector<int> max_delay;
    vector<bool> is_ok;
public:
    void ReadFromFile(const char*, const char*, const char*, const char*, const char*);
    void ComputeRatio(const char*);
    void SetLinkWeight();
    void StartDeployment();
    void SplitString(const string& s, vector<string>& v, const string& c);
    void Update(int i);
    void ComputeDelay();
    void SaveToFile(const char*);
    double CheckDelay(int);
};

#endif 