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
#include <iomanip>
#include "FloatArithmetic.h"
#include "Uniforms.h"

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
	wireframe = true;
	shouldDrawNormals = false;
	globalTesselation = 0;
	nodeRadiuses = NULL;
	nodeCenters = NULL;
	shouldRender = true;
	threshold = 0.85;
	skinningMatrices = NULL;
	transformMatrices = NULL;
	skinningType = CPU;
	animSkl = NULL;
}


SQMControler::~SQMControler(void)
{
	delete sqmALgorithm;
	if (icosahedron) delete icosahedron;
	if (buffer1) delete buffer1;
	if (buffer2) delete buffer2;
	if (nodeRadiuses) delete nodeRadiuses;
	if (nodeCenters) delete nodeCenters;
	if (skinningMatrices) delete skinningMatrices;
	if (transformMatrices) delete transformMatrices;
	if (animSkl) delete animSkl;
}

void SQMControler::generateTextures() {
	nodeRadiuses = new GLTexture(GL_TEXTURE_2D);
	nodeRadiuses->Bind();

	nodeCenters = new GLTexture(GL_TEXTURE_2D);
	nodeCenters->Bind();
}

#pragma region Saving and Loading

void SQMControler::newFile() {
	selected = NULL;
	if (sqmALgorithm != NULL) {
		delete sqmALgorithm;
	}
	sqmALgorithm = new SQMAlgorithm();
	drawSkeleton();
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
	drawSkeleton();
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
	/* //mado vypis :D
	of << "PseudoRoot*-*0,0,0*0,0,0*0*" << endl;
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		if (node->getParent() != NULL) {
			glm::vec3 pos1 = node->getPosition_glm();
			glm::vec3 pos2 = node->getParent()->getPosition_glm();
			of << node->getIdStr() << "*" << node->getParent()->getIdStr() << "*" << pos2.x << "," << pos2.y << "," << pos2.z << "*" << pos1.x << "," << pos1.y << "," << pos1.z << "*" << node->getNodeRadius() << "*"  << endl;
		} else {
			glm::vec3 pos1 = node->getPosition_glm();
			of << node->getIdStr() << "*PseudoRoot*0,0,0*" << pos1.x << "," << pos1.y << "," << pos1.z << "*" << node->getNodeRadius() << "*" << endl;
		}

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			queue.push_back((*childs)[i]);
		}
	}*/
}

void SQMControler::exportMeshToFile(string fileName) {
	SQMState state = sqmALgorithm->getState();
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		writeMesh(sqmALgorithm->getMesh(), fileName);
	}
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());
	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		if (node->getIdStr() == "0-0-2" && node->isBranchNode())
			writeTriMesh(node->getPolyhedron());

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++)
			queue.push_back((*childs)[i]);
	}
}

#pragma endregion

#pragma region Aniamtion

void SQMControler::setBindPose() {
	if (animSkl) delete animSkl;
	animSkl = sqmALgorithm->getRoot()->exportToAnimationSkeleton(NULL);
}

void SQMControler::setReferencePose() {
	if (animSkl == NULL) return;

	AnimationSkeleton *skl = sqmALgorithm->getRoot()->exportToAnimationSkeleton(NULL);
	animSkl->CalculateCorrespondingDoF(skl);
	//skl->CalculateCorrespondingDoF(animSkl);
	delete skl;
}

void SQMControler::saveAnimation(string fileName) {
	if (animSkl == NULL) return;

	ofstream errorLog("log.txt");
	ofstream of(fileName);
	assert(of.good());
	boost::archive::xml_oarchive oa(of);
	try {
		oa << BOOST_SERIALIZATION_NVP(animSkl);	
	} catch (boost::archive::archive_exception e) {
		errorLog << "Exception: " << e.what() << endl;
		throw e;
	}
}

#pragma endregion

#pragma region Node Selection

