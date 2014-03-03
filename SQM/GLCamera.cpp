#include "stdafx.h"
#include "GLCamera.h"
#include <math.h>
#include <gtc\type_ptr.hpp>
#include "m_math.h"

#define X 0
#define Y 1
#define Z 2

GLCamera::GLCamera(void) {
	color[0] = 0.9; color[1] = 0.45; color[2] = 0.0;
	width = 1;
	height = 1;
	viewport = glm::vec4(0, 0, width, height);
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
	if (dist < 0.5)
		dist = 0.5;

	update();
}

void GLCamera::setWidth(float newWidth) {
	width = newWidth;
	viewport.z = width;
}

void GLCamera::setHeight(float newHeight) {
	height = newHeight;
	viewport.w = height;
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
	CVector3 rightVector = Normalize(Cross(upVector, normal));
	upVector = Normalize(Cross(normal, rightVector));

	right[X] = rightVector.x; right[Y] = rightVector.y; right[Z] = rightVector.z;

	calculateMatrices();
}

void GLCamera::calculateMatrices() {
	glm::vec3 eyev(eye[X], eye[Y], eye[Z]);
	glm::vec3 lookv(look[X], look[Y], look[Z]);
	glm::vec3 upv(up[X], up[Y], up[Z]);
	view = glm::lookAt(eyev, lookv, upv);
	//normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelview)));
}

void GLCamera::lookFromCamera(GLint mvpLoc) {
	glm::mat4 MVPmatrix = projection*view;
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVPmatrix));
}

void GLCamera::getCameraMatrices(GLint pLoc, GLint vLoc) {
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void GLCamera::setupModelViewMatrix(GLint mvLoc) {
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void GLCamera::setupNormalMatrix(glm::mat4 &model, GLint nmLoc) {
	glm::mat3 NM = glm::mat3(view*model);
	NM = glm::transpose(glm::inverse(NM));
	glUniformMatrix3fv(nmLoc, 1, GL_FALSE, glm::value_ptr(NM));
}

void GLCamera::setupNormalMatrix(GLint nmLoc) {
	glUniformMatrix3fv(nmLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

glm::mat4 GLCamera::cameraModelMatrix() {
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(look[X], look[Y], look[Z]));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0));

	return modelMatrix;
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

	right[X] = -1;
	right[Y] = 0;
	right[Z] = 0;

	calculateMatrices();
}

GLCamera::~GLCamera(void)
{
}

glm::vec3 GLCamera::getRight() {
	return glm::vec3(right[X], right[Y], right[Z]);
}

glm::vec3 GLCamera::getUp() {
	return glm::vec3(up[X], up[Y], up[Z]);
}

glm::vec3 GLCamera::getView() {
	glm::vec3 lookV(look[X], look[Y], look[Z]);
	glm::vec3 eyeV(eye[X], eye[Y], eye[Z]);
	return glm::normalize(eyeV - lookV);
}

glm::vec3 GLCamera::getEye() {
	return glm::vec3(eye[X], eye[Y], eye[Z]);
}

void GLCamera::mousePositionTo3D(int x_cursor, int y_cursor, GLdouble &x, GLdouble &y, GLdouble &z) {
	GLfloat winX, winY, winZ = 0;
	// obtain the Z position (not world coordinates but in range 0 - 1)
	winX = (float)x_cursor;
	winY = (float)viewport[3] - (float)y_cursor;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	//when clicking out of object
	winZ = 0;//0.998555;
	// obtain the world coordinates
	//GLint sucess = gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
	glm::vec3 window(winX, winY, winZ);
	glm::vec3 coor = glm::unProject(window, view, projection, viewport);
	x = coor.x;
	y = coor.y;
	z = coor.z;
}
