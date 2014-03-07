#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

namespace glm {
	bool operator==(const glm::vec3 &vecA, const glm::vec3 &vecB);
	bool operator!=(const glm::vec3 &vecA, const glm::vec3 &vecB);

	bool IsZeroLength(vec3 v);

	quat QuatBetweenVectors(vec3 start, vec3 dest, vec3 fallbackAxis = glm::vec3(0));

	quat QuatBetweenVectorsWithAxis(vec3 start, vec3 dest, vec3 axis);
}