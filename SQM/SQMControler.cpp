#include "stdafx.h"
#include "SQMControler.h"
#pragma warning(push, 0)
#include <boost\archive\archive_exception.hpp>
#include <boost\archive\basic_xml_iarchive.hpp>
#include <boost\archive\xml_iarchive.hpp>
#include <boost\archive\xml_oarchive.hpp>
#pragma pop
#include <iostream>
#include <fstream>
#include "FloatArithmetic.h"

using namespace std;

SQMControler::SQMControler(void)
{
	sqmALgorithm = new SQMAlgorithm();
	selected = NULL;
}


SQMControler::~SQMControler(void)
{
	delete sqmALgorithm;
}

#pragma region Saving and Loading

void SQMControler::loadSkeletonFromFile(string fileName) {
	ofstream errorLog("log.txt");
	ifstream inputFile(fileName);
	assert(inputFile.good());
	boost::archive::xml_iarchive inputArchive(inputFile);
	SkeletonNode *node = NULL;
	try {
		inputArchive >> BOOST_SERIALIZATION_NVP(node);	
	} catch (boost::archive::archive_exception e) {
		errorLog << "Exception: " << e.what() << endl;
		throw e;
	}
	selected = NULL;
	SQMNode *sqmNode = new SQMNode(*node, NULL);
	sqmALgorithm->setRoot(sqmNode);
	delete node;
}

void SQMControler::saveSkeletonToFile(string fileName) {
	ofstream errorLog("log.txt");
	ofstream of(fileName);
	assert(of.good());
	boost::archive::xml_oarchive oa(of);
	SkeletonNode *node = new SkeletonNode(0, 0, 0);
	SkeletonNode *node2 = new SkeletonNode(0, 150, 0);
	SkeletonNode *node3 = new SkeletonNode(-50, -50, 0);
	SkeletonNode *node4 = new SkeletonNode(50, -50, 0);
	SkeletonNode *node5 = new SkeletonNode(-25, -100, 0);
	SkeletonNode *node6 = new SkeletonNode(25, -100, 0);
	SkeletonNode *node7 = new SkeletonNode(0, 200, 0);
	SkeletonNode *node8 = new SkeletonNode(-25, 125, 0);
	SkeletonNode *node9 = new SkeletonNode(25, 125, 0);
	SkeletonNode *node10 = new SkeletonNode(-50, 175, 0);
	SkeletonNode *node11 = new SkeletonNode(50, 175, 0);
	node->addChild(node2);
	node->addChild(node3);
	node->addChild(node4);
	node3->addChild(node5);
	node4->addChild(node6);
	node2->addChild(node7);
	node2->addChild(node8);
	node2->addChild(node9);
	node8->addChild(node10);
	node9->addChild(node11);
	//TODO convert SQM tree to skeleton nodes
	try {
		oa << BOOST_SERIALIZATION_NVP(node);	
	} catch (boost::archive::archive_exception e) {
		errorLog << "Exception: " << e.what() << endl;
		throw e;
	}
}

#pragma endregion

#pragma region Node Selection

bool SQMControler::selectNodeInRay(OpenMesh::Vec3f position, OpenMesh::Vec3f direction) {
	vector<SQMNode*> stack;
	stack.push_back(sqmALgorithm->getRoot());
	float minDistance = 0;
	SQMNode *tempClosest = NULL;

	while (!stack.empty()) {
		SQMNode *node = stack.back();
		stack.pop_back();
		float distance = 0;
		bool ok = closestNodeRayIntersection(node, position, direction, distance);
		if (ok && (distance < minDistance || tempClosest == NULL)) {
			minDistance = distance;
			tempClosest = node;
		}
		for (vector<SQMNode*>::iterator it = node->getDescendants()->begin(); it != node->getDescendants()->end(); it++) {
			stack.push_back(*it);
		}
	}

	selected = tempClosest;
	return selected != NULL;
}

bool SQMControler::closestNodeRayIntersection(SQMNode *node, OpenMesh::Vec3f rayPosition, OpenMesh::Vec3f direction, float& dist) {	
	float r = node->getNodeRadius();
	// (origin_2 - origin_1)
	OpenMesh::Vec3f oo = rayPosition - node->getPosition();
	// A = v^2
	float A = dot(direction, direction);
	// -B = v^T * (o_2 - o_1)
	float B = -2.0 * dot(direction, oo);
	// C = (o_2 - o_1)^2 - r^2
	float C = dot(oo, oo) - r*r;
	// Discriminant
	float D = B * B - 4.0f * A * C;

	// No collision
	if (D < 0) return false; 

	float sD = sqrtf(D);
	float t1 = 0.5 * (B + sD) / A; //if (t1 < Ray.Bias) t1 = Double.MaxValue;
	float t2 = 0.5 * (B - sD) / A; //if (t2 < Ray.Bias) t2 = Double.MaxValue;
	float t = (t1 < t2) ? t1 : t2;

	//if (t < Ray.Bias || t >= ray.HitParam) return; // collisions beyond current ray param are ignored

	if (t > 0.0) {
		dist = t;
		return true;
	}
	return false;
}

#pragma endregion

void SQMControler::straightenSkeleton() {
	sqmALgorithm->straightenSkeleton();
}

void SQMControler::computeConvexHull() {
	sqmALgorithm->computeConvexHull();
}

#pragma region drawing

void SQMControler::draw() {
	sqmALgorithm->draw();
	if (selected != NULL) {
		selected->draw(CVector3(0, 1, 0), CVector3(1, 1, 0));
	}
}

void SQMControler::getBoundingSphere(float &x, float &y, float &z, float &d) {
	sqmALgorithm->getBoundingSphere(x, y, z, d);
}

void SQMControler::subdivideConvexHull() {
	sqmALgorithm->subdivideConvexHull();
}

void SQMControler::joinBNPs() {
	sqmALgorithm->joinBNPs();
}

void SQMControler::executeSQMAlgorithm() {
	sqmALgorithm->executeSQMAlgorithm();
}

void SQMControler::executeSQMAlgorithm(SQMState state) {
	sqmALgorithm->executeSQMAlgorithm(state);
}

#pragma endregion
