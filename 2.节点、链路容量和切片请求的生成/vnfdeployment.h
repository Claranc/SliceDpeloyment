#ifndef H_VNFDEPLOYMENT_H
#define H_VNFDEPLOYMENT_H
#include "standardinclude.h"

//定义VNF种类
enum VNFTYPE {
	AF, NEF, NSSF, AUSF, NRF, AMF, PCF, SMF, UDM, UDR, RAN, UPF
};

//定义切片请求类型
typedef struct SliceReq{
	string slice_id;
	vector<VNFTYPE> vnf_type;
	vector<vector<int>> vnf_params;
} SliceReq;

class CreateVNF {
public:
	void ReadFromFile(vector<vector<int>> &AdjacencyMatrix, const char *filename);
	void SetNodeCapacity(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity);
	map<string, vector<vector<int>>> CreateSliceReq(int sum);
	void SetLineBandwidth(vector<vector<int>> &, const char *);
	void SaveToFile(vector<vector<int>> &NodeCapacity, map<string, vector<vector<int>>> &slice_req, const char* FILENAME_NODECAPACITY, const char* FILENAME_SLILEREQ);
};

#endif