#include "slice_req.h"

#define SLICE_REQ_NUM 5
#define SLICE_NODE_CAPACITY "slicereq_node_capacity.txt"
#define SLICE_TOPOLOGY "slice_topology.txt"

int  main() {
	srand((unsigned)time(NULL));
	shared_ptr<SliceReq> p(new SliceReq);
	//SliceReq *p = new SliceReq;
	p->CreateSF(SLICE_REQ_NUM);
	p->GetSliceVNF();
	p->GetSliceCapacity();
	p->WriteToFile(SLICE_TOPOLOGY, SLICE_NODE_CAPACITY);
	return 0;
}