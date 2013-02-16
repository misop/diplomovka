#pragma once
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <vector>
#include <sstream>

using namespace std;

class QhullController
{
public:
	QhullController(void);
	~QhullController(void);

	vector<vector<int> > generateConvexHullFacets(vector<OpenMesh::Vec4f> vertices);
};

