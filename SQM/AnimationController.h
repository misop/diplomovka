#pragma once
#include <vector>
#include "SQMAlgorithm.h"
#include "GLArrayBuffer.h"
#include "Utility.h"
#include "GLCamera.h"
#include "AnimationSkeleton.h"
#include "AnimationCounter.h"
#include "Interpolation.h"
#include "AssimpObject.h"

class AnimationController
{
	vector<GLProgram *> programs;
	vector<OpenGLShaders *> shaders;
	vector<AssimpObject *> models;
	vector<int> objects;
	vector<glm::mat4> modelMatrices;
	vector<vector<ObjectPose> > poses;
	vector<ObjectPose> anim_poses;
	vector<vector<float> > knots;
	vector<AnimTimer> timers;
	vector<int> movement;
	bool animateCamera;
	bool canDraw;
public:

	AnimationController(void);
	~AnimationController(void);

	void LoadScene();
	void LoadModel(string fileName, string texturesFileName);
	void LoadCameraMovement(string fileName);
	void LoadMovement(string fileName);
	void LoadModelMatrix(string fileName);

	void ClearShaders();
	void InitShaders();

	void Draw(GLCamera *camera);
};