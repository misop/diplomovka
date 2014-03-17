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
#include <string>

#define generic GenericFromFreeTypeLibrary
#include <freetype\ft2build.h>
#include <freetype\freetype.h>
#undef generic


#pragma region Init

AnimationController::AnimationController(void)
{
	canDraw = true;
	skybox = NULL;
	skyboxTexture = NULL;
}


AnimationController::~AnimationController(void)
{
	ClearShaders();
	if (skybox) delete skybox;
	if (skyboxTexture) delete skyboxTexture;
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
}

#pragma endregion

#pragma region File Load

void AnimationController::LoadScene() {
	bool prevDraw = canDraw;
	canDraw = false;

	ifstream inputFile("scenes/scene.scn");
	//cant create variables in case
	char command;
	string str1, str2;
	int int1, int2, int3;
	glm::vec3 pos, scale, euler;
	char c;

	while (!inputFile.eof()) {
		inputFile >> command;
		switch (command) {
		case '#':
			getline(inputFile, str1);
			break;
		case 'm':
			inputFile >> str1 >> str2;
			LoadModel(str1, str2);
			break;
		case 'v':
			inputFile >> int1 >> str1 >> str2;
			LoadAnimatedModel(int1, str1, str2);
			break;
		case 'c':
			inputFile >> animateCamera >> str1;
			LoadCameraMovement(str1);
			break;
		case 'a':
			inputFile >> str1;
			LoadMovement(str1);
			break;
		case 'o':
			inputFile >> int1 >> int2 >> c;
			inputFile >> pos.x >> pos.y >> pos.z >> c >> scale.x >> scale.y >> scale.z >> c >> euler.x >> euler.y >> euler.z;
			objects.push_back(glm::ivec3(int1, int1, int1));
			movement.push_back(int2);
			LoadModelMatrix(pos, scale, euler);
			break;
		case 'p':
			inputFile >> int1 >> int2 >> int3 >> c;
			inputFile >> pos.x >> pos.y >> pos.z >> c >> scale.x >> scale.y >> scale.z >> c >> euler.x >> euler.y >> euler.z;
			objects.push_back(glm::ivec3(int1, int1, int1 + int2));
			movement.push_back(int3);
			LoadModelMatrix(pos, scale, euler);
			break;
		default:
			break;
		}
	}

	InitSkybox();
	InitShaders();

	canDraw = prevDraw;
}

void AnimationController::LoadModel(string fileName, string texturesFileName) {
	models.push_back(new AssimpObject());
	models.back()->LoadFromFile(fileName);
	models.back()->LoadTexturesFromFile(texturesFileName);
}

void AnimationController::LoadAnimatedModel(int files, string fileName, string texturesFileName) {
	shared_ptr<GLTexture> diffuse, displacement, normal;
	for (int i = 0; i < files; i++) {
		string wholeFileName = fileName;
		if (i < 10) wholeFileName += "0";
		wholeFileName += to_string(i);
		wholeFileName += ".obj";
		models.push_back(new AssimpObject());
		models.back()->LoadFromFile(wholeFileName);
		if (i == 0) {
			models.back()->LoadTexturesFromFile(texturesFileName);
			diffuse = models.back()->diffuseTexture;
			displacement = models.back()->displacementTexture;
			normal = models.back()->normalTexture;
		} else {
			models.back()->diffuseTexture = diffuse;
			models.back()->displacementTexture = displacement;
			models.back()->normalTexture = normal;
		}
	}
}

void AnimationController::LoadCameraMovement(string fileName) {
	ifstream inputFile(fileName);
	knots.push_back(vector<float>());
	poses.push_back(vector<ObjectPose>());

	float prevKnot = 0;
	string str1;
	char c, command;
	float knot;
	glm::vec3 from, to, up;

	while (!inputFile.eof()) {
		inputFile >> command;
		switch (command)
		{
		case '#':
			getline(inputFile, str1);
			break;
		case 'k':
			inputFile >> knot >> c >> from.x >> from.y >> from.z >> c >> to.x >> to.y >> to.z >> c >> up.x >> up.y >> up.z;
			knots.back().push_back(prevKnot + knot);
			poses.back().push_back(CreateLookAtPose(from, to, up));
			prevKnot += knot;
			break;
		default:
			break;
		}
	}

	timers.push_back(AnimTimer(knots.back().back()));
	anim_poses.push_back(ObjectPose());
}

void AnimationController::LoadMovement(string fileName) {
	ifstream inputFile(fileName);
	knots.push_back(vector<float>());
	poses.push_back(vector<ObjectPose>());

	float prevKnot = 0;
	string str1;
	char c, command;
	float knot;
	glm::vec3 pos, euler, scale;

	while (!inputFile.eof()) {
		inputFile >> command;
		switch (command)
		{
		case '#':
			getline(inputFile, str1);
			break;
		case 'k':
			inputFile >> knot >> c >> pos.x >> pos.y >> pos.z >> c >> euler.x >> euler.y >> euler.z >> c >> scale.x >> scale.y >> scale.z;
			knots.back().push_back(prevKnot + knot);
			poses.back().push_back(ObjectPose(pos, euler, scale));
			prevKnot += knot;
			break;
		default:
			break;
		}
	}

	timers.push_back(AnimTimer(knots.back().back()));
	anim_poses.push_back(ObjectPose());
}

