#include <glm.hpp>
#include <vector>
#include "m_math.h"

struct SkinSkeleton {
	CVector3 position;
	Quaternion axisAngle;
	SkinSkeleton *parent;
	glm::mat4 matrix;

	std::vector<SkinSkeleton*> nodes;

	SkinSkeleton();
	SkinSkeleton(float x, float y, float z);
	SkinSkeleton(SkinSkeleton *father, float x, float y, float z);
	SkinSkeleton(CVector3 pos);
	SkinSkeleton(CVector3 pos, Quaternion axis_Angle);
	~SkinSkeleton();

	void CalculateCorrespondingDoF(SkinSkeleton *another);
	glm::mat4 ComputeLocalMatrix();
	void ComputeSkinningMatrices();
	void InvertMatrices();
	void PrecomputeFinalSkinningMatrices(SkinSkeleton *another);
};