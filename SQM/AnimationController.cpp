#include "stdafx.h"
#include "AnimationController.h"
#include <fstream>
#pragma warning(push, 0)
#include <boost\archive\archive_exception.hpp>
#include <boost\archive\basic_xml_iarchive.hpp>
#include <boost\archive\xml_iarchive.hpp>
#include <boost\archive\xml_oarchive.hpp>
#pragma pop
#include "GLCamera.h"
#include "GLProgram.h"
#include <gtc\type_ptr.hpp>
#include <map>
#include <deque>

#define DONT_LOAD 0
#define LOAD_DONT_ANIMATE 1
#define LOAD_ANIMATE 2

#pragma region Init

AnimationController::AnimationController(void)
{
	canDraw = true;
}


AnimationController::~AnimationController(void)
{
	ClearShaders();
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	for (int i = 0; i < animations.size(); i++) {
		delete animations[i];
	}
}

#pragma endregion

#pragma region File Load

void AnimationController::LoadScene() {
	bool prevDraw = canDraw;
	canDraw = false;

	ifstream inputFile("scenes/scene.txt");
	string comment;
	inputFile >> comment;
	inputFile >> comment;
	inputFile >> comment;
	int n;
	inputFile >> n;
	//load models
	for (int i = 0; i < n; i++) {
		int loadAnimation = DONT_LOAD;
		string modelName;
		vector<float> speeds, counterss;
		vector<int> loadAnimations;
		vector<string> modelMatrixFiles;

		inputFile >> loadAnimation >> modelName;
		if (loadAnimation != DONT_LOAD) {
			int m = 0;
			inputFile >> m;
			for (int j = 0; j < m; j++) {
				float speed = 0, counter = 0;
				string modelMatrixFile;

				inputFile >> loadAnimation;

				if (loadAnimation == LOAD_ANIMATE) {
					inputFile >> speed >> counter;
				}
				if (loadAnimation != DONT_LOAD) {
					inputFile >> modelMatrixFile;
				}
				loadAnimations.push_back(loadAnimation);
				speeds.push_back(speed);
				counterss.push_back(counter);
				modelMatrixFiles.push_back(modelMatrixFile);
			}
		}
		LoadSkeleton(modelName, loadAnimations, speeds, counterss, modelMatrixFiles);
	}
	inputFile >> n;
	//load animations
	for (int i = 0; i < n; i++) {
		int model;
		float speed;
		string animName, modelMatrixFile;
		inputFile >> model >> speed >> animName;
		inputFile >> modelMatrixFile;
		objects.push_back(model);
		LoadAnimation(animName, speed);
		LoadModelMatrix(modelMatrixFile);
	}

	InitShaders();

	canDraw = prevDraw;
}

void AnimationController::LoadSkeleton(string fileName, vector<int> loadAnimation, vector<float> speed, vector<float> counter, vector<string> modelMatrix) {
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
	SQMNode *sqmNode = new SQMNode(*node, NULL);
	SQMAlgorithm sqmAlgorithm;
	sqmAlgorithm.setUseCPUSkinning(false);
	sqmAlgorithm.setRoot(sqmNode);
	sqmAlgorithm.executeSQMAlgorithm(SQMFinalPlacement);
	LoadDefaultAnimation(sqmAlgorithm, loadAnimation, speed, counter, modelMatrix);
	CreateBuffers(sqmAlgorithm);
	delete node;
}

void AnimationController::LoadAnimation(string fileName, float speed) {
	ofstream errorLog("log.txt");
	ifstream inputFile(fileName);
	assert(inputFile.good());
	boost::archive::xml_iarchive inputArchive(inputFile);
	AnimationSkeleton *node = NULL;
	try {
		inputArchive >> BOOST_SERIALIZATION_NVP(node);	
	} catch (boost::archive::archive_exception e) {
		errorLog << "Exception: " << e.what() << endl;
		throw e;
	}
	int bones = RecalculateIDs(node);
	animations.push_back(node);
	animationBones.push_back(bones);
	LoadMatrices(speed);
}

void AnimationController::LoadMatrices(float speed) {
	int keyframes = animations.back()->axisAngles.size();

	skiningMatrices.push_back(vector<vector<float> >(0));
	for (int i = 0; i < keyframes; i++) {
		animations.back()->ComputeSkinningMatrices(i);
		skiningMatrices.back().push_back(vector<float>(animationBones.back()*16));
		deque<AnimationSkeleton*> queue;
		queue.push_back(animations.back());

		while (!queue.empty()) {
			AnimationSkeleton *skin = queue.front();
			queue.pop_front();

			int id = skin->id * 16;
			glm::mat4 mat = skin->matrix;
			float *matPtr = glm::value_ptr(mat);
			for (int j = 0; j < 16; j++) {
				skiningMatrices.back()[i][id + j] = matPtr[j];
			}

			for (int j = 0; j < skin->nodes.size(); j++) {
				queue.push_back(skin->nodes[j]);
			}
		}
	}

	counters.push_back(AnimationCounter(speed, 0, keyframes));
}

