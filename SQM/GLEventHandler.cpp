#include "stdafx.h"
#include "GLEventHandler.h"
#include <math.h>
#include <gtc\type_ptr.hpp>


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
		//bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		glm::vec3 pos(x, y, z);
		glm::vec3 dir = glm::normalize(pos - glCamera->getEye());

		sqmControler->selectNodeInRay(glCamera->getEye(), dir);
	}
	if (state == NodeEditState && mouse == MIDDLE_MOUSE_DOWN && sqmControler->getSelected() != NULL) {
		GLdouble x = 0, y = 0, z = 0;
		//bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		//if (sucess) {
		//find plane ray intersection
		glm::vec3 eye = glCamera->getEye();
		CVector3 rayOrigin(eye.x, eye.y, eye.z);
		CVector3 planeOrigin(glCamera->look);
		CVector3 direction = Normalize(CVector3(x, y, z) - rayOrigin);
		CVector3 normal = Normalize(rayOrigin - planeOrigin);
		CVector3 P = PlaneRayIntersection(rayOrigin, direction, planeOrigin, normal);
		sqmControler->insertNode(P.x, P.y, P.z);
		//}

		mouse = 0;
	}
	/*if (state == NodeEditState && mouse == RIGHT_MOUSE_DOWN && sqmControler->getSelected() != NULL) {
		GLdouble x = 0, y = 0, z = 0;
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		glm::vec3 pos(x, y, z);
		glm::vec3 dir = glm::normalize(pos - glCamera->getEye());
		SQMNode *inRay = sqmControler->getNodeInRay(pos, dir);
		SQMNode *selected = sqmControler->getSelected();

		if (inRay != NULL && inRay != selected && selected->isValidCycleNode()) {
			sqmControler->createCycle(selected, inRay);
		}
	}*/
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
	if (sqmControler->getSelected() == NULL || mouse == RIGHT_MOUSE_DOWN) {
		mouseMovedForCamera(positionX, positionY);
		return;
	}
	if (sqmControler->getSelected() != NULL) {
		moveHorizontal(-dx*0.7);
		moveVertical(-dy*0.7);
	}
	lastX = positionX;
	lastY = positionY;
}

#pragma endregion

#pragma region Selected Node Editing

void GLEventHandler::moveHorizontal(float dist) {
	SQMNode *node= sqmControler->getSelected();
	glm::vec3 pos = node->getPosition_glm();
	pos = pos + glCamera->getRight()*dist;
	sqmControler->setSelectedPosition(pos);
}

void GLEventHandler::moveVertical(float dist) {
	SQMNode *node= sqmControler->getSelected();
	glm::vec3 pos = node->getPosition_glm();
	pos = pos + glCamera->getUp()*dist;
	sqmControler->setSelectedPosition(pos);
}

#pragma endregion