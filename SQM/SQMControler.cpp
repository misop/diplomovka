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
#include <gtc\type_ptr.hpp>
#include "FloatArithmetic.h"

#define TESSELATION_LEVEL 15.0

using namespace std;

SQMControler::SQMControler(void)
{
	sqmALgorithm = new SQMAlgorithm();
	selected = NULL;
	buffer1 = NULL;
	buffer2 = NULL;
	icosahedron = NULL;
	selectedIndex = -1;
}


SQMControler::~SQMControler(void)
{
	delete sqmALgorithm;
	if (icosahedron) delete icosahedron;
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

void SQMControler::draw(ShaderUniforms *uniforms) {
	/*sqmALgorithm->draw();
	if (selected != NULL) {
	selected->draw(CVector3(0, 1, 0), CVector3(1, 1, 0));
	}*/
	glUniformMatrix4fv(uniforms->ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	glUniform1i(uniforms->SelectedNodeLoc, 0);
	glUniform1f(uniforms->TessLevelInner, 1.0);
	glUniform1f(uniforms->TessLevelOuter, 1.0);
    glPatchParameteri(GL_PATCH_VERTICES, 2);
	if (buffer2) buffer1->DrawElement(0, GL_LINES);
	
	glUniform1f(uniforms->TessLevelInner, TESSELATION_LEVEL);
	glUniform1f(uniforms->TessLevelOuter, TESSELATION_LEVEL);
	
    glPatchParameteri(GL_PATCH_VERTICES, 3);
	for (int i = 0; i < modelMatrices.size(); i++) {
		glUniformMatrix4fv(uniforms->ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		glUniform1i(uniforms->SelectedNodeLoc, (selectedIndex == i) ? 1 : 0);
		icosahedron->DrawElement(0, GL_PATCHES);
	}
	//if (buffer1) buffer1->Draw(GL_POINTS);
}

void SQMControler::drawRefresh() {
	if (buffer1) delete buffer1;
	if (buffer2) delete buffer2;

	buffer1 = new GLArrayBuffer();
	buffer2 = new GLArrayBuffer();

	vector<float> nodePoints;
	vector<int> lineIndices;
	vector<float> lineColors;
	drawSkeleton(nodePoints, lineIndices, modelMatrices);
	//drawSkeletonNodes(nodePoints, nodeColors);
	//drawSkeletonLines(lineIndices);

	for (int i = 0; i < nodePoints.size(); i++) {
		lineColors.push_back(0.0);
		lineColors.push_back(1.0);
		lineColors.push_back(0.0);
	}

	buffer1->Bind();
	buffer1->BindBufferData(nodePoints, 3, GL_STATIC_DRAW);
	buffer1->BindBufferData(lineColors, 3, GL_STATIC_DRAW);
	buffer1->BindElement(lineIndices, GL_STATIC_DRAW);
}

void SQMControler::drawSkeleton(vector<float> &points, vector<int> &indices, vector<glm::mat4> &modelMatrices) {
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	//*3 for number of elements
	points.reserve(3*sqmALgorithm->getNumberOfNodes());
	indices.reserve(3*sqmALgorithm->getNumberOfNodes()*2);
	modelMatrices.reserve(3*sqmALgorithm->getNumberOfNodes());

	points.clear();
	indices.clear();
	modelMatrices.clear();
	selectedIndex = -1;

	int index = 0;
	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		OpenMesh::Vec3f position = node->getPosition();
		float radius = node->getNodeRadius();
		//create and store model matrix
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(position[0], position[1], position[2]));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));
		modelMatrices.push_back(modelMatrix);
		//store node position
		points.push_back(position[0]);
		points.push_back(position[1]);
		points.push_back(position[2]);
		//store node color
		if (node == selected) selectedIndex = index;
		//add childs to queue and form indices for lines
		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			SQMNode *child = (*childs)[i];
			indices.push_back(index);
			//index of current + all nodes before + self
			indices.push_back(index + queue.size() + 1);
			queue.push_back(child);
		}

		index++;
	}
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

void SQMControler::drawSkeletonLines(std::vector<int> &indices) {
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	//*3 for number of elements; *2 because each nodes has one incoming and one outgoing line
	indices.reserve(3*sqmALgorithm->getNumberOfNodes()*2);

	indices.clear();

	int index = 0;
	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			SQMNode *child = (*childs)[i];
			indices.push_back(index);
			//index of current + all nodes before + self
			indices.push_back(index + queue.size() + 1);
			queue.push_back(child);
		}
		index++;
	}
}

void SQMControler::createIcosahedron() {
	const int faces[] = {
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,

		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,

		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,

		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10 };

	const float vertices[] = {
		0.000f,  0.000f,  1.000f,
		0.894f,  0.000f,  0.447f,
		0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		0.276f, -0.851f,  0.447f,
		0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		0.724f, -0.526f, -0.447f,
		0.000f,  0.000f, -1.000f };

	const float vetriceColors[] = {
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f };

	vector<float> points;
	points.assign(vertices, vertices + 36);
	vector<float> colors;
	colors.assign(vetriceColors, vetriceColors + 36);
	vector<int> indices;
	indices.assign(faces, faces + 60);

	if (icosahedron) delete icosahedron;
	icosahedron = new GLArrayBuffer();

	icosahedron->Bind();
	icosahedron->BindBufferData(points, 3, GL_STATIC_DRAW);
	icosahedron->BindBufferData(colors, 3, GL_STATIC_DRAW);
	icosahedron->BindElement(indices, GL_STATIC_DRAW);
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
