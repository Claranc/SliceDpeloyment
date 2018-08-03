#include "stdincludes.h"

//����VNF����
enum VNFTYPE {
	AF, NEF, NSSF, AUSF, NRF, AMF, PCF, SMF, UDM, UDR, RAN, UPF
};

//������Ƭ��������
typedef struct SliceReq{
	string slice_id;
	vector<int> vnf_type;
	vector<vector<int>> vnf_params;
} SliceReq;

class VNFDeployment {
public:
	void ReadFromFile(vector<vector<int>> &AdjacencyMatrix, const char *fileAdjacencyMatrix, \
		vector<vector<int>> &NodeCapacity, const char *fileNodeCapacity, map<string, vector<vector<int>>> &slice_req, const char* fileSliceReq, vector<vector<int>> &, const char *);
	void SetLineWeight(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity, \
		map<string, vector<vector<int>>> &slice_req, vector<vector<vector<int>>> &);
	void StartDeployment(vector<vector<int>> &AdjacencyMatrix, vector<vector<int>> &NodeCapacity, \
		map<string, vector<vector<int>>> &slice_req, vector<vector<vector<int>>> &, map<string, vector<int>> &, vector<vector<int>> &, int, int, int);
	void SaveResultToFile(map<string, vector<int>> &, const char *);
};
