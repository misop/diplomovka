#pragma once
#include "m_math.h"
#include <vector>
#include <boost/serialization/version.hpp>

using namespace std;

class SkeletonNode
{
	friend class boost::serialization::access;
public:
	CVector3 point;
	CVector3 scale;
	CVector3 rotate;
	float radius;
	float tessLevel;
	int id;
	bool cyclic;
	bool capsule;
	vector<SkeletonNode*> nodes;
public:
	SkeletonNode();
	SkeletonNode(float x, float y, float z);
	SkeletonNode(float x, float y, float z, float Radius);
	~SkeletonNode(void);

	void addChild(SkeletonNode *node);
protected:
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_NVP(id);
		ar & BOOST_SERIALIZATION_NVP(cyclic);
		ar & BOOST_SERIALIZATION_NVP(point);
		if (version > 0) {
			ar & BOOST_SERIALIZATION_NVP(radius);
		}
		if (version > 1) {
			ar & BOOST_SERIALIZATION_NVP(capsule);
			ar & BOOST_SERIALIZATION_NVP(tessLevel);
			ar & BOOST_SERIALIZATION_NVP(scale);
			ar & BOOST_SERIALIZATION_NVP(rotate);
		}
		ar & BOOST_SERIALIZATION_NVP(nodes);
	}
};

BOOST_CLASS_VERSION(SkeletonNode, 2)