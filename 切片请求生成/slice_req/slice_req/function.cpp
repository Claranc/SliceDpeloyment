#include "slice_req.h"

//产生切片请求
Three_D SliceReq::CreateSF(int slicenum) {
	for (int i = 0; i < slicenum; ++i) {
		int serial_number = 0; //VNF编号
		Two_D slice; //保存VNF拓扑
		Three_D capacity;
		int linknum = 2 + rand() % 3; //链总数
		int mc_size = 8 + rand() % 8; //主链长度
		vector<int> mc_chain;
		for (int k = 0; k < mc_size; ++k) {
			mc_chain.push_back(++serial_number);
		}
        slice.push_back({ linknum });
		slice.push_back(mc_chain);
		Two_D mc_capacity;
		//生成主链容量
		for (int m = 0; m < mc_size; ++m) {
			int vcpu = rand() % 4 + 1;
			int mem = (int)pow(2, 1 + rand() % 3); 
			int disk = 5 + rand() % 96;
			int bandwidth = 10 + rand() % 50;
			vector<int> temp{ vcpu, mem, disk, bandwidth };
			mc_capacity.push_back(temp);
		}
        capacity.push_back({ {{linknum}} });
		capacity.push_back(mc_capacity);
		//生成副链
		for (int j = 1; j < linknum; ++j) {
			vector<int> vc_chain;
			Two_D vc_capacity;
			int vc_size = 4 + rand() % (mc_size - 6);
			int flag = rand() % 4;
			//与主链只有一个交点
			if (flag) {
				int Inter_section = 1 + rand() % (mc_size - 1);
				vc_chain.push_back(Inter_section);
				vc_capacity.push_back({ 1 });
				for (int l = 1; l < vc_size; ++l) {
					vc_chain.push_back(++serial_number);
					int vcpu = rand() % 4 + 1;
					int mem = (int)pow(2, 1 + rand() % 3);
					int disk = 5 + rand() % 96;
					int bandwidth = 10 + rand() % 50;
					vector<int> temp{ vcpu, mem, disk, bandwidth };
					vc_capacity.push_back(temp);
				}
			}
			//与主链有两个交点
			else {
				vc_size++; //避免出现{3,3}这种情况
				vc_capacity.push_back({ 2 });
				int inter_section1 = 1 + rand() % (mc_size-1);
				int inter_section2 = 1 + rand() % mc_size;
				vc_chain.push_back(inter_section1);
				for (int l = 2; l < vc_size; ++l) {
					vc_chain.push_back(++serial_number);
					int vcpu = rand() % 4 + 1;
					int mem = (int)pow(2, 1 + rand() % 3);
					int disk = 5 + rand() % 96;
                    int bandwidth = 10 + rand() % 50;
					vector<int> temp{ vcpu, mem, disk, bandwidth };
					vc_capacity.push_back(temp);
				}
				vc_chain.push_back(inter_section2);
			}
			slice.push_back(vc_chain);
			capacity.push_back(vc_capacity);
		}
		SliceVNF.push_back(slice);
		VNFCapacity.push_back(capacity);
	}
	return SliceVNF;
}

//获取切片请求
void SliceReq::GetSliceVNF() {
	for (auto &a : SliceVNF) {
		for (auto &b : a) {
			for (auto c : b) {
				cout << c << " -> ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

//获取切片请求容量
void SliceReq::GetSliceCapacity() {
	for (auto &a : VNFCapacity) {
		for (auto &b : a) {
			for (auto &c : b) {
				for (auto d : c) {
					cout << d << ",";
				}
				cout << "; ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void SliceReq::CreateMaxDelay(int SLICE_REQ_NUM, const char *delay_file) {
    for (int i = 0; i < SLICE_REQ_NUM; i++) {
        int temp = 17 + rand() % 8;
        max_delay.push_back(temp);
    }
    ofstream fout(delay_file, ios::out);
    if (!fout.is_open()) {
        cerr << "cannot open " << delay_file << endl;
        exit(-1);
    }
    for (int i = 0; i < max_delay.size(); i++) {
        fout << max_delay[i] << " ";
    }
    fout.close();
}

//输出到文件中
void SliceReq::WriteToFile(const char *filename_vnf, const char *filename_capacity) {
	//保存切片拓扑到文件中
	ofstream fout_vnf(filename_vnf, ios::out);
    int id = 1;
	for (auto &a : SliceVNF) {
        fout_vnf << "new ";
		for (auto &b : a) {
            if (1 == b.size()) {
                fout_vnf << b[0] << endl;
                continue;
            }
			for (auto c : b) {
				fout_vnf << c << " ";
			}
			fout_vnf << endl;
		}
	}
	fout_vnf.close();
	//保存切片请求容量到文件中
	ofstream fout_ca(filename_capacity, ios::out);
    id = 1;
	for (auto &a : VNFCapacity) {
		fout_ca << "new ";
		for (auto &b : a) {
            if (1 == b.size()) {
                fout_ca << b[0][0] << endl;
                continue;
            }
			for (auto &c : b) {
				for (auto d : c) {
					fout_ca << d << " ";
				}
				fout_ca << ";";
			}
			fout_ca << endl;
		}
	}
	fout_ca.close();
}