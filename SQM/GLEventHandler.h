#pragma once
#include "GLCamera.h"
#include "SQMControler.h"

#define LEFT_MOUSE_DOWN 1
#define RIGHT_MOUSE_DOWN 2

typedef enum {
	CameraMoveState = 0,
	NodeEditState
} EventState;

class GLEventHandler
{
	GLCamera *glCamera;
	SQMControler *sqmControler;
	int mouse;
	int lastX;
	int lastY;
public:
	EventState state;

	GLEventHandler(GLCamera *newGlCamera, SQMControler *newSqmControler);
	~GLEventHandler(void);

#pragma region General Event Handling
	void mouseDown(int positionX, int positionY, int mouseFlags);
	void mouseMoved(int positionX, int positionY);
	void mouseUp(int positionX, int positionY);
#pragma endregion
	
#pragma region Camera Event Handling
	void mouseMovedForCamera(int positionX, int positionY);
#pragma endregion

#pragma region Node Edit Event Handling
	void mouseMovedForNodeEdit(int positionX, int positionY);
#pragma endregion
};
