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

SQMNode* SQMControler::getSelected() {
	if (sqmALgorithm->getState() == SQMStart) {
		return selected;
	}
	return NULL;
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

void SQMControler::draw(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera) {
	SQMState state = sqmALgorithm->getState();
	if (state == SQMStart || state == SQMStraighten) {
		drawSkeleton(uniforms, programs, camera);
	}
	if (state == SQMComputeConvexHull || state == SQMSubdivideConvexHull) {
		drawBNPs(uniforms, programs, camera);
	}
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		drawMesh(uniforms, programs, camera);
	}
}

void SQMControler::drawSkeleton(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera) {
	programs->SklLines->Use();
	camera->lookFromCamera(uniforms->MVPmatrixSklLines);
	if (buffer1) buffer1->DrawElement(0, GL_LINES);

	programs->SklNodes->Use();
	camera->lookFromCamera(uniforms->MVPmatrixSklNodes);
	glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(uniforms->TessLevelInner, TESSELATION_LEVEL);
	glUniform1f(uniforms->TessLevelOuter, TESSELATION_LEVEL);
	glUniform1i(uniforms->CameraLoc, 1);
	icosahedron->DrawElement(0, GL_PATCHES);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glUniform1i(uniforms->CameraLoc, 0);
	for (int i = 0; i < modelMatrices.size(); i++) {
		glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		glUniform1i(uniforms->SelectedNodeLoc, (selectedIndex == i) ? 1 : 0);
		icosahedron->DrawElement(0, GL_PATCHES);
	}
}

void SQMControler::drawBNPs(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera) {
	//show camera
	programs->SklNodes->Use();
	camera->lookFromCamera(uniforms->MVPmatrixSklNodes);
	glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(uniforms->TessLevelInner, TESSELATION_LEVEL);
	glUniform1f(uniforms->TessLevelOuter, TESSELATION_LEVEL);
	glUniform1i(uniforms->CameraLoc, 1);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw BNPs
	programs->BNPs->Use();
	camera->lookFromCamera(uniforms->MVPmatrixBNPs);
	buffer1->DrawElement(0, GL_TRIANGLES);
}

void SQMControler::drawBNPs() {
	if (buffer1) delete buffer1;

	deque<SQMNode *> queue;
	queue.push_back(sqmALgorithm->getRoot());

	vector<float> points;
	vector<int> indices;

	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		if (node->isBranchNode()) {
			convertTriMeshToArray(node->getPolyhedron(), points, indices);
		}

		vector<SQMNode *> *childs = node->getDescendants();
		for (int i = 0; i < childs->size(); i++) {
			queue.push_back((*childs)[i]);
		}
	}

	buffer1 = new GLArrayBuffer();
	buffer1->Bind();
	buffer1->BindBufferData(points, 3, GL_STATIC_DRAW);
	buffer1->BindElement(indices, GL_STATIC_DRAW);
}

void SQMControler::drawMesh(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera) {
	//show camera
	programs->SklNodes->Use();
	camera->lookFromCamera(uniforms->MVPmatrixSklNodes);
	glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(uniforms->TessLevelInner, TESSELATION_LEVEL);
	glUniform1f(uniforms->TessLevelOuter, TESSELATION_LEVEL);
	glUniform1i(uniforms->CameraLoc, 1);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw BNPs
	programs->BNPs->Use();
	camera->lookFromCamera(uniforms->MVPmatrixBNPs);
	buffer1->DrawElement(0, GL_TRIANGLES);
}

void SQMControler::drawMeshForTesselation(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera) {
	//show camera
	programs->SklNodes->Use();
	camera->lookFromCamera(uniforms->MVPmatrixSklNodes);
	glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(uniforms->TessLevelInner, TESSELATION_LEVEL);
	glUniform1f(uniforms->TessLevelOuter, TESSELATION_LEVEL);
	glUniform1i(uniforms->CameraLoc, 1);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw BNPs
	//TODO:: tesselation shader for triangle mesh parts
	camera->lookFromCamera(uniforms->MVPmatrixBNPs);
	buffer1->DrawElement(0, GL_PATCHES);
	//TODO:: teselation shaders for quad mesh parts
	camera->lookFromCamera(uniforms->MVPmatrixBNPs);
	buffer1->DrawElement(1, GL_PATCHES);
}

void SQMControler::drawMesh() {
	if (buffer1) delete buffer1;
	vector<float> points;
	vector<int> indices;

	convertMeshToArray(sqmALgorithm->getMesh(), points, indices);

	buffer1 = new GLArrayBuffer();
	buffer1->Bind();
	buffer1->BindBufferData(points, 3, GL_STATIC_DRAW);
	buffer1->BindElement(indices, GL_STATIC_DRAW);
}

void SQMControler::drawRefresh() {
	//return if we are not drawing skeleton
	if (!(sqmALgorithm->getState() == SQMStart || sqmALgorithm->getState() == SQMStraighten)) return;
	if (buffer1) delete buffer1;
	if (buffer2) delete buffer2;

	buffer1 = new GLArrayBuffer();
	buffer2 = new GLArrayBuffer();

	vector<float> linePoints;
	vector<int> lineIndices;
	drawSkeleton(linePoints, lineIndices);

	buffer1->Bind();
	buffer1->BindBufferData(linePoints, 3, GL_STATIC_DRAW);
	buffer1->BindElement(lineIndices, GL_STATIC_DRAW);
}

void SQMControler::drawMeshForTesselation() {
	if (buffer1) delete buffer1;
	vector<float> points;
	vector<int> triIndices;
	vector<int> quadIndices;

	convertMeshToArray(sqmALgorithm->getMesh(), points, triIndices, quadIndices);

	buffer1 = new GLArrayBuffer();
	buffer1->Bind();
	buffer1->BindBufferData(points, 3, GL_STATIC_DRAW);
	buffer1->BindElement(triIndices, GL_STATIC_DRAW);
	buffer1->BindElement(quadIndices, GL_STATIC_DRAW);
}

void SQMControler::drawSkeleton(vector<float> &points, vector<int> &indices) {
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
		glm::vec3 pos(position[0], position[1], position[2]);
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
			OpenMesh::Vec3f childPosition = child->getPosition();
			glm::vec3 childPos(childPosition[0], childPosition[1], childPosition[2]);
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
	if (state == SQMComputeConvexHull || state == SQMSubdivideConvexHull) drawBNPs();
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) drawMesh();
}

#pragma endregion