bool SQMControler::selectNodeInRay(glm::vec3 position, glm::vec3 direction) {
	SQMNode *inRay = getNodeInRay(position, direction);

	if (selected != inRay) {
		selected = inRay;
		drawSkeleton();
	}

	return selected != NULL;
}

SQMNode* SQMControler::getNodeInRay(glm::vec3 position, glm::vec3 direction) {
	vector<SQMNode*> stack;
	stack.push_back(sqmALgorithm->getRoot());
	float minDistance = 0;
	SQMNode *tempClosest = NULL;

	while (!stack.empty()) {
		SQMNode *node = stack.back();
		stack.pop_back();
		if (node == NULL) continue;
		float distance = 0;
		bool ok = raySphereIntersection(position, direction, node->getPosition_glm(), node->getNodeRadius(), distance);
		if (ok && (distance < minDistance || tempClosest == NULL)) {
			minDistance = distance;
			tempClosest = node;
		}
		for (vector<SQMNode*>::iterator it = node->getDescendants()->begin(); it != node->getDescendants()->end(); it++) {
			stack.push_back(*it);
		}
	}

	return tempClosest;
}

SQMNode* SQMControler::getSelected() {
	if (sqmALgorithm->getState() == SQMStart) {
		return selected;
	}
	return NULL;
}

SQMState SQMControler::getState() {
	return sqmALgorithm->getState();
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
	drawSkeleton();
}

void SQMControler::createCycle(SQMNode* from, SQMNode *to) {
	if (to->isAncestor(from)) {
		to->createCycle(from);
	} else {
		from->createCycle(to);
	}
	sqmALgorithm->setHasCycle(true);
	drawSkeleton();
}

void SQMControler::setWireframe(bool newWireframe) {
	wireframe = newWireframe;
}

bool SQMControler::getWireframe() {
	return wireframe;
}

void SQMControler::setShouldDrawNormals(bool newShouldDrawNormals) {
	shouldDrawNormals = newShouldDrawNormals;
}

bool SQMControler::getShouldDrawNormals() {
	return shouldDrawNormals;
}

int SQMControler::getNumOfSkinningMatrices() {
	//can not be less than 1
	return max(sqmALgorithm->getNumberOfSkinningMatrices(), 1);
}

void SQMControler::setGlobalTesselation(float newGlobalTesselation) {
	globalTesselation = newGlobalTesselation;
}

float SQMControler::getThreshold() {
	return threshold;
}

void SQMControler::setThreshold(float th) {
	threshold = th;
}

void SQMControler::setSelectedPosition(OpenMesh::Vec3f pos) {
	selected->setPosition(pos);
	//drawSkeleton();
}

void SQMControler::setSelectedAndDescendantsPosition(OpenMesh::Vec3f pos) {
	selected->setPositionAndAdjustDescendants(pos);
	//drawSkeleton();
}

void SQMControler::setSelectedPosition(glm::vec3 pos) {
	setSelectedPosition(OpenMesh::Vec3f(pos.x, pos.y, pos.z));
}

void SQMControler::setSelectedAndDescendantsPosition(glm::vec3 pos) {
	setSelectedAndDescendantsPosition(OpenMesh::Vec3f(pos.x, pos.y, pos.z));
}

void SQMControler::setSelectedX(float x) {
	selected->setX(x);
	drawSkeleton();
}

void SQMControler::setSelectedY(float y) {
	selected->setY(y);
	drawSkeleton();
}

void SQMControler::setSelectedZ(float z) {
	selected->setZ(z);
	drawSkeleton();
}

void SQMControler::setSelectedRadius(float radius) {
	selected->setNodeRadius(radius);
	drawSkeleton();
}

void SQMControler::setSelectedTessLevel(float tessLevel) {
	selected->setTessLevel(tessLevel);
}

void SQMControler::setSmoothingAlgorithm(SQMSmoothingAlgorithm smoothingAlgorithm) {
	sqmALgorithm->setSmoothingAlgorithm(smoothingAlgorithm);
}

void SQMControler::setSelectedScaleX(float value) {
	selected->setScaleX(value);
	drawSkeleton();
}

