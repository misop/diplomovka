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
	GLArrayBuffer *buffer2;
	GLArrayBuffer *icosahedron;
	vector<glm::mat4> modelMatrices;
	int selectedIndex;
public:
	SQMNode *selected;

	SQMControler(void);
	~SQMControler(void);

	void newFile();
	void loadSkeletonFromFile(string fileName);
	void saveSkeletonToFile(string fileName);
	void exportSkeletonToFile(string fileName);

	bool selectNodeInRay(OpenMesh::Vec3f position, OpenMesh::Vec3f direction);
	bool closestNodeRayIntersection(SQMNode *node, OpenMesh::Vec3f rayPosition, OpenMesh::Vec3f direction, float& dist);

	void straightenSkeleton();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);

	void insertNode(float x, float y, float z);
	void setSelectedPosition(OpenMesh::Vec3f pos);
	void setSelectedX(float x);
	void setSelectedY(float y);
	void setSelectedZ(float z);
	void setSelectedRadius(float radius);
	
	void draw(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera);
	void drawSkeleton(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera);
	void drawSkeleton(std::vector<float> &points, std::vector<int> &indices);
	void drawBNPs(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera);
	void drawBNPs();
	void drawMesh(ShaderUniforms *uniforms, OpenGLPrograms *programs, GLCamera *camera);
	void drawMesh();
	void drawRefresh();
	void getBoundingSphere(float &x, float &y, float &z, float &d);
	void createIcosahedron();
};

