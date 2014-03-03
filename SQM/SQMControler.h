#pragma once
#include <string>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "SQMAlgorithm.h"
#include "GLArrayBuffer.h"
#include "GLCamera.h"
#include "GLTexture.h"

using namespace std;

typedef enum {
	CPU = 0,
	GPUVertex,
	GPUTessLast,
	GPUTessAvarage
} SkinningType;

class SQMControler
{
private:
	SQMAlgorithm *sqmALgorithm;
	GLArrayBuffer *buffer1;
	GLArrayBuffer *buffer2;
	GLArrayBuffer *icosahedron;
	vector<glm::mat4> modelMatrices;
	int selectedIndex;
	bool wireframe;
	bool shouldDrawNormals;
	float globalTesselation;
	GLTexture *nodeRadiuses;
	GLTexture *nodeCenters;
	bool shouldRender;
	float threshold;
	float *skinningMatrices;
	float *transformMatrices;
	SkinningType skinningType;
	AnimationSkeleton *animSkl;
public:
	SQMNode *selected;

	SQMControler(void);
	~SQMControler(void);

	void generateTextures();

	void newFile();
	void loadSkeletonFromFile(string fileName);
	void saveSkeletonToFile(string fileName);
	void exportMeshToFile(string fileName);
	
	void setBindPose();
	void setReferencePose();
	void saveAnimation(string fileName);

	bool selectNodeInRay(glm::vec3 position, glm::vec3 direction);
	SQMNode* getNodeInRay(glm::vec3 position, glm::vec3 direction);
	void fillRadiusTable();
	void fillCentersTable();

	void restart();
	void straightenSkeleton();
	void computeConvexHull();
	void subdivideConvexHull();
	void joinBNPs();
	void executeSQMAlgorithm();
	void executeSQMAlgorithm(SQMState state);

	void setWireframe(bool newWireframe);
	bool getWireframe();
	void setShouldDrawNormals(bool newShouldDrawNormals);
	bool getShouldDrawNormals();
	void setGlobalTesselation(float newGlobalTesselation);
	bool setSkinningType(SkinningType newSkinningType);
	int getNumOfSkinningMatrices();
	SQMState getState();

	void insertNode(float x, float y, float z);
	void createCycle(SQMNode* from, SQMNode *to);
	void setSelectedPosition(glm::vec3 pos);
	void setSelectedPosition(OpenMesh::Vec3f pos);
	void setSelectedAndDescendantsPosition(glm::vec3 pos);
	void setSelectedAndDescendantsPosition(OpenMesh::Vec3f pos);
	float getThreshold();
	void setThreshold(float th);
	void setSelectedX(float x);
	void setSelectedY(float y);
	void setSelectedZ(float z);
	void setSelectedRadius(float radius);
	void setSelectedTessLevel(float tessLevel);
	void setSmoothingAlgorithm(SQMSmoothingAlgorithm smoothingAlgorithm);
	void setSelectedScaleX(float value);
	void setSelectedScaleY(float value);
	void setSelectedScaleZ(float value);
	void setSelectedRotateX(float value);
	void setSelectedRotateY(float value);
	void setSelectedRotateZ(float value);
	void setUseCapsules(bool useCapsules);
	SQMNode* getSelected();
	
	void drawTriNormals();
	void drawNormals();
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