void SQMControler::setSelectedScaleY(float value) {
	selected->setScaleY(value);
	drawSkeleton();
}

void SQMControler::setSelectedScaleZ(float value) {
	selected->setScaleZ(value);
	drawSkeleton();
}

void SQMControler::setSelectedRotateX(float value) {
	selected->setRotateX(value);
	drawSkeleton();
}

void SQMControler::setSelectedRotateY(float value) {
	selected->setRotateY(value);
	drawSkeleton();
}

void SQMControler::setSelectedRotateZ(float value) {
	selected->setRotateZ(value);
	drawSkeleton();
}

void SQMControler::setUseCapsules(bool useCapsules) {
	sqmALgorithm->setUseCapsules(useCapsules);
}

bool SQMControler::setSkinningType(SkinningType newSkinningType) {
	bool shouldRecalculate = (skinningType == 0 && newSkinningType > 0) || (skinningType > 0 && newSkinningType == 0);
	skinningType = newSkinningType;
	if (skinningType == CPU) {
		sqmALgorithm->setUseCPUSkinning(true);
	} else {
		sqmALgorithm->setUseCPUSkinning(false);
	}
	return shouldRecalculate;
}

#pragma region drawing

void SQMControler::draw(OpenGLPrograms *programs, GLCamera *camera) {
	SQMState state = sqmALgorithm->getState();
	if (state == SQMStart || state == SQMStraighten) {
		drawSkeleton(programs, camera);
	}
	if (state == SQMComputeConvexHull || state == SQMSubdivideConvexHull) {
		drawBNPs(programs, camera);
	}
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		//drawMesh(programs, camera);
		drawMeshForTesselation(programs, camera);
	}
}

void SQMControler::drawSkeleton(OpenGLPrograms *programs, GLCamera *camera) {
	programs->SklLines->Use();
	camera->lookFromCamera(programs->SklLines->getUniformLocation(MVP_MATRIX_STR));
	glUniform3f(programs->SklLines->getUniformLocation(DIFFUSE_COLOR_STR), 0.19, 0.47, 0.92);
	if (buffer1) buffer1->DrawElement(0, GL_LINES);

	programs->SklNodes->Use();
	camera->lookFromCamera(programs->SklNodes->getUniformLocation(MVP_MATRIX_STR));
	camera->getViewMatrix(programs->SklNodes->getUniformLocation(VIEW_MATRIX_STR));
	glUniformMatrix4fv(programs->SklNodes->getUniformLocation(MODEL_MATRIX_STR), 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_INNER_STR), TESSELATION_LEVEL);
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_OUTER_STR), TESSELATION_LEVEL);
	glUniform3fv(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 1, camera->color);
	icosahedron->DrawElement(0, GL_PATCHES);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	for (int i = 0; i < modelMatrices.size(); i++) {
		glUniformMatrix4fv(programs->SklNodes->getUniformLocation(MODEL_MATRIX_STR), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		camera->setupNormalMatrix(modelMatrices[i], programs->SklNodes->getUniformLocation(NORMAL_MATRIX_STR));
		if (selectedIndex == i) {
			glUniform3f(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 0.91, 0.67, 0.1);
		} else {
			glUniform3f(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 0.76, 0.22, 0.22);
		}
		icosahedron->DrawElement(0, GL_PATCHES);
	}
}

