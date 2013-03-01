#include "stdafx.h"
#include "GLEventHandler.h"


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
	if (state == NodeEditState) {
		//select node
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