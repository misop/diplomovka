#pragma once
#include "MyMesh.h"

struct EdgeLength {
	MyMesh::HalfedgeHandle hehandle;
	float length;

	EdgeLength(MyMesh::HalfedgeHandle halfedgeHandle, float edgeLength) : hehandle(halfedgeHandle), length(edgeLength) { };
};

struct EdgeLengthComp {
	bool operator() (const EdgeLength& lhs, const EdgeLength& rhs) const {		
		return lhs.length < rhs.length;
	}
};

bool EdgeLengthCompare(EdgeLength el1, EdgeLength el2);