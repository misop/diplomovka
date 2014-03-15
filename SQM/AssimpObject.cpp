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
	diffuseTexture = NULL;
	displacementTexture = NULL;
	normalTexture = NULL;
}


AssimpObject::~AssimpObject(void)
{
	delete buffer;
	if (diffuseTexture) delete diffuseTexture;
	if (displacementTexture) delete displacementTexture;
	if (normalTexture) delete normalTexture;
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

void AssimpObject::LoadTexture(GLTexture **texture, string img) {
    ilInit();

    ILuint texIL;
    ilGenImages(1, &texIL);
    ilBindImage(texIL);
    // nacitanie textury zo suboru
	//wstring widestr(img.begin(), img.end());
	ILboolean result = ilLoadImage((ILstring)img.c_str());
    if (!result) {
        ilDeleteImages(1, &texIL);
        return;
    }
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    // generovanie OpenGL textur
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // nastavenie parametrov textury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if ((*texture)) delete (*texture);
	(*texture) = new GLTexture(GL_TEXTURE_2D);
	(*texture)->Bind();
	(*texture)->TexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	(*texture)->TexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	(*texture)->TexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	(*texture)->TexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	(*texture)->RGBATexture(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetData());

    // zmazanie OpenIL textury
    ilDeleteImages(1, &texIL);
}

void AssimpObject::LoadDiffuseTexture(string img) {
	LoadTexture(&diffuseTexture, img);

}

void AssimpObject::LoadDisplacementTexture(string img) {
	LoadTexture(&displacementTexture, img);
}

void AssimpObject::LoadNormalTexture(string img) {
	LoadTexture(&normalTexture, img);
}

void AssimpObject::LoadTexturesFromFile(string fileName) {
	ifstream inputFile(fileName);
	string comment, texName;
	inputFile >> comment;
	//load diffuse
	bool load = false;
	inputFile >> load;
	if (load) {
		inputFile >> texName;
		LoadDiffuseTexture(texName);
	}
	//load displacement
	inputFile >> load;
	if (load) {
		inputFile >> texName;
		LoadDisplacementTexture(texName);
	}
	//load normal
	inputFile >> load;
	if (load) {
		inputFile >> texName;
		LoadNormalTexture(texName);
	}
}