#ifndef SLICE_REQ_H
#define SLICE_REQ_H

#include "stdinclude.h"

class SliceReq {
private:
	Three_D SliceVNF;
	Four_D VNFCapacity;
public:
	Three_D CreateSF(int slicenum);
	void GetSliceVNF();
	void WriteToFile(const char *filename_vnf, const char *filename_capacity);
	void GetSliceCapacity();
};

enum  vctype {
	BRANCH, CROSS
};

#endif