void SQMControler::drawBNPs(OpenGLPrograms *programs, GLCamera *camera) {
	//show camera
	programs->SklNodes->Use();
	camera->lookFromCamera(programs->SklNodes->getUniformLocation(MVP_MATRIX_STR));
	camera->getViewMatrix(programs->SklNodes->getUniformLocation(VIEW_MATRIX_STR));
	glUniformMatrix4fv(programs->SklNodes->getUniformLocation(MODEL_MATRIX_STR), 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_INNER_STR), TESSELATION_LEVEL);
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_OUTER_STR), TESSELATION_LEVEL);
	glUniform3fv(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 1, camera->color);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw BNPs
	programs->BNPs->Use();
	camera->lookFromCamera(programs->BNPs->getUniformLocation(MVP_MATRIX_STR));
	glUniform3f(programs->BNPs->getUniformLocation(DIFFUSE_COLOR_STR), 0.0, 0.75, 0.75);
	buffer1->DrawElement(0, GL_TRIANGLES);
	//draw normals
	if (shouldDrawNormals) {
		programs->SklLines->Use();
		camera->lookFromCamera(programs->SklLines->getUniformLocation(MVP_MATRIX_STR));
		glUniform3f(programs->SklLines->getUniformLocation(DIFFUSE_COLOR_STR), 1.0, 0.0, 0.0);
		buffer2->Draw(GL_LINES);
	}
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
	buffer1->BindBufferDataf(points, 3, GL_STATIC_DRAW);
	buffer1->BindElement(indices, GL_STATIC_DRAW);

	drawTriNormals();
}

void SQMControler::drawTriNormals() {
	if (buffer2) delete buffer2;

	vector<float> points;
	deque<SQMNode*> queue;
	queue.push_back(sqmALgorithm->getRoot());

	while (!queue.empty()) {
		SQMNode *node = queue.front();
		queue.pop_front();

		if (node->isBranchNode()) {
			calculateTriMeshNormals(node->getPolyhedron(), points);
		}

		vector<SQMNode*> *childs = node->getNodes();
		for (int i = 0; i < childs->size(); i++) {
			SQMNode *child = (*childs)[i];
			queue.push_back(child);
		}
	}

	buffer2 = new GLArrayBuffer();
	buffer2->Bind();
	buffer2->BindBufferDataf(points, 3, GL_STATIC_DRAW);
}

void SQMControler::drawNormals() {
	if (buffer2) delete buffer2;

	vector<float> points;
	calculateMeshNormals(sqmALgorithm->getMesh(), points);

	buffer2 = new GLArrayBuffer();
	buffer2->Bind();
	buffer2->BindBufferDataf(points, 3, GL_STATIC_DRAW);
}

void SQMControler::drawMesh(OpenGLPrograms *programs, GLCamera *camera) {
	//show camera
	programs->SklNodes->Use();
	camera->lookFromCamera(programs->SklNodes->getUniformLocation(MVP_MATRIX_STR));
	camera->getViewMatrix(programs->SklNodes->getUniformLocation(VIEW_MATRIX_STR));
	glUniformMatrix4fv(programs->SklNodes->getUniformLocation(MODEL_MATRIX_STR), 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_INNER_STR), TESSELATION_LEVEL);
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_OUTER_STR), TESSELATION_LEVEL);
	glUniform3fv(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 1, camera->color);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw BNPs
	programs->BNPs->Use();
	camera->lookFromCamera(programs->BNPs->getUniformLocation(MVP_MATRIX_STR));
	glUniform3f(programs->BNPs->getUniformLocation(DIFFUSE_COLOR_STR), 0.0, 0.75, 0.75);
	buffer1->DrawElement(0, GL_TRIANGLES);
}

