#pragma once
#include <string>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "SQMAlgorithm.h"
#include "GLArrayBuffer.h"
#include "GLCamera.h"

using namespace std;

class SQMControler
{
private:
	SQMAlgorithm *sqmALgorithm;
	GLArrayBuffer *buffer1;
	GLArrayBuffer *icosahedron;
	vector<glm::mat4> modelMatrices;
	int selectedIndex;
	bool wireframe;
public:
	SQMNode *selected;

	SQMControler(void);
	~SQMControler(void);

	void newFile();
	void loadSkeletonFromFile(string fileName);
	void saveSkeletonToFile(string fileName);
	void exportSkeletonToFile(string fileName);

	bool selectNodeInRay(glm::vec3 position, glm::vec3 direction);

	void restart();
	void straightenSkeleton();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);

	void setWireframe(bool newWireframe);
	bool getWireframe();

	void insertNode(float x, float y, float z);
	void setSelectedPosition(OpenMesh::Vec3f pos);
	void setSelectedPosition(glm::vec3 pos);
	void setSelectedX(float x);
	void setSelectedY(float y);
	void setSelectedZ(float z);
	void setSelectedRadius(float radius);
	void setSelectedTessLevel(float tessLevel);
	SQMNode* getSelected();
	
	void draw(OpenGLPrograms *programs, GLCamera *camera);
	void drawSkeleton(OpenGLPrograms *programs, GLCamera *camera);
	void drawSkeleton();
	void drawSkeleton(std::vector<float> &points, std::vector<int> &indices);
	void drawBNPs(OpenGLPrograms *programs, GLCamera *camera);
	void drawBNPs();
	void drawMesh(OpenGLPrograms *programs, GLCamera *camera);
	void drawMesh();
	void drawMeshForTesselation(OpenGLPrograms *programs, GLCamera *camera);
	void drawMeshForTesselation();
	void getBoundingSphere(float &x, float &y, float &z, float &d);
	void createIcosahedron();
};