void AnimationController::LoadDefaultAnimation(SQMAlgorithm &sqmAlgorithm, vector<int> loadAnimation, vector<float> speed, vector<float> counter, vector<string> modelMatrix) {
	for (int i = 0; i < loadAnimation.size(); i++) {
		if (loadAnimation[i] != DONT_LOAD) {
			objects.push_back(models.size());

			int skinningMatricesCount = sqmAlgorithm.getNumberOfSkinningMatrices();
			animationBones.push_back(skinningMatricesCount);

			skiningMatrices.push_back(vector<vector<float> >(0));
			skiningMatrices.back().push_back(vector<float>(skinningMatricesCount*16));
			skiningMatrices.back().push_back(vector<float>(skinningMatricesCount*16));

			sqmAlgorithm.getSkinningMatrices(&skiningMatrices.back()[1][0]);

			glm::mat4 M(1.0);
			for (int i = 0; i < skinningMatricesCount; i++) {
				int id = i * 16;
				float *matPtr = glm::value_ptr(M);
				for (int j = 0; j < 16; j++) {
					skiningMatrices.back()[0][id + j] = matPtr[j];
				}
			}

			if (loadAnimation[i] == LOAD_DONT_ANIMATE) {
				counters.push_back(AnimationCounter(0, 1, 2));
			} else {
				counters.push_back(AnimationCounter(counter[i], speed[i], 0, 2));
				counters.back().cyclic = true;
			}
			LoadModelMatrix(modelMatrix[i]);
		}
	}
}

void AnimationController::CreateBuffers(SQMAlgorithm &sqmAlgorithm) {
	vector<float> points;
	vector<float> normals;
	vector<int> indices;
	vector<int> skinMatrices;
	vector<float> skinWeights;

	convertMeshToArray(sqmAlgorithm.getMesh(), points, normals, indices);
	sqmAlgorithm.getRoot()->getMeshData(skinMatrices, skinWeights);

	GLArrayBuffer *buffer = new GLArrayBuffer();
	buffer->Bind();
	buffer->BindBufferDataf(points, 3, GL_STATIC_DRAW);
	buffer->BindBufferDataf(normals, 3, GL_STATIC_DRAW);
	buffer->BindBufferDatai(skinMatrices, 4, GL_STATIC_DRAW);
	buffer->BindBufferDataf(skinWeights, 4, GL_STATIC_DRAW);

	buffer->BindElement(indices, GL_STATIC_DRAW);

	models.push_back(buffer);
}

void AnimationController::LoadModelMatrix(string fileName) {
	ifstream inputFile(fileName);
	string comment;
	inputFile >> comment;
	float tx = 0, ty = 0, tz = 0, sx = 1, sy = 1, sz = 1, rx = 0, ry = 1, rz = 0, a = 0;
	inputFile >> tx >> ty >> tz >> sx >> sy >> sz >> rx >> ry >> rz >> a;
	glm::mat4 R = glm::rotate(glm::mat4(1.0), a, glm::vec3(rx, ry, rz));
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(sx, sy, sz));
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(tx, ty, tz));
	glm::mat4 M = T * S * R;
	modelMatrices.push_back(M);
}

#pragma endregion

#pragma region Shaders

void AnimationController::ClearShaders() {
	for (int i = 0; i < shaders.size(); i++) {
		delete shaders[i];
		shaders[i] = NULL;
	}
	for (int i = 0; i < programs.size(); i++) {
		delete programs[i];
		programs[i] = NULL;
	}
	shaders.clear();
	programs.clear();
}

void AnimationController::InitShaders() {
	bool prevDraw = canDraw;
	canDraw = false;
	ClearShaders();

	map<std::string, std::string> replaceMap;
	int matrices = 1;
	for (int i = 0; i < animationBones.size(); i++) {
		if (animationBones[i] > matrices) matrices = animationBones[i];
	}
	string matricesStr = std::to_string(matrices);
	replaceMap.insert(std::pair<std::string, std::string>("fill_in_skinnig_matrices_number", matricesStr));

	OpenGLShaders *phongShaders = new OpenGLShaders();
	phongShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongShaders->vert->Load("shaders/phong.vert", replaceMap);
	phongShaders->vert->Compile();

	phongShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	phongShaders->frag->Load("shaders/phong.frag");
	phongShaders->frag->Compile();

	GLProgram *phongProgram = new GLProgram("Phong");
	phongProgram->AttachShaders(phongShaders);
	phongProgram->Link();
	phongProgram->SaveProgramLog();

	shaders.push_back(phongShaders);
	programs.push_back(phongProgram);

	canDraw = prevDraw;
}

#pragma endregion

#pragma region Draw

void AnimationController::Draw(GLCamera *camera) {
	if (!canDraw) return;

	programs[0]->Use();
	//set matrices
	camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	for (int i = 0; i < objects.size(); i++) {
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		camera->setupNormalMatrix(modelMatrices[i], NORMAL_MATRIX);
		//set skinning matrices
		glUniformMatrix4fv(SKINNING_MATRICES, animationBones[i]*16, GL_FALSE, &skiningMatrices[i][counters[i].nextFrame][0]);
		glUniformMatrix4fv(programs[0]->getUniformLocation(SKINNING_REF_MATRIX_STR), animationBones[i]*16, GL_FALSE, &skiningMatrices[i][counters[i].frame][0]);
		//set animation step
		glUniform1f(programs[0]->getUniformLocation(ANIM_PARAM_STR), counters[i].counter);
		//draw model
		models[objects[i]]->DrawElement(0, GL_TRIANGLES);
		counters[i].Animate();
	}
}

#pragma endregion