void SQMControler::drawMeshForTesselation(OpenGLPrograms *programs, GLCamera *camera) {
	glActiveTexture(GL_TEXTURE0);
	nodeRadiuses->Bind();
	glActiveTexture(GL_TEXTURE1);
	nodeCenters->Bind();
	int skinningMatricesCount = sqmALgorithm->getNumberOfSkinningMatrices();

	//show camera
	programs->SklNodes->Use();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	camera->lookFromCamera(programs->SklNodes->getUniformLocation(MVP_MATRIX_STR));
	camera->getViewMatrix(programs->SklNodes->getUniformLocation(VIEW_MATRIX_STR));
	glUniformMatrix4fv(programs->SklNodes->getUniformLocation(MODEL_MATRIX_STR), 1, GL_FALSE, glm::value_ptr(camera->cameraModelMatrix()));
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_INNER_STR), TESSELATION_LEVEL);
	glUniform1f(programs->SklNodes->getUniformLocation(TESS_LEVEL_OUTER_STR), TESSELATION_LEVEL);
	glUniform3fv(programs->SklNodes->getUniformLocation(DIFFUSE_COLOR_STR), 1, camera->color);
	icosahedron->DrawElement(0, GL_PATCHES);
	//draw tri patches
	glm::vec3 eye = camera->getEye();
	programs->TriMeshTess->Use();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	camera->lookFromCamera(programs->TriMeshTess->getUniformLocation(MVP_MATRIX_STR));
	camera->setupNormalMatrix(programs->TriMeshTess->getUniformLocation(NORMAL_MATRIX_STR));
	camera->getViewMatrix(programs->TriMeshTess->getUniformLocation(VIEW_MATRIX_STR));
	glUniform3f(programs->TriMeshTess->getUniformLocation(LIGHT_POSITION_STR), eye.x, eye.y, eye.z);
	glUniform4f(programs->TriMeshTess->getUniformLocation(AMBIENT_COLOR_STR), 0.0, 0.75, 0.75, 0.1);
	glUniform3f(programs->TriMeshTess->getUniformLocation(DIFFUSE_COLOR_STR), 0.0, 0.75, 0.75);
	glUniform1i(programs->TriMeshTess->getUniformLocation(WIREFRAME_STR), wireframe ? 1 : 0);
	glUniform1i(programs->TriMeshTess->getUniformLocation(SKINNING_TYPE_STR), skinningType);
	if (skinningMatricesCount > 0) {
		glUniformMatrix4fv(programs->TriMeshTess->getUniformLocation(SKINNING_MATRICES_STR), skinningMatricesCount, GL_FALSE, skinningMatrices);
		glUniformMatrix4fv(programs->TriMeshTess->getUniformLocation(TRANSFORM_MATRICES_STR), skinningMatricesCount, GL_FALSE, transformMatrices);
	}
	buffer1->DrawElement(0, GL_PATCHES);
	//draw quad patches
	programs->QuadMeshTess->Use();
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	camera->lookFromCamera(programs->QuadMeshTess->getUniformLocation(MVP_MATRIX_STR));
	camera->setupNormalMatrix(programs->QuadMeshTess->getUniformLocation(NORMAL_MATRIX_STR));
	camera->getViewMatrix(programs->QuadMeshTess->getUniformLocation(VIEW_MATRIX_STR));
	camera->setupModelViewMatrix(programs->QuadMeshTess->getUniformLocation(MODEL_VIEW_MATRIX_STR));
	//glUniform3f(programs->QuadMeshTess->getUniformLocation(LIGHT_POSITION_STR), eye.x, eye.y, eye.z);
	glUniform4f(programs->QuadMeshTess->getUniformLocation(AMBIENT_COLOR_STR), 0.0, 0.75, 0.75, 0.1);
	glUniform3f(programs->QuadMeshTess->getUniformLocation(DIFFUSE_COLOR_STR), 0.0, 0.75, 0.75);
	glUniform1f(programs->QuadMeshTess->getUniformLocation(TESS_LEVEL_INNER_STR), globalTesselation);
	glUniform1f(programs->QuadMeshTess->getUniformLocation(THRESHOLD_STR), threshold);
	glUniform1i(programs->QuadMeshTess->getUniformLocation(WIREFRAME_STR), wireframe ? 1 : 0);
	glUniform1i(programs->QuadMeshTess->getUniformLocation(MAX_ID_STR), sqmALgorithm->getNumberOfNodes() - 1);
	glUniform1i(programs->QuadMeshTess->getUniformLocation(SKINNING_TYPE_STR), skinningType);
	if (skinningMatricesCount > 0) {
		glUniformMatrix4fv(programs->QuadMeshTess->getUniformLocation(SKINNING_MATRICES_STR), skinningMatricesCount, GL_FALSE, skinningMatrices);
		glUniformMatrix4fv(programs->QuadMeshTess->getUniformLocation(TRANSFORM_MATRICES_STR), skinningMatricesCount, GL_FALSE, transformMatrices);
	}
	//glUniform1i(programs->QuadMeshTess->getUniformLocation(RADIUS_TEXTURE_STR), 0);
	//glUniform1i(programs->QuadMeshTess->getUniformLocation("ZCTS"), 1);
	//glUniform1i(8, 1);
	buffer1->DrawElement(1, GL_PATCHES);
	//draw normals
	if (shouldDrawNormals && sqmALgorithm->getState() == SQMJoinBNPs) {
		programs->SklLines->Use();
		camera->lookFromCamera(programs->SklLines->getUniformLocation(MVP_MATRIX_STR));
		glUniform3f(programs->SklLines->getUniformLocation(DIFFUSE_COLOR_STR), 1.0, 0.0, 0.0);
		buffer2->Draw(GL_LINES);
	}
}

