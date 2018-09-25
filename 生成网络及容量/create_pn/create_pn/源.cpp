/**
* ���ܣ�
*	����BA�ޱ��������ڽӾ��󣬲��������ļ���
* ���룺
*	1. �����С
*	2. ������
*	3. ��ʼ�ڵ���
* �����
*	�������ڽӾ�����ļ�
*/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
using namespace std;

#define NETWORK_SIZE	50	//����Ĵ�С
#define M		        2		//�����һ���µĽڵ㣬Ҫ���ӵ�M���Ѿ����ڵĵ���
#define M_0		        2	//��ʼʱ�̵�����ڵ���

/*
* ���������򣺣�1��degree����ʾ�ڵ�Ķ�
* 	       ��2��weight����ʾ��ѡ��ĸ��ʣ�Ȩ��
* */
typedef struct Node {
	int degree;
	double weight;
	double probabilityDistribution;	//�����˸������еĸ��������ͨ������0~1֮�����������������ߡ�
} Node;
Node decisionMaking[NETWORK_SIZE + 1];

int adjacentMatrix[NETWORK_SIZE + 1][NETWORK_SIZE + 1];
int initalNetwork[M_0 + 1];

void initalNetwork_M0_connected();
void updateDecisionMakingData();
void generateFreeScaleNetwork();
void showAdjacentMatrix();
void createNodeCapacity();
void writeDataToFile();

int main() {
	if (M > M_0 || NETWORK_SIZE < M_0) {
		cerr << "There are something wrong with your parameters" << endl;
		exit(-1);
	}
	srand((unsigned)time(NULL));
    initalNetwork_M0_connected();
	generateFreeScaleNetwork();
	writeDataToFile();
	//showAdjacentMatrix();
	createNodeCapacity();
	return 0;
}

/*
* ��ʼ������NETWORK_SIZE�����ѡ��M_0���ڵ㹹����ͨ�����硣
* */
void initalNetwork_M0_connected() {
	int i;
	int j;

	for (i = 1; i <= NETWORK_SIZE; i++) {
		for (j = 1; j <= NETWORK_SIZE; j++) {
			adjacentMatrix[i][j] = 0;
		}
	}
	//�������M_0���ڵ�,���ظ�
	for (i = 1; i <= M_0; i++) {
		initalNetwork[i] = rand() % NETWORK_SIZE + 1;
		for (j = 1; j < i; j++) {
			if (initalNetwork[i] == initalNetwork[j]) {
				i--;
				break;
			}
		}
	}
	//����������k=1��
	for (i = 1; i < M_0; i++) {
		adjacentMatrix[initalNetwork[i]][initalNetwork[i + 1]] = 1;
		adjacentMatrix[initalNetwork[i + 1]][initalNetwork[i]] = 1;
	}
	adjacentMatrix[initalNetwork[M_0]][initalNetwork[1]] = 1;
	adjacentMatrix[initalNetwork[1]][initalNetwork[M_0]] = 1;
	updateDecisionMakingData();
}

/*
* ����BA�ޱ������ģ��
* */
void generateFreeScaleNetwork() {
	int i;
	int j = 1;
	int k;
	int random_auxiliary_old[NETWORK_SIZE + 1];
	int random_auxiliary[NETWORK_SIZE + 1 - M_0];
	int length = 0;

	/*
	* Ҫ��֤ÿ������һ��<�µ�>�Ľڵ㣬����Ҫ���ѡ���ظ��Ľڵ���룬���Ұѳ�ʼ�����е�M_0���ڵ���ɾ�����ҳ��½ڵ��ţ���
	* */
	for (i = 1; i <= NETWORK_SIZE; i++) {
		random_auxiliary_old[i] = i;
	}

	for (i = 1; i <= M_0; i++) {
		random_auxiliary_old[initalNetwork[i]] = 0;
	}
	for (i = 1; i <= NETWORK_SIZE; i++) {
		if (random_auxiliary_old[i] != 0) {
			random_auxiliary[j++] = random_auxiliary_old[i];
		}
	}

	/*
	* ����µĽڵ�
	* */
	int new_node_index;
	int new_node_value;
	double random_decision = 0.0;
	int targetNode;			//���ڴ洢�ҵ����Ѿ��������еĵ�
	length = NETWORK_SIZE - M_0;
	int flag;
	for (i = 1; i <= NETWORK_SIZE - M_0; i++) {
		//�õ�����Ĳ��ظ��ļ���㣬��ʹ��Vector������ô�鷳
		new_node_index = rand() % length + 1;
		new_node_value = random_auxiliary[new_node_index];
		random_auxiliary[new_node_index] = random_auxiliary[length--];
		for (j = 1; j <= M; j++) {		//���ݸ������ӵ��Ѵ��������е�M���ڵ㣬�������رߣ�������������
			flag = 0;
			while (!flag) {
				random_decision = (rand() % 1000) / (double)1000;
				for (k = 1; k <= NETWORK_SIZE; k++) {
					if (decisionMaking[k].probabilityDistribution >= random_decision && decisionMaking[k].degree != 0 && adjacentMatrix[new_node_value][k] != 1 && new_node_value != k) {
						targetNode = k;
						flag = 1;
						break;
					}
				}
			}
			adjacentMatrix[new_node_value][targetNode] = 1;
			adjacentMatrix[targetNode][new_node_value] = 1;
		}
		updateDecisionMakingData();		//else��ѡ�ļ���ڵ�����������е�M���߶����Ӻ��ٸ���
	}
}

