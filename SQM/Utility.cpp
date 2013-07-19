#include "stdafx.h"
#include "Utility.h"
#include <GL\glew.h>
#include <GL\wglew.h>
#include <sstream>

using namespace std;

template <typename T> string ToString(T& a) {
	stringstream ss;
	ss << a;
	return ss.str();
}