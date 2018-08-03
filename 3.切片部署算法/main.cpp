/**
* ���������ƬVNF������BA�ޱ��������㷨
* ���룺
*	1.�����ڵ��������ļ�
*	2.������Ƭ�������ݵ��ļ�
*	2.�����ڽӾ������ݵ��ļ�
* �����
*	��Ƭ�Ĳ�����
*/
#include "class.h"

//���������ڽӾ����ļ���
#define FILENAME_INPUT "freeScaleNetwork.txt"

//��������ڵ��������ļ���
#define FILENAME_INTPUT_NODECAPACITY "node_capacity.txt"

//����������·�������ļ���
#define FILENAME_INPUT_LINEWIDTH "line_bandwidth.txt"

//����������Ƭ������ļ���
#define FILENAME_INTPUT_SLILEREQ "slice_req.txt"

//���岿����������ļ���
#define FILENAME_DEPLOYMENT_RESULT "slice_deployment_result.txt"

#define NODEDELAY 5 //�ڵ㴦��ʱ��
#define LINEDELAY 6 //��·����ʱ��
#define UPROBABILITY 180 //�ڵ�ʹ�ø���Ȩ��


int main() {
	vector<vector<int>> AdjacencyMatrix; //�ڽӾ���
	vector<vector<int>> NodeCapacity; //�ڵ�����
	vector<vector<int>> LineBandWidth; //��·����
	map<string, vector<vector<int>>> SliceReq; //��Ƭ����
	vector<vector<vector<int>>> ShortestPath; //�ڵ���·��
	map<string, vector<int>> DeploymentResult; //��Ƭ������
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