/*
* ͨ��adjacentMatrix����decisionMaking����
* */
void updateDecisionMakingData() {
	int i;
	int j;
	int totalDegree = 0;

	for (i = 1; i <= NETWORK_SIZE; i++)
		decisionMaking[i].degree = 0;
	for (i = 1; i <= NETWORK_SIZE; i++) {
		for (j = 1; j <= NETWORK_SIZE; j++) {
			decisionMaking[i].degree += adjacentMatrix[i][j];
		}
	}
	for (i = 1; i <= NETWORK_SIZE; i++)
		totalDegree += decisionMaking[i].degree;
	for (i = 1; i <= NETWORK_SIZE; i++) {
		decisionMaking[i].weight = decisionMaking[i].degree / (double)totalDegree;
	}
	decisionMaking[1].probabilityDistribution = decisionMaking[1].weight;
	for (i = 2; i <= NETWORK_SIZE; i++) {
		decisionMaking[i].probabilityDistribution = decisionMaking[i - 1].probabilityDistribution + decisionMaking[i].weight;
	}
}

void showAdjacentMatrix() {
	int i;
	int j;
	int numberOfEage = 0;
	cout << "-------------------------------------show adjacentMatrix-----------------------------------------\n";
	for (i = 1; i <= NETWORK_SIZE; i++) {
		for (j = 1; j <= NETWORK_SIZE; j++) {
			cout << adjacentMatrix[i][j] << " ";
			if (adjacentMatrix[i][j] == 1) {
				numberOfEage++;
			}
		}
		cout << endl;
	}
	cout << "the number of eage is " << numberOfEage / 2 << endl;
}

void writeDataToFile() {
	ofstream fout("freeScaleNetwork.txt", ios::out);
	int i;
	int j;

	for (i = 1; i <= NETWORK_SIZE; i++) {
		for (j = 1; j <= NETWORK_SIZE; j++) {
			fout << adjacentMatrix[i][j] << " ";
		}
		fout << endl;
	}
	fout.close();
}

void createNodeCapacity() {
	vector<vector<int>> NodeCapacity;
    ofstream fout1("nodecapacity.txt", ios::out);
	for (int i = 0; i < NETWORK_SIZE; ++i) {
		int vcpu, mem, disk;
		vcpu = rand() % 50 + 1000;
		mem = (int)pow(6, 8 + rand() % 4);
		disk = 15000 + rand() % 1501;
		vector<int> ca;
		ca.insert(ca.end(), { vcpu, mem, disk });
		NodeCapacity.push_back(ca);
	}
    for (auto &a : NodeCapacity) {
        for (auto b : a) {
            fout1 << b << " ";
        }
        fout1 << endl;
    }
    fout1.close();
    vector<vector<int>> LinkBandwidth;
    ofstream fout2("LinkBandwidth.txt", ios::out);
    for (int i = 0; i < NETWORK_SIZE; ++i) {
        for (int j = 0; j < NETWORK_SIZE; ++j) {
            if (1 == adjacentMatrix[i][j]) {
                fout2 << 1510 + rand() % 50 << " ";
            }
            else {
                fout2 << 0 << " ";
            }
        }
        fout2 << endl;
    }
    fout2.close();
}