void AnimationController::LoadModelMatrix(string fileName) {
	ifstream inputFile(fileName);
	string comment;
	inputFile >> comment;
	glm::vec3 pos, scale, euler;
	char c;
	inputFile >> pos.x >> pos.y >> pos.z >> c >> scale.x >> scale.y >> scale.z >> c >> euler.x >> euler.y >> euler.z;
	LoadModelMatrix(pos, scale, euler);
}

void AnimationController::LoadModelMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 euler) {
	glm::quat qx = glm::angleAxis(euler.x, glm::vec3(1, 0, 0));
	glm::quat qy = glm::angleAxis(euler.y, glm::vec3(0, 1, 0));
	glm::quat qz = glm::angleAxis(euler.z, glm::vec3(0, 0, 1));
	glm::quat q = qz * qy * qx;
	glm::mat4 R = glm::toMat4(q);
	glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
	glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
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

	OpenGLShaders *skyboxShaders = new OpenGLShaders();
	skyboxShaders->vert = new GLShader(GL_VERTEX_SHADER);
	skyboxShaders->vert->Load("shaders/skybox.vert");
	skyboxShaders->vert->Compile();

	skyboxShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	skyboxShaders->frag->Load("shaders/skybox.frag");
	skyboxShaders->frag->Compile();

	GLProgram *skyboxProgram = new GLProgram("Skybox");
	skyboxProgram->AttachShaders(skyboxShaders);
	skyboxProgram->Link();
	skyboxProgram->SaveProgramLog();

	shaders.push_back(skyboxShaders);
	programs.push_back(skyboxProgram);

	canDraw = prevDraw;
}

void AnimationController::InitSkybox() {
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	GLfloat cube_vertices[] = {1,1,1,    -1,1,1,   -1,-1,1,  1,-1,1,  
		1,1,1,    1,-1,1,   1,-1,-1,  1,1,-1,
		1,1,1,    1,1,-1,   -1,1,-1,  -1,1,1,
		-1,1,1,   -1,1,-1,  -1,-1,-1, -1,-1,1,
		-1,-1,-1, 1,-1,-1,  1,-1,1,   -1,-1,1,
		1,-1,-1,  -1,-1,-1, -1,1,-1,  1,1,-1 };
	GLuint cube_indices[] = {0,1,2,3,  4,5,6,7,  8,9,10,11,   12,13,14,15,   16,17,18,19,  20,21,22,23};

	vector<float> vertices(begin(cube_vertices), end(cube_vertices));
	vector<int> indices(begin(cube_indices), end(cube_indices));

	if (skybox) delete skybox;
	skybox = new GLArrayBuffer();
	skybox->Bind();
	skybox->BindBufferDataf(vertices, 3, GL_STATIC_DRAW);
	skybox->BindElement(indices, GL_STATIC_DRAW);

	skyboxModel = glm::scale(glm::mat4(1.0), glm::vec3(8000, 8000, 8000));
	skyboxModel *= glm::rotate(glm::mat4(1.0), 180.0f, glm::vec3(0, 1, 0));

	if (skyboxTexture) delete skyboxTexture;
	skyboxTexture = new GLTexture(GL_TEXTURE_CUBE_MAP);
	skyboxTexture->LoadCubeTextureFromImages("skybox/grimnight_posX.png", "skybox/grimnight_negX.png",
		"skybox/grimnight_posY.png", "skybox/grimnight_negY.png",
		"skybox/grimnight_posZ.png", "skybox/grimnight_negZ.png");
}

#pragma endregion

#pragma region Draw

void AnimationController::Draw(GLCamera *camera) {
	if (!canDraw) return;
	//animate camera
	glm::mat4 view_matrix;
	if (animateCamera) {
		ObjectPose pose = IntepolatePoseCubic(knots[0], poses[0], timers[0].time, true);
		view_matrix = glm::inverse(pose.GetMatrix());
	}
	//skybox
	programs[1]->Use();	
	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(skyboxModel));
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}

	glActiveTexture(GL_TEXTURE0);
	skyboxTexture->Bind();
	skybox->DrawElement(0, GL_QUADS);
	//objects
	programs[0]->Use();
	glUniform1f(programs[0]->getUniformLocation(SCREEN_HEIGHT_STR), (float)camera->height);
	//set matrices
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}

	timers[0].Next();
	for (int i = 1; i < poses.size(); i++) {
		//anim_poses[i] = IntepolatePoseCubicDerivedRot(knots[i], poses[i], poses[i][0].quat, timers[i].time, true);
		anim_poses[i] = IntepolatePoseCubic(knots[i], poses[i], timers[i].time, true);
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
		int idx = objects[i].y;
		//set textures
		glActiveTexture(GL_TEXTURE0);
		models[idx]->diffuseTexture->Bind();
		//draw model
		models[idx]->buffer->DrawElement(0, GL_PATCHES);
		//animate
		objects[i].y++;
		if (objects[i].y >= objects[i].z) objects[i].y = objects[i].x;
	}
}

#pragma endregion