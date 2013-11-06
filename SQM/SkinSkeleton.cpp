#include "stdafx.h"
#include "SkinSkeleton.h"
#include <gtc\type_ptr.hpp>

SkinSkeleton::SkinSkeleton() : parent(NULL), position(CVector3()), axisAngle(Quaternion()) {
}

SkinSkeleton::SkinSkeleton(SkinSkeleton *father, float x, float y, float z) : parent(father), position(CVector3(x, y, z)), axisAngle(Quaternion()) {
}

SkinSkeleton::SkinSkeleton(float x, float y, float z) : parent(NULL), position(CVector3(x, y, z)), axisAngle(Quaternion()) {
}

SkinSkeleton::SkinSkeleton(CVector3 pos) : parent(NULL), position(pos), axisAngle(Quaternion()) {
}

SkinSkeleton::SkinSkeleton(CVector3 pos, Quaternion axis_Angle) : parent(NULL), position(pos), axisAngle(axis_Angle) {
}

SkinSkeleton::~SkinSkeleton() {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}

void SkinSkeleton::CalculateCorrespondingDoF(SkinSkeleton *another) {
	//have to be the same skeleton just posed diferently
	//this is in bind position
	if (parent != NULL) {
		CVector3 u = Normalize(position - parent->position);
		CVector3 v = Normalize(another->position - another->parent->position);

		Quaternion q = SQMQuaternionBetweenVectors(u, v);
		axisAngle = QuaternionToAxisAngle(q);
		another->axisAngle = axisAngle;
		axisAngle.s = 0;
	}

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->CalculateCorrespondingDoF(another->nodes[i]);
	}
}

glm::mat4 SkinSkeleton::ComputeLocalMatrix() {
	//transform axis angle to matrix
	CVector3 t = (position - parent->position);
	float c0 = cos(axisAngle.s);
	float s0 = sin(axisAngle.s);
	CVector3 a = Normalize(CVector3(axisAngle.i, axisAngle.j, axisAngle.k));
	float local[16] = {a.x*a.x + c0*(1-a.x*a.x)  ,  a.x*a.y*(1-c0)+a.z*s0        ,  a.x*a.z*(1-c0) - a.y*s0   ,   t.x,
					   a.x*a.y*(1-c0) - a.z*s0   ,  a.y*a.y + c0*(1 - a.y*a.y)   ,  a.y*a.z*(1-c0) - a.x*s0   ,   t.y,
					   a.x*a.z*(1-c0) + a.y*s0   ,  a.y*a.z*(1-c0) - a.x*s0      ,  a.z*a.z + c0*(1-a.z*a.z)  ,   t.z,
					   0                         ,  0                            ,  0                         ,   1};
	glm::mat4 result = glm::make_mat4(local);
	return result;
}

void SkinSkeleton::ComputeSkinningMatrices() {
	if (parent == NULL) {
		matrix = glm::mat4();
	} else {
		glm::mat4 local = ComputeLocalMatrix();
		matrix = parent->matrix * local;
	}

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->ComputeSkinningMatrices();
	}
}

void SkinSkeleton::InvertMatrices() {
	matrix = glm::inverse(matrix);

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->InvertMatrices();
	}
}

void SkinSkeleton::PrecomputeFinalSkinningMatrices(SkinSkeleton *another) {
	//another is in Bind position
	matrix = matrix * another->matrix;

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->PrecomputeFinalSkinningMatrices(another->nodes[i]);
	}
}