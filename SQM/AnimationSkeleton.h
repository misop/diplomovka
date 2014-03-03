#pragma once
#include "m_math.h"
#include <glm.hpp>
#include <vector>
#include <boost/serialization/version.hpp>

using namespace std;

class AnimationSkeleton
{
	friend class boost::serialization::access;
public:
	int id;
	CVector3 position;
	vector<Quaternion> axisAngles;
	vector<Quaternion> quaternions;
	AnimationSkeleton *parent;
	glm::mat4 matrix;

	std::vector<AnimationSkeleton*> nodes;

	AnimationSkeleton(void);
	AnimationSkeleton(AnimationSkeleton *father, float x, float y, float z);
	~AnimationSkeleton(void);
	
	void CalculateCorrespondingDoF(AnimationSkeleton *reference);
	void CalculateCorrespondingDoF(AnimationSkeleton *reference, glm::mat4 M);
	glm::mat4 ComputeLocalMatrix(int idx);
	void ComputeSkinningMatrices(int idx);
protected:
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_NVP(position);
		ar & BOOST_SERIALIZATION_NVP(axisAngles);
		ar & BOOST_SERIALIZATION_NVP(quaternions);
		ar & BOOST_SERIALIZATION_NVP(parent);
		ar & BOOST_SERIALIZATION_NVP(nodes);
	}
};

CVector3 TransformCPoint(CVector3 u, glm::mat4 M);
glm::mat4 AddRotation(glm::mat4 M, CVector4 axisAngle, CVector3 center);
int RecalculateIDs(AnimationSkeleton* node);

BOOST_CLASS_VERSION(AnimationSkeleton, 0)