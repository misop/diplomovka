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
#include <memory>
#include <fstream>
#include <assimp\mesh.h>

#define generic GenericFromFreeTypeLibrary
#include <freetype\ft2build.h>
#include <freetype\freetype.h>
#undef generic


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
	int n;
	inputFile >> n;
	//load models
	for (int i = 0; i < n; i++) {
		string modelName, texturesFileName;
		inputFile >> modelName >> texturesFileName;
		LoadModel(modelName, texturesFileName);
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
		int objectID = 0, movementID = -1;
		string modelMatrixFile;
		char c;
		inputFile >> objectID >> movementID >> c >> modelMatrixFile;
		objects.push_back(objectID);
		movement.push_back(movementID);
		LoadModelMatrix(modelMatrixFile);
	}
	inputFile >> n;
	//load objects with just position
	for (int i = 0; i < n; i++) {
		int objectID = 0, movementID = -1;
		glm::vec3 pos(0);
		char c;
		inputFile >> objectID >> movementID >> c >> pos.x >> pos.y >> pos.z;
		objects.push_back(objectID);
		movement.push_back(movementID);
		glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
		modelMatrices.push_back(T);
	}

	InitShaders();

	canDraw = prevDraw;
}

void AnimationController::LoadModel(string fileName, string texturesFileName) {
	models.push_back(new AssimpObject());
	models.back()->LoadFromFile(fileName);
	models.back()->LoadTexturesFromFile(texturesFileName);
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

void AnimationController::LoadModelMatrix(string fileName) {
	ifstream inputFile(fileName);
	string comment;
	inputFile >> comment;
	float tx = 0, ty = 0, tz = 0, sx = 1, sy = 1, sz = 1, rx = 0, ry = 1, rz = 0;
	char c;
	inputFile >> tx >> ty >> tz >> c >> sx >> sy >> sz >> c >> rx >> ry >> rz;
	glm::quat qx = glm::angleAxis(rx, glm::vec3(1, 0, 0));
	glm::quat qy = glm::angleAxis(ry, glm::vec3(0, 1, 0));
	glm::quat qz = glm::angleAxis(rz, glm::vec3(0, 0, 1));
	glm::quat q = qz * qy * qx;
	glm::mat4 R = glm::toMat4(q);
	//glm::mat4 R = glm::rotate(glm::mat4(1.0), a, glm::vec3(rx, ry, rz));
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

	OpenGLShaders *phongShaders = new OpenGLShaders();
	phongShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongShaders->vert->Load("shaders/phong.vert");
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
	glUniform1f(programs[0]->getUniformLocation(SCREEN_HEIGHT_STR), (float)camera->height);
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
	
	for (int i = 0; i < objects.size(); i++) {
		if (movement[i] != -1) {
			glm::mat4 model = anim_poses[movement[i]].GetMatrix();
			model = model*modelMatrices[i];
			glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
			camera->setupNormalMatrix(model, NORMAL_MATRIX);
		} else {
			glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
			camera->setupNormalMatrix(modelMatrices[i], NORMAL_MATRIX);
		}
		int idx = objects[i];
		//set textures
		glActiveTexture(GL_TEXTURE0);
		models[idx]->diffuseTexture->Bind();
		//draw model
		models[idx]->buffer->DrawElement(0, GL_PATCHES);
	}
}

#pragma endregion