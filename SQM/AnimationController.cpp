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
	inputFile >> comment;
	inputFile >> comment;
	int n;
	inputFile >> n;
	//load models
	for (int i = 0; i < n; i++) {
		string modelName;
		inputFile >> modelName;
		LoadSkeleton(modelName);
	}
	inputFile >> n;
	//load animations
	for (int i = 0; i < n; i++) {
		string animationName;
		inputFile >> animationName;
		LoadAnimation(animationName);
	}
	inputFile >> n;
	//load camera animation
	string cameraFile;
	inputFile >> animateCamera >> cameraFile;
	LoadCameraMovement(cameraFile);
	//load movement
	for (int i = 0; i < n; i++) {
		string movementName;
		inputFile >> movementName;
		LoadMovement(movementName);
	}
	inputFile >> n;
	//load objects with pose files
	for (int i = 0; i < n; i++) {
		int objectID = 0, movementID = -1, animationID = 0, frame = 0;
		float speed = 0, counter = 0;
		string modelMatrixFile;
		char c;
		inputFile >> objectID >> movementID >> animationID >> c >> speed >> counter >> frame >> c >> modelMatrixFile;
		int keyframes = skiningMatrices[animationID].size();
		objects.push_back(objectID);
		counters.push_back(AnimationCounter(animationID, counter, speed, frame, keyframes));
		movement.push_back(movementID);
		LoadModelMatrix(modelMatrixFile);
	}
	inputFile >> n;
	//load objects with just position
	for (int i = 0; i < n; i++) {
		int objectID = 0, movementID = -1, animationID = 0, frame = 0;
		float speed = 0, counter = 0;
		glm::vec3 pos(0);
		char c;
		inputFile >> objectID >> movementID >> animationID >> c >> speed >> counter >> frame >> c >> pos.x >> pos.y >> pos.z;
		int keyframes = skiningMatrices[animationID].size();
		objects.push_back(objectID);
		counters.push_back(AnimationCounter(animationID, counter, speed, frame, keyframes));
		movement.push_back(movementID);
		glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
		modelMatrices.push_back(T);
	}

	InitShaders();

	canDraw = prevDraw;
}

void AnimationController::LoadSkeleton(string fileName) {
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
	LoadDefaultAnimation(sqmAlgorithm);
	CreateBuffers(sqmAlgorithm);
	delete node;
}

void AnimationController::LoadAnimation(string fileName) {
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
	LoadMatrices();
}

void AnimationController::LoadCameraMovement(string fileName) {
	ifstream inputFile(fileName);
	string comment;
	inputFile >> comment;
	int n;
	inputFile >> n;
	knots.push_back(vector<float>(n));
	poses.push_back(vector<ObjectPose>(n));
	for (int i = 0; i < n; i++) {
		int knot;
		glm::vec3 from, to, up;
		inputFile >> knot >> from.x >> from.y >> from.z >> to.x >> to.y >> to.z >> up.x >> up.y >> up.z;
		knots.back()[i] = knot;
		poses.back()[i] = CreateLookAtPose(from, to, up);
	}

	timers.push_back(AnimTimer(knots.back().back()));
	anim_poses.push_back(ObjectPose());
}

void AnimationController::LoadMovement(string fileName) {
	ifstream inputFile(fileName);
	string comment;
	inputFile >> comment;
	int n;
	inputFile >> n;
	knots.push_back(vector<float>(n));
	poses.push_back(vector<ObjectPose>(n));
	for (int i = 0; i < n; i++) {
		int knot;
		glm::vec3 pos, euler, scale;
		inputFile >> knot >> pos.x >> pos.y >> pos.z >> euler.x >> euler.y >> euler.z >> scale.x >> scale.y >> scale.z;
		knots.back()[i] = knot;
		poses.back()[i] = ObjectPose(pos, euler, scale);
	}

	timers.push_back(AnimTimer(knots.back().back()));
	anim_poses.push_back(ObjectPose());
}

void AnimationController::LoadMatrices() {
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
}

void AnimationController::LoadDefaultAnimation(SQMAlgorithm &sqmAlgorithm) {
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

	phongShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
	phongShaders->ctrl->Load("shaders/phong.ctrl");
	phongShaders->ctrl->Compile();

	phongShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
	phongShaders->eval->Load("shaders/phong.eval");
	phongShaders->eval->Compile();

	phongShaders->geom = new GLShader(GL_GEOMETRY_SHADER);
	phongShaders->geom->Load("shaders/phong.geom");
	phongShaders->geom->Compile();

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
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		ObjectPose pose = IntepolatePoseCubic(knots[0], poses[0], timers[0].time, true);
		glm::mat4 model = glm::inverse(pose.GetMatrix());
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}

	timers[0].Next();
	for (int i = 1; i < poses.size(); i++) {
		anim_poses[i] = IntepolatePoseCubicDerivedRot(knots[i], poses[i], poses[i][0].quat, timers[i].time, true);
		timers[i].Next();
	}
	
	//glPatchParameteri(GL_PATCH_VERTICES, 3);
	for (int i = 0; i < objects.size(); i++) {
		if (movement[i] != -1) {
			glm::mat4 model = anim_poses[movement[i]].GetMatrix();
			model = model*modelMatrices[i];
			//model = glm::translate(model, glm::vec3(50*i, 0, 50*i));
			glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
			camera->setupNormalMatrix(model, NORMAL_MATRIX);
		} else {
			glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
			camera->setupNormalMatrix(modelMatrices[i], NORMAL_MATRIX);
		}
		//set skinning matrices
		int matrixID = counters[i].animationID;
		glUniformMatrix4fv(programs[0]->getUniformLocation(SKINNING_MATRICES_STR), animationBones[matrixID]*16, GL_FALSE, &skiningMatrices[matrixID][counters[i].nextFrame][0]);
		glUniformMatrix4fv(programs[0]->getUniformLocation(SKINNING_REF_MATRIX_STR), animationBones[matrixID]*16, GL_FALSE, &skiningMatrices[matrixID][counters[i].frame][0]);
		//set animation step
		glUniform1f(programs[0]->getUniformLocation(ANIM_PARAM_STR), counters[i].counter);
		//draw model
		//models[objects[i]]->DrawElement(0, GL_TRIANGLES);
		models[objects[i]]->DrawElement(0, GL_PATCHES);
		counters[i].Animate();
	}
}

#pragma endregion