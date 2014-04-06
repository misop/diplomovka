#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\custom\aditions.h>
#include <vector>

using namespace std;

#define DELTA_TIME 0.02

struct LightAnim
{
	glm::vec4 pos;
	glm::vec3 color;
	glm::vec4 attenuation;
	int anim;
	LightAnim() : pos(glm::vec4(0)), color(glm::vec3(0)), attenuation(glm::vec4(0)), anim(-1) { };
	LightAnim(glm::vec3 _pos, glm::vec3 _col, glm::vec4 _att, int _anim) : pos(glm::vec4(_pos, 1.0)), color(_col), attenuation(glm::vec4(_att)), anim(_anim) { };
};

struct AnimTimer
{
	float time;
	float maxTime;
	AnimTimer() : time(0), maxTime(0) { };
	AnimTimer(float _maxTime) : time(0), maxTime(_maxTime) { };

	void Next() {
		time += DELTA_TIME;
		if (time >= maxTime) time = 0;
	};
};

struct ObjectPose
{
	glm::vec3 pos;
	glm::quat quat;
	glm::vec3 scale;

	ObjectPose() : pos(glm::vec3(0)), quat(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1)) { };
	ObjectPose(glm::vec3 &_pos, glm::vec3 &_euler, glm::vec3 &_scale) : pos(_pos), scale(_scale) {
		glm::vec3 radEuler(glm::radians(_euler.x), glm::radians(_euler.y), glm::radians(_euler.z));
		quat = glm::quat(radEuler);
	};
	glm::mat4 GetMatrix() {
		//translation matrix
		glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
		//scale matrix
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
		//rotation matrix
		glm::mat4 R = glm::mat4_cast(quat);

		return (T * R * S);
	};
	glm::mat4 GetInverseMatrix() {
		//translation matrix
		glm::mat4 T = glm::translate(glm::mat4(1.0), -pos);
		//scale matrix
		glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(1.0/scale.x, 1.0/scale.y, 1.0/scale.z));
		//rotation matrix
		glm::mat4 R = glm::transpose(glm::mat4_cast(quat));

		return (S * R * T);
	};
};

//pose for cameras look at
ObjectPose CreateLookAtPose(glm::vec3 &from, glm::vec3 &to, glm::vec3 &up);

//find time interval
bool FindInterval(std::vector<float> &knots, float time, int &span);

// preinterpoluj zadane hodnoty v danom case, pouzije sa linearna interpolacia
glm::vec3 IntepolateVectorsLinear(std::vector<float> &knots, std::vector<glm::vec3> &values, float time, bool closed);

// preinterpoluj zadane hodnoty v danom case, pouzije sa kubicka interpolacia
glm::vec3 IntepolateVectorsCubic(std::vector<float> &knots, std::vector<glm::vec3> &values, float time, bool closed);

// preinterpoluj zadane kvaterniony v danom case, pouzije sa linearna sfericka interpolacia
glm::quat IntepolateQuaternionsLinear(std::vector<float> &knots, std::vector<glm::quat> &quaternions, float time, bool closed);

// preinterpoluj zadane kvaterniony v danom case, pouzije sa kubicka sfericka interpolacia
glm::quat DoubleArc(glm::quat &Q1, glm::quat &Q2);
glm::quat BisectArc(glm::quat &Q1, glm::quat &Q2);
glm::quat IntepolateQuaternionsCubic(std::vector<float> &knots, std::vector<glm::quat> &quaternions, float time, bool closed);

// preinterpoluj zadane pozy v danom case, pouzije sa linearna interpolacia
ObjectPose IntepolatePoseLinear(std::vector<float> &knots, std::vector<ObjectPose> &poses, float time, bool closed);

// preinterpoluj zadane pozy v danom case, pouzije sa kubicka interpolacia
ObjectPose IntepolatePoseCubic(std::vector<float> &knots, std::vector<ObjectPose> &poses, float time, bool closed);

// preinterpoluj zadane pozy v danom case, pouzije sa kubicka interpolacia
ObjectPose IntepolatePoseCubicDerivedRot(std::vector<float> &knots, std::vector<ObjectPose> &poses, glm::quat base, float time, bool closed);