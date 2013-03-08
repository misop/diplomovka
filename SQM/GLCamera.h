#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>

class GLCamera
{
public:
	OpenMesh::Vec3f pos;
	OpenMesh::Vec3f dir;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	float fovy;
	float aspect;
	float width;
	float height;

	float theta;
	float fi;
	float dist;

	float eye[3];
	float look[3];
	float up[3];
	float right[3];

	void setFi(float newFi);
	void setTheta(float newTheta);
	void setDist(float newDist);

	void strafeHorizontal(float strafe);
	void strafeVertical(float strafe);

	void update();
	void calculateMatrices();

	void lookFromCamera();
	void reset();

	OpenMesh::Vec3f getRight();
	OpenMesh::Vec3f getUp();
	OpenMesh::Vec3f getView();
	OpenMesh::Vec3f getEye();

	GLCamera(void);
	~GLCamera(void);
};

