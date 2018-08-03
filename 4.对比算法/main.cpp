/**
* 设计网络切片VNF部署在BA无标度网络的算法
* 输入：
*	1.包含节点容量的文件
*	2.包含切片请求数据的文件
*	2.包含邻接矩阵内容的文件
* 输出：
*	切片的部署结果
*/
#include "class.h"

//定义读入的邻接矩阵文件名
#define FILENAME_INPUT "freeScaleNetwork.txt"

//定义输入节点容量的文件名
#define FILENAME_INTPUT_NODECAPACITY "node_capacity.txt"

//定义输入链路容量的文件名
#define FILENAME_INPUT_LINEWIDTH "line_bandwidth.txt"

//定义输入切片请求的文件名
#define FILENAME_INTPUT_SLILEREQ "slice_req.txt"

//定义部署结果的输出文件名
#define FILENAME_DEPLOYMENT_RESULT "slice_deployment_result.txt"

#define NODEDELAY 5 //节点处理时延
#define LINEDELAY 6 //链路传输时延
#define UPROBABILITY 180 //节点使用概率权重


int main() {
	vector<vector<int>> AdjacencyMatrix; //邻接矩阵
	vector<vector<int>> NodeCapacity; //节点容量
	vector<vector<int>> LineBandWidth; //链路带宽
	map<string, vector<vector<int>>> SliceReq; //切片请求
	vector<vector<vector<int>>> ShortestPath; //节点间的路径
	map<string, vector<int>> DeploymentResult; //切片部署结果
	VNFDeployment p;
	p.ReadFromFile(AdjacencyMatrix, FILENAME_INPUT, NodeCapacity, FILENAME_INTPUT_NODECAPACITY, SliceReq, FILENAME_INTPUT_SLILEREQ, LineBandWidth, FILENAME_INPUT_LINEWIDTH);
	p.SetLineWeight(AdjacencyMatrix, NodeCapacity, SliceReq, ShortestPath);
	/*AdjacencyMatrix = {
		{ 0, 1, 1, 0, 0 },
		{ 1, 0, 0, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 1, 0, 0 } };*/
	p.StartDeployment(AdjacencyMatrix, NodeCapacity, SliceReq, ShortestPath, DeploymentResult, LineBandWidth, NODEDELAY, LINEDELAY, UPROBABILITY);
	p.SaveResultToFile(DeploymentResult, FILENAME_DEPLOYMENT_RESULT);
	return 0;
}