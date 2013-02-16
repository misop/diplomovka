#pragma once
#include "SQMNode.h"

class SQMAlgorithm {
	SQMNode *root;
	int drawingMode;
	MyMesh* mesh;
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

