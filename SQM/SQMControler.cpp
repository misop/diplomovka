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
	buffer1 = NULL;
	buffer2 = NULL;
}


SQMControler::~SQMControler(void)
{
	delete sqmALgorithm;
	if (buffer1) delete buffer1;
	if (buffer2) delete buffer2;
}

#pragma region Saving and Loading

void SQMControler::newFile() {
	selected = NULL;
	if (sqmALgorithm != NULL) {
		delete sqmALgorithm;
	}
	sqmALgorithm = new SQMAlgorithm();
	drawRefresh();
}

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
	if (sqmALgorithm != NULL) {
		delete sqmALgorithm;
	}
	sqmALgorithm = new SQMAlgorithm();
	sqmALgorithm->setRoot(sqmNode);
	delete node;
	drawRefresh();
}

void SQMControler::saveSkeletonToFile(string fileName) {
	ofstream errorLog("log.txt");
	ofstream of(fileName);
	assert(of.good());
	boost::archive::xml_oarchive oa(of);
	SkeletonNode *node = sqmALgorithm->getRoot()->exportToSkeletonNode();
	/*SkeletonNode *node = new SkeletonNode(0, 0, 0);
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
	node9->addChild(node11);*/
	//TODO convert SQM tree to skeleton nodes
	try {
		oa << BOOST_SERIALIZATION_NVP(node);	
	} catch (boost::archive::archive_exception e) {
		errorLog << "Exception: " << e.what() << endl;
		throw e;
	}
	delete node;
}

void SQMControler::exportSkeletonToFile(string fileName) {
	SQMState state = sqmALgorithm->getState();
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		writeMesh(sqmALgorithm->getMesh(), fileName);
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
		if (node == NULL) continue;
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

	if (selected != tempClosest) {
		selected = tempClosest;
		drawRefresh();
	}

	return selected != NULL;
}

bool SQMControler::closestNodeRayIntersection(SQMNode *node, OpenMesh::Vec3f rayPosition, OpenMesh::Vec3f direction, float& dist) {	
	if (node == NULL) return false;
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

void SQMControler::insertNode(float x, float y, float z) {
	selected->addDescendant(x, y, z);
	sqmALgorithm->setNumberOfNodes(sqmALgorithm->getNumberOfNodes() + 1);
	drawRefresh();
}

void SQMControler::setSelectedPosition(OpenMesh::Vec3f pos) {
	selected->setPosition(pos);
	drawRefresh();
}
void SQMControler::setSelectedX(float x) {
	selected->setX(x);
	drawRefresh();
}

void SQMControler::setSelectedY(float y) {
	selected->setY(y);
	drawRefresh();
}

void SQMControler::setSelectedZ(float z) {
	selected->setZ(z);
	drawRefresh();
}

void SQMControler::setSelectedRadius(float radius) {
	selected->setNodeRadius(radius);
	drawRefresh();
}

#pragma region drawing

void SQMControler::draw() {
	/*sqmALgorithm->draw();
	if (selected != NULL) {
	selected->draw(CVector3(0, 1, 0), CVector3(1, 1, 0));
	}*/
	if (buffer2) buffer2->Draw(GL_LINES);
	if (buffer1) buffer1->Draw(GL_POINTS);
}

void SQMControler::drawRefresh() {
	if (buffer1) delete buffer1;
	if (buffer2) delete buffer2;

	buffer1 = new GLArrayBuffer();
	buffer2 = new GLArrayBuffer();

	vector<float> nodesPoints;
	vector<float> nodesColors;
	vector<float> linePoints;
	vector<float> lineColors;
	drawSkeletonNodes(nodesPoints, nodesColors);
	drawSkeletonLines(linePoints, lineColors);

	buffer1->Bind();
	buffer1->BindBufferData(nodesPoints, 3, GL_STATIC_DRAW);
	buffer1->BindBufferData(nodesColors, 3, GL_STATIC_DRAW);

	buffer2->Bind();
	buffer2->BindBufferData(linePoints, 3, GL_STATIC_DRAW);
	buffer2->BindBufferData(lineColors, 3, GL_STATIC_DRAW);
}

void SQMControler::drawSkeletonNodes(vector<float> &points, vector<float> &colors) {
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	//*3 for number of elements
	points.reserve(3*sqmALgorithm->getNumberOfNodes());
	colors.reserve(3*sqmALgorithm->getNumberOfNodes());

	points.clear();
	colors.clear();

	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		OpenMesh::Vec3f position = node->getPosition();
		points.push_back(position[0]);
		points.push_back(position[1]);
		points.push_back(position[2]);

		colors.push_back(1.0);
		if (node == selected) {
			colors.push_back(1.0);
		} else {
			colors.push_back(0.0);
		}
		colors.push_back(0.0);

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			queue.push_back((*childs)[i]);
		}
	}
}

void SQMControler::drawSkeletonLines(std::vector<float> &points, std::vector<float> &colors) {
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	//*3 for number of elements; *2 because each nodes has one incoming and one outgoing line
	points.reserve(3*sqmALgorithm->getNumberOfNodes()*2);
	colors.reserve(3*sqmALgorithm->getNumberOfNodes()*2);

	points.clear();
	colors.clear();

	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		OpenMesh::Vec3f position = node->getPosition();

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			SQMNode *child = (*childs)[i];
			OpenMesh::Vec3f childPosition = child->getPosition();
			queue.push_back(child);
			//line from node
			points.push_back(position[0]);
			points.push_back(position[1]);
			points.push_back(position[2]);
			//to child
			points.push_back(childPosition[0]);
			points.push_back(childPosition[1]);
			points.push_back(childPosition[2]);
			//set color for each vertex
			colors.push_back(0.0);
			colors.push_back(1.0);
			colors.push_back(0.0);
			colors.push_back(0.0);
			colors.push_back(1.0);
			colors.push_back(0.0);
		}
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
	drawRefresh();
}

#pragma endregion
