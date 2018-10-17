#include "class.h"

//字符串分割
void SA::SplitString(const string& s, vector<string>& v, const string& c) {
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

//读取文件
void SA::ReadFromFile(const char *adjacency_file, const char *nodecapacity_file, const char *linkbandwidth_file, \
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

//设置链路的权重
void SA::SetLinkWeight() {
    int SIZE = AdjacencyMatrix.size();
    vector<double> U_temp(SIZE, 0);
    U = U_temp;
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
            //设置两两节点间的权值
            //LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + (0.338 * UseCount[j] * pow(flow, 12.15) + 0.51*flow);
            LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + PROBABILITY*U[j];
            if (i == j) {
                LineWeight[i][j] = double(INT_MAX);
            }
        }
    }
}

//部署函数
void SA::StartDeployment() {
    time_t start_time;
    start_time = time(NULL);
    int SIZE = AdjacencyMatrix.size();
    for (int p = 0; p < SIZE; p++) {
        D[p][p] = INT_MAX;
    }
    for (int i = 0; i < SliceReq.size(); i++) {
        vector<int> copy;
        Two_D result_temp;
        int chain_size = SliceReq[i].size();
        vector<int> chain_result;
        //将usecount最小的一个节点作为主链的初始节点
        int first_min = INT_MAX;
        int first;
        for (int k = 0; k < SIZE; k++) {
            if (UseCount[k] < first_min) {
                first_min = UseCount[k];
                first = k;
            }
        }
        chain_result.push_back(first);
        copy.push_back(first);
        //Update(first);
        //从初始节点开始以及找到权值跟它最低的节点作为下一节点
        int mc_size = SliceReq[i][0].size();
        int start_node = first;
        int end_node = start_node;
        //开始部署主链
        for (int k = 1; k < mc_size; k++) {
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
            copy.push_back(end_node);
            //Update(end_node);
            start_node = end_node;
        }
        result_temp.push_back(chain_result);
        chain_result.clear();
        //开始部署从链
        for (int j = 1; j < chain_size; j++) {
            bool flag1, flag2 = false;
            //起始VNF是否在主链中
            if (SliceReq[i][j][0] <= SliceReq[i][0].size()) {
                flag1 = true;
            }
            //最末的VNF是否在主链中
            if (SliceReq[i][j][SliceReq[i][j].size() - 1] <= SliceReq[i][0].size()) {
                flag2 = true;
            }
            vector<int> vc_result;
            int start = result_temp[0][SliceReq[i][j][0] - 1];
            int end;
            vc_result.push_back(start);
            //最末的节点不在主链中
            if (!flag2) {
                for (int k = 0; k < SliceReq[i][j].size() - 1; k++) {
                    int temp = INT_MAX;
                    for (int l = 0; l < SIZE; l++) {
                        if (temp > LineWeight[start][l]) {
                            //拒绝重复的节点出现
                            bool flag = false;
                            for (auto a : copy) {
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
                    copy.push_back(end);
                    //Update(end);
                    start = end;
                }
            }
            //最末的节点在主链中
            else {
                for (int k = 0; k < SliceReq[i][j].size() - 2; k++) {
                    int temp = INT_MAX;
                    for (int l = 0; l < SIZE; l++) {
                        if (temp > LineWeight[start][l]) {
                            //拒绝重复的节点出现
                            bool flag = false;
                            for (auto a : copy) {
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
                    copy.push_back(end);
                    //Update(end);
                    start = end;
                }
                end = result_temp[0][SliceReq[i][j][SliceReq[i][j].size() - 1] - 1];
                vc_result.push_back(end);
            }
            result_temp.push_back(vc_result);
        }
        RESULT.push_back(result_temp);
        //test
        /* cout << "result_start = " << endl;
        cout << result_temp.size() << endl;
        for (auto &a : result_temp) {
        for (auto b : a) {
        cout << b << " ";
        }
        cout << endl;
        }
        cout << endl;
        cout << "usecount_start = " << endl;
        for (auto a : UseCount) {
        cout << a << " ";
        }
        cout << endl;*/
        //test end
        //开始使用模拟退火算法调整
        double e = 1e-16, at = 0.999, T = 1.0;
        //e表示终止温度  at表示温度的变化率  T是初始温度
        int L = 4000; //L为最大迭代次数
        while (L--) {
            //找出标号最大的VNF
            int ch_size = SliceReq[i].size();
            int last_size = SliceReq[i][ch_size - 1].size();
            int t1 = SliceReq[i][ch_size - 1][last_size - 1];
            int t2 = SliceReq[i][ch_size - 1][last_size - 2];
            int vnf_size = t1 > t2 ? t1 : t2;
            vector<int> random_node;
            while (1) {
                int k1 = 1 + rand() % vnf_size, k2 = 1 + rand() % vnf_size, k3 = 1 + rand() % vnf_size, k4 = 1 + rand() % vnf_size;
                set<int> temp{ k1, k2, k3, k4 };
                if (temp.size() == 4) {
                    random_node.push_back(k1);
                    random_node.push_back(k2);
                    random_node.push_back(k3);
                    random_node.push_back(k4);
                    break;
                }
            }
            vector<vector<int>> sa_result = RESULT[i];
            //test
           /*cout << " zhiqian = " << endl;
            vector<vector<int>> sa_result = RESULT[i];
            for (auto &a : RESULT[i]) {
                for (auto b : a) {
                    cout << b << " ";
                }
                cout << endl;
            }
            for (auto a : random_node) {
                cout << a << " ";
            }
            cout << endl;*/
            //test end
            double ob1 = CheckDelay(i);
            //cout << "delay_qian = " << ob1 << endl;
            //随机扰动，找到随机4个节点，把他们改成他们相邻的一个随机节点
            set<int> new_value;
            for (int k = 0; k < 4; k++) {
                int new_result;
                bool flag = false; //找到了k所在的节点
                bool flag2 = false; //找不到合适的邻居节点
                for (int x = 0; x < SliceReq[i].size(); x++) {
                    for (int y = 0; y < SliceReq[i][x].size(); y++) {
                        if (SliceReq[i][x][y] == random_node[k]) {
                            int start_result = RESULT[i][x][y];
                            while (1) {
                                vector<int> rand_range;
                                for (int b = 0; b < SIZE; b++) {
                                    if (AdjacencyMatrix[start_result][b] == 1) {
                                        rand_range.push_back(b);
                                    }
                                }
                                int temp = rand() % rand_range.size();
                                temp = rand_range[temp];
                                int value_size = new_value.size();
                                new_value.insert(temp);
                                if (new_value.size() == value_size) {
                                    //continue;
                                    flag2 = true;
                                    new_result = start_node;
                                    break;
                                }
                                for (auto a : copy) {
                                    if (temp == a) {
                                        flag2 = true;
                                    }
                                }
                                if (temp != start_result) {
                                    if (!flag2) {
                                        new_result = temp;
                                    }
                                    else {
                                        new_result = start_result;
                                    }
                                    flag = true;
                                    break;
                                }
                            }
                            if (flag) {
                                break;
                            }
                        }
                    }
                    if (flag) {
                        break;
                    }
                }
                for (int q = 0; q < SliceReq[i].size(); q++) {
                    for (int t = 0; t < SliceReq[i][q].size(); t++) {
                        if (SliceReq[i][q][t] == random_node[k]) {
                            if (flag2) {
                                break;
                            }
                            //cout << RESULT[i][q][t] << "->" << new_result << endl;
                            RESULT[i][q][t] = new_result;
                        }
                    }
                    if (flag2) {
                        break;
                    }
                }
            }
            //test
            /*cout << " zhihou = " << endl;
            for (auto &a : RESULT[i]) {
                for (auto b : a) {
                    cout << b << " ";
                }
                cout << endl;
            }*/
            //test end
            bool flag3 = true; //新解是否被接受
            double ob2 = CheckDelay(i);
            //cout << "delay = " << ob2 << endl;
            if (ob2 > ob1) {
                double delta = ob2 - ob1;
                double sj = (double)(rand() % 10000) / 10000;     //sj为0~1之间的随机数
                if (exp(-delta / T) < sj) {
                    RESULT[i] = sa_result;
                    flag3 = false;
                }
            }
            T *= at;
            if (T < e) {
                break;
            }
            if (flag3) {
                /*for (auto &a : RESULT[i]) {
                    for (auto b : a) {
                        cout << b << " ";
                    }
                    cout << endl;
                }
                cout << endl;
                cout << "ob2 = " << ob2 << endl;*/
            }
        }
        //cout << "sss = " << CheckDelay(0) << endl;
        cout << "tag: " << i << endl;
        set<int> t;
        for (auto &a : RESULT[i]) {
            for (auto b : a) {
                int size = t.size();
                t.insert(b);
                if (t.size() - size == 1) {
                    UseCount[b]++;
                }
            }
        }
        /*if (i % 10 == 9) {
            ComputeDelay();
            ComputeRatio("max_delay.txt");
        }*/
        if (i % 10 == 9) {
            time_t now_time;
            now_time = time(NULL);
            int delta = now_time - start_time;
            ofstream fout("time_sa.csv", ios::app);
            if (!fout.is_open()) {
                cerr << "cannot open time_sa.csv" << endl;
            }
            fout << i + 1 << "," << delta << endl;
            fout.close();
        }
    }
}


//每部署完一个节点就更新网络的相关的权重值
void SA::Update(int j) {
    int SIZE = AdjacencyMatrix.size();
    UseCount[j]++;
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += UseCount[i];
    }
    for (int i = 0; i < SIZE; i++) {
        U[i] = (double)UseCount[i] / (double)sum;
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + PROBABILITY*U[j];
            if (i == j) {
                LineWeight[i][j] == (double)INT_MAX;
            }
        }
        //LineWeight[i][j] = PRODELAY*(D[i][j] - 1) + LINKDELAY * D[i][j] + (0.338 * UseCount[j] * pow(flow, 12.15) + 0.51*flow);
    }
}

double SA::CheckDelay(int i) {
    vector<int> tem = UseCount;
    set<int> t;
    for (auto &a : RESULT[i]) {
        for (auto b : a) {
            int size = t.size();
            t.insert(b);
            if (t.size() - size == 1) {
                tem[b]++;
            }
        }
    }
    /*cout << "temp = " << endl;
    for (auto a : temp) {
        cout << a << " ";
    }
    cout << endl;*/
    int SIZE = AdjacencyMatrix.size();
    double sum = 0;
    int chain_size = RESULT[i].size();
    for (int j = 0; j < chain_size; j++) {
        for (int k = 1; k < RESULT[i][j].size(); k++) {
            int start = RESULT[i][j][k - 1];
            int end = RESULT[i][j][k];
            //sum = sum + PRODELAY*(D[start][end] - 1) + LINKDELAY * D[start][end] + (0.338 * tem[end] * pow(traffic, 12.15) + 0.51*traffic);
            sum = sum + PRODELAY*(D[start][end] - 1) + LINKDELAY * D[start][end] + 0.04*(1.0 * UseCount[end] * pow(traffic, 12.15) + 0.51*traffic);
            //cout << sum << endl;
        }
    }
    return sum;
}

//计算时延的函数
void SA::ComputeDelay() {
    delay.clear();
    cout << "UseCount = " << endl;
    for (auto a : UseCount) {
        cout << a << " ";
    }
    cout << endl;
    cout << "result = " << endl;
   /* for (auto &a : RESULT) {
        for (auto &b : a) {
            for (auto c : b) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << endl;
    }*/
    int SIZE = AdjacencyMatrix.size();
    //求出每一套切片的时延
    //traffic = 1;
    for (int i = 0; i < RESULT.size(); i++) {
        double sum = 0;
        int chain_size = RESULT[i].size();
        for (int j = 0; j < chain_size; j++) {
            for (int k = 1; k < RESULT[i][j].size(); k++) {
                int start = RESULT[i][j][k - 1];
                int end = RESULT[i][j][k];
                sum = sum + PRODELAY*(D[start][end] - 1) + LINKDELAY * D[start][end] + 0.04*(0.338 * UseCount[end] * pow(traffic, 12.15) + 0.51*traffic);
                //cout << sum << endl;
            }
            //cout << "sum = " << sum << endl;
        }
        delay.push_back(sum);
    }
    //test
    /*cout << "delay = " << endl;
    for (auto a : delay) {
    cout << a << endl;
    }
    cout << endl;*/
    //test end
}

void SA::SaveToFile(const char * filename) {
    ofstream fout(filename, ios::app);
    if (!fout.is_open()) {
        cerr << "cannot open " << filename << endl;
        exit(-1);
    }
    double sum = 0;
    for (auto a : delay) {
        sum += a;
    }
    sum /= SliceReq.size();
    fout << traffic << "," << sum << endl;
    fout.close();
}

void SA::ComputeRatio(const char *filename) {
    ifstream fin(filename, ios::in);
    if (!fin.is_open()) {
        cerr << "cannot open " << filename << endl;
        exit(-1);
    }
    while (1) {
        string linestr;
        getline(fin, linestr);
        if (linestr.size() == 0) {
            break;
        }
        vector<string> temp;
        SplitString(linestr, temp, " ");
        for (int i = 0; i < temp.size(); i++) {
            max_delay.push_back(stoi(temp[i]));
        }
    }
    cout << "size = " << delay.size() << endl;
    int SIZE = AdjacencyMatrix.size();
    int count = 0;
    int sum = 0;
    for (auto a : delay) {
        cout << a << endl;
    }
    cout << endl;
    for (int i = 0; i < RESULT.size(); i++) {
        sum++;
        if (delay[i] <= max_delay[i]) {
            count++;
        }
    }
    max_delay.clear();
    double ratio = (double)count / (double)sum;
    cout << "ratio = " << ratio << endl;
    ofstream fout("ratio_sa.csv", ios::app);
    if (!fout.is_open()) {
        cerr << "cannot open  ratio_sa.txt" << endl;
        exit(-1);
    }
    fout << RESULT.size() << "," << ratio << endl;
}
