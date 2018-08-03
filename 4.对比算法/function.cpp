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

void VNFDeployment::ReadFromFile(vector<vector<int>> &AdjacencyMatrix, const char *fileAdjacencyMatrix, vector<vector<int>> &NodeCapacity, \
	const char *fileNodeCapacity, map<string, vector<vector<int>>> &slice_req, const char* fileSliceReq, vector<vector<int>> &LineWidth, const char *fileLineWidth) {
	//读入邻接矩阵
	ifstream fin_AdjacencyMatrix(fileAdjacencyMatrix, ios::in);
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
	//test
	/*for (auto &c : AdjacencyMatrix) {
		for (auto d : c) {
			cout << d << " ";
		}
		cout << endl;
	}*/
	//test end
	//读入节点容量
	ifstream fin_NodeCapacity(fileNodeCapacity, ios::in);
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
	//test
	/*for (auto &c : NodeCapacity) {
		for (auto d : c) {
			cout << d << " ";
		}
		cout << endl;
	}*/
	//test end
	//读入链路容量
	ifstream fin_LineWidth(fileLineWidth, ios::in);
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
		LineWidth.push_back(li);
		LineStr.clear();
	}
	//test
	/*for (auto &c : LineWidth) {
	for (auto d : c) {
	cout << d << " ";
	}
	cout << endl;
	}*/
	//test end
	//读入切片请求
	ifstream fin_SliceReq(fileSliceReq, ios::in);
	if (!fin_SliceReq.is_open()) {
		cerr << "The SliceReq file cannot be open!!!" << endl;
		exit(-1);
	}
	while (getline(fin_SliceReq, LineStr)) {
		auto a = LineStr.find_first_of("_");
		string slice_id = LineStr.substr(0, a);
		vector<vector<int>> slice_params;
		string datastr = LineStr.substr(a + 1);
		vector<string> data;
		SplitString(datastr, data, ";");
		for (auto &a : data) {
			stringstream ls(a);
			string temp;
			vector<int> data_column;
			while (ls >> temp) {
				data_column.push_back(stoi(temp));
			}
			slice_params.push_back(data_column);
		}
		slice_req.insert(make_pair(slice_id, slice_params));
	}
	//test
	/*for (auto &a : slice_req) {
		cout << a.first << endl;
		for (auto &b : a.second) {
			for (auto c : b) {
				cout << c << " ";
			}
			cout << endl;
		}
	}*/
	//test end
	//检验文件
	if (AdjacencyMatrix.size() != NodeCapacity.size()) {
		cerr << "The files are not compatible" << endl;
		exit(-1);
	}
}

void VNFDeployment::SetLineWeight(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity, \
	map<string, vector<vector<int>>> &slice_req, vector<vector<vector<int>>> &ShortestPath) {
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
	/*for (auto &a : ShortestPath) {
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

void VNFDeployment::StartDeployment(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity, \
	map<string, vector<vector<int>>> &slice_req, vector<vector<vector<int>>> &ShortestPath, \
	map<string, vector<int>> &DeploymentResult, vector<vector<int>> &LineBandWidth, int NODEDELAY, int LINEDELAY, int UPROBABILITY) {
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
	/*cout << "D = " << endl;
	for (auto &c : D) {
		for (auto d : c) {
			cout << d << " ";
		}
		cout << endl;
	}*/
	//开始部署切片网元
	for(auto &a : slice_req) {
		int NUM = a.second[0].size();
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
			if (i/SIZE > 0 && i/SIZE < NUM - 1) {
				for (int j = SIZE * (i/SIZE-1); j < SIZE * (i/SIZE); j++) {
					LineWeight[i][j] = 1;
				}
				LineWeight[i][i - SIZE] = 0;
				for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
					LineWeight[i][j] = 1;
				}
				LineWeight[i][i + SIZE] = 0;
			}
			//第一层
			else if(i/SIZE == 0) {
				for (int j = SIZE * (i / SIZE + 1); j < SIZE * (i / SIZE + 2); j++) {
					LineWeight[i][j] = 1;
				}
				LineWeight[i][i + SIZE] = 0;
			}
			//最后一层
			else if(i/SIZE == NUM - 1){
				for (int j = SIZE * (i / SIZE - 1); j < SIZE * (i / SIZE); j++) {
					LineWeight[i][j] = 1;
				}
				LineWeight[i][i - SIZE] = 0;
			}
		} 
		//test
		/*for (auto &a : LineWeight) {
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
						//LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE] + UPROBABILITY * U[j%SIZE];
						//对比算法不考虑节点使用概率这个因素
						LineWeight[i][j] = NODEDELAY*(D[i%SIZE][j%SIZE] - 1) + LINEDELAY * D[i%SIZE][j%SIZE];
					}
					else {
						//LineWeight[i][j] = UPROBABILITY * U[j%SIZE];
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
			Weight[0][i] = 1;
			Weight[i][0] = 1;
			Weight[newsize + 1][newsize + 1 - i] = 1;
			Weight[newsize + 1 - i][newsize + 1] = 1;
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
		/*cout << "single = " << endl;
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
					if (seq == 0 && !(a.second[seq + 1][0] < NodeCapacity[(k - 1) % SIZE][0] && a.second[seq + 1][1] < NodeCapacity[(k - 1) % SIZE][1] \
						&& a.second[seq + 1][2] < NodeCapacity[(k - 1) % SIZE][2])) {
						continue;
					}
					min = Dis[0][i];
					k = i;
				}
			}
			book[k] = 1;
			for (int j = 0; j < newsize + 2; j++) {
				if (Dis[k][j] < INT_MAX) {
					if (Dis[0][j] > Dis[0][k] + Dis[k][j]) {
						int cur = (j - 1) / SIZE; //cur代表是第几个VNF
						if (cur == NUM || a.second[cur + 1][0] < NodeCapacity[(j - 1) % SIZE][0] && a.second[cur + 1][1] < NodeCapacity[(j - 1) % SIZE][1] \
							&& a.second[cur + 1][2] < NodeCapacity[(j - 1) % SIZE][2]) {
							int start = (k - 1) % SIZE;
							int end = (j - 1) % SIZE;
							int bandwidth_flag = 0;
							if (cur > 0 && cur < NUM) {
								for (int x = 1; x < ShortestPath[start][end].size(); x++) {
									//cout << start << " : " << end << endl;
									if (start != end && LineBandWidth[ShortestPath[start][end][x - 1]][ShortestPath[start][end][x]] < a.second[a.second.size() - 1][cur - 1]) {
										bandwidth_flag = 1;
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
				NodeCapacity[temp][0] -= a.second[i + 1][0];
				NodeCapacity[temp][1] -= a.second[i + 1][1];
				NodeCapacity[temp][2] -= a.second[i + 1][2];
			}
			DeploymentResult.insert(make_pair(a.first, save_path));
			int sum = 0;
			for (int z = 0; z < SIZE; z++) {
				sum += UseCount[z];
			}
			for (int z = 0; z < SIZE; z++) {
				U[z] = (double)UseCount[z] / (double)sum;
			}
			//输出节点使用频数和节点容量
			//test
			/*for (int i = 0; i < SIZE; i++) {
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
			//break;
		}
		else {
			cout << a.first <<" can not be deployed !!!" << endl;
			//break;
		}
	}
}

void VNFDeployment::SaveResultToFile(map<string, vector<int>> &DeploymentResult, const char *filename) {
	ofstream fout(filename, ios::out);
	for (auto &a : DeploymentResult) {
		fout << a.first << ": ";
		for (auto b : a.second) {
			fout << b << " ";
		}
		fout << endl;
	}
}