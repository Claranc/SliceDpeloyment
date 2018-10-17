#include "class.h"

#define ADJACENCYMATRIX_FILE "freeScaleNetwork.txt"
#define NODECAPACITY_FILE "nodecapacity.txt"
#define LINKBANDWIDTH_FILE "linkbandwidth.txt"
#define SLICETOPOLOGY_FILE "slice_topology.txt"
#define SLICEREQ_FILE "slicereq_node_capacity.txt"
#define MAX_DELAY "max_delay.txt"

#define DATA_FILE "result_sa50_200.csv"

int main() {
    SA p;
    srand((unsigned) time(NULL));
    ofstream fout1(DATA_FILE, ios::trunc);
    if (!fout1.is_open()) {
        cerr << "cannot open " << DATA_FILE << endl;
        exit(-1);
    }
    fout1.close();
    //shared_ptr<VTB> p (new VTB);    
    //shared_ptr<TX> p (new TX);
    //shared_ptr<SA> p(new SA);
    p.ReadFromFile(ADJACENCYMATRIX_FILE, NODECAPACITY_FILE, LINKBANDWIDTH_FILE, SLICETOPOLOGY_FILE, SLICEREQ_FILE);
    p.SetLinkWeight();
    p.StartDeployment(); 
   for (int i = 10; i <= 10; i++) {        
        double t = (double)i / 10.0;
        p.traffic = t;
        p.ComputeDelay();
        p.SaveToFile(DATA_FILE);
    }
}