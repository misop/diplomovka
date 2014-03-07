#include "stdafx.h"
#include "Interpolation.h"
#include <glm\gtx\norm.hpp>


ObjectPose CreateLookAtPose(glm::vec3 &from, glm::vec3 &to, glm::vec3 &up) {
	ObjectPose pose;
	pose.pos = from;
	// vyrob vektory predstavujuce suradnice bazickych vektorov rotacie vo svetovom systeme
	glm::vec3 dir = from - to;
	float length = glm::length(dir);
	if (length == 0) return pose;
	dir = glm::normalize(dir);
	// vektorovy sucin pre direction a up vektory dava right vektor
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	// vektorovy sucin pre direction a up vektory dava right vektor
	glm::vec3 new_up = glm::cross(dir, right);

	float d1 = glm::dot(right, dir);
	length = glm::length(new_up);

	// tieto 3 vektory tvoria stlpce rotacnej matice
	glm::mat3 rotation_matrix(right, new_up, dir);
	// vyrob kvaternion z rotacnej matice
	pose.quat = glm::quat(rotation_matrix);

	return pose;
};

//find time interval
bool FindInterval(std::vector<float> &knots, float time, int &span) {
	// najdi interval, v ktorom sa nachadza dany cas
	if (time >= (knots[knots.size() - 1])) {
		span = knots.size() - 1;
		return false;
	}
	if (time <= knots[0]) {
		span = 0;
		return false;
	}

	span = -1;
	while (time > knots[span + 1]) {
		span++;
	}

	// mame span interval, znormalizuj cas v ramci spanu
	if (knots[span] == knots[span + 1]) {
		return false;
	}

	return true;
}

// preinterpoluj zadane hodnoty v danom case, pouzije sa linearna interpolacia
glm::vec3 IntepolateVectorsLinear(std::vector<float> &knots, std::vector<glm::vec3> &values, float time, bool closed) {
	if (knots.size() < 1 || values.size() < 1 || knots.size() != values.size()) return glm::vec3(0);

	int span = 0;
	if (!FindInterval(knots, time, span)) return values[span];

	float u = (time - knots[span]) / (knots[span + 1] - knots[span]);

	// interpoluj hodnoty
	return ((1 - u) * values[span] + u * values[span + 1]);
}

// preinterpoluj zadane hodnoty v danom case, pouzije sa kubicka interpolacia
glm::vec3 IntepolateVectorsCubic(std::vector<float> &knots, std::vector<glm::vec3> &values, float time, bool closed) {
	if (knots.size() < 1 || values.size() < 1 || knots.size() != values.size()) return glm::vec3(0.0);

	int span = 0;
	if (!FindInterval(knots, time, span)) return values[span];

	// pre koncove hodnoty span intervalu, vypocitaj dotykove vektory
	// pouzijeme kardinalny splajn s parametrom tension
	float tension = 0;
	glm::vec3 prev_value = values[span];
	float prev_time = knots[span];
	if (span > 0) {
		prev_value = values[span - 1];
		prev_time = knots[span - 1];
	} else if (span == 0 && closed) {
		prev_value = values[knots.size() - 2];
		prev_time = knots[knots.size() - 2] - knots.back();
	}
	glm::vec3 next_value = values[span + 1];
	float next_time = knots[span + 1];
	if (span < int(values.size() - 2)) {
		next_value = values[span + 2];
		next_time = knots[span + 2];
	} else if (span >= int(values.size() - 2) && knots.size() > 1 && closed) {
		next_value = values[1];
		next_time = knots.back() + knots[1];
	}
	glm::vec3 begin_tangent(0);
	glm::vec3 end_tangent(0);
	//vektory na konci budu nulove
	if (knots[span + 1] - prev_time != 0 && (span != 0 || closed)) {
		begin_tangent = (1.0f - tension) * (values[span + 1] - prev_value) / (knots[span + 1] - prev_time);
	}
	if (next_time - knots[span] != 0 && (span < int(values.size() - 2) || closed)) {
		end_tangent = (1 - tension) * (next_value - values[span]) / (next_time - knots[span]);
	}

	// mame span interval, znormalizuj cas v ramci spanu
	float u = (time - knots[span]) / (knots[span + 1] - knots[span]);

	// vypocitaj interpolovanu hodnotu na zaklade hermitovych polynomov
	glm::vec3 result = (2*u*u*u - 3*u*u + 1)*values[span] +
		(u*u*u - 2*u*u + u)*(knots[span + 1] - knots[span])*begin_tangent + 
		(-2*u*u*u + 3*u*u)*values[span + 1] + 
		(u*u*u - u*u)*(knots[span + 1] - knots[span])*end_tangent;
	return result;
}

