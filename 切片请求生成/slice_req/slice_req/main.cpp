#include "slice_req.h"

#define SLICE_REQ_NUM 100
#define SLICE_NODE_CAPACITY "slicereq_node_capacity.txt"
#define SLICE_TOPOLOGY "slice_topology.txt"
#define MAX_DELAY "max_delay.txt"

int  main() {
	srand((unsigned)time(NULL));
	shared_ptr<SliceReq> p(new SliceReq);
	//SliceReq *p = new SliceReq;
	p->CreateSF(SLICE_REQ_NUM);
	p->GetSliceVNF();
	p->GetSliceCapacity();
	p->WriteToFile(SLICE_TOPOLOGY, SLICE_NODE_CAPACITY);
    p->CreateMaxDelay(SLICE_REQ_NUM, MAX_DELAY);
	return 0;
}