#include "stdafx.h"
#include "GLEventHandler.h"
#include <math.h>


GLEventHandler::GLEventHandler(GLCamera *newGlCamera, SQMControler *newSqmControler)
{
	glCamera = newGlCamera;
	sqmControler = newSqmControler;
	state = NodeEditState;
}

GLEventHandler::~GLEventHandler(void)
{
}

#pragma region General Event Handling

void GLEventHandler::mouseDown(int positionX, int positionY, int mouseFlags) {
	mouse = mouseFlags;
	lastX = positionX;
	lastY = positionY;
	if (state == NodeEditState && mouse == LEFT_MOUSE_DOWN) {
		GLdouble x = 0, y = 0, z = 0;
		bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		OpenMesh::Vec3f pos(x, y, z);
		OpenMesh::Vec3f dir = (pos - glCamera->getEye()).normalize();

		sqmControler->selectNodeInRay(glCamera->getEye(), dir);
	}
	if (state == NodeEditState && mouse == MIDDLE_MOUSE_DOWN && sqmControler->selected != NULL) {
		GLdouble x = 0, y = 0, z = 0;
		bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		SkeletonNode *skeletonNode = new SkeletonNode(x, y, z);
		SQMNode *node = new SQMNode(*skeletonNode, sqmControler->selected);
		sqmControler->selected->addDescendant(node);
		delete skeletonNode;

		mouse = 0;
	}
}

void GLEventHandler::mouseMoved(int positionX, int positionY) {
	if (mouse == 0 || mouse == MIDDLE_MOUSE_DOWN)
		return;

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
	int dx = positionX - lastX;
	int dy = positionY - lastY;
	if (sqmControler->selected == NULL || mouse == RIGHT_MOUSE_DOWN) {
		mouseMovedForCamera(positionX, positionY);
		return;
	}
	if (sqmControler->selected != NULL) {
		moveHorizontal(-dx*0.7);
		moveVertical(-dy*0.7);
	}
	lastX = positionX;
	lastY = positionY;
}

#pragma endregion

#pragma region Selected Node Editing

void GLEventHandler::moveHorizontal(float dist) {
	SQMNode *node= sqmControler->selected;
	OpenMesh::Vec3f pos = node->getPosition();
	pos = pos + glCamera->getRight()*dist;
	node->setPosition(pos);
}

void GLEventHandler::moveVertical(float dist) {
	SQMNode *node= sqmControler->selected;
	OpenMesh::Vec3f pos = node->getPosition();
	pos = pos + glCamera->getUp()*dist;
	node->setPosition(pos);
}

#pragma endregion

#pragma region Utility

bool GLEventHandler::mousePositionTo3D(int x_cursor, int y_cursor, GLdouble &x, GLdouble &y, GLdouble &z) {
	GLfloat winX, winY;
	// obtain the Z position (not world coordinates but in range 0 - 1)
	GLfloat z_cursor = 0;
	winX = (float)x_cursor;
	winY = (float)glCamera->viewport[3] - (float)y_cursor;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);
	//when clicking out of object
	z_cursor = 0.998555;
	// obtain the world coordinates
	GLint sucess = gluUnProject(winX, winY, z_cursor, glCamera->modelview, glCamera->projection, glCamera->viewport, &x, &y, &z);

	return sucess == GLU_TRUE;
}

#pragma endregion