#include <glm.hpp>
#include <vector>
#include "m_math.h"

struct SkinSkeleton {
	int id;
	CVector3 position;
	Quaternion axisAngle;
	Quaternion quaternion;
	SkinSkeleton *parent;
	glm::mat4 matrix;

	std::vector<SkinSkeleton*> nodes;

	SkinSkeleton();
	SkinSkeleton(float x, float y, float z);
	SkinSkeleton(SkinSkeleton *father, float x, float y, float z);
	SkinSkeleton(CVector3 pos);
	SkinSkeleton(CVector3 pos, Quaternion axis_Angle);
	~SkinSkeleton();

	bool isBNP();

	void CalculateCorrespondingDoF(SkinSkeleton *another);
	glm::mat4 ComputeLocalMatrix();
	void ComputeSkinningMatrices();
	void InvertMatrices();
	void PrecomputeFinalSkinningMatrices(SkinSkeleton *another);

	void ComputeCompoundRotation();
};