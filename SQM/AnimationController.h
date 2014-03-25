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
#include "GLText.h"
#include "GLFrameBuffer.h"

class AnimationController
{
	glm::vec3 sun;
	GLTexture *toonShadingTexture;
	GLTexture *noiseTexture;
	GLArrayBuffer *skybox;
	GLText *text;
	GLFrameBuffer *ssaoFbo;
	GLFrameBuffer *ssaoFbo2;
	GLFrameBuffer *upscaleFBO;
	GLFrameBuffer *blurXFBO;
	GLFrameBuffer *blurYFBO;
	GLFrameBuffer *edgeFbo;
	GLFrameBuffer *shadowmap;
	GLArrayBuffer *quad;
	glm::mat4 skyboxModel;
	glm::mat4 depthMVP;
	glm::mat4 biasMatrix;
	GLTexture *skyboxTexture;
	vector<GLProgram *> programs;
	vector<OpenGLShaders *> shaders;
	vector<AssimpObject *> models;
	vector<glm::ivec3> objects;
	vector<glm::mat4> modelMatrices;
	vector<vector<ObjectPose> > poses;
	vector<ObjectPose> anim_poses;
	vector<vector<float> > knots;
	vector<AnimTimer> timers;
	vector<int> movement;
	bool animateCamera;
	bool canDraw;
	unsigned long time;
	int frames;
	double fps;
	double perFrame;
public:
	bool drawWireframe;
	bool useSSAO;
	bool useDispalcement;
	bool useNormals;
	bool pause;
	bool useToonShading;
	bool drawText;
	float PixelsPerEdge;
	bool drawDebug;

	AnimationController(void);
	~AnimationController(void);

	void LoadScene();
	void LoadModel(string fileName, string texturesFileName);
	void LoadAnimatedModel(int files, string fileName, string texturesFileName);
	void LoadCameraMovement(string fileName);
	void LoadMovement(string fileName);
	void LoadModelMatrix(string fileName);
	void LoadModelMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 euler);

	void ClearShaders();
	void InitShaders();
	void InitSkybox();
	void InitToonShadingTexture();
	void InitFont();
	void InitFBOs();
	
	void Draw(GLCamera *camera);
	glm::mat4 AnimateCamera();
	void GetShadowMaps(GLCamera *camera);
	void DrawSkybox(GLCamera *camera, glm::mat4 &view_matrix);
	void MoveTimers();
	void DrawModels(GLCamera *camera, glm::mat4 &view_matrix, bool texture = true, bool tessellate = true, bool ssao = false, bool screen = false);
	void BindTextures(int idx, bool bind);
	void DrawText(GLCamera *camera);
	void DrawToTexture(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawDebug(GLCamera *camera);
	void DrawWireframe(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawPhong(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawPhongNormal(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawPhongAll(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawToon(GLCamera *camera, glm::mat4 &view_matrix);
	void DrawToonAll(GLCamera *camera, glm::mat4 &view_matrix);
	void DetectEdges();
	void Smooth(bool horizontal);
	void Postprocess(GLCamera *camera);
	
	void TessellateMore();
	void TessellateLess();
};