void SQMControler::drawMesh() {
	if (buffer1) delete buffer1;
	vector<float> points;
	vector<int> indices;

	convertMeshToArray(sqmALgorithm->getMesh(), points, indices);

	buffer1 = new GLArrayBuffer();
	buffer1->Bind();
	buffer1->BindBufferDataf(points, 3, GL_STATIC_DRAW);
	buffer1->BindElement(indices, GL_STATIC_DRAW);
}

void SQMControler::drawSkeleton() {
	//return if we are not drawing skeleton
	if (!(sqmALgorithm->getState() == SQMStart || sqmALgorithm->getState() == SQMStraighten)) return;
	if (buffer1) delete buffer1;

	buffer1 = new GLArrayBuffer();

	vector<float> linePoints;
	vector<int> lineIndices;
	drawSkeleton(linePoints, lineIndices);

	buffer1->Bind();
	buffer1->BindBufferDataf(linePoints, 3, GL_STATIC_DRAW);
	buffer1->BindElement(lineIndices, GL_STATIC_DRAW);
}

void SQMControler::drawMeshForTesselation() {
	if (buffer1) delete buffer1;
	if (skinningMatrices) delete skinningMatrices;
	if (transformMatrices) delete transformMatrices;
	vector<float> points;
	vector<float> vertex_normals;
	vector<int> triIndices;
	vector<int> quadIndices;
	vector<float> tessLevels;
	vector<float> nodePositions;
	//vector<float> data;
	vector<int> skinningData;
	vector<int> data;

	int skinningMatricesCount = sqmALgorithm->getNumberOfSkinningMatrices();
	skinningMatrices = new float[skinningMatricesCount*16];
	sqmALgorithm->getSkinningMatrices(skinningMatrices);
	transformMatrices = new float[skinningMatricesCount*16];
	sqmALgorithm->getTransformMatrices(transformMatrices);

	convertMeshToArray(sqmALgorithm->getMesh(), points, vertex_normals, triIndices, quadIndices);
	sqmALgorithm->getRoot()->getMeshTessDatai(tessLevels, nodePositions, skinningData, data);

	buffer1 = new GLArrayBuffer();
	buffer1->Bind();
	buffer1->BindBufferDataf(points, 3, GL_STATIC_DRAW);
	buffer1->BindBufferDataf(vertex_normals, 3, GL_STATIC_DRAW);
	buffer1->BindBufferDataf(tessLevels, 1, GL_STATIC_DRAW);
	buffer1->BindBufferDataf(nodePositions, 3, GL_STATIC_DRAW);
	buffer1->BindBufferDatai(skinningData, 2, GL_STATIC_DRAW);
	buffer1->BindBufferDatai(data, 2, GL_STATIC_DRAW);

	buffer1->BindElement(triIndices, GL_STATIC_DRAW);
	buffer1->BindElement(quadIndices, GL_STATIC_DRAW);

	drawNormals();
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
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(position[0], position[1], position[2]));
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(position[0], position[1], position[2]));
		modelMatrix = glm::rotate(modelMatrix, node->getRotateX(), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, node->getRotateY(), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, node->getRotateZ(), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, node->getScalev());
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
	vector<int> indices;
	indices.assign(faces, faces + 60);

	if (icosahedron) delete icosahedron;
	icosahedron = new GLArrayBuffer();

	icosahedron->Bind();
	icosahedron->BindBufferDataf(points, 3, GL_STATIC_DRAW);
	icosahedron->BindElement(indices, GL_STATIC_DRAW);
}

