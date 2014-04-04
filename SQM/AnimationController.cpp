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
#include <iomanip>

#define generic GenericFromFreeTypeLibrary
#include <freetype\ft2build.h>
#include <freetype\freetype.h>
#undef generic

#pragma region Defines

#define PHONG                  0
#define SKYBOX                 1
#define TEXT                   2
#define SHADOW_MAP             3
#define SSAO_FBO               4
#define WIREFRAME              5
#define PHONG_NORMAL           6
#define PHONG_ALL              7
#define TOON                   8
#define TOON_ALL               9
#define SOBEL                 10
#define BILATERAL             11
#define SSAO                  12
#define EDGE_FBO              13
#define POST_PROCESS_AO       14
#define TEXTURED_QUAD         15
#define OREN_NAYAR            16
#define COOK_TORRANCE         17

#define DIFFUSE_TEXTURE            GL_TEXTURE0
#define DISPLACEMENT_TEXTURE       GL_TEXTURE1
#define NORMAL_TEXTURE             GL_TEXTURE2
#define SHADOWMAP_TEXTURE          GL_TEXTURE4
#define TOON_TEXTURE               GL_TEXTURE5
#define SCREEN_NORMALS_TEXTURE     GL_TEXTURE6
#define DEPTH_TEXTURE              GL_TEXTURE7

#define SCREEN_PX_WIDTH    1920
#define SCREEN_PX_HEIGHT   1080
#define DEBUG_SIZE          256
#define EDGE_TEX_SIZE      1024
#define MAP_SIZE           2048
#define SSAO_TEX_SIZE       512
#define DEBUG_SIZE          256

#pragma endregion

#pragma region Init

AnimationController::AnimationController(void)
{
	canDraw = true;
	skybox = NULL;
	skyboxTexture = NULL;
	toonShadingTexture = NULL;
	text = NULL;
	quad = NULL;
	ssaoFbo = NULL;
	ssaoFbo2 = NULL;
	upscaleFBO = NULL;
	blurXFBO = NULL;
	blurYFBO = NULL;
	edgeFbo = NULL;
	shadowmap = NULL;
	noiseTexture = NULL;
	drawWireframe = false;
	useSSAO = false;
	useDispalcement = false;
	pause = false;
	useToonShading = false;
	drawText = true;
	drawDebug = true;
	useNormals = true;
	PixelsPerEdge = 500;
	time = GetTickCount();
	frames = 0;
	fps = 0;
	perFrame = 0;
	orenNayarModel = glm::translate(glm::mat4(1.0), glm::vec3(-450, 120, -1050)) * glm::scale(glm::mat4(1.0), glm::vec3(8.0));
	cookTorranceModel = glm::translate(glm::mat4(1.0), glm::vec3(600, 120, -1050)) * glm::scale(glm::mat4(1.0), glm::vec3(8.0));
	//sunPos = glm::vec3(0, 1500, 1200);
	sunPos = glm::vec3(-1780, 2000, 2400);
	sunLook = glm::vec3(0, 0, -500);
	sunFar = glm::length(glm::vec3(0, 0, -2250) - sunPos);
	sunNear = glm::length(glm::vec3(0, 300, 1200) - sunPos);
	sun = glm::normalize(sunLook - sunPos);
	sunColor = glm::vec3(245.0/255.0, 234.0/255.0, 246.0/255.0);
}


