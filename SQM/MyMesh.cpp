#include "stdafx.h"
#include "MyMesh.h"
#include "FloatArithmetic.h"

bool OpenMeshEqualVHandles(MyMesh::VHandle& h1, MyMesh::VHandle& h2) {
	return h1.idx() == h2.idx();
}