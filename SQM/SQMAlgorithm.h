#pragma once
#include "SQMNode.h"
#include <fstream>
#include <ostream>

class SQMAlgorithm {
	SQMNode *root;
	int drawingMode;
	MyMesh* mesh;
	filebuf *fb;
	ostream *os;
public:
	SQMAlgorithm(void);
	~SQMAlgorithm(void);

	void setRoot(SQMNode *newRoot);

	void straightenSkeleton();

	void draw();
	void draw2();

	void getBoundingSphere(float &x, float &y, float &z, float &d);
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
};

