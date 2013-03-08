#include "stdafx.h"
#include "GLEventHandler.h"
#include <math.h>


GLEventHandler::GLEventHandler(GLCamera *newGlCamera, SQMControler *newSqmControler)
{
	glCamera = newGlCamera;
	sqmControler = newSqmControler;
	state = CameraMoveState;
}


GLEventHandler::~GLEventHandler(void)
{
}

#pragma region General Event Handling

void GLEventHandler::mouseDown(int positionX, int positionY, int mouseFlags) {
	mouse = mouseFlags;
	lastX = positionX;
	lastY = positionY;
	if (state == NodeEditState && (mouse == LEFT_MOUSE_DOWN)) {
		//select node
		/*float x = lastX;
		float y = glCamera->height - lastY;
		OpenMesh::Vec3f camera = glCamera->getEye();
		OpenMesh::Vec3f view = glCamera->getView();
		OpenMesh::Vec3f v = glCamera->getUp();
		OpenMesh::Vec3f h = glCamera->getRight();
		float rad = glCamera->fovy * M_PI / 180.0;
		float vLength = tan(rad / 2.0);
		float hLength = vLength * glCamera->aspect;
		v = v * vLength;
		h = h * hLength;

		x -= glCamera->width / 2.0;
		y -= glCamera->height / 2.0;
		//x /= glCamera->width / 2.0;
		//y /= glCamera->height / 2.0;
		float z = glCamera->height / (2.0 * tan(rad / 2.0));

		OpenMesh::Vec3f pos = h*x + v*y - z*view;
		//pos[0] *= glCamera->width;
		//pos[1] *= glCamera->height;
		OpenMesh::Vec3f dir = (pos - camera).normalize();

		//glCamera->pos = glCamera->getEye();
		//glCamera->dir = dir;
		OpenMesh::Vec3f ppp(-50, 175, 0);
		OpenMesh::Vec3f oldDir = dir;
		oldDir = (ppp - camera).normalize();
		sqmControler->selectNodeInRay(camera, dir);*/

		float x_cursor = lastX;
		float y_cursor = lastY;
		GLfloat winX, winY;
		/*GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);*/

		// obtain the Z position (not world coordinates but in range 0 - 1)
		GLfloat z_cursor = 0;
		winX = (float)x_cursor;
		winY = (float)glCamera->viewport[3] - (float)y_cursor;
		glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);

		// obtain the world coordinates
		GLdouble x, y, z;
		gluUnProject(winX, winY, z_cursor, glCamera->modelview, glCamera->projection, glCamera->viewport, &x, &y, &z);
		OpenMesh::Vec3f pos(x, y, z);
		OpenMesh::Vec3f dir = (pos - glCamera->getEye()).normalize();
		pos = glCamera->getEye();
		
		bool found = sqmControler->selectNodeInRay(pos, dir);

		if (found) {
			mouse = 0;
		}
	}
}

void GLEventHandler::mouseMoved(int positionX, int positionY) {
	if (state == CameraMoveState) {
		mouseMovedForCamera(positionX, positionY);
	}
	if (state == NodeEditState) {
		mouseMovedForNodeEdit(positionX, positionY);
	}
}

void GLEventHandler::mouseUp(int positionX, int positionY) {
	//cleanup
	mouse = 0;
	if (state == NodeEditState) {
		//finish editing in some way
		//sqmControler->selected = NULL;
	}
}

#pragma endregion

#pragma region Camera Event Handling

void GLEventHandler::mouseMovedForCamera(int positionX, int positionY) {
	int dx = positionX - lastX;
	int dy = positionY - lastY;
	if (mouse == RIGHT_MOUSE_DOWN) {
		glCamera->setFi(glCamera->fi - dx*0.3);
		glCamera->setTheta(glCamera->theta - dy*0.3);
	}
	if (mouse == LEFT_MOUSE_DOWN) {
		glCamera->strafeHorizontal(dx*0.3);
		glCamera->strafeVertical(dy*0.3);
	}
	lastX = positionX;
	lastY = positionY;
}

#pragma endregion

#pragma region Node Edit Event Handling

void GLEventHandler::mouseMovedForNodeEdit(int positionX, int positionY) {
	if (sqmControler->selected == NULL) {
		mouseMovedForCamera(positionX, positionY);
		return;
	}
	//todo node movement
}

#pragma endregion