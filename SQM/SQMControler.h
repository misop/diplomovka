#pragma once
#include "SQMAlgorithm.h"
#include <string>

using namespace std;

class SQMControler
{
private:
	SQMAlgorithm *sqmALgorithm;
public:
	SQMControler(void);
	~SQMControler(void);

	void loadSkeletonFromFile(string fileName);
	void saveSkeletonToFile(string fileName);

	void straightenSkeleton();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);

	void draw();
	void getBoundingSphere(float &x, float &y, float &z, float &d);
};

