#pragma once
#include <vector>
#include "SQMAlgorithm.h"
#include "GLArrayBuffer.h"
#include "Utility.h"
#include "GLCamera.h"
#include "AnimationSkeleton.h"
#include "AnimationCounter.h"

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
	bool canDraw;
public:

	AnimationController(void);
	~AnimationController(void);

	void LoadScene();
	void LoadSkeleton(string fileName, vector<int> loadAnimation, vector<float> speed, vector<float> counter, vector<string> modelMatrix);
	void LoadAnimation(string fileName, float speed);
	void LoadDefaultAnimation(SQMAlgorithm &sqmAlgorithm, vector<int> loadAnimation, vector<float> speed, vector<float> counter, vector<string> modelMatrix);
	void CreateBuffers(SQMAlgorithm &sqmAlgorithm);
	void LoadMatrices(float speed);
	void LoadModelMatrix(string fileName);

	void ClearShaders();
	void InitShaders();

	void Draw(GLCamera *camera);
};