AnimationController::~AnimationController(void)
{
	ClearShaders();
	if (skybox) delete skybox;
	if (skyboxTexture) delete skyboxTexture;
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	delete toonShadingTexture;
	delete noiseTexture;
	delete text;
	delete quad;
	delete ssaoFbo;
	delete ssaoFbo2;
	delete upscaleFBO;
	delete blurXFBO;
	delete blurYFBO;
	delete edgeFbo;
	delete shadowmap;
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

	InitToonShadingTexture();
	InitSkybox();
	InitShaders();
	InitFont();
	InitFBOs();

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
#pragma region Phong
	OpenGLShaders *phongShaders = new OpenGLShaders();
	phongShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongShaders->vert->Load("shaders/phong_noTess.vert");
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
#pragma endregion
#pragma region Skybox
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

	OpenGLShaders *textShaders = new OpenGLShaders();
	textShaders->vert = new GLShader(GL_VERTEX_SHADER);
	textShaders->vert->Load("shaders/text.vert");
	textShaders->vert->Compile();

	textShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	textShaders->frag->Load("shaders/text.frag");
	textShaders->frag->Compile();

	GLProgram *textProgram = new GLProgram("Text");
	textProgram->AttachShaders(textShaders);
	textProgram->Link();
	textProgram->SaveProgramLog();

	shaders.push_back(textShaders);
	programs.push_back(textProgram);
#pragma endregion
#pragma region Shadowmap
	OpenGLShaders *shadowmapShaders = new OpenGLShaders();
	shadowmapShaders->vert = new GLShader(GL_VERTEX_SHADER);
	shadowmapShaders->vert->Load("shaders/ShadowMap.vert");
	shadowmapShaders->vert->Compile();

	shadowmapShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	shadowmapShaders->frag->Load("shaders/ShadowMap.frag");
	shadowmapShaders->frag->Compile();

	GLProgram *shadowmapProgram = new GLProgram("Shadowmap");
	shadowmapProgram->AttachShaders(shadowmapShaders);
	shadowmapProgram->Link();
	shadowmapProgram->SaveProgramLog();

	shaders.push_back(shadowmapShaders);
	programs.push_back(shadowmapProgram);
#pragma endregion
#pragma region SSAO FBO
	OpenGLShaders *fboShaders = new OpenGLShaders();
	fboShaders->vert = new GLShader(GL_VERTEX_SHADER);
	fboShaders->vert->Load("shaders/FBO.vert");
	fboShaders->vert->Compile();

	fboShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	fboShaders->frag->Load("shaders/FBO.frag");
	fboShaders->frag->Compile();

	GLProgram *fboProgram = new GLProgram("FBO");
	fboProgram->AttachShaders(fboShaders);
	fboProgram->Link();
	fboProgram->SaveProgramLog();

	shaders.push_back(fboShaders);
	programs.push_back(fboProgram);
#pragma endregion
#pragma region Wireframe
	OpenGLShaders *phongWireframeShaders = new OpenGLShaders();
	phongWireframeShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongWireframeShaders->vert->Load("shaders/phong.vert");
	phongWireframeShaders->vert->Compile();

	phongWireframeShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
	phongWireframeShaders->ctrl->Load("shaders/phong.ctrl");
	phongWireframeShaders->ctrl->Compile();

	phongWireframeShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
	phongWireframeShaders->eval->Load("shaders/phong.eval");
	phongWireframeShaders->eval->Compile();

	phongWireframeShaders->geom = new GLShader(GL_GEOMETRY_SHADER);
	phongWireframeShaders->geom->Load("shaders/phong.geom");
	phongWireframeShaders->geom->Compile();

	phongWireframeShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	phongWireframeShaders->frag->Load("shaders/phong_wireframe.frag");
	phongWireframeShaders->frag->Compile();

	GLProgram *phongWireframeProgram = new GLProgram("Phong_wireframe");
	phongWireframeProgram->AttachShaders(phongWireframeShaders);
	phongWireframeProgram->Link();
	phongWireframeProgram->SaveProgramLog();

	shaders.push_back(phongWireframeShaders);
	programs.push_back(phongWireframeProgram);
#pragma endregion
#pragma region Phong Normal
	OpenGLShaders *phongNormalShaders = new OpenGLShaders();
	phongNormalShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongNormalShaders->vert->Load("shaders/phong_noTess.vert");
	phongNormalShaders->vert->Compile();

	phongNormalShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	phongNormalShaders->frag->Load("shaders/phong_normal.frag");
	phongNormalShaders->frag->Compile();

	GLProgram *phongNormalProgram = new GLProgram("Phong_normal");
	phongNormalProgram->AttachShaders(phongNormalShaders);
	phongNormalProgram->Link();
	phongNormalProgram->SaveProgramLog();

	shaders.push_back(phongNormalShaders);
	programs.push_back(phongNormalProgram);
#pragma endregion
#pragma region Phong ALL
	OpenGLShaders *phongAllShaders = new OpenGLShaders();
	phongAllShaders->vert = new GLShader(GL_VERTEX_SHADER);
	phongAllShaders->vert->Load("shaders/phong.vert");
	phongAllShaders->vert->Compile();

	phongAllShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
	phongAllShaders->ctrl->Load("shaders/phong.ctrl");
	phongAllShaders->ctrl->Compile();

	phongAllShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
	phongAllShaders->eval->Load("shaders/phong.eval");
	phongAllShaders->eval->Compile();

	phongAllShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	phongAllShaders->frag->Load("shaders/phongAll.frag");
	phongAllShaders->frag->Compile();

	GLProgram *phongAllProgram = new GLProgram("Phong_all");
	phongAllProgram->AttachShaders(phongAllShaders);
	phongAllProgram->Link();
	phongAllProgram->SaveProgramLog();

	shaders.push_back(phongAllShaders);
	programs.push_back(phongAllProgram);
#pragma endregion
#pragma region Toon
	OpenGLShaders *toonShaders = new OpenGLShaders();
	toonShaders->vert = new GLShader(GL_VERTEX_SHADER);
	toonShaders->vert->Load("shaders/phong_noTess.vert");
	toonShaders->vert->Compile();

	toonShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	toonShaders->frag->Load("shaders/toon.frag");
	toonShaders->frag->Compile();

	GLProgram *toonProgram = new GLProgram("Toon");
	toonProgram->AttachShaders(toonShaders);
	toonProgram->Link();
	toonProgram->SaveProgramLog();

	shaders.push_back(toonShaders);
	programs.push_back(toonProgram);
#pragma endregion
#pragma region Phong ALL
	OpenGLShaders *toonAllShaders = new OpenGLShaders();
	toonAllShaders->vert = new GLShader(GL_VERTEX_SHADER);
	toonAllShaders->vert->Load("shaders/phong.vert");
	toonAllShaders->vert->Compile();

	toonAllShaders->ctrl = new GLShader(GL_TESS_CONTROL_SHADER);
	toonAllShaders->ctrl->Load("shaders/phong.ctrl");
	toonAllShaders->ctrl->Compile();

	toonAllShaders->eval = new GLShader(GL_TESS_EVALUATION_SHADER);
	toonAllShaders->eval->Load("shaders/phong.eval");
	toonAllShaders->eval->Compile();

	toonAllShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	toonAllShaders->frag->Load("shaders/toonAll.frag");
	toonAllShaders->frag->Compile();

	GLProgram *toonAllProgram = new GLProgram("Toon_all");
	toonAllProgram->AttachShaders(toonAllShaders);
	toonAllProgram->Link();
	toonAllProgram->SaveProgramLog();

	shaders.push_back(toonAllShaders);
	programs.push_back(toonAllProgram);
#pragma endregion
#pragma region Sobel
	OpenGLShaders *sobelShaders = new OpenGLShaders();
	sobelShaders->vert = new GLShader(GL_VERTEX_SHADER);
	sobelShaders->vert->Load("shaders/sobel.vert");
	sobelShaders->vert->Compile();

	sobelShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	sobelShaders->frag->Load("shaders/sobel.frag");
	sobelShaders->frag->Compile();

	GLProgram *sobelProgram = new GLProgram("Sobel");
	sobelProgram->AttachShaders(sobelShaders);
	sobelProgram->Link();
	sobelProgram->SaveProgramLog();

	shaders.push_back(sobelShaders);
	programs.push_back(sobelProgram);
#pragma endregion
#pragma region Bilateral
	OpenGLShaders *bilateralShaders = new OpenGLShaders();
	bilateralShaders->vert = new GLShader(GL_VERTEX_SHADER);
	bilateralShaders->vert->Load("shaders/gaussian.vert");
	bilateralShaders->vert->Compile();

	bilateralShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	bilateralShaders->frag->Load("shaders/gaussian.frag");
	bilateralShaders->frag->Compile();

	GLProgram *bilateralProgram = new GLProgram("Bilateral");
	bilateralProgram->AttachShaders(bilateralShaders);
	bilateralProgram->Link();
	bilateralProgram->SaveProgramLog();

	shaders.push_back(bilateralShaders);
	programs.push_back(bilateralProgram);
#pragma endregion
#pragma region SSAO
	OpenGLShaders *ssaoShaders = new OpenGLShaders();
	ssaoShaders->vert = new GLShader(GL_VERTEX_SHADER);
	ssaoShaders->vert->Load("shaders/ssao.vert");
	ssaoShaders->vert->Compile();

	ssaoShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	ssaoShaders->frag->Load("shaders/ssao.frag");
	ssaoShaders->frag->Compile();

	GLProgram *ssaoProgram = new GLProgram("SSAO");
	ssaoProgram->AttachShaders(ssaoShaders);
	ssaoProgram->Link();
	ssaoProgram->SaveProgramLog();

	shaders.push_back(ssaoShaders);
	programs.push_back(ssaoProgram);
#pragma endregion
#pragma region Edge FBO
	OpenGLShaders *edgeFBOShaders = new OpenGLShaders();
	edgeFBOShaders->vert = new GLShader(GL_VERTEX_SHADER);
	edgeFBOShaders->vert->Load("shaders/edgeFBO.vert");
	edgeFBOShaders->vert->Compile();

	edgeFBOShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	edgeFBOShaders->frag->Load("shaders/edgeFBO.frag");
	edgeFBOShaders->frag->Compile();

	GLProgram *edgeFBOProgram = new GLProgram("Edge_FBO");
	edgeFBOProgram->AttachShaders(edgeFBOShaders);
	edgeFBOProgram->Link();
	edgeFBOProgram->SaveProgramLog();

	shaders.push_back(edgeFBOShaders);
	programs.push_back(edgeFBOProgram);
#pragma endregion
#pragma region Postprocess SSAO
	OpenGLShaders *postSSAOShaders = new OpenGLShaders();
	postSSAOShaders->vert = new GLShader(GL_VERTEX_SHADER);
	postSSAOShaders->vert->Load("shaders/postSSAO.vert");
	postSSAOShaders->vert->Compile();

	postSSAOShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	postSSAOShaders->frag->Load("shaders/postSSAO.frag");
	postSSAOShaders->frag->Compile();

	GLProgram *postSSAOProgram = new GLProgram("Post_SSAO");
	postSSAOProgram->AttachShaders(postSSAOShaders);
	postSSAOProgram->Link();
	postSSAOProgram->SaveProgramLog();

	shaders.push_back(postSSAOShaders);
	programs.push_back(postSSAOProgram);
#pragma endregion
#pragma region Textured Quad
	OpenGLShaders *quadShaders = new OpenGLShaders();
	quadShaders->vert = new GLShader(GL_VERTEX_SHADER);
	quadShaders->vert->Load("shaders/texturedQuad.vert");
	quadShaders->vert->Compile();

	quadShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	quadShaders->frag->Load("shaders/texturedQuad.frag");
	quadShaders->frag->Compile();

	GLProgram *quadProgram = new GLProgram("Textured_Quad");
	quadProgram->AttachShaders(quadShaders);
	quadProgram->Link();
	quadProgram->SaveProgramLog();

	shaders.push_back(quadShaders);
	programs.push_back(quadProgram);
#pragma endregion
#pragma region Oren Nayar
	OpenGLShaders *orenNayarShaders = new OpenGLShaders();
	orenNayarShaders->vert = new GLShader(GL_VERTEX_SHADER);
	orenNayarShaders->vert->Load("shaders/phong_noTess.vert");
	orenNayarShaders->vert->Compile();

	orenNayarShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	orenNayarShaders->frag->Load("shaders/oren_nayar.frag");
	orenNayarShaders->frag->Compile();

	GLProgram *orenNayarProgram = new GLProgram("Oren_Nayar");
	orenNayarProgram->AttachShaders(orenNayarShaders);
	orenNayarProgram->Link();
	orenNayarProgram->SaveProgramLog();

	shaders.push_back(orenNayarShaders);
	programs.push_back(orenNayarProgram);
#pragma endregion
#pragma region Cook Torrance
	OpenGLShaders *cookTorranceShaders = new OpenGLShaders();
	cookTorranceShaders->vert = new GLShader(GL_VERTEX_SHADER);
	cookTorranceShaders->vert->Load("shaders/phong_noTess.vert");
	cookTorranceShaders->vert->Compile();

	cookTorranceShaders->frag = new GLShader(GL_FRAGMENT_SHADER);
	cookTorranceShaders->frag->Load("shaders/cook_torrance.frag");
	cookTorranceShaders->frag->Compile();

	GLProgram *cookTorranceProgram = new GLProgram("Cook_Torrance");
	cookTorranceProgram->AttachShaders(cookTorranceShaders);
	cookTorranceProgram->Link();
	cookTorranceProgram->SaveProgramLog();

	shaders.push_back(cookTorranceShaders);
	programs.push_back(cookTorranceProgram);
#pragma endregion

	canDraw = prevDraw;
}

