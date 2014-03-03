#include "stdafx.h"
#include "AnimationSkeleton.h"
#include <gtc\type_ptr.hpp>
#include <gtc\matrix_transform.hpp>
#include "FloatArithmetic.h"
#include <deque>

AnimationSkeleton::AnimationSkeleton(void)
{
}

AnimationSkeleton::AnimationSkeleton(AnimationSkeleton *father, float x, float y, float z) : parent(father), position(CVector3(x, y, z)) {
}


AnimationSkeleton::~AnimationSkeleton(void)
{
}

void AnimationSkeleton::CalculateCorrespondingDoF(AnimationSkeleton *reference) {
	axisAngles.push_back(CVector4(0, 0, 0, 1));
	quaternions.push_back(Quaternion());
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->CalculateCorrespondingDoF(reference->nodes[i], glm::mat4(1.0));
	}
}

void AnimationSkeleton::CalculateCorrespondingDoF(AnimationSkeleton *reference, glm::mat4 M) {
	//have to be the same skeleton just posed differently
	//this is in bind position
	CVector4 axisAngle = CVector4(0, 0, 0, 1);

	//rotation from bind to this skeleton
	CVector3 bPoint = TransformCPoint(position, M);
	CVector3 bpPoint = TransformCPoint(parent->position, M);
	CVector3 u = Normalize(bPoint - bpPoint);
	CVector3 v = Normalize(reference->position - reference->parent->position);

	Quaternion q = SQMQuaternionBetweenVectors(u, v);
	axisAngle = QuaternionToAxisAngle(q);
	axisAngle.s = axisAngle.s*180.0f/M_PI;

	M = AddRotation(M, axisAngle, reference->parent->position);

	axisAngles.push_back(axisAngle);
	quaternions.push_back(q);

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->CalculateCorrespondingDoF(reference->nodes[i], M);
	}
}

glm::mat4 AnimationSkeleton::ComputeLocalMatrix(int idx) {
	if (idx == -1) return glm::mat4(1.0);
	idx = min(idx, (int)axisAngles.size() - 1);
	//should have parent
	CVector3 parentPos = parent->position;
	//translate to parent, rotate, translate back
	float angle = axisAngles[idx].s;//*180.0f/M_PI;
	glm::vec3 pos = glm::vec3(parentPos.x, parentPos.y, parentPos.z);
	glm::vec3 axis = glm::vec3(axisAngles[idx].i, axisAngles[idx].j, axisAngles[idx].k);
	//translate parent to origin
	glm::mat4 translate = glm::translate(glm::mat4(1), -pos);
	glm::mat4 rotate;
	glm::mat4 translateBack = glm::translate(glm::mat4(1), pos);
	//rotate by axis angle
	if (glm::length(axis) != 0) {
		rotate =  glm::rotate(glm::mat4(1), angle, axis);
	}
	//translate back to parent
	glm::mat4 result = translateBack * rotate * translate;

	return result;
}

void AnimationSkeleton::ComputeSkinningMatrices(int idx) {
	if (parent == NULL) {
		matrix = glm::mat4();
	} else {
		glm::mat4 local = ComputeLocalMatrix(idx);
		matrix = local * parent->matrix;
	}

	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->ComputeSkinningMatrices(idx);
	}
}

CVector3 TransformCPoint(CVector3 u, glm::mat4 M) {
	glm::vec4 v(u.x, u.y, u.z, 1.0);
	glm::vec4 r = M * v;
	return CVector3(r.x, r.y, r.z);
}

glm::mat4 AddRotation(glm::mat4 M, CVector4 axisAngle, CVector3 center) {
	if (IsZeroLength(CVector3(axisAngle.i, axisAngle.j, axisAngle.k))) return M;

	glm::vec3 c(center.x, center.y, center.z);
	glm::vec3 axis(axisAngle.i, axisAngle.j, axisAngle.k);
	float angle = axisAngle.s;
	//float angle = axisAngle.s*180.0f/M_PI;

	glm::mat4 T = glm::translate(glm::mat4(), -c);
	glm::mat4 R = glm::rotate(glm::mat4(), angle, axis);
	glm::mat4 TB = glm::translate(glm::mat4(), c);

	return TB * R * T * M;
}

int RecalculateIDs(AnimationSkeleton* node) {
	deque<AnimationSkeleton*> queue;
	queue.push_back(node);

	int id = 0;
	while (!queue.empty()) {
		AnimationSkeleton *skin = queue.front();
		queue.pop_front();

		skin->id = id;
		id++;

		for (int i = 0; i < skin->nodes.size(); i++) {
			queue.push_back(skin->nodes[i]);
		}
	}
	return id;
}