void SQMControler::fillRadiusTable() {
	shouldRender = false;

	//GLenum error;
	//string s;
	//error = glGetError();
	//s = string((char*)gluErrorString(error));

	int nodes = sqmALgorithm->getNumberOfNodes();
	float *table = new float[nodes*nodes];
	for (int i = 0; i < nodes*nodes; i++) {
		table[i] = 0;
	}

	sqmALgorithm->getRoot()->fillRadiusTable(table, nodes);

	nodeRadiuses->Bind();
	nodeRadiuses->FunctionTexture(nodes, nodes, table);

	/*ofstream f;
	f.open ("log_radiuses.txt");
	for (int i = 0; i < nodes; i++) {
	for (int j = 0; j < nodes; j++) {
	f << table[i*nodes + j] << " ";
	}
	f << endl;
	}
	f.close();*/

	delete [] table;
	shouldRender = true;
}

void SQMControler::fillCentersTable() {
	shouldRender = false;
	int nodes = sqmALgorithm->getNumberOfNodes();
	float *table = new float[nodes*3];
	for (int i = 0; i < nodes*3; i++) {
		table[i] = 0;
	}

	sqmALgorithm->getRoot()->fillCentersTable(table);

	nodeCenters->Bind();
	nodeCenters->FunctionTexture(3, nodes, table);

	/*ofstream f;
	f.open ("log_centers.txt");
	for (int i = 0; i < nodes * 3; i++) {
	f << std::fixed << std::setw( 5 ) << std::setprecision( 2 ) 
	<< std::setfill( '0' ) << table[i] << " ";
	if (i % 3 == 2) f << endl;
	}
	/*for (int i = 0; i < nodes; i++) {
	for (int j = 0; j < 3; j++) {
	f << std::fixed << std::setw( 5 ) << std::setprecision( 2 ) 
	<< std::setfill( '0' ) << table[i*nodes + j] << " ";
	}
	f << endl;
	}
	f.close();*/

	delete [] table;
	shouldRender = true;
}

void SQMControler::getBoundingSphere(float &x, float &y, float &z, float &d) {
	sqmALgorithm->getBoundingSphere(x, y, z, d);
}

void SQMControler::restart() {
	selected = NULL;
	if (sqmALgorithm->getState() == SQMStart) return;
	sqmALgorithm->restart();
	drawSkeleton();
}

void SQMControler::subdivideConvexHull() {
	sqmALgorithm->subdivideConvexHull();
}

void SQMControler::joinBNPs() {
	sqmALgorithm->joinBNPs();
}

void SQMControler::executeSQMAlgorithm() {
	selected = NULL;
	sqmALgorithm->executeSQMAlgorithm();
}

void SQMControler::executeSQMAlgorithm(SQMState state) {
	try {
		sqmALgorithm->executeSQMAlgorithm(state);
	} catch (exception e) {
		saveSkeletonToFile("logs/log.skl");
		throw e;
	}
	state = sqmALgorithm->getState();
	selected = NULL;

	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		fillRadiusTable();
	}
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) {
		fillCentersTable();
	}

	if (state == SQMStart || state == SQMStraighten) drawSkeleton();
	if (state == SQMComputeConvexHull || state == SQMSubdivideConvexHull) drawBNPs();
	if (state == SQMJoinBNPs || state == SQMFinalPlacement) drawMeshForTesselation();//drawMesh();
}

#pragma endregion