// preinterpoluj zadane kvaterniony v danom case, pouzije sa linearna sfericka interpolacia
glm::quat IntepolateQuaternionsLinear(std::vector<float> &knots, std::vector<glm::quat> &quaternions, float time, bool closed) {
	if (knots.size() < 1 || quaternions.size() < 1 || knots.size() != quaternions.size())
		return glm::quat(1, 0, 0, 0);

	int span = 0;
	if (!FindInterval(knots, time, span)) return quaternions[span];

	// mame span interval, znormalizuj cas v ramci spanu
	float u = (time - knots[span]) / (knots[span + 1] - knots[span]);

	// interpoluj hodnoty dvoch kvaternionov
	// pomocou slerp - linearne interpolacia na sfere
	return glm::slerp(quaternions[span], quaternions[span + 1], u);
}

glm::quat DoubleArc(glm::quat &Q1, glm::quat &Q2)
{
	// vypocitaj inverzny kvaternion k Q1
	/*aiQuaternion Q1inverse = Q1;
	float norm = sqrt(Q1.x*Q1.x + Q1.y*Q1.y + Q1.z*Q1.z + Q1.w*Q1.w);
	norm = 1 / (norm*norm);
	Q1inverse.Conjugate();
	Q1inverse.x = norm * Q1inverse.x;
	Q1inverse.y = norm * Q1inverse.y;
	Q1inverse.z = norm * Q1inverse.z;
	Q1inverse.w = norm * Q1inverse.w;

	//vypocitaj zdvojenie obluka
	aiQuaternion Q = (Q2 * Q1inverse * Q2);
	Q.Normalize();
	return Q;*/

	// druhy sposob vypoctu zdvojenia
	float dot = glm::dot(Q1, Q2);
	glm::quat Q;
	Q.x = 2 * dot * Q2.x - Q1.x;
	Q.y = 2 * dot * Q2.y - Q1.y;
	Q.z = 2 * dot * Q2.z - Q1.z;
	Q.w = 2 * dot * Q2.w - Q1.w;
	return glm::normalize(Q);
}

glm::quat BisectArc(glm::quat &Q1, glm::quat &Q2)
{
	glm::quat result;
	result.x = Q1.x + Q2.x;
	result.y = Q1.y + Q2.y;
	result.z = Q1.z + Q2.z;
	result.w = Q1.w + Q2.w;

	return glm::normalize(result);
}

// preinterpoluj zadane kvaterniony v danom case, pouzije sa kubicka sfericka interpolacia
glm::quat IntepolateQuaternionsCubic(std::vector<float> &knots, std::vector<glm::quat> &quaternions, float time, bool closed) {
	if (knots.size() < 1 || quaternions.size() < 1 || knots.size() != quaternions.size()) return glm::quat(1, 0, 0, 0);

	int span = 0;
	if (!FindInterval(knots, time, span)) return quaternions[span];

	float u = (time - knots[span]) / (knots[span + 1] - knots[span]);

	// daj 4 za sebou iduce kvaterniony, pomocou ktorych ideme interpolovat medzi Qj a Qjplus1
	glm::quat Qjminus1 = quaternions[span];
	if (span > 0)
		Qjminus1 = quaternions[span - 1];
	glm::quat Qj = quaternions[span];
	glm::quat Qjplus1 = quaternions[span+1];
	glm::quat Qjplus2 = quaternions[span+1];
	if (span < int(quaternions.size() - 2))
		Qjplus2 = quaternions[span+2];

	// vypocitaj dalsie 2 pomocne bezierove vrcholy
	glm::quat Lj = DoubleArc(Qjminus1, Qj);
	glm::quat Ljplus1 = DoubleArc(Qj, Qjplus1);
	glm::quat Aj = BisectArc(Lj, Qjplus1);
	glm::quat Ajplus1 = BisectArc(Ljplus1, Qjplus2);
	glm::quat Bjplus1 = DoubleArc(Ajplus1, Qjplus1);

	// posun Bezierove vrcholy Aj a Bjplus1 blizsie k riadiacim vrcholom Qj a Qjplus1
	// teneto parameter definuje kvalitu interpolacnej krivky
	float tension = 0.6666f;
	Aj.x = tension * Qj.x + (1 - tension) * Aj.x;
	Aj.y = tension * Qj.y + (1 - tension) * Aj.y;
	Aj.z = tension * Qj.z + (1 - tension) * Aj.z;
	Aj.w = tension * Qj.w + (1 - tension) * Aj.w;
	Aj = glm::normalize(Aj);
	Bjplus1.x = tension * Qjplus1.x + (1 - tension) * Bjplus1.x;
	Bjplus1.y = tension * Qjplus1.y + (1 - tension) * Bjplus1.y;
	Bjplus1.z = tension * Qjplus1.z + (1 - tension) * Bjplus1.z;
	Bjplus1.w = tension * Qjplus1.w + (1 - tension) * Bjplus1.w;
	Bjplus1 = glm::normalize(Bjplus1);

	// mame bezierovy vrcholy na 4D sfere, pouzi deCasteljauov algoritmus a slerp
	glm::quat P1, P2, P3, P4, P5, Q;
	P1 = glm::slerp(Qj, Aj, u);
	P2 = glm::slerp(Aj, Bjplus1, u);
	P3 = glm::slerp(Bjplus1, Qjplus1, u);
	P4 = glm::slerp(P1, P2, u);
	P5 = glm::slerp(P2, P3, u);
	Q = glm::slerp(P4, P5, u);

	return Q;
}

// preinterpoluj zadane pozy v danom case, pouzije sa linearna interpolacia
ObjectPose IntepolatePoseLinear(std::vector<float> &knots, std::vector<ObjectPose> &poses, float time, bool closed) {
	ObjectPose result;

	// nacitaj klucove hodnoty do vektora
	std::vector<glm::vec3> vector_pos(poses.size());
	std::vector<glm::vec3> vector_scale(poses.size());
	std::vector<glm::quat> vector_rot(poses.size());
	for (int i = 0; i < (int)poses.size(); i++)
	{
		vector_pos[i] = poses[i].pos;
		vector_scale[i] = poses[i].scale;
		vector_rot[i] = poses[i].quat;
	}

	// interpoluj kazdu suradnicu
	result.pos = IntepolateVectorsLinear(knots, vector_pos, time, closed);
	result.scale = IntepolateVectorsLinear(knots, vector_scale, time, closed);
	result.quat = IntepolateQuaternionsLinear(knots, vector_rot, time, closed);

	return result;
}

// preinterpoluj zadane pozy v danom case, pouzije sa kubicka interpolacia
ObjectPose IntepolatePoseCubic(std::vector<float> &knots, std::vector<ObjectPose> &poses, float time, bool closed) {
	ObjectPose result;

	// nacitaj klucove hodnoty do vektora
	std::vector<glm::vec3> vector_pos(poses.size());
	std::vector<glm::vec3> vector_scale(poses.size());
	std::vector<glm::quat> vector_rot(poses.size());
	for (int i = 0; i < (int)poses.size(); i++)
	{
		vector_pos[i] = poses[i].pos;
		vector_scale[i] = poses[i].scale;
		vector_rot[i] = poses[i].quat;
	}

	// interpoluj kazdy realny parameter
	result.pos = IntepolateVectorsCubic(knots, vector_pos, time, closed);
	result.scale = IntepolateVectorsCubic(knots, vector_scale, time, closed);
	//result.quat = IntepolateQuaternionsCubic(knots, vector_rot, time, closed);
	result.quat = IntepolateQuaternionsLinear(knots, vector_rot, time, closed);

	return result;
}

// preinterpoluj zadane pozy v danom case, pouzije sa kubicka interpolacia
ObjectPose IntepolatePoseCubicDerivedRot(std::vector<float> &knots, std::vector<ObjectPose> &poses, glm::quat base, float time, bool closed) {
	ObjectPose result;
	if (poses.size() < 1 || knots.size() < 1) return result;

	// nacitaj klucove hodnoty do vektora
	std::vector<glm::vec3> vector_pos(poses.size());
	std::vector<glm::vec3> vector_scale(poses.size());
	std::vector<glm::quat> vector_rot(poses.size());
	for (int i = 0; i < (int)poses.size(); i++)
	{
		vector_pos[i] = poses[i].pos;
		vector_scale[i] = poses[i].scale;
		vector_rot[i] = poses[i].quat;
	}

	// interpoluj kazdy realny parameter
	result.pos = IntepolateVectorsCubic(knots, vector_pos, time, closed);
	result.scale = IntepolateVectorsCubic(knots, vector_scale, time, closed);

	// rotacia je urcena derivaciou interpolacnej krivky
	// zatial sa derivacia vypocita numericky, nie z analytickeho vyjadrenia
	glm::vec3 prev = result.pos = IntepolateVectorsCubic(knots, vector_pos, time - 0.001, closed);
	glm::vec3 next = result.pos = IntepolateVectorsCubic(knots, vector_pos, time + 0.001, closed);
	glm::vec3 dir = next - prev;

	if (glm::length2(dir) != 0)
	{
		dir = glm::normalize(dir);
		glm::vec3 up(0, 1, 0);

		// vektorovy sucin pre direction a up vektory dava right vektor
		glm::vec3 right = glm::normalize(glm::cross(dir, up));

		// vektorovy sucin pre direction a up vektory dava right vektor
		glm::vec3 new_up = glm::cross(right, dir);

		float d1 = glm::dot(right, dir);
		float length = glm::length(new_up);

		// tieto 3 vektory tvoria stlpce rotacnej matice
		glm::mat3 rotation_matrix(right, dir, new_up);
		// vyrob kvaternion z rotacnej matice
		result.quat = glm::normalize(glm::quat(rotation_matrix) * base);
	}
	else {
		result.quat = IntepolateQuaternionsCubic(knots, vector_rot, time, closed);
	}

	return result;
}