#include "class.h"

double traffic = 0.8;
double NODEDELAY = 1;
double LINEDELAY = 1;
//double UPROBABILITY = 600;

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

void VTB::SetLinkWeight() {
    int SIZE = AdjacencyMatrix.size();
    vector<vector<int>> D = AdjacencyMatrix;
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
    //输出距离矩阵
    //test
    /*for (auto &a : D) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }*/
    //test end
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
    //test
   /* for (auto &a : ShortestPath) {
        for (auto &b : a) {
            for (auto c : b) {
                cout << c << ",";
    }
    cout << " ";
    }
    cout << endl;
    }*/
    //test end
}


void VTB::SliceDeployment() {
    int SIZE = AdjacencyMatrix.size();
    vector<double> U(SIZE, 0);
    //从路径矩阵中提取距离矩阵
    vector<vector<int>> D;
    vector<int> UseCount(SIZE, 0);
    for (int i = 0; i < SIZE; i++) {
        vector<int> D_column(SIZE, 0);
        D.push_back(D_column);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            D[i][j] = ShortestPath[i][j].size() - 1;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        D[i][i] = 0;
    }
    //test
     for (auto &a : D) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }
    //test end
    //开始部署切片网元
    int count = 0;
    double delay = 0;
    for (int order = 0; order < SliceReq.size(); order++) {
        count++;
        int NUM = SliceReq[order][0].size();
        //NUM = 2;
        int newsize = SIZE * NUM;
        vector<vector<double>> LineWeight;
        vector<vector<int>> SavePath(newsize + 2, {});
        for (int i = 0; i < newsize; i++) {
            vector<double> LineWeight_column(newsize, 0);
            LineWeight.push_back(LineWeight_column);
        }
        for (int i = 0; i < newsize; i++) {
            //中间层
            if (i / SIZE > 0 && i / SIZE < NUM - 1) {
                for (int j = SIZE * (i / SIZE - 1); j < SIZE * (i / SIZE); j++) {
                    LineWeight[i][j] = 1;
                }
                LineWeight[i][i - SIZE] = 0;
                for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
                    LineWeight[i][j] = 1;
                }
                LineWeight[i][i + SIZE] = 0;
            }
            //第一层
            else if (i / SIZE == 0) {
                for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
                    LineWeight[i][j] = 1;
                }
                LineWeight[i][i + SIZE] = 0;
            }
            //最后一层
            else if (i / SIZE == NUM - 1) {
                for (int j = SIZE * (i / SIZE - 1); j < SIZE * (i / SIZE); j++) {
                    LineWeight[i][j] = 1;
                }
                LineWeight[i][i - SIZE] = 0;
            }
        }
        //test
       /* for (auto &a : LineWeight) {
        for (auto b : a) {
        cout << b << " ";
        }
        cout << endl;
        }*/
        //test end
        //给每条边定义相应的加权值
        for (int i = 0; i < newsize; i++) {
            for (int j = 0; j < newsize; j++) {
                if (1 == LineWeight[i][j]) {
                    if (D[i%SIZE][j%SIZE] > 0) {
                       // LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE] + UPROBABILITY * U[j%SIZE];
                       LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE] + (0.338 * UseCount[j%SIZE]*pow(traffic,12.15)+0.51*traffic);
                       //cout << "vistual = " << (0.338 * UseCount[j%SIZE] * pow(traffic, 12.15) + 0.51*traffic) << endl;
                        //LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE];
                    }
                    else {
                        //LineWeight[i][j] = UPROBABILITY * U[j%SIZE];
                        //LineWeight[i][j] = INT_MAX;
                    }
                }
            }
        }
        //test
        /*cout << "without = " << endl;
        for (auto &a : LineWeight) {
        for (auto b : a) {
        cout << b << " ";
        }
        cout << endl;
        }*/
        //test end
        vector<vector<double>> Weight;
        // 加上起始点和终结点
        for (int i = 0; i < newsize + 2; i++) {
            vector<double> column(newsize + 2, 0);
            Weight.push_back(column);
        }
        for (int i = 0; i < newsize; i++) {
            for (int j = 0; j < newsize; j++) {
                Weight[i + 1][j + 1] = LineWeight[i][j];
            }
        }
        for (int i = 1; i < SIZE + 1; i++) {
            /*
            Weight[0][i] = 1+UPROBABILITY * U[(i-1)%SIZE];
            Weight[i][0] = 1+UPROBABILITY * U[(i - 1) % SIZE];
            Weight[newsize + 1][newsize + 1 - i] =1+ UPROBABILITY * U[(i - 1) % SIZE];
            Weight[newsize + 1 - i][newsize + 1] = 1+UPROBABILITY * U[(i - 1) % SIZE];
            */
            Weight[0][i] = 1 + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[i][0] = 1 + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[newsize + 1][newsize + 1 - i] = 1 + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[newsize + 1 - i][newsize + 1] = 1 + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        }
        //test
        /*cout << "add = " << endl;
        for (auto &a : Weight) {
        for (auto b : a) {
        cout << b << " ";
        }
        cout << endl;
        }*/
        //test end
        //把带权值的邻接矩阵转换成单向的
        for (int i = 1; i < newsize + 2; i++) {
            for (int j = 0; j < i; j++) {
                Weight[i][j] = 0;
            }
        }
        //test
      /* cout << "single = " << endl;
        for (auto &a : Weight) {
        for (auto b : a) {
        cout << b << " ";
        }
        cout << endl;
        }*/
        //test end
        //开始部署
        vector<int> PathResult(newsize + 2, newsize + 2);
        for (int i = 0; i < newsize + 2; i++) {
            if (Weight[0][i]) {
                PathResult[i] = i;
            }
        }
        vector<vector<double>> Dis = Weight;
        for (auto &a : Dis) {
            for (auto &b : a) {
                if (0 == b) {
                    b = INT_MAX;
                }
            }
        }
        vector<int> book(newsize + 2, 0);
        book[0] = 1;
        int k = 0;
        for (int m = 0; m < newsize + 2; m++) {
            int min = INT_MAX;
            for (int i = 0; i < newsize + 2; i++) {
                if (min > Dis[0][i] && !book[i]) {
                    int seq = (k - 1) % SIZE;
                    //判断第一个VNF是否能被容纳
                   /* if (seq == 0 && !(SliceReqCapacity[order][0][0][0] < NodeCapacity[(k - 1) % SIZE][0] && SliceReqCapacity[order][0][0][1] < NodeCapacity[(k - 1) % SIZE][1] \
                        && SliceReqCapacity[order][0][0][2] < NodeCapacity[(k - 1) % SIZE][2])) {
                        continue;
                    }*/
                    min = Dis[0][i];
                    k = i;
                }
            }
            book[k] = 1;
            for (int j = 0; j < newsize + 2; j++) {
                if (Dis[k][j] < INT_MAX) {
                    if (Dis[0][j] > Dis[0][k] + Dis[k][j]) {
                        int cur = (j - 1) / SIZE; //cur代表是第几个VNF
                        if (1) {
                            int start = (k - 1) % SIZE;
                            int end = (j - 1) % SIZE;
                            int bandwidth_flag = 0;
                            if (cur > 0 && cur < NUM) {
                                for (int x = 1; x < ShortestPath[start][end].size(); x++) {
                                    //cout << start << " : " << end << endl;
                                    if (start != end && LinkBandwidth[ShortestPath[start][end][x - 1]][ShortestPath[start][end][x]] < SliceReqCapacity[order][0][cur][3]) {
                                        bandwidth_flag = 0;
                                        break;
                                    }
                                }
                            }
                            if (!bandwidth_flag) {
                                int flag = 0;
                                if (SavePath[k].size() && cur < NUM) {
                                    int p = SavePath[k][SavePath[k].size() - 1];
                                    while (1) {
                                        if ((p - 1) % SIZE == (j - 1) % SIZE) {
                                            flag = 1;
                                            break;
                                        }
                                        else {
                                            if (SavePath[p].size()) {
                                                p = SavePath[p][SavePath[p].size() - 1];
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (!flag) {
                                    SavePath[j].push_back(k);
                                    Dis[0][j] = Dis[0][k] + Dis[k][j];
                                    PathResult[j] = k;
                                }
                            }
                        }
                    }
                }
            }
        }
        //test
        /*cout << " path = " << endl;
        for (auto a : PathResult) {
        cout << a << " ";
        }
        cout << endl;
        cout << "D = " << endl;
        for (auto a : Dis[0]) {
        cout << a << " ";
        }
        cout << endl;*/
        //test end
        if (Dis[0][newsize + 1] < INT_MAX) {
            cout << "Dmin = " << Dis[0][newsize + 1] << endl;
            int p = PathResult[newsize + 1];
            vector<int> PlaceResult{ p };
            int temp = newsize + 2;
            while (temp != PathResult[p]) {
                temp = PathResult[p];
                p = temp;
                PlaceResult.insert(PlaceResult.begin(), temp);
            }
            vector<int> save_path;
            for (int i = 0; i < PlaceResult.size(); i++) {
                int temp = (PlaceResult[i] - 1) % SIZE;
                save_path.push_back(temp);
                //cout << temp << " ";
                UseCount[temp]++;
                NodeCapacity[temp][0] -= SliceReqCapacity[order][0][i][0];
                NodeCapacity[temp][1] -= SliceReqCapacity[order][0][i][1];
                NodeCapacity[temp][2] -= SliceReqCapacity[order][0][i][2];
            }
            //RESULT.push_back(save_path);
            int sum = 0;
            for (int z = 0; z < SIZE; z++) {
                sum += UseCount[z];
            }
            for (int z = 0; z < SIZE; z++) {
                U[z] = (double)UseCount[z] / (double)sum;
            }
            for (int i = 1; i < save_path.size(); i++) {
                delay += D[save_path[i - 1]][save_path[i]];
            }
           /* cout << "Ui = " << endl;
            for (auto a : U) {
                cout << a << endl;
            }
            cout << endl;*/
            //输出节点使用频数和节点容量
            //test
           /* for (int i = 0; i < SIZE; i++) {
            cout << UseCount[i] << " ";
            }
            cout << endl;
            cout << "Capacity = " << endl;
            for (auto &a : NodeCapacity) {
            for (auto b : a) {
            cout << b << " ";
            }
            cout << endl;
            }*/
            //test end
            //部署从链

        }
        else {
            cout << order << " can not be deployed !!!" << endl;
        }
        cout << "tag: " << count << endl;
        if (0 == count % 10) {
            double max = 0;
            double sum = 0;
            for (auto b : U) {
                sum += b;
                if (b > max) {
                    max = b;
                }
            }
            double aver = sum / (double)SIZE;
            double Do = 0;
            for (int i = 0; i < SIZE; i++) {
                Do += (U[i] - aver) * (U[i] - aver);
            }
            Do /= SIZE;
            cout << "Do: " << Do << endl;
            cout << "Umax: " << max << endl;
            //fout << "Slice: " << count << ", " << "Umax: " << max << "," << "D: " << Do << ", " << "Delay: " << (double)delay / (double)count << endl;
        }
    }
    cout << "RESULT = " << endl;
    cout << "usecount = " << endl;
    for (auto a : UseCount) {
        cout << a << endl;
    }
    cout << endl;
    
}


vector<int> VTB::VTBProcessing(int length, int order, vector<int> &UseCount, vector<double> &U, int ingress = -1, int outgress = -1) {
    int SIZE = AdjacencyMatrix.size();
    //从路径矩阵中提取距离矩阵
    vector<vector<int>> D;
    for (int i = 0; i < SIZE; i++) {
        vector<int> D_column(SIZE, 0);
        D.push_back(D_column);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            D[i][j] = ShortestPath[i][j].size() - 1;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        D[i][i] = 0;
    }
    //test
    /*for (auto &a : D) {
        for (auto b : a) {
            cout << b << " ";
        }
        cout << endl;
    }*/
    //test end
    //开始部署切片网元
    double delay = 0;
    int NUM = length;
    //NUM = 2;
    int newsize = SIZE * NUM;
    vector<vector<double>> LineWeight;
    vector<vector<int>> SavePath(newsize + 2, {});
    for (int i = 0; i < newsize; i++) {
        vector<double> LineWeight_column(newsize, 0);
        LineWeight.push_back(LineWeight_column);
    }
    for (int i = 0; i < newsize; i++) {
        //中间层
        if (i / SIZE > 0 && i / SIZE < NUM - 1) {
            for (int j = SIZE * (i / SIZE - 1); j < SIZE * (i / SIZE); j++) {
                LineWeight[i][j] = 1;
            }
            LineWeight[i][i - SIZE] = 0;
            for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
                LineWeight[i][j] = 1;
            }
            LineWeight[i][i + SIZE] = 0;
        }
        //第一层
        else if (i / SIZE == 0) {
            for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
                LineWeight[i][j] = 1;
            }
            LineWeight[i][i + SIZE] = 0;
        }
        //最后一层
        else if (i / SIZE == NUM - 1) {
            for (int j = SIZE * (i / SIZE - 1); j < SIZE * (i / SIZE); j++) {
                LineWeight[i][j] = 1;
            }
            LineWeight[i][i - SIZE] = 0;
        }
    }
    //test
    /* for (auto &a : LineWeight) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }*/
    //test end
    //给每条边定义相应的加权值
    for (int i = 0; i < newsize; i++) {
        for (int j = 0; j < newsize; j++) {
            if (1 == LineWeight[i][j]) {
                if (D[i%SIZE][j%SIZE] > 0) {
                    // LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE] + UPROBABILITY * U[j%SIZE];
                    LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE] + (0.338 * UseCount[j%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
                    //cout << "vistual = " << (0.338 * UseCount[j%SIZE] * pow(traffic, 12.15) + 0.51*traffic) << endl;
                    //LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE];
                }
                else {
                    //LineWeight[i][j] = UPROBABILITY * U[j%SIZE];
                    //LineWeight[i][j] = INT_MAX;
                }
            }
        }
    }
    //test
    /*cout << "without = " << endl;
    for (auto &a : LineWeight) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }*/
    //test end
    vector<vector<double>> Weight;
    // 加上起始点和终结点
    for (int i = 0; i < newsize + 2; i++) {
        vector<double> column(newsize + 2, 0);
        Weight.push_back(column);
    }
    for (int i = 0; i < newsize; i++) {
        for (int j = 0; j < newsize; j++) {
            Weight[i + 1][j + 1] = LineWeight[i][j];
        }
    }
    if (-1 == ingress) {
        for (int i = 1; i < SIZE + 1; i++) {
            Weight[0][i] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[i][0] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        }
    }
    else {
        for (int i = 1; i < SIZE + 1; i++) {
            Weight[0][i] = NODEDELAY*(D[ingress][(i-1)%SIZE] - 1) + LINEDELAY * D[ingress][(i-1)%SIZE] + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[i][0] = NODEDELAY*(D[ingress][(i - 1) % SIZE] - 1) + LINEDELAY * D[ingress][(i - 1) % SIZE] + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        }
    }
    if (-1 == outgress) {
        for (int i = 1; i < SIZE + 1; i++) {
            Weight[newsize + 1][newsize + 1 - i] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[newsize + 1 - i][newsize + 1] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        }
    }
    else {
        for (int i = 1; i < SIZE + 1; i++) {
            Weight[newsize + 1][newsize + 1 - i] = NODEDELAY*(D[(i-1)%SIZE][outgress] - 1) + LINEDELAY * D[(i-1)%SIZE][outgress] + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
            Weight[newsize + 1 - i][newsize + 1] = NODEDELAY*(D[(i-1)%SIZE][outgress] - 1) + LINEDELAY *D[(i-1)%SIZE][outgress] + (0.338 * UseCount[(i-1)%SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        }
    }
   /* for (int i = 1; i < SIZE + 1; i++) {
        Weight[0][i] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        Weight[i][0] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        Weight[newsize + 1][newsize + 1 - i] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
        Weight[newsize + 1 - i][newsize + 1] = 1 + (0.338 * UseCount[(i - 1) % SIZE] * pow(traffic, 12.15) + 0.51*traffic);
    }*/
    //test
    /*cout << "add = " << endl;
    for (auto &a : Weight) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }*/
    //test end
    //把带权值的邻接矩阵转换成单向的
    for (int i = 1; i < newsize + 2; i++) {
        for (int j = 0; j < i; j++) {
            Weight[i][j] = 0;
        }
    }
    //test
    /* cout << "single = " << endl;
    for (auto &a : Weight) {
    for (auto b : a) {
    cout << b << " ";
    }
    cout << endl;
    }*/
    //test end
    //开始部署
    vector<int> PathResult(newsize + 2, newsize + 2);
    for (int i = 0; i < newsize + 2; i++) {
        if (Weight[0][i]) {
            PathResult[i] = i;
        }
    }
    vector<vector<double>> Dis = Weight;
    for (auto &a : Dis) {
        for (auto &b : a) {
            if (0 == b) {
                b = INT_MAX;
            }
        }
    }
    vector<int> book(newsize + 2, 0);
    book[0] = 1;
    int k = 0;
    for (int m = 0; m < newsize + 2; m++) {
        int min = INT_MAX;
        for (int i = 0; i < newsize + 2; i++) {
            if (min > Dis[0][i] && !book[i]) {
                int seq = (k - 1) % SIZE;
                //判断第一个VNF是否能被容纳
                /* if (seq == 0 && !(SliceReqCapacity[order][0][0][0] < NodeCapacity[(k - 1) % SIZE][0] && SliceReqCapacity[order][0][0][1] < NodeCapacity[(k - 1) % SIZE][1] \
                && SliceReqCapacity[order][0][0][2] < NodeCapacity[(k - 1) % SIZE][2])) {
                continue;
                }*/
                min = Dis[0][i];
                k = i;
            }
        }
        book[k] = 1;
        for (int j = 0; j < newsize + 2; j++) {
            if (Dis[k][j] < INT_MAX) {
                if (Dis[0][j] > Dis[0][k] + Dis[k][j]) {
                    int cur = (j - 1) / SIZE; //cur代表是第几个VNF
                    if (1) {
                        int start = (k - 1) % SIZE;
                        int end = (j - 1) % SIZE;
                        int bandwidth_flag = 0;
                        if (cur > 0 && cur < NUM) {
                            for (int x = 1; x < ShortestPath[start][end].size(); x++) {
                                //cout << start << " : " << end << endl;
                                if (start != end && LinkBandwidth[ShortestPath[start][end][x - 1]][ShortestPath[start][end][x]] < SliceReqCapacity[order][0][cur][3]) {
                                    bandwidth_flag = 0;
                                    break;
                                }
                            }
                        }
                        if (!bandwidth_flag) {
                            int flag = 0;
                            if (SavePath[k].size() && cur < NUM) {
                                int p = SavePath[k][SavePath[k].size() - 1];
                                while (1) {
                                    if ((p - 1) % SIZE == (j - 1) % SIZE) {
                                        flag = 1;
                                        break;
                                    }
                                    else {
                                        if (SavePath[p].size()) {
                                            p = SavePath[p][SavePath[p].size() - 1];
                                        }
                                        else {
                                            break;
                                        }
                                    }
                                }
                            }
                            if (!flag) {
                                SavePath[j].push_back(k);
                                Dis[0][j] = Dis[0][k] + Dis[k][j];
                                PathResult[j] = k;
                            }
                        }
                    }
                }
            }
        }
    }
    //test
    /*cout << " path = " << endl;
    for (auto a : PathResult) {
    cout << a << " ";
    }
    cout << endl;
    cout << "D = " << endl;
    for (auto a : Dis[0]) {
    cout << a << " ";
    }
    cout << endl;*/
    //test end
    if (Dis[0][newsize + 1] < INT_MAX) {
        cout << "Dmin = " << Dis[0][newsize + 1] << endl;
        int p = PathResult[newsize + 1];
        vector<int> PlaceResult{ p };
        int temp = newsize + 2;
        while (temp != PathResult[p]) {
            temp = PathResult[p];
            p = temp;
            PlaceResult.insert(PlaceResult.begin(), temp);
        }
        vector<int> save_path;
        for (int i = 0; i < PlaceResult.size(); i++) {
            int temp = (PlaceResult[i] - 1) % SIZE;
            save_path.push_back(temp);
            //cout << temp << " ";
            UseCount[temp]++;
            NodeCapacity[temp][0] -= SliceReqCapacity[order][0][i][0];
            NodeCapacity[temp][1] -= SliceReqCapacity[order][0][i][1];
            NodeCapacity[temp][2] -= SliceReqCapacity[order][0][i][2];
        }
        //RESULT.push_back(save_path);
        int sum = 0;
        for (int z = 0; z < SIZE; z++) {
            sum += UseCount[z];
        }
        for (int z = 0; z < SIZE; z++) {
            U[z] = (double)UseCount[z] / (double)sum;
        }
        for (int i = 1; i < save_path.size(); i++) {
            delay += D[save_path[i - 1]][save_path[i]];
        }
        return save_path;
    }
    else {
        cout << " can not be deployed !!!" << endl;
    }
    cout << "RESULT = " << endl;
    cout << "usecount = " << endl;
    for (auto a : UseCount) {
        cout << a << endl;
    }
    cout << endl;
}

void VTB::StartDeployment() {
    int SIZE = AdjacencyMatrix.size();
    vector<double> U(SIZE, 0);
    vector<int> UseCount_temp(SIZE, 0);
    UseCount = UseCount_temp;
    for (int order = 0; order < SliceReq.size(); order++) {
        Two_D result_temp;
        vector<int> temp = VTBProcessing(SliceReq[order][0].size(), order, UseCount, U);
         result_temp.push_back(temp);
        for (int i = 1; i < SliceReq[order].size(); i++) {
            int ingress = -1;
            int outgress = -1;
            int chain_size = SliceReq[order][0].size();
            int desc = 0;
            if (SliceReq[order][i][0] <= chain_size) {
                ingress = temp[SliceReq[order][i][0]-1];
                desc--;
            }
            int last = SliceReq[order][i][SliceReq[order][i].size() - 1];
            if (last <= chain_size) {
                outgress = temp[last-1];
                desc--;
            }
            vector<int> vc_temp = VTBProcessing(SliceReq[order][i].size() + desc, order, UseCount, U, ingress, outgress);
            result_temp.push_back(vc_temp);
        }
        for (auto &a : result_temp) {
            for (auto b : a) {
                cout << b << " ";
            }
            cout << endl;
        }
        cout << endl;
        RESULT.push_back(result_temp);
        if (5 == order % 10) {
            for (auto a : UseCount) {
                cout << a << "  ";
            }
            cout << endl;
            break;
        }
    }
    cout << "Usecount = " << endl;
    for (auto a : UseCount) {
        cout << a << endl;
    }
    cout << endl;
}

void VTB::ComputeDelay() {
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
    int SIZE = AdjacencyMatrix.size();
    //从路径矩阵中提取距离矩阵
    vector<vector<int>> D;
    for (int i = 0; i < SIZE; i++) {
        vector<int> D_column(SIZE, 0);
        D.push_back(D_column);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            D[i][j] = ShortestPath[i][j].size() - 1;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        D[i][i] = 0;
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
                sum = sum + NODEDELAY*(D[start][end] - 1) + LINEDELAY * D[start][end] + (0.338 * UseCount[end] * pow(traffic, 12.15) + 0.51*traffic);
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