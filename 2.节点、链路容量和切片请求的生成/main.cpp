/**
 * 设计网络拓扑容量以及切片的请求数据
 * 输入：
 *	1.切片数量
 *	2.包含邻接矩阵内容的文件
 * 输出：
 *	节点的容量以及切片的请求文件
 */
#include "vnfdeployment.h"

//定义部署的切片数量
#define SLICESUM 10

//定义读入的邻接矩阵文件名
#define FILENAME_INPUT "freeScaleNetwork.txt"

//定义输出节点容量的文件名
#define FILENAME_OUTPUT_NODECAPACITY "node_capacity.txt"

//定义输出切片请求的文件名
#define FILENAME_OUTPUT_SLILEREQ "slice_req.txt"

//定义输出链路带宽的文件名
#define FILE_OUTPUT_LINEBANDWIDTH "line_bandwidth.txt"

int main() {
	vector<vector<int>> AdjacencyMatrix; //保存邻接矩阵
	vector<vector<int>> NodeCapacity; //保存节点 容量
	srand((unsigned)time(NULL));
	CreateVNF p;
	p.ReadFromFile(AdjacencyMatrix, FILENAME_INPUT);
	p.SetNodeCapacity(AdjacencyMatrix, NodeCapacity);
	p.SetLineBandwidth(AdjacencyMatrix, FILE_OUTPUT_LINEBANDWIDTH);
	map<string, vector<vector<int>>> slice_req = p.CreateSliceReq(SLICESUM); //保存切片请求
	//test
	/*for (auto a : slice_req) {
		cout << a.first << endl;
		for (auto &b : a.second) {
			for (auto c : b) {
				cout << c << " ";
			}
			cout << endl;
		}
	}*/
	//test end
	p.SaveToFile(NodeCapacity, slice_req, FILENAME_OUTPUT_NODECAPACITY, FILENAME_OUTPUT_SLILEREQ);
	return 0;
}