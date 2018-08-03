/**
 * ����������������Լ���Ƭ����������
 * ���룺
 *	1.��Ƭ����
 *	2.�����ڽӾ������ݵ��ļ�
 * �����
 *	�ڵ�������Լ���Ƭ�������ļ�
 */
#include "vnfdeployment.h"

//���岿�����Ƭ����
#define SLICESUM 10

//���������ڽӾ����ļ���
#define FILENAME_INPUT "freeScaleNetwork.txt"

//��������ڵ��������ļ���
#define FILENAME_OUTPUT_NODECAPACITY "node_capacity.txt"

//���������Ƭ������ļ���
#define FILENAME_OUTPUT_SLILEREQ "slice_req.txt"

//���������·������ļ���
#define FILE_OUTPUT_LINEBANDWIDTH "line_bandwidth.txt"

int main() {
	vector<vector<int>> AdjacencyMatrix; //�����ڽӾ���
	vector<vector<int>> NodeCapacity; //����ڵ� ����
	srand((unsigned)time(NULL));
	CreateVNF p;
	p.ReadFromFile(AdjacencyMatrix, FILENAME_INPUT);
	p.SetNodeCapacity(AdjacencyMatrix, NodeCapacity);
	p.SetLineBandwidth(AdjacencyMatrix, FILE_OUTPUT_LINEBANDWIDTH);
	map<string, vector<vector<int>>> slice_req = p.CreateSliceReq(SLICESUM); //������Ƭ����
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