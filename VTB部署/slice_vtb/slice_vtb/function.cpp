#include "class.h"

//字符串分割
void SplitString(const string& s, vector<string>& v, const string& c) {
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

void VTB::ReadFromFile(const char *adjacency_file, const char *nodecapacity_file, const char *linkbandwidth_file, \
    const char *slicevnf_file, const char *slicereqcapacity_file) {
    //读入邻接矩阵
    ifstream fin_AdjacencyMatrix(adjacency_file, ios::in);
    if (!fin_AdjacencyMatrix.is_open()) {
        cerr << "The AdjacencyMatrix file cannot be open!!!" << endl;
        exit(-1);
    }
    string LineStr;
    while (getline(fin_AdjacencyMatrix, LineStr)) {
        if (0 == LineStr.size())
            break;
        vector<int> Matrix_column;
        stringstream ss(LineStr);
        string params;
        while (ss >> params) {
            Matrix_column.push_back(stoi(params));
        }
        AdjacencyMatrix.push_back(Matrix_column);
        LineStr.clear();
    }
    fin_AdjacencyMatrix.close();
    //读入节点容量
    ifstream fin_NodeCapacity(nodecapacity_file, ios::in);
    if (!fin_NodeCapacity.is_open()) {
        cerr << "The NodeCapacity file cannot be open!!!" << endl;
        exit(-1);
    }
    while (getline(fin_NodeCapacity, LineStr)) {
        if (0 == LineStr.size())
            break;
        vector<int> li;
        stringstream ss(LineStr);
        string params;
        while (ss >> params) {
            li.push_back(stoi(params));
        }
        NodeCapacity.push_back(li);
        LineStr.clear();
    }
    fin_NodeCapacity.close();
    //读入链路容量
    ifstream fin_LineWidth(linkbandwidth_file, ios::in);
    if (!fin_LineWidth.is_open()) {
        cerr << "The LineWidth file cannot be open!!!" << endl;
        exit(-1);
    }
    while (getline(fin_LineWidth, LineStr)) {
        if (0 == LineStr.size())
            break;
        vector<int> li;
        stringstream ss(LineStr);
        string params;
        while (ss >> params) {
            li.push_back(stoi(params));
        }
        LinkBandwidth.push_back(li);
        LineStr.clear();
    }
    fin_LineWidth.close();
    //读入切片拓扑
    ifstream fin_slicevnf(slicevnf_file, ios::in);
    if (!fin_slicevnf) {
        cerr << "The slice vnf file cannot open !!!" << endl;
        exit(-1);
    }
    while (1) {
        getline(fin_slicevnf, LineStr);
        if (0 == LineStr.size()) {
            break;
        }
        if ("new" == LineStr.substr(0, 3)) {
            Two_D slice_temp;
            int linknum = stoi(LineStr.substr(4));
            for (int i = 1; i <= linknum; ++i) {
                getline(fin_slicevnf, LineStr);
                vector<int> li;
                stringstream ss(LineStr);
                string params;
                while (ss >> params) {
                    li.push_back(stoi(params));
                }
                slice_temp.push_back(li);
                LineStr.clear();
            }
            SliceReq.push_back(slice_temp);
        }
    }
    fin_slicevnf.close();
    //test
    /*for (auto &a : SliceReq) {
        for (auto &b : a) {
            for (auto c : b) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << endl;
    }*/
    //读入切片容量请求
    ifstream fin_slicereq(slicereqcapacity_file, ios::in);
    if (!fin_slicereq) {
        cerr << "The slice vnf file cannot open !!!" << endl;
        exit(-1);
    }
    while (1) {
        getline(fin_slicereq, LineStr);
        if (0 == LineStr.size()) {
            break;
        }
        if ("new" == LineStr.substr(0, 3)) {

        }
    }
    fin_slicereq.close();
}