void AnimationController::InitToonShadingTexture() {
	delete toonShadingTexture;
	toonShadingTexture = new GLTexture(GL_TEXTURE_2D);
	toonShadingTexture->Bind();
	toonShadingTexture->LoadRGBATextureFromImage("./textures/toon.png");
	toonShadingTexture->TexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	toonShadingTexture->TexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

void AnimationController::InitFont() {
	delete text;
	text = new GLText("fonts/Consolas_Bold.ttf");
}

void AnimationController::InitFBOs() {
	//debug quad vao
	GLfloat verts[] = {0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0};
	vector<float> vertices(std::begin(verts), std::end(verts));
	delete quad;
	quad = new GLArrayBuffer();
	quad->Bind();
	quad->BindBufferDataf(vertices, 2, GL_STATIC_DRAW);
	glBindVertexArray(0);
	//bias matrix
	//biasMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5));
	//biasMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.5)) * biasMatrix; 
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	//fbos
	bool good = true;
	delete shadowmap;
	shadowmap = new GLFrameBuffer();
	good &= shadowmap->CreateGeneralFBO(MAP_SIZE, MAP_SIZE, 1, true);

	delete ssaoFbo;
	ssaoFbo = new GLFrameBuffer();
	good &= ssaoFbo->CreateGeneralFBO(SSAO_TEX_SIZE, SSAO_TEX_SIZE, 2, true);

	delete ssaoFbo2;
	ssaoFbo2 = new GLFrameBuffer();
	good &= ssaoFbo2->CreateGeneralFBO(SSAO_TEX_SIZE, SSAO_TEX_SIZE, 1, false);

	delete blurXFBO;
	blurXFBO = new GLFrameBuffer();
	good &= blurXFBO->CreateGeneralFBO(SSAO_TEX_SIZE, SSAO_TEX_SIZE, 1, false);

	delete blurYFBO;
	blurYFBO = new GLFrameBuffer();
	good &= blurYFBO->CreateGeneralFBO(SSAO_TEX_SIZE, SSAO_TEX_SIZE, 1, false);

	delete upscaleFBO;
	upscaleFBO = new GLFrameBuffer();
	good &= upscaleFBO->CreateGeneralFBO(SCREEN_PX_WIDTH, SCREEN_PX_HEIGHT, 1, false);

	delete edgeFbo;
	edgeFbo = new GLFrameBuffer();
	good &= edgeFbo->CreateGeneralFBO(EDGE_TEX_SIZE, EDGE_TEX_SIZE, 1, true);

	delete noiseTexture;
	noiseTexture = new GLTexture(GL_TEXTURE_2D);
	noiseTexture->LoadRGBATextureFromImage("./textures/noise.png");
}

#pragma endregion

#pragma region Draw

void AnimationController::Draw(GLCamera *camera) {
	if (!canDraw) return;

	unsigned long t = GetTickCount();
	unsigned long diff = t - time;
	if (diff >= 100) {
		time = t;
		perFrame = (double)diff / (double)frames;
		fps = 1000.0/ perFrame;
		frames = 0;
	}
	frames++;

	glm::mat4 view_matrix = AnimateCamera();
	DrawSkybox(camera, view_matrix);

	MoveTimers();

	DrawToTexture(camera, view_matrix);
	GetShadowMaps(camera);

	//objects
	if (drawWireframe) {
		DrawWireframe(camera, view_matrix);
	} else if (useDispalcement) {
		if (useToonShading)
			DrawToonAll(camera, view_matrix);
		else
			DrawPhongAll(camera, view_matrix);
	} else {
		if (useToonShading)
			DrawToon(camera, view_matrix);
		else if (useNormals)
			DrawPhongNormal(camera, view_matrix);
		else
			DrawPhong(camera, view_matrix);
	}

	DrawOrenNayarFrog(camera, view_matrix);
	DrawCookTorranceFrog(camera, view_matrix);

	Postprocess(camera);

	DrawDebug(camera);
	DrawText(camera);
}

void AnimationController::SetLight() {
	glUniformMatrix4fv(SHADOW_MATRIX, 1, GL_FALSE, glm::value_ptr(depthMVP));
	glUniform4f(SUN, sun.x, sun.y, sun.z, 0);
	glUniform4f(SUN_COLOR, sunColor.x, sunColor.y, sunColor.z, 1);
}

#pragma region Different shaders
void AnimationController::DrawWireframe(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[WIREFRAME]->Use();
	glUniform1f(SCREEN_HEIGHT, (float)camera->height);
	glUniform1f(PIXELS_PER_EDGE, PixelsPerEdge);
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}

	DrawModels(camera, view_matrix);
}


