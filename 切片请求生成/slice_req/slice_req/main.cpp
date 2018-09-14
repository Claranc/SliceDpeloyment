#include "slice_req.h"

#define SLICE_REQ_NUM 5
#define SLICE_NODE_CAPACITY "slicereq_node_capacity"

int  main() {
	srand((unsigned)time(NULL));
	shared_ptr<SliceReq> p(new SliceReq);
	p->CreateSF(SLICE_REQ_NUM);
	p->GetSliceVNF();
	p->GetSliceCapacity();
	return 0;
}