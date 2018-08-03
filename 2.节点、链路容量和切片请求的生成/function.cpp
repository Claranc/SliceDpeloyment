#include "vnfdeployment.h"

//���ļ��ж����ڽӾ���
void CreateVNF::ReadFromFile(vector<vector<int>> &AdjacencyMatrix, const char* filename) {
	ifstream fin(filename, ios::in);
	if (!fin.is_open()) {
		cerr << "The file cannot open!!!" << endl;
		exit(-1);
	}
	string LineStr;
	while (getline(fin, LineStr)) {
		if (LineStr.size() == 0)
			break;
		vector<int> Matrix_column;
		stringstream ss(LineStr);
		string params;
		while (ss >> params) {
			Matrix_column.push_back(stoi(params));
		}
		AdjacencyMatrix.push_back(Matrix_column);
	}
	//test
	/*for (auto &c : AdjacencyMatrix) {
		for (auto d : c) {
			cout << d << " ";
		}
		cout << endl;
	}*/
	//test end
}

//����ÿ���ڵ������
void CreateVNF::SetNodeCapacity(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity) {
	for (size_t i = 0; i < AdjacencyMatrix.size(); i++) {
		int vcpu, mem, disk;
		vcpu = rand() % 50 + 100;
		mem = (int)pow(2, 8 + rand() % 4);
		disk = 1500 + rand() % 1501;
		vector<int> ca;
		ca.insert(ca.end(), {vcpu, mem, disk});
		NodeCapacity.push_back(ca);
	}
	//test
	/*for (auto &c : NodeCapacity) {
		for (auto d : c) {
			cout << d << " ";
		}
		cout << endl;
	}*/
	//test end
}

void CreateVNF::SetLineBandwidth(vector<vector<int>> &AdjacencyMatrix, const char *filename) {
	ofstream fout(filename, ios::out);
	for (auto &a : AdjacencyMatrix) {
		for (auto b : a) {
			if (!b) {
				fout << 0 << " ";
			}
			else {
				fout << 151 + rand() % 50 << " ";
			}
		}
		fout << endl;
	}
}

//������Ƭ����
map<string,vector<vector<int>>>CreateVNF::CreateSliceReq(int sum) {
	map<string, vector<vector<int>>> slice;
	for (int m = 0; m < sum; m++) {
		string slice_id = to_string(1 + rand() % 10000); //��Ƭid
		vector<int> myvector;
		for (int i = 1; i <= 12; ++i)
			myvector.push_back(i);
		random_shuffle(myvector.begin(), myvector.end());
		random_shuffle(myvector.begin(), myvector.end(), [](int i) {return rand() % i; });
		//test
		/*for (auto c : a) {
		cout << c << " ";
		}
		cout << endl;*/
		//test end
		int K = 3 + rand() % 10; //������Ƭ��VNF����
		vector<int> b(myvector.begin(), myvector.begin() + K);
		vector<vector<int>> params;
		params.push_back(b);
		for (int i = 0; i < K; i++) {
			int vcpu = rand() % 4 + 1;
			int mem = (int)pow(2, 1 + rand() % 3);
			int disk = 5 + rand() % 96;
			params.push_back({ vcpu, mem, disk });
		}
		//����VNF��·�������
		vector<int> linewidth;
		for (int i = 0; i < K - 1; i++) {
			int p = 1 + rand() % 4;
			linewidth.push_back(p);
		}
		params.push_back(linewidth);
		//test
		/*for (auto &c : params) {
			for (auto d : c) {
				cout << d << " ";
			}
			cout << endl;
		}*/
		//test end
		slice.insert(make_pair(slice_id, params));
	}
	return slice;
}

//�����������������Ƭ�������������ļ���
void CreateVNF::SaveToFile(vector<vector<int>> &NodeCapacity, map<string, vector<vector<int>>> &slice_req, const char* FILENAME_NODECAPACITY, const char* FILENAME_SLILEREQ) {
	//���ڵ�����������ļ���
	ofstream fout_capacity(FILENAME_NODECAPACITY, ios::out);
	for (auto &c : NodeCapacity) {
		for (auto d : c) {
			fout_capacity << d << " ";
		}
		fout_capacity << endl;
	}
	//����Ƭ����������ļ���
	ofstream fout_slicereq(FILENAME_SLILEREQ, ios::out);
	for (auto &a : slice_req) {
		fout_slicereq << a.first << "_";
		for (auto &b : a.second) {
			for (auto c : b) {
				fout_slicereq << c << " ";
			}
			fout_slicereq << ";";
		}
		fout_slicereq << endl;
	}
}