void AnimationController::DrawOrenNayarFrog(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[OREN_NAYAR]->Use();
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(orenNayarModel));
	if (animateCamera) {
		glm::mat3 NM = glm::mat3(view_matrix*orenNayarModel);
		NM = glm::transpose(glm::inverse(NM));
		glUniformMatrix3fv(NORMAL_MATRIX, 1, GL_FALSE, glm::value_ptr(NM));
	} else {
		camera->setupNormalMatrix(orenNayarModel, NORMAL_MATRIX);
	}
	BindTextures(7, true);
	models[7]->buffer->DrawElement(0, GL_TRIANGLES);
	BindTextures(7, false);
}

void AnimationController::DrawCookTorranceFrog(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[COOK_TORRANCE]->Use();
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(cookTorranceModel));
	if (animateCamera) {
		glm::mat3 NM = glm::mat3(view_matrix*cookTorranceModel);
		NM = glm::transpose(glm::inverse(NM));
		glUniformMatrix3fv(NORMAL_MATRIX, 1, GL_FALSE, glm::value_ptr(NM));
	} else {
		camera->setupNormalMatrix(cookTorranceModel, NORMAL_MATRIX);
	}
	BindTextures(7, true);
	models[7]->buffer->DrawElement(0, GL_TRIANGLES);
	BindTextures(7, false);
}

void AnimationController::DrawPhong(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[PHONG]->Use();
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	DrawModels(camera, view_matrix, true, false);
}

void AnimationController::DrawPhongNormal(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[PHONG_NORMAL]->Use();
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	DrawModels(camera, view_matrix, true, false);
}

void AnimationController::DrawPhongAll(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[PHONG_ALL]->Use();
	glUniform1f(SCREEN_HEIGHT, (float)camera->height);
	glUniform1f(PIXELS_PER_EDGE, PixelsPerEdge);
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	DrawModels(camera, view_matrix);
}

void AnimationController::DrawToon(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[TOON]->Use();
	glActiveTexture(GL_TEXTURE5);
	toonShadingTexture->Bind();
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	DrawModels(camera, view_matrix, true, false);
}

void AnimationController::DrawToonAll(GLCamera *camera, glm::mat4 &view_matrix) {
	programs[PHONG_ALL]->Use();
	glActiveTexture(GL_TEXTURE5);
	toonShadingTexture->Bind();
	glUniform1f(SCREEN_HEIGHT, (float)camera->height);
	glUniform1f(PIXELS_PER_EDGE, PixelsPerEdge);
	if (animateCamera) {
		camera->getProjectionMatrix(PROJECTION_MATRIX);
		glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
	} else {
		camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
	}
	SetLight();

	DrawModels(camera, view_matrix);
}

void AnimationController::DetectEdges() {
	programs[SOBEL]->Use();
	float factor = (float)1/(float)MAP_SIZE;

	glUniform1f(TEXEL_WIDTH, factor);
	glUniform1f(TEXEL_HEIGHT, factor);
}

void AnimationController::Smooth(bool horizontal) {
	programs[BILATERAL]->Use();
	float factor = (float)1/(float)MAP_SIZE;

	glUniform1f(TEXEL_SIZE, factor);
	glUniform1i(HORIZONTAL, horizontal);
}
#pragma endregion

void AnimationController::DrawModels(GLCamera *camera, glm::mat4 &view_matrix, bool texture, bool tessellate, bool ssao, bool screen) {
	for (int i = 0; i < objects.size(); i++) {
		glm::mat4 model = modelMatrices[i];
		if (movement[i] != -1) model = anim_poses[movement[i]].GetMatrix() * model;
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
		if (ssao) {
			glm::mat3 NormalMatrix;
			if (animateCamera) {
				NormalMatrix = glm::mat3(view_matrix * model);
				if (screen) NormalMatrix = glm::mat3(camera->projection) * NormalMatrix;
			}
			else {
				NormalMatrix = glm::mat3(camera->view * model);
				if (screen) NormalMatrix = glm::mat3(camera->projection) * NormalMatrix;
			}
			NormalMatrix = glm::transpose(glm::inverse(NormalMatrix));
			glUniformMatrix3fv(NORMAL_MATRIX, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
		} else {
			if (camera) {
				if (animateCamera) {
					glm::mat3 NM = glm::mat3(view_matrix*model);
					NM = glm::transpose(glm::inverse(NM));
					glUniformMatrix3fv(NORMAL_MATRIX, 1, GL_FALSE, glm::value_ptr(NM));
				} else {
					camera->setupNormalMatrix(model, NORMAL_MATRIX);
				}
			}
		}
		int idx = objects[i].y;
		if (texture) {
			BindTextures(idx, true);
		}
		//draw model
		if (tessellate)
			models[idx]->buffer->DrawElement(0, GL_PATCHES);
		else
			models[idx]->buffer->DrawElement(0, GL_TRIANGLES);
		//unbind textures
		if (texture) {
			BindTextures(idx, false);
		}
		//animate
		if (pause) continue;
		objects[i].y++;
		if (objects[i].y >= objects[i].z) objects[i].y = objects[i].x;
	}
}

void AnimationController::BindTextures(int idx, bool bind) {
	//set material
	glUniform4f(MATERIAL, models[idx]->material.x, models[idx]->material.y, models[idx]->material.z, models[idx]->material.w);
	//set textures
	glActiveTexture(GL_TEXTURE0);
	if (bind)
		models[idx]->diffuseTexture->Bind();
	else
		models[idx]->diffuseTexture->Unbind();
	if (models[idx]->displacementTexture) {
		glActiveTexture(GL_TEXTURE1);
		if (bind)
			models[idx]->displacementTexture->Bind();
		else
			models[idx]->displacementTexture->Unbind();
	}
	if (models[idx]->normalTexture) {
		glActiveTexture(GL_TEXTURE2);
		if (bind)
			models[idx]->normalTexture->Bind();
		else
			models[idx]->normalTexture->Unbind();
	}
}

glm::mat4 AnimationController::AnimateCamera() {
	//animate camera
	glm::mat4 view_matrix;
	if (animateCamera) {
		ObjectPose pose = IntepolatePoseCubic(knots[0], poses[0], timers[0].time, true);
		view_matrix = glm::inverse(pose.GetMatrix());
	}
	return view_matrix;
}

void AnimationController::GetShadowMaps(GLCamera *camera) {
	bool paused = pause;
	pause = true;

	shadowmap->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, MAP_SIZE, MAP_SIZE);
	//glCullFace(GL_FRONT);
	programs[SHADOW_MAP]->Use();

	glm::mat4 proj = glm::ortho<float>(-2690, 2680, -1300, 1500, 1800, 6400);
	glm::mat4 view = glm::lookAt(sunPos, sunLook, glm::vec3(0, 1, 0));
	/*glm::vec4 t1 = view * glm::vec4(-1780,0,1200, 1);
	glm::vec4 t2 = view * glm::vec4(-1780,0,-2540, 1);
	glm::vec4 t3 = view * glm::vec4(1980,0,1200, 1);
	glm::vec4 t4 = view * glm::vec4(1980,0,-2540, 1);
	float x,y,z ,mx, my, mz;
	x = min(t1.x, min(t2.x, min(t3.x, t4.x)));
	mx = max(t1.x, max(t2.x, max(t3.x, t4.x)));
	y = min(t1.y, min(t2.y, min(t3.y, t4.y)));
	my = max(t1.y, max(t2.y, max(t3.y, t4.y)));
	z = min(t1.z, min(t2.z, min(t3.z, t4.z)));
	mz = max(t1.z, max(t2.z, max(t3.z, t4.z)));*/
	depthMVP = proj * view;
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view));
	DrawModels(NULL, glm::mat4(1.0), false, false, false);

	//glCullFace(GL_BACK);
	shadowmap->Unbind();
	glViewport(0, 0, camera->width, camera->height);

	glActiveTexture(GL_TEXTURE4);
	shadowmap->attachedTextures[0]->Bind();

	pause = paused;
}

void AnimationController::DrawSkybox(GLCamera *camera, glm::mat4 &view_matrix) {
	//skybox
	programs[SKYBOX]->Use();	
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
}

void AnimationController::DrawToTexture(GLCamera *camera, glm::mat4 &view_matrix) {
	bool paused = pause;
	pause = true;

	if (useSSAO) {
#pragma region SSAO buffers
		ssaoFbo->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SSAO_TEX_SIZE, SSAO_TEX_SIZE);

		programs[SSAO_FBO]->Use();
		if (animateCamera) {
			camera->getProjectionMatrix(PROJECTION_MATRIX);
			glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
		} else {
			camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
		}
		DrawModels(camera, view_matrix, false, false, true);

		ssaoFbo->Unbind();
#pragma endregion
#pragma region SSAO
		ssaoFbo2->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 ProjectionMatrix = camera->projection;//glm::ortho<float>(0.0f, SSAO_TEX_SIZE, 0.0f, SSAO_TEX_SIZE, -1.0f, 1.0f);
		glm::mat4 ProjectionMatrixINV = glm::inverse(camera->projection);
		//glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(SSAO_TEX_SIZE, SSAO_TEX_SIZE, 0));

		programs[SSAO]->Use();
		glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(PROJECTION_MATRIX_INV, 1, GL_FALSE, glm::value_ptr(ProjectionMatrixINV));
		//glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniform1f(SCREEN_WIDTH, SSAO_TEX_SIZE);
		glUniform1f(SCREEN_HEIGHT, SSAO_TEX_SIZE);

		glActiveTexture(GL_TEXTURE0);
		noiseTexture->Bind();
		glActiveTexture(GL_TEXTURE1);
		ssaoFbo->attachedTextures[0]->Bind();
		glActiveTexture(GL_TEXTURE2);
		ssaoFbo->attachedTextures[1]->Bind();

		quad->Draw(GL_QUADS);

		ssaoFbo2->Unbind();
#pragma endregion
#pragma region Blur X SSAO
		blurXFBO->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SSAO_TEX_SIZE, SSAO_TEX_SIZE);

		programs[BILATERAL]->Use();
		glUniform1f(TEXEL_SIZE, (float)1 / (float)SSAO_TEX_SIZE);
		glUniform1i(HORIZONTAL, true);

		glActiveTexture(GL_TEXTURE0);
		ssaoFbo2->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		blurXFBO->Unbind();
#pragma endregion
#pragma region Blur Y SSAO
		blurYFBO->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SSAO_TEX_SIZE, SSAO_TEX_SIZE);

		programs[BILATERAL]->Use();
		glUniform1f(TEXEL_SIZE, (float)1 / (float)SSAO_TEX_SIZE);
		glUniform1i(HORIZONTAL, false);

		glActiveTexture(GL_TEXTURE0);
		blurXFBO->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		blurYFBO->Unbind();
#pragma endregion
#pragma region Upscale SSAO
		upscaleFBO->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SCREEN_PX_WIDTH, SCREEN_PX_HEIGHT);

		programs[TEXTURED_QUAD]->Use();

		glActiveTexture(GL_TEXTURE0);
		blurYFBO->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		upscaleFBO->Unbind();
#pragma endregion
	}
	if (useToonShading) {
#pragma region edges
		edgeFbo->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, EDGE_TEX_SIZE, EDGE_TEX_SIZE);

		programs[EDGE_FBO]->Use();
		if (animateCamera) {
			camera->getProjectionMatrix(PROJECTION_MATRIX);
			glUniformMatrix4fv(VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view_matrix));
		} else {
			camera->getCameraMatrices(PROJECTION_MATRIX, VIEW_MATRIX);
		}
		DrawModels(camera, view_matrix, false, false, true);

		edgeFbo->Unbind();
		glViewport(0, 0, camera->width, camera->height);
#pragma endregion
	}

	glViewport(0, 0, camera->width, camera->height);

	pause = paused;
}

void AnimationController::MoveTimers() {
	if (pause) return;

	timers[0].Next();
	for (int i = 1; i < poses.size(); i++) {
		//anim_poses[i] = IntepolatePoseCubicDerivedRot(knots[i], poses[i], poses[i][0].quat, timers[i].time, true);
		anim_poses[i] = IntepolatePoseCubic(knots[i], poses[i], timers[i].time, true);
		timers[i].Next();
	}
}

void AnimationController::Postprocess(GLCamera *camera) {
	glm::mat4 ProjectionMatrix = glm::ortho(0.0f, camera->width, 0.0f, camera->height, -1.0f, 1.0f);
	glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(camera->width, camera->height, 0));

	if (useToonShading) {
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		DetectEdges();
		glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		edgeFbo->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	if (useSSAO) {
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		//glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		//glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
		//glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
		//glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

		programs[POST_PROCESS_AO]->Use();

		glActiveTexture(GL_TEXTURE0);
		upscaleFBO->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void AnimationController::DrawText(GLCamera *camera) {
	programs[TEXT]->Use();
	glUniform1i(SWAP, true);

	//text->RenderText(20, camera->height - 20, "ABCDabcdefghijklmnopqrstuvwxyz", camera);
	std::ostringstream strs;
	strs << setprecision(2) << fps << " fps (" << setprecision(2)  << perFrame << " ms)";

	string active_program;

	if (drawWireframe) {
		active_program = "Wireframe";
	} else if (useDispalcement) {
		if (useToonShading)
			active_program = "Toon + All";
		else
			active_program = "Phong + All";;
	} else {
		if (useToonShading)
			active_program = "Toon";
		else if (useNormals)
			active_program = "Phong + Normals";
		else
			active_program = "Phong";
	}
	if (useSSAO)
		active_program += " + SSAO";

	glm::mat4 proj = glm::ortho(0.0f, camera->width, 0.0f, camera->height, -1.0f, 1.0f);
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj));

	int offset = 20;
	text->RenderText(20, camera->height - offset, strs.str());
	offset += 20;

	text->RenderText(20, camera->height - offset, active_program);
	offset += 20;

	if (!drawText) return;

	text->RenderText(20, camera->height - offset, "P = pause animation");
	offset += 20;

	text->RenderText(20, camera->height - offset, "T = toon shading");
	offset += 20;

	text->RenderText(20, camera->height - offset, "W = wireframe");
	offset += 20;

	text->RenderText(20, camera->height - offset, "Q/E = tessellate more/less");
	offset += 20;
}

void AnimationController::DrawDebug(GLCamera *camera) {
	if (!drawDebug) return;

	programs[TEXT]->Use();
	glUniform1i(SWAP, false);

	glm::mat4 proj = glm::ortho(0.0f, camera->width, 0.0f, camera->height, -1.0f, 1.0f);
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj));
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(5, 5, 0));
	ModelMatrix = ModelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(DEBUG_SIZE, DEBUG_SIZE, 0));
	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glActiveTexture(GL_TEXTURE0);
	shadowmap->attachedTextures[0]->Bind();

	quad->Draw(GL_QUADS);

	if (useSSAO) {
		ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		ssaoFbo->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		ssaoFbo2->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);

		ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
		glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		blurYFBO->attachedTextures[0]->Bind();

		quad->Draw(GL_QUADS);
	}

	/*programs[SSAO]->Use();

	glm::mat4 proj_inv = glm::inverse(camera->projection);
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj_inv));
	glUniformMatrix4fv(PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(proj));
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform1f(SCREEN_WIDTH, DEBUG_SIZE);
	glUniform1f(SCREEN_HEIGHT, DEBUG_SIZE);

	glActiveTexture(GL_TEXTURE0);
	noiseTexture->Bind();
	glActiveTexture(GL_TEXTURE1);
	ssaoFbo->attachedTextures[0]->Bind();
	glActiveTexture(GL_TEXTURE2);
	ssaoFbo->attachedTextures[1]->Bind();

	quad->Draw(GL_QUADS);*/

	/*ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glActiveTexture(GL_TEXTURE0);
	fbo->attachedTextures[1]->Bind();

	quad->Draw(GL_QUADS);

	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(DEBUG_SIZE + 5, 0, 0)) * ModelMatrix;
	glUniformMatrix4fv(MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glActiveTexture(GL_TEXTURE0);
	shadowmap->attachedTextures[0]->Bind();

	quad->Draw(GL_QUADS);*/
}

#pragma endregion

#pragma region Setters

void AnimationController::TessellateMore() {
	PixelsPerEdge = max(PixelsPerEdge - 50.0f, 50.0f);
}

void AnimationController::TessellateLess() {
	PixelsPerEdge = min(PixelsPerEdge + 50.0f, 1080.0f);
}

#pragma endregion