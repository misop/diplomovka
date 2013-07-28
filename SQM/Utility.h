#pragma once
#include <string>
#include <GL\glew.h>
#include <GL\wglew.h>
#include "GLProgram.h"
#include "GLShader.h"
#include <glm.hpp>
#include "MyMesh.h"
#include "FloatArithmetic.h"

struct OpenGLPrograms {
	GLProgram *SklLines;
	GLProgram *SklNodes;
	GLProgram *BNPs;
	GLProgram *TriMeshTess;
	GLProgram *QuadMeshTess;

	OpenGLPrograms() : SklLines(NULL), SklNodes(NULL), BNPs(NULL), TriMeshTess(NULL), QuadMeshTess(NULL) { }
	~OpenGLPrograms() {
		if (SklLines) delete SklLines;
		if (SklNodes) delete SklNodes;
		if (BNPs) delete BNPs;
		if (TriMeshTess) delete TriMeshTess;
		if (QuadMeshTess) delete QuadMeshTess;
	}
};

struct OpenGLShaders {
	GLShader *vert;
	GLShader *ctrl;
	GLShader *eval;
	GLShader *geom;
	GLShader *frag;

	OpenGLShaders() : vert(NULL), ctrl(NULL), eval(NULL), geom(NULL), frag(NULL) { }
	~OpenGLShaders() {
		if (vert) delete vert;
		if (ctrl) delete ctrl;
		if (eval) delete eval;
		if (geom) delete geom;
		if (frag) delete frag;
	}
};

#pragma region Template Functions
template <typename T> std::string ToString(T& a);
#pragma endregion

#pragma region Ray Intersections
bool raySphereIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f sphere_center, float sphere_radius, float &t_param);
bool raySphereIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, glm::vec3 sphere_center, float sphere_radius, float &t_param);
bool rayTriangleIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f V0, OpenMesh::Vec3f V1, OpenMesh::Vec3f V2, float &t_param);
#pragma endregion