#include "LIE.h"

bool LIE::containsVertice(int verticeIndex) {
	if (verticeIndex == vertice1 || verticeIndex == vertice2)
		return true;

	return false;
}

int LIE::otherVerticeIndex(int verticeIndex) {
	if (verticeIndex == vertice1)
		return vertice2;
	else
		return vertice1;
}