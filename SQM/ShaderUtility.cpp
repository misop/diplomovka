#include "stdafx.h"
#include "ShaderUtility.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

std::string readShaderFile(std::string fileName) {
	ifstream input(fileName);
	stringstream buffer;
	buffer << input.rdbuf();
	input.close();

	return buffer.str();
}