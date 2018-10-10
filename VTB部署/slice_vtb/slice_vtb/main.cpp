#include "class.h"

#define ADJACENCYMATRIX_FILE "freeScaleNetwork.txt"
#define NODECAPACITY_FILE "nodecapacity.txt"
#define LINKBANDWIDTH_FILE "linkbandwidth.txt"
#define SLICETOPOLOGY_FILE "slice_topology.txt"
#define SLICEREQ_FILE "slicereq_node_capacity.txt"

int main() {
    shared_ptr<VTB> p (new VTB);    
    //shared_ptr<TX> p (new TX);
    p->ReadFromFile(ADJACENCYMATRIX_FILE, NODECAPACITY_FILE, LINKBANDWIDTH_FILE, SLICETOPOLOGY_FILE, SLICEREQ_FILE);
    p->SetLinkWeight();
    p->StartDeployment();
    p->ComputeDelay();
    return 0;
}