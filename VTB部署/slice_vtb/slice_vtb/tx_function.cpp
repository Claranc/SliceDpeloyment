#include "class.h"

double flow = 0.1;
double PRODELAY = 1;
double LINKDELAY = 1;

//字符串分割
void TX::SplitString(const string& s, vector<string>& v, const string& c) {
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

void TX::ReadFromFile(const char *adjacency_file, const char *nodecapacity_file, const char *linkbandwidth_file, \
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
    //读入切片容量请求
    ifstream fin_slicereq(slicereqcapacity_file, ios::in);
    if (!fin_slicereq) {
        cerr << "The slice vnf file cannot open !!!" << endl;
        exit(-1);
    }
    while (1) {
        Three_D SliceReq_temp;
        getline(fin_slicereq, LineStr);
        if (0 == LineStr.size()) {
            break;
        }
        if ("new" == LineStr.substr(0, 3)) {
            int linknum = stoi(LineStr.substr(4));
            for (int i = 0; i < linknum; i++) {
                getline(fin_slicereq, LineStr);
                vector<string> data;
                SplitString(LineStr, data, ";");
                if (0 == i) {
                    Two_D slice_chain;
                    for (int j = 0; j < data.size(); j++) {
                        vector<int> li;
                        stringstream ss(data[j]);
                        string params;
                        while (ss >> params) {
                            li.push_back(stoi(params));
                        }
                        slice_chain.push_back(li);
                    }
                    SliceReq_temp.push_back(slice_chain);
                }
                else {
                    Two_D slice_chain{ { stoi(data[0]) } };
                    for (int j = 1; j < data.size(); j++) {
                        vector<int> li;
                        stringstream ss(data[j]);
                        string params;
                        while (ss >> params) {
                            li.push_back(stoi(params));
                        }
                        slice_chain.push_back(li);
                    }
                    SliceReq_temp.push_back(slice_chain);
                }
            }
        }
        SliceReqCapacity.push_back(SliceReq_temp);
    }
    fin_slicereq.close();
}

void TX::SetLinkWeight() {
    int SIZE = AdjacencyMatrix.size();
    vector<int> Usetemp(SIZE, 0);
    UseCount = Usetemp;
    for (int i = 0; i < SIZE; i++) {
        vector<double> LineWeight_column(SIZE, 0);
        LineWeight.push_back(LineWeight_column);
    }
    D = AdjacencyMatrix;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (0 == D[i][j] && i != j) {
                D[i][j] = INT_MAX;
            }
        }
    }
    vector<vector<int>> path = D;
    for (auto &a : path) {
        for (vector<int>::iterator it = a.begin(); it != a.end(); it++) {
            if (*it < INT_MAX) {
                *it = it - a.begin();
            }
            else
                *it = D.size();
        }
    }
    for (auto l = 0; l < SIZE; l++) {
        vector<int> book(SIZE, 0);
        book[l] = 1;
        for (auto m = 0; m < SIZE; m++) {
            int min = INT_MAX;
            int k = 0;
            for (auto i = 0; i < SIZE; i++) {
                if (D[l][i] < min && 0 == book[i]) {
                    min = D[l][i];
                    k = i;
                }
            }
            book[k] = 1;
            for (auto n = 0; n < SIZE; n++) {
                if (1 == AdjacencyMatrix[k][n]) {
                    if (D[l][n] > D[l][k] + D[k][n]) {
                        D[l][n] = D[l][k] + D[k][n];
                        path[l][n] = path[l][k];
                    }
                }
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        vector<vector<int>> temp;
        for (int j = 0; j < SIZE; j++) {
            vector<int> a;
            int p = path[i][j];
            while (p != j) {
                a.push_back(p);
                p = path[p][j];
            }
            a.insert(a.cbegin(), i);
            a.push_back(j);
            temp.push_back(a);
        }
        ShortestPath.push_back(temp);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + (0.338 * UseCount[j] * pow(flow, 12.15) + 0.51*flow);
            if (i == j) {
                LineWeight[i][j] = double(INT_MAX);
            }
        }
    }
}

void TX::StartDeployment() {
    int SIZE = AdjacencyMatrix.size();
    for (int i = 0; i < SliceReq.size(); i++) {
        Two_D result_temp;
        int chain_size = SliceReq[i].size();
        vector<int> chain_result;
        int first_min = INT_MAX;
        int first;
        for (int k = 0; k < SIZE; k++) {
            if (UseCount[k] <= first_min) {
                first_min = UseCount[k];
                first = k;
            }
        }
        chain_result.push_back(first);
        Update(first);
        int mc_size = SliceReq[i][0].size();
        int start_node = first;
        int end_node = start_node;
        for (int k = 0; k < mc_size ; k++) {
            int temp = INT_MAX;
            for (int l = 0; l < SIZE; l++) {
                if (temp > LineWeight[start_node][l]) {
                    //不能出现重复的
                    bool flag = false;
                    for (auto a : chain_result) {
                        if (a == l) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) {
                        continue;
                    }
                    temp = LineWeight[start_node][l];
                    end_node = l;
                }
            }
            chain_result.push_back(end_node);
            Update(end_node);
            start_node = end_node;
        }
        result_temp.push_back(chain_result);
        chain_result.clear();
        for (int j = 1; j < chain_size; j++) {
            bool flag1, flag2 = false;
            if (SliceReq[i][j][0] <= SliceReq[i][0].size()) {
                flag1 = true;
            }
            if (SliceReq[i][j][SliceReq[i][j].size() - 1] <= SliceReq[i][0].size()) {
                flag2 = true;
            }
            vector<int> vc_result;
            int start = result_temp[0][SliceReq[i][j][0] - 1];
            int end;
            vc_result.push_back(start);
            if (!flag2) {
                for (int k = 1; k < SliceReq[i][j].size()-1; k++) {
                    int temp = INT_MAX;
                    for (int l = 0; l < SIZE; l++) {
                        if (temp > LineWeight[start][l]) {
                            bool flag = false;
                            for (auto a : vc_result) {
                                if (a == l) {
                                    flag = true;
                                    break;
                                }
                            }
                            if (flag) {
                                continue;
                            }
                            temp = LineWeight[start][l];
                            end = l;
                        }
                    }
                    vc_result.push_back(end);
                    Update(end);
                    start = end;
                }
            } 
            else {
                for (int k = 1; k < SliceReq[i][j].size() - 2; k++) {
                    int temp = INT_MAX;
                    for (int l = 0; l < SIZE; l++) {
                        if (temp > LineWeight[start][l]) {
                            bool flag = false;
                            for (auto a : vc_result) {
                                if (a == l) {
                                    flag = true;
                                    break;
                                }
                            }
                            if (flag) {
                                continue;
                            }
                            temp = LineWeight[start][l];
                            end = l;
                        }
                    }
                    vc_result.push_back(end);
                    Update(end);
                    start = end;
                }
                end = result_temp[0][SliceReq[i][j][SliceReq[i][j].size() - 1] - 1];
                vc_result.push_back(end);
            }
            result_temp.push_back(vc_result);
            RESULT.push_back(result_temp);
        }
        //test
       /* cout << "result = " << endl;
        cout << result_temp.size() << endl;
        for (auto &a : result_temp) {
            for (auto b : a) {
                cout << b << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        cout << "usecount = " << endl;
        for (auto a : UseCount) {
            cout << a << " ";
        }
        cout << endl;
        //test end
    }
}

void TX::Update(int j) {
    int SIZE = AdjacencyMatrix.size();
    UseCount[j]++;
    for (int i = 0; i < SIZE; i++) {
        LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + (0.338 * UseCount[j] * pow(flow, 12.15) + 0.51*flow);
        if (i == j) {
            LineWeight[i][j] == (double)INT_MAX;
        }
    }
}

void TX::ComputeDelay() {
    int SIZE = AdjacencyMatrix.size();
    cout << "result = " << endl;
    for (auto &a : RESULT) {
        for (auto &b : a) {
            for (auto c : b) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    //求出每一套切片的时延
    vector<double> delay;
    for (int i = 0; i < RESULT.size(); i++) {
        double sum = 0;
        int chain_size = RESULT[i].size();
        for (int j = 0; j < chain_size; j++) {
            for (int k = 1; k < RESULT[i][j].size(); k++) {
                int start = RESULT[i][j][k - 1];
                int end = RESULT[i][j][k];
                sum = sum + PRODELAY*(D[start][end] - 1) + LINKDELAY * D[start][end] + (0.338 * UseCount[end] * pow(flow, 12.15) + 0.51*flow);
            }
            delay.push_back(sum);
        }
    }
    //test
    cout << "delay = " << endl;
    for (auto a : delay) {
        cout << a << endl;
    }
    cout << endl;
    //test end
}