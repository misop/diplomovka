#include "stdafx.h"
#include "AssimpObject.h"
#include <assimp/Importer.hpp> 
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <IL/il.h>

AssimpObject::AssimpObject(void)
{
	buffer = new GLArrayBuffer();
	material = glm::vec4(0);
}


AssimpObject::~AssimpObject(void)
{
	delete buffer;
	//if (diffuseTexture) delete diffuseTexture;
	//if (displacementTexture) delete displacementTexture;
	//if (normalTexture) delete normalTexture;
}

void AssimpObject::LoadFromFile(string fileName) {	
	Assimp::Importer importer;
	const aiScene* ai_scene = importer.ReadFile(fileName, aiProcessPreset_TargetRealtime_Quality);

	if (ai_scene->mNumMeshes == 0) return;

	aiMesh* ai_mesh = ai_scene->mMeshes[0];

	vector<float> verts;
	for (int i = 0; i < ai_mesh->mNumVertices; i++) {
		verts.push_back(ai_mesh->mVertices[i].x);
		verts.push_back(ai_mesh->mVertices[i].y);
		verts.push_back(ai_mesh->mVertices[i].z);
	}
	vector<float> normals;
	if(ai_mesh->mNormals != NULL) {
		for (int i = 0; i < ai_mesh->mNumVertices; i++) {
			normals.push_back(ai_mesh->mNormals[i].x);
			normals.push_back(ai_mesh->mNormals[i].y);
			normals.push_back(ai_mesh->mNormals[i].z);
		}
	}
	vector<float> uvs;
	if(ai_mesh->HasTextureCoords(0)) {
		for (int i = 0; i < ai_mesh->mNumVertices; i++) {
			uvs.push_back(ai_mesh->mTextureCoords[0][i].x);
			uvs.push_back(ai_mesh->mTextureCoords[0][i].y);
		}
	}
	vector<int> indices;
	for (int i = 0; i < ai_mesh->mNumFaces; i++) {
		indices.push_back(ai_mesh->mFaces[i].mIndices[0]);
		indices.push_back(ai_mesh->mFaces[i].mIndices[1]);
		indices.push_back(ai_mesh->mFaces[i].mIndices[2]);
	}

	buffer->Bind();
	buffer->BindBufferDataf(verts, 3, GL_STATIC_DRAW);
	buffer->BindBufferDataf(normals, 3, GL_STATIC_DRAW);
	if (uvs.size() > 0) buffer->BindBufferDataf(uvs, 2, GL_STATIC_DRAW);

	buffer->BindElement(indices, GL_STATIC_DRAW);
}

void AssimpObject::LoadTexturesFromFile(string fileName) {
	ifstream inputFile(fileName);

	string str1;
	float float1, float2;
	char command;

	while (!inputFile.eof()) {
		inputFile >> command;
		switch (command)
		{
		case '#':
			getline(inputFile, str1);
			break;
		case 'a':
			inputFile >> str1;
			diffuseTexture = shared_ptr<GLTexture>(new GLTexture(GL_TEXTURE_2D));
			diffuseTexture->LoadRGBATextureFromImage(str1);
			break;
		case 'd':
			inputFile >> str1;
			displacementTexture = shared_ptr<GLTexture>(new GLTexture(GL_TEXTURE_2D));
			displacementTexture->LoadRGBATextureFromImage(str1);
			break;
		case 'n':
			inputFile >> str1;
			normalTexture = shared_ptr<GLTexture>(new GLTexture(GL_TEXTURE_2D));
			normalTexture->LoadRGBATextureFromImage(str1);
			break;
		case 'm':
			inputFile >> float1 >> float2;
			material = glm::vec4(float1, float2, 0, 0);
		default:
			break;
		}
	}
}