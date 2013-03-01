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

void SQMControler::selectNodeInRay(OpenMesh::Vec3f position, OpenMesh::Vec3f direction) {
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
}

bool closestNodeRayIntersection(SQMNode *node, OpenMesh::Vec3f rayPosition, OpenMesh::Vec3f direction, float& dist) {
	//initialize
	OpenMesh::Vec3f u = direction;
	float nodeRadius = node->getNodeRadius();
	OpenMesh::Vec3f p = rayPosition;
	OpenMesh::Vec3f position = node->getPosition();
	//calculate intersections -> always 2
	OpenMesh::Vec3f CA = p - position;
	//float a = 1.0;//dot(u, u); u is unit vector simplified
	//float b = 2*dot(u, p - CA);//old
	float b = 2*dot(u, p - position);
	float c = dot(CA, CA) - nodeRadius*nodeRadius;
	float delta = b*b - c;//b*b - 4.0*a*c; simplified
	if (delta < FLOAT_ZERO) {
		return false;
	}
	//determinate correct intersection
	float d1 = (-b - sqrt(delta))/2.0;//(-b - sqrt(delta)) / (2.0*a); simplified
	float d2 = (-b + sqrt(delta))/2.0;//(-b + sqrt(delta)) / (2.0*a); simplified
	//parameter has to be bigger than 0 thus moving in the direction of leading vector
	if (d1 > 0 && d2 < 0) {
		dist = d1;
	} else if (d1 < 0 && d2 > 0) {
		dist = d2;
	} else if (d1 > 0 && d2 > 0) {
		dist = d1 < d2 ? d1 : d2;
	}

	return true;
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
