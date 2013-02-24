#pragma once
#include "SQMNode.h"
#include <fstream>
#include <ostream>

typedef enum {
	SQMStart = 0,
	SQMStraighten,
	SQMComputeConvexHull,
	SQMSubdivideConvexHull,
	SQMJoinBNPs
} SQMState;

class SQMAlgorithm {
	SQMNode *root;
	int drawingMode;
	SQMState sqmState;
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
	void executeSQMAlgorithm(SQMState state);
};

