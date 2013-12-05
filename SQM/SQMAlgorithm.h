#pragma once
#include "SQMNode.h"
#include <fstream>
#include <ostream>
#include <time.h>

typedef enum {
	SQMStart = 0,
	SQMStraighten,
	SQMComputeConvexHull,
	SQMSubdivideConvexHull,
	SQMJoinBNPs,
	SQMFinalPlacement
} SQMState;

class SQMAlgorithm {
	SQMNode *root;
	SQMNode *resetRoot;
	int drawingMode;
	SQMState sqmState;
	MyMesh* mesh;
	filebuf *fb;
	ostream *os;
	int numOfNodes;
	int numOfSkinMatrices;
	SQMSmoothingAlgorithm smoothingAlgorithm;
	SkinSkeleton *skeleton;
	clock_t totalClocks;
	clock_t algorithmClocks;
	bool useCapsules;
	bool useCPUSkinning;
	
	SQMNode* findBNPInTree();
	void swapRoot(SQMNode *node);
	void fixWorm();
public:
	SQMAlgorithm(void);
	~SQMAlgorithm(void);

	void setRoot(SQMNode *newRoot);
	void setNumberOfNodes(int newNumberOfNodes);
	void setSmoothingAlgorithm(SQMSmoothingAlgorithm sqmSmoothingAlgorithm);
	void setUseCapsules(bool newUseCapsules);
	void setUseCPUSkinning(bool newUseCPUSkinning);

	SQMNode* getRoot();
	SQMState getState();
	MyMesh* getMesh();
	int getNumberOfNodes();
	int getNumberOfSkinningMatrices();
	void getSkinningMatrices(float* matrix);
	void getTransformMatrices(float* matrix);

	void straightenSkeleton();

	int countNodes();
	void refreshIDs();
	void calculateSkinSkeletonIDs();
	void rotateCycleOneRings();

	void getBoundingSphere(float &x, float &y, float &z, float &d);
	void updateResetRoot();
	void restart();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void finalVertexPlacement();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);
};

