#include "stdafx.h"
#include "GLCamera.h"
#include <math.h>
#include "m_math.h"

#define X 0
#define Y 1
#define Z 2

GLCamera::GLCamera(void) {
	/*fi = 0;
	theta = 90;
	dist = 700;

	eye[X] = 0;
	eye[Y] = 0;
	eye[Z] = 700;

	look[X] = 0;
	look[Y] = 0;
	look[Z] = 0;

	up[X] = 0;
	up[Y] = 1;
	up[Z] = 0;

	right[X] = 1;
	right[Y] = 0;
	right[Z] = 0;*/
	reset();
	dir = OpenMesh::Vec3f(0, 0, 0);
	pos = OpenMesh::Vec3f(0, 0, 0);
}

void GLCamera::setFi(float newFi) {
	fi = newFi;
	if (fi >= 360.0) fi -= 360.0;
	if (fi < 0) fi += 360.0;

	update();
}

void GLCamera::setTheta(float newTheta) {
	theta = newTheta;
	if (theta >= 179) theta = 179;
	if (theta <= 1) theta = 1;

	update();
}

void GLCamera::setDist(float newDist) {
	dist = newDist;
	if (dist < 0)
		dist = 0.1;

	update();
}

void GLCamera::strafeHorizontal(float strafe) {
	look[X] += right[X]*strafe;
	look[Y] += right[Y]*strafe;
	look[Z] += right[Z]*strafe;

	update();
}

void GLCamera::strafeVertical(float strafe) {
	look[X] += up[X]*strafe;
	look[Y] += up[Y]*strafe;
	look[Z] += up[Z]*strafe;

	update();
}

void GLCamera::update() {
	up[X] = 0; up[Y] = 1; up[Z] = 0;

	eye[X] = look[X] + sin(M_PI / 180 * theta) * sin(M_PI / 180 * fi) * dist;
	eye[Y] = look[Y] + cos(M_PI / 180 * theta) * dist;
	eye[Z] = look[Z] + sin(M_PI / 180 * theta) * cos(M_PI / 180 * fi) * dist;

	CVector3 eyeVector(eye);
	CVector3 lookVector(look);
	CVector3 normal = Normalize(lookVector - eyeVector);
	CVector3 upVector(up);
	//upVector = upVector - normal*(Dot(upVector, normal));
	//upVector = Normalize(upVector);
	CVector3 rightVector = Normalize(Cross(upVector, normal));
	//upVector = Normalize(Cross(rightVector, normal));
	upVector = Normalize(Cross(normal, rightVector));

	//up[X] = upVector.x; up[Y] = upVector.y; up[Z] = upVector.z;
	right[X] = rightVector.x; right[Y] = rightVector.y; right[Z] = rightVector.z;

	calculateMatrices();
}

void GLCamera::calculateMatrices() {
	glPushMatrix();

	glLoadIdentity();
	gluLookAt(eye[X], eye[Y], eye[Z], look[X], look[Y], look[Z], up[X], up[Y], up[Z]);

	//get matrices
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	glPopMatrix();
}

void GLCamera::lookFromCamera() {
	glLoadIdentity();
	gluLookAt(eye[X], eye[Y], eye[Z], look[X], look[Y], look[Z], up[X], up[Y], up[Z]);

	//show camera position
	glColor3f(0.9, 0.45, 0);
	glPushMatrix();
	glTranslatef(look[X], look[Y],look[Z]);
	gluSphere(gluNewQuadric(), 2, 10, 10);
	glPopMatrix();
}

void GLCamera::reset() {
	fi = 0;
	theta = 90;
	dist = 700;

	eye[X] = 0;
	eye[Y] = 0;
	eye[Z] = 700;

	look[X] = 0;
	look[Y] = 0;
	look[Z] = 0;

	up[X] = 0;
	up[Y] = 1;
	up[Z] = 0;

	right[X] = 1;
	right[Y] = 0;
	right[Z] = 0;

	calculateMatrices();
}

GLCamera::~GLCamera(void)
{
}

OpenMesh::Vec3f GLCamera::getRight() {
	return OpenMesh::Vec3f(right);
}

OpenMesh::Vec3f GLCamera::getUp() {
	return OpenMesh::Vec3f(up);
}

OpenMesh::Vec3f GLCamera::getView() {
	OpenMesh::Vec3f lookV(look);
	OpenMesh::Vec3f eyeV(eye);
	return (eyeV - lookV).normalize();
}

OpenMesh::Vec3f GLCamera::getEye() {
	return OpenMesh::Vec3f(eye);
}
