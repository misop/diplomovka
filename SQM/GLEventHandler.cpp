#include "stdafx.h"
#include "GLEventHandler.h"
#include <math.h>
#include <gtc\type_ptr.hpp>


GLEventHandler::GLEventHandler(GLCamera *newGlCamera, SQMControler *newSqmControler)
{
	glCamera = newGlCamera;
	sqmControler = newSqmControler;
	animControler = NULL;
	state = NodeEditState;
}

GLEventHandler::GLEventHandler(GLCamera *newGlCamera, SQMControler *newSqmControler, AnimationController *newAnimControler) {
	glCamera = newGlCamera;
	sqmControler = newSqmControler;
	animControler = newAnimControler;
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
	if (state == NodeRotateState) {
		GLdouble x = 0, y = 0, z = 0;
		//bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		glm::vec3 pos(x, y, z);
		glm::vec3 dir = glm::normalize(pos - glCamera->getEye());

		sqmControler->selectNodeInRay(glCamera->getEye(), dir);
		if (sqmControler->selected != NULL) {
			selectedX = positionX;
			selectedY = positionY;
		}
	}
	if ((state == NodeEditState || state == NodeEditDescendantsState) && mouse == LEFT_MOUSE_DOWN) {
		GLdouble x = 0, y = 0, z = 0;
		//bool sucess = mousePositionTo3D(positionX, positionY, x, y, z);
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		glm::vec3 pos(x, y, z);
		glm::vec3 dir = glm::normalize(pos - glCamera->getEye());

		sqmControler->selectNodeInRay(glCamera->getEye(), dir);
		if (sqmControler->selected != NULL) {
			selectedX = positionX;
			selectedY = positionY;
		}
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
	if (state == NodeEditState && mouse == RIGHT_MOUSE_DOWN && sqmControler->getSelected() != NULL) {
		GLdouble x = 0, y = 0, z = 0;
		glCamera->mousePositionTo3D(positionX, positionY, x, y, z);
		glm::vec3 pos(x, y, z);
		glm::vec3 dir = glm::normalize(pos - glCamera->getEye());
		SQMNode *inRay = sqmControler->getNodeInRay(pos, dir);
		SQMNode *selected = sqmControler->getSelected();

		//selected->getParent()->removeDescendant(selected);
		if (inRay != NULL && inRay != selected && selected->isValidCycleNode()) {
			sqmControler->createCycle(selected, inRay);
		}
	}
}

void GLEventHandler::mouseMoved(int positionX, int positionY) {
	if (mouse == 0)// || mouse == MIDDLE_MOUSE_DOWN)
		return;

	if (state == CameraMoveState) {
		mouseMovedForCamera(positionX, positionY);
	}
	if (state == NodeEditState || state == NodeEditDescendantsState) {
		mouseMovedForNodeEdit(positionX, positionY);
	}
	if (state == NodeRotateState) {
		mouseMovedForNodeRotate(positionX, positionY);
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
		if (state == NodeEditState || state == NodeEditDescendantsState) {
			moveHorizontal(-dx*0.7);
			moveVertical(-dy*0.7);
		}
		sqmControler->drawSkeleton();
	}
	lastX = positionX;
	lastY = positionY;
}


void GLEventHandler::mouseMovedForNodeRotate(int positionX, int positionY) {
	int dx = positionX - lastX;
	int dy = positionY - lastY;
	if (sqmControler->getSelected() == NULL) {
		mouseMovedForCamera(positionX, positionY);
		return;
	}
	if (sqmControler->getSelected() != NULL) {
		if (state == NodeRotateState) {
			float delta = (float)dx*0.1;
			CVector3 axis(1, 0, 0);
			//we should rotate dy arround right camera axis and dx arround up camera axis
			if (mouse == LEFT_MOUSE_DOWN) {
				axis = CVector3(0, 1, 0);
			}
			if (mouse == MIDDLE_MOUSE_DOWN) {
				axis = CVector3(0, 0, 1);
			}
			if (mouse == RIGHT_MOUSE_DOWN) {
				axis = CVector3(1, 0, 0);
			}
			rotate(delta, axis);
			sqmControler->drawSkeleton();
		}
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
	if (state == NodeEditState) sqmControler->setSelectedPosition(pos);
	if (state == NodeEditDescendantsState) sqmControler->setSelectedAndDescendantsPosition(pos);
}

void GLEventHandler::moveVertical(float dist) {
	SQMNode *node= sqmControler->getSelected();
	glm::vec3 pos = node->getPosition_glm();
	pos = pos + glCamera->getUp()*dist;
	if (state == NodeEditState) sqmControler->setSelectedPosition(pos);
	if (state == NodeEditDescendantsState) sqmControler->setSelectedAndDescendantsPosition(pos);
}

void GLEventHandler::rotate(float delta, CVector3 axis) {
	SQMNode *node= sqmControler->getSelected();
	CVector4 q = QuaternionFromAngleAxis(delta, axis);
	node->rotateDescendants(q);
}

#pragma endregion


#pragma region Keys

void GLEventHandler::KeyDown(char c) {
	if (!animControler) return;
	switch (c) {
	case 'W':
		//alterWireframe
		animControler->drawWireframe = !animControler->drawWireframe;
		break;
	case 'T':
		//alter toon
		animControler->useToonShading = !animControler->useToonShading;
		break;
	case 'P':
		//pause animation
		animControler->pause = !animControler->pause;
		break;
	case 'O':
		//pause animation
		animControler->drawText = !animControler->drawText;
		break;
	case 'Q':// 'k' = '+'
		//tessellate more
		animControler->TessellateMore();
		break;
	case 'E':// 'm' = '-'
		//tessellate less
		animControler->TessellateLess();
		break;
	case 'A':
		//alter dispalcement
		animControler->useDispalcement = !animControler->useDispalcement;
		break;
	case 'N':
		//alter dispalcement
		animControler->useNormals = !animControler->useNormals;
		break;
	case 'D':
		//alter dispalcement
		animControler->drawDebug = !animControler->drawDebug;
		break;
	case 'S':
		//alter SSAO
		animControler->useSSAO = !animControler->useSSAO;
		break;
	default:
		break;
	}
}

#pragma endregion