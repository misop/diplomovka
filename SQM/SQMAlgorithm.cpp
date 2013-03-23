#include "stdafx.h"
#include "SQMAlgorithm.h"
#include <OpenMesh\Core\System\mostream.hh>

SQMAlgorithm::SQMAlgorithm(void) : root(NULL)
{
	drawingMode = 0;
	sqmState = SQMStart;
	mesh = NULL;
	fb = new filebuf();
	os = new ostream(fb);
	omerr().connect(*os);
}


SQMAlgorithm::~SQMAlgorithm(void)
{
	if (mesh != NULL) {
		delete mesh;
	}
	if (fb->is_open())
		fb->close();
	delete fb;
	delete os;
}

#pragma region Setters

void SQMAlgorithm::setRoot(SQMNode *newRoot) {
	if (root)
		delete root;

	root = newRoot;
	drawingMode = 0;
	sqmState = SQMStart;
}

#pragma endregion

#pragma region Getters

SQMNode* SQMAlgorithm::getRoot() {
	return root;
}

#pragma endregion

#pragma region Drawing

void SQMAlgorithm::draw() {
	if (root) {
		switch (drawingMode)
		{
		case 2:
			root->draw2();
			break;
		case 3:
			root->draw2();
			draw2();
			break;
		default:
			root->draw();
			break;
		}
	}
}

void SQMAlgorithm::draw2() {
	//draw polyhedron
	/*glColor3f(1, 1, 1);
	if (mesh != NULL) {
		OpenMesh::PolyConnectivity::FaceIter it = mesh->faces_begin();
		OpenMesh::PolyConnectivity::FaceIter fit = mesh->faces_end();
		for (; it != fit; ++it)	{
			glBegin(GL_LINES);
			MyMesh::ConstFaceVertexIter fvit = mesh->cfv_begin(it);
			MyMesh::ConstFaceVertexIter ffvit = mesh->cfv_end(it);
			glVertex3fv(&mesh->point(fvit)[0]);
			fvit++;
			for (; fvit != ffvit; ++fvit) {
				glVertex3fv(&mesh->point(fvit)[0]);
				glVertex3fv(&mesh->point(fvit)[0]);
			}
			fvit = mesh->cfv_begin(it);
			glVertex3fv(&mesh->point(fvit)[0]);
			glEnd();
		}
	}*/
	drawMeshHalfEdgesWithArrows(mesh);
}

void SQMAlgorithm::getBoundingSphere(float &x, float &y, float &z, float &d) {
	if (!root) {//if there is no root there is no bounding sphere
		x = y = z = d = 0;
		return;
	}
	vector<SQMNode*> stack;
	stack.push_back(root);
	float minX = root->getPosition()[0], maxX = root->getPosition()[0];//initialize the starting variables
	float minY = root->getPosition()[1], maxY = root->getPosition()[1];
	float minZ = root->getPosition()[2], maxZ = root->getPosition()[2];
	while (!stack.empty()) {
		SQMNode* node = stack.back();
		stack.pop_back();
		if (node->getPosition()[0] < minX)
			minX = node->getPosition()[0];
		if (node->getPosition()[0] > maxX)
			maxX = node->getPosition()[0];
		if (node->getPosition()[1] < minY)
			minY = node->getPosition()[1];
		if (node->getPosition()[1] > maxY)
			maxY = node->getPosition()[1];
		if (node->getPosition()[2] < minZ)
			minZ = node->getPosition()[2];
		if (node->getPosition()[2] > maxZ)
			maxZ = node->getPosition()[2];
		stack.insert(stack.end(), node->getNodes()->begin(), node->getNodes()->end());
	}
	float dx = maxX - minX;
	float dy = maxY - minY;
	float dz = maxZ - minZ;
	float maxD = dx > dy ? dx : dy;
	d = maxD > dz ? maxD : dz;
	d += 20;
	x = (maxX - minX)/2 + minX;
	y = (maxY - minY)/2 + minY;
	z = (maxZ - minZ)/2 + minZ;
}

#pragma endregion

#pragma region SQM Algorithm

void SQMAlgorithm::straightenSkeleton() {
	fb->open("log.txt", ios::out);
	(*os) << "Skeleton straightening\n";
	root->straightenSkeleton(OpenMesh::Vec3f(0, 0, 0));
	sqmState = SQMStraighten;
}

void SQMAlgorithm::computeConvexHull() {
	(*os) << "Convex hull computation\n";
	vector<SQMNode*> stack;
	stack.push_back(root);
	while (!stack.empty()) {
		SQMNode *node = stack.back();
		stack.pop_back();
		if (node->isBranchNode())
			node->calculateConvexHull();
		stack.insert(stack.end(), node->getNodes()->begin(), node->getNodes()->end());
	}
	drawingMode = 2;
	sqmState = SQMComputeConvexHull;
}

void SQMAlgorithm::subdivideConvexHull() {
	(*os) << "Convex hull subdivision\n";
	root->subdividePolyhedra(NULL, 0);
	sqmState = SQMSubdivideConvexHull;
}

void SQMAlgorithm::joinBNPs() {
	//deletition is messed up :-(
	//maybe delete just faces and let vertices be?
	//filebuf fb;
	//fb.open ("test.txt", ios::out);
	//ostream os(&fb);
	(*os) << "BNP joining\n";
	//omerr().connect(os);

	if (mesh != NULL) {
		delete mesh;
	}
	mesh = new MyMesh();
	vector<MyMesh::VertexHandle> vector;
	root->joinBNPs(mesh, NULL, vector, OpenMesh::Vec3f(0, 0, 0));
	drawingMode = 3;
	sqmState = SQMJoinBNPs;

	fb->close();
}

void SQMAlgorithm::executeSQMAlgorithm() {
	straightenSkeleton();
	computeConvexHull();
	subdivideConvexHull();
	joinBNPs();
}


void SQMAlgorithm::executeSQMAlgorithm(SQMState state) {
	if (sqmState < state) {
		if (sqmState < SQMStraighten && state >= SQMStraighten) {
			straightenSkeleton();
		}
		if (sqmState < SQMComputeConvexHull && state >= SQMComputeConvexHull) {
			computeConvexHull();
		}
		if (sqmState < SQMSubdivideConvexHull && state >= SQMSubdivideConvexHull) {
			subdivideConvexHull();
		}
		if (sqmState < SQMJoinBNPs && state >= SQMJoinBNPs) {
			joinBNPs();
		}
	}
}

#pragma endregion
