#pragma once
#include "MyMesh.h"
#include <vector>

struct LIE {
	int vertice1;
	int vertice2;

	std::vector<MyTriMesh::EdgeHandle> edges;
	LIE(int v1, int v2) : vertice1(v1), vertice2(v2) { };

	bool containsVertice(int verticeIndex);
	int otherVerticeIndex(int verticeIndex);
};