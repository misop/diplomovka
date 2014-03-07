#pragma once
#include <vector>
#include "SQMAlgorithm.h"
#include "GLArrayBuffer.h"
#include "Utility.h"
#include "GLCamera.h"
#include "AnimationSkeleton.h"
#include "AnimationCounter.h"
#include "Interpolation.h"

class AnimationController
{
	vector<GLProgram *> programs;
	vector<OpenGLShaders *> shaders;
	vector<GLArrayBuffer *> models;
	vector<int> objects;
	vector<AnimationSkeleton *> animations;
	vector<int> animationBones;
	vector<AnimationCounter> counters;
	vector<vector<vector<float> > > skiningMatrices;
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
	void LoadSkeleton(string fileName);
	void LoadAnimation(string fileName);
	void LoadCameraMovement(string fileName);
	void LoadMovement(string fileName);
	void LoadDefaultAnimation(SQMAlgorithm &sqmAlgorithm);
	void CreateBuffers(SQMAlgorithm &sqmAlgorithm);
	void LoadMatrices();
	void LoadModelMatrix(string fileName);

	void ClearShaders();
	void InitShaders();

	void Draw(GLCamera *camera);
};