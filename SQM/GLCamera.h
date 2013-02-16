#pragma once

class GLCamera
{
public:
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

	void lookFromCamera();

	GLCamera(void);
	~GLCamera(void);
};

