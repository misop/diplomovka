#pragma once
#include "SQMAlgorithm.h"
#include <string>

using namespace std;

class SQMControler
{
private:
	SQMAlgorithm *sqmALgorithm;
public:
	SQMNode *selected;

	SQMControler(void);
	~SQMControler(void);

	void loadSkeletonFromFile(string fileName);
	void saveSkeletonToFile(string fileName);

	bool selectNodeInRay(OpenMesh::Vec3f position, OpenMesh::Vec3f direction);
	bool closestNodeRayIntersection(SQMNode *node, OpenMesh::Vec3f rayPosition, OpenMesh::Vec3f direction, float& dist);

	void straightenSkeleton();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);

	void draw();
	void getBoundingSphere(float &x, float &y, float &z, float &d);
};

