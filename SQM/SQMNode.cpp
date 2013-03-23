#include "stdafx.h"
#include "SQMNode.h"
#include <algorithm>
#include "SphereDelaunay.h"
#include "FloatArithmetic.h"

#define BIAS 0.1

#pragma region Init

SQMNode::SQMNode(void) {
	position = OpenMesh::Vec3f(0, 0, 0);
	parent = NULL;
	polyhedron = NULL;
}

SQMNode::SQMNode(SkeletonNode &node, SQMNode* newParent) : parent(newParent) {
	position = OpenMesh::Vec3f(node.point.x, node.point.y, node.point.z);
	//nodeRadius = (float)(rand()%100)/100*10 + 5;
	nodeRadius = 10;
	for (int i = 0; i < node.nodes.size(); i++) {
		SQMNode *newNode = new SQMNode(*node.nodes[i], this);
		nodes.push_back(newNode);
	}
	polyhedron = NULL;
}


SQMNode::~SQMNode(void) {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	if (polyhedron != NULL) delete polyhedron;
}

#pragma endregion

#pragma region Getters

bool SQMNode::isBranchNode() {
	int requiredConections = 3;//branch nodes has at least 3 connections
	if (parent) requiredConections--; //parent counts as one conection

	return nodes.size() >= requiredConections;
}

bool SQMNode::isLeafNode() {
	return nodes.size() == 0;
}

OpenMesh::Vec3f SQMNode::getPosition() {
	return position;
}

vector<SQMNode*>* SQMNode::getNodes() {
	return &nodes;
}

MyMesh* SQMNode::getPolyhedron() {
	return polyhedron;
}

vector<SQMNode*>* SQMNode::getDescendants() {
	return &nodes;
}

vector<MyMesh::VertexHandle>* SQMNode::getIntersectionVHandles() {
	return &intersectionVHandles;
}

float SQMNode::getNodeRadius() {
	return nodeRadius;
}

#pragma endregion

#pragma region Setters

void SQMNode::setNodeRadius(float newNodeRadius) {
	nodeRadius = newNodeRadius;
}

void SQMNode::setPosition(OpenMesh::Vec3f newPosition) {
	position = newPosition;
}

void SQMNode::addDescendant(SQMNode* node) {
	nodes.push_back(node);
}

#pragma endregion

#pragma region Drawing

void SQMNode::draw() {
	draw(CVector3(0, 1, 0), CVector3(1, 0, 0));
}

void SQMNode::draw(CVector3 lineColor, CVector3 nodeColor) {
	//draw connections
	glColor3f(lineColor.x, lineColor.y, lineColor.z);
	glBegin(GL_LINES);
	for (int i = 0; i < nodes.size(); i++) {
		OpenMesh::Vec3f nodePosition = nodes[i]->getPosition();
		glVertex3f(position[0], position[1], position[2]);
		glVertex3f(nodePosition[0], nodePosition[1], nodePosition[2]);
	}
	glEnd();
	//draw ball
	glColor3f(nodeColor.x, nodeColor.y, nodeColor.z);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	gluSphere(gluNewQuadric(), nodeRadius, 10, 10);
	glPopMatrix();
	glColor3f(0, 0, 1);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->draw();
	}
}


void SQMNode::draw2() {
	if (polyhedron != NULL) {
		//draw polyhedron
		float color[] = {0, 1, 0};
		drawMeshHalfEdgesWithArrowsAndColor(polyhedron, color);
		/*glColor3f(1, 1, 1); 
		glBegin(GL_LINES);
		for (int i = 0; i < triangles2.size(); i++) {
			OpenMesh::Vec3i triangle = triangles2[i];
			OpenMesh::Vec3f V0 = intersections[triangle[0]];
			OpenMesh::Vec3f V1 = intersections[triangle[1]];
			OpenMesh::Vec3f V2 = intersections[triangle[2]];
			
			glVertex3fv(V0.values_);
			glVertex3fv(V1.values_);
			
			glVertex3fv(V1.values_);
			glVertex3fv(V2.values_);
			
			glVertex3fv(V2.values_);
			glVertex3fv(V0.values_);
		}*/
		/*for (int i = 0; i < centers2.size(); i++) {
			OpenMesh::Vec3f P = centers2[i];
			OpenMesh::Vec3f Q = P + (normals2[i] * 10.0);
			
			glVertex3fv(P.values_);
			glVertex3fv(Q.values_);
		}*/

		glEnd();
	}
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->draw2();
	}
}

#pragma endregion

#pragma region Skeleton Straightening

void SQMNode::straightenSkeleton(OpenMesh::Vec3f *lineVector) {
	if (lineVector != NULL) {//straighten self
		OpenMesh::Vec3f newPosition = ((position - parent->getPosition()).length()*(*lineVector)) + parent->getPosition();
		CVector4 quaternion = QuaternionBetweenVectors(CVector3(newPosition.values_), CVector3(position.values_));
		axisAngle = QuaternionToAxisAngle(quaternion);
		position = newPosition;
	}
	if (this->isBranchNode()) {//if this is branch node recalculate new vectors and intersections
		for (int i = 0; i < nodes.size(); i++) {//specifical order parent intersection needs to be last in vector
			OpenMesh::Vec3f u = nodes[i]->getPosition() - position;
			u = u.normalize();
			nodes[i]->straightenSkeleton(&u);
			intersections.push_back(position + u * nodeRadius);
		}
		if (parent) {//also calculate intersection with parent
			OpenMesh::Vec3f u = parent->getPosition() - position;
			u = u.normalize();
			intersections.push_back(position + u * nodeRadius);
		}
	} else {//else just straighten conected nodes
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->straightenSkeleton(lineVector);
		}
	}
}

void SQMNode::straightenSkeleton(OpenMesh::Vec3f lineVector) {
	if (!OpenMeshVec3fZero(lineVector)) {//straighten self
		OpenMesh::Vec3f newPosition = ((position - parent->getPosition()).length()*lineVector) + parent->getPosition();
		CVector4 quaternion = QuaternionBetweenVectors(CVector3(newPosition.values_), CVector3(position.values_));
		axisAngle = QuaternionToAxisAngle(quaternion);
		position = newPosition;
	}
	if (this->isBranchNode()) {//if this is branch node recalculate new vectors and intersections
		for (int i = 0; i < nodes.size(); i++) {
			OpenMesh::Vec3f u = nodes[i]->getPosition() - position;
			u = u.normalize();
			nodes[i]->straightenSkeleton(u);
			intersections.push_back(position + u * nodeRadius);
		}
		if (parent) {//also calculate intersection with parent
			OpenMesh::Vec3f u = parent->getPosition() - position;
			u = u.normalize();
			intersections.push_back(position + u * nodeRadius);
		}
	} else {//else just straighten conected nodes
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->straightenSkeleton(lineVector);
		}
	}
}

#pragma endregion

#pragma region BNP generation

void SQMNode::calculateConvexHull() {
	vector<OpenMesh::Vec3i> triangles;
	Delaunay_on_sphere(intersections, triangles);
	triangles2 = triangles;
	createPolyhedra(triangles);
}

void SQMNode::createPolyhedra(vector<OpenMesh::Vec3i> triangles) {
	//TODO normal swaping
	//calculate triangle normals
	vector<OpenMesh::Vec3f> normals;
	vector<OpenMesh::Vec3f> centers;
	for (int i = 0; i < triangles.size(); i++) {
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec3f A = intersections[triangle.values_[0]];
		OpenMesh::Vec3f B = intersections[triangle.values_[1]];
		OpenMesh::Vec3f C = intersections[triangle.values_[2]];
		OpenMesh::Vec3f normal = cross(B - A, C - A);
		normal = normal.normalize();
		OpenMesh::Vec3f center((A[0] + B[0] + C[0])/3.0, (A[1] + B[1] + C[1])/3.0, (A[2] + B[2] + C[2])/3.0);
		if (checkPolyhedronOrientation(i, center, normal, triangles)) {
			normal = -normal;
			triangles[i] = flipVec3i(triangle);
		}

		normals.push_back(normal);		
		centers.push_back(center);
	}
	normals2 = normals;
	centers2 = centers;
	//create map edge vector<faces>
	map<OpenMesh::Vec2i, vector<int>, OpenMeshVec2iComp> edgeFaceIndexMap;
	for (int i = 0; i < triangles.size(); i++) {
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec2i u1(triangle.values_[0], triangle.values_[1]);
		OpenMesh::Vec2i u2(triangle.values_[1], triangle.values_[2]);
		OpenMesh::Vec2i u3(triangle.values_[2], triangle.values_[0]);
		vector<int> faces1;
		vector<int> faces2;
		vector<int> faces3;
		for (int j = 0; j < triangles.size(); j++) {
			OpenMesh::Vec3i triangle2 = triangles[j];
			OpenMesh::Vec2i v1(triangle2.values_[0], triangle2.values_[1]);
			OpenMesh::Vec2i v2(triangle2.values_[1], triangle2.values_[2]);
			OpenMesh::Vec2i v3(triangle2.values_[2], triangle2.values_[0]);
			if (OpenMeshVec2iEqual(u1, v1) || OpenMeshVec2iEqual(u1, v2) || OpenMeshVec2iEqual(u1, v3)) {
				faces1.push_back(j);
			}
			if (OpenMeshVec2iEqual(u2, v1) || OpenMeshVec2iEqual(u2, v2) || OpenMeshVec2iEqual(u2, v3)) {
				faces2.push_back(j);
			}
			if (OpenMeshVec2iEqual(u3, v1) || OpenMeshVec2iEqual(u3, v2) || OpenMeshVec2iEqual(u3, v3)) {
				faces3.push_back(j);
			}
		}
		edgeFaceIndexMap.insert(pair<OpenMesh::Vec2i, vector<int> >(u1, faces1));
		edgeFaceIndexMap.insert(pair<OpenMesh::Vec2i, vector<int> >(u2, faces2));
		edgeFaceIndexMap.insert(pair<OpenMesh::Vec2i, vector<int> >(u3, faces3));
	}
	//for each triangle create 6 new triangles and translate new vertices and translate vertices with face normals add only unique
	vector<OpenMesh::Vec3f> vertices;
	vector<OpenMesh::Vec3i> faces;
	for (int i = 0; i < triangles.size(); i++) {
		//get triangle points and create new ones
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec3f v1 = intersections[triangle.values_[0]];
		OpenMesh::Vec3f v2 = intersections[triangle.values_[1]];
		OpenMesh::Vec3f v3 = intersections[triangle.values_[2]];

		OpenMesh::Vec3f u12 = 0.5*v1 + 0.5*v2;
		OpenMesh::Vec3f u23 = 0.5*v2 + 0.5*v3;
		OpenMesh::Vec3f u31 = 0.5*v3 + 0.5*v1;
		OpenMesh::Vec3f center = centers[i];
		//translate points
		vector<int> u12NormalIndexis = getNormalIndexis(edgeFaceIndexMap[OpenMesh::Vec2i(triangle.values_[0], triangle.values_[1])], i);
		int i1 = u12NormalIndexis[0];
		int i2 = u12NormalIndexis[1];
		u12 = translatedPointToSphereWithFaceNormals(u12, normals[i1], normals[i2], centers[i1], centers[i2]);

		vector<int> u23NormalIndexis = getNormalIndexis(edgeFaceIndexMap[OpenMesh::Vec2i(triangle.values_[1], triangle.values_[2])], i);
		i1 = u23NormalIndexis[0];
		i2 = u23NormalIndexis[1];
		u23 = translatedPointToSphereWithFaceNormals(u23, normals[i1], normals[i2], centers[i1], centers[i2]);

		vector<int> u31NormalIndexis = getNormalIndexis(edgeFaceIndexMap[OpenMesh::Vec2i(triangle.values_[2], triangle.values_[0])], i);
		i1 = u31NormalIndexis[0];
		i2 = u31NormalIndexis[1];
		u31 = translatedPointToSphereWithFaceNormals(u31, normals[i1], normals[i2], centers[i1], centers[i2]);

		center = translatedPointToSphereWithFaceNormals(center, normals[i], normals[i], center, center);
		//add only unique points to vertex list
		int v1Index = getPointPositionInArrayOrAdd(v1, vertices);
		int v2Index = getPointPositionInArrayOrAdd(v2, vertices);
		int v3Index = getPointPositionInArrayOrAdd(v3, vertices);

		int u12Index = getPointPositionInArrayOrAdd(u12, vertices);
		int u23Index = getPointPositionInArrayOrAdd(u23, vertices);
		int u31Index = getPointPositionInArrayOrAdd(u31, vertices);

		int centerIndex = getPointPositionInArrayOrAdd(center, vertices);
		//add new triangles to face list
		faces.push_back(OpenMesh::Vec3i(v1Index, u12Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u12Index, v2Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(v2Index, u23Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u23Index, v3Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(v3Index, u31Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u31Index, v1Index, centerIndex));
		//faces.push_back(OpenMesh::Vec3i(v1Index, v2Index, v3Index));
	}
	//create OpenMesh mesh from indexed face
	openMeshFromIdexedFace(vertices, faces);
}

bool SQMNode::checkPolyhedronOrientation(int index, OpenMesh::Vec3f center, OpenMesh::Vec3f normal, vector<OpenMesh::Vec3i>& triangles) {
	//BNP generation can generate swapped faces which face inside the polyhedra thus we need to swap the normals of those faces
	for (int i = 0; i < triangles.size(); i++) {
		if (i == index)
			continue;
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec3f A = intersections[triangle.values_[0]];
		OpenMesh::Vec3f B = intersections[triangle.values_[1]];
		OpenMesh::Vec3f C = intersections[triangle.values_[2]];
		float t = 0;
		if (rayTriangleIntersection(center, normal, A, B, C, t)) {
			//should change normal sign
			return true;
		}
	}
	//should not change sign of normal
	return false;
}

OpenMesh::Vec3f SQMNode::translatedPointToSphereWithFaceNormals(OpenMesh::Vec3f p, OpenMesh::Vec3f n1, OpenMesh::Vec3f n2, OpenMesh::Vec3f center1, OpenMesh::Vec3f center2) {
	//calculate leading vector
	OpenMesh::Vec3f u = (n1 + n2);//B-A
	if (u.norm() < FLOAT_ZERO) {
		OpenMesh::Vec3f center = 0.5*center1 + 0.5*center2;
		u = (p - center).normalize();
	} else {
		u = u.normalize();
	}
	float t;
	if (raySphereIntersection(p, u, position, nodeRadius, t)) {
		return p + (t*u);
	} else {
		return p;
	}
}

vector<int> SQMNode::getNormalIndexis(vector<int> indexis, int index) {
	//if there are two indexis
	if (indexis.size() == 2)
		return indexis;
	//if there is only one index (error?)
	if (indexis.size() == 1) {
		vector<int> result;
		result.push_back(indexis[0]);
		result.push_back(indexis[0]);
		return result;
	}
	//if there is none
	if (indexis.size() == 0) {
		throw exception("missing normal vector indexis");
	}
	//if there are many indexis the normal is equal to face normal
	vector<int> result;
	result.push_back(index);
	result.push_back(index);
	return result;
}

void SQMNode::openMeshFromIdexedFace(vector<OpenMesh::Vec3f> vertices, vector<OpenMesh::Vec3i> faces) {
	if (polyhedron != NULL) delete polyhedron;
	polyhedron = new MyMesh();
	polyhedronPoints.clear();
	intersectionVHandles.clear();
	intersectionVHandles.resize(intersections.size());

	for (int i = 0; i < vertices.size(); i++) {
		polyhedronPoints.push_back(polyhedron->add_vertex(MyMesh::Point(vertices[i].values_)));
		//store intersections vertex handles
		int position = getPointPositionInArray(vertices[i], intersections);//specific order equal to intersection vector
		if (position != -1) {
			intersectionVHandles[position] = polyhedronPoints.back();
		}
	}
	for (int i = 0; i < faces.size(); i++) {
		OpenMesh::Vec3i face = faces[i];
		polyhedron->add_face(polyhedronPoints[face.values_[0]], polyhedronPoints[face.values_[1]], polyhedronPoints[face.values_[2]]);
	}
}

//-----------------------TEST------------------------

void SQMNode::createPolyhedraFromCenter(vector<OpenMesh::Vec3i> triangles) {
	//get the mesh center
	OpenMesh::Vec3f meshCenter = polyhedronBoundingBoxCenter();
	//get the center of each triangle
	vector<OpenMesh::Vec3f> centers;
	for (int i = 0; i < triangles.size(); i++) {
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec3f A = intersections[triangle.values_[0]];
		OpenMesh::Vec3f B = intersections[triangle.values_[1]];
		OpenMesh::Vec3f C = intersections[triangle.values_[2]];
		OpenMesh::Vec3f center((A[0] + B[0] + C[0])/3.0, (A[1] + B[1] + C[1])/3.0, (A[2] + B[2] + C[2])/3.0);	
		centers.push_back(center);
	}
	//for each triangle create 6 new triangles and translate new vertices and translate vertices with face normals add only unique
	vector<OpenMesh::Vec3f> vertices;
	vector<OpenMesh::Vec3i> faces;
	for (int i = 0; i < triangles.size(); i++) {
		//get triangle points and create new ones
		OpenMesh::Vec3i triangle = triangles[i];
		OpenMesh::Vec3f v1 = intersections[triangle.values_[0]];
		OpenMesh::Vec3f v2 = intersections[triangle.values_[1]];
		OpenMesh::Vec3f v3 = intersections[triangle.values_[2]];

		OpenMesh::Vec3f u12 = 0.5*v1 + 0.5*v2;
		OpenMesh::Vec3f u23 = 0.5*v2 + 0.5*v3;
		OpenMesh::Vec3f u31 = 0.5*v3 + 0.5*v1;
		OpenMesh::Vec3f center = centers[i];
		//translate points
		u12 = translatePointToSphereFromCenter(u12, meshCenter);

		u23 = translatePointToSphereFromCenter(u23, meshCenter);

		u31 = translatePointToSphereFromCenter(u31, meshCenter);

		center = translatePointToSphereFromCenter(center, meshCenter);
		//add only unique points to vertex list
		int v1Index = getPointPositionInArrayOrAdd(v1, vertices);
		int v2Index = getPointPositionInArrayOrAdd(v2, vertices);
		int v3Index = getPointPositionInArrayOrAdd(v3, vertices);

		int u12Index = getPointPositionInArrayOrAdd(u12, vertices);
		int u23Index = getPointPositionInArrayOrAdd(u23, vertices);
		int u31Index = getPointPositionInArrayOrAdd(u31, vertices);

		int centerIndex = getPointPositionInArrayOrAdd(center, vertices);
		//add new triangles to face list
		faces.push_back(OpenMesh::Vec3i(v1Index, u12Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u12Index, v2Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(v2Index, u23Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u23Index, v3Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(v3Index, u31Index, centerIndex));
		faces.push_back(OpenMesh::Vec3i(u31Index, v1Index, centerIndex));
		//faces.push_back(OpenMesh::Vec3i(v1Index, v2Index, v3Index));
	}
	//create OpenMesh mesh from indexed face
	openMeshFromIdexedFace(vertices, faces);
}

OpenMesh::Vec3f SQMNode::polyhedronBoundingBoxCenter() {
	float minX = intersections[0][0], minY = intersections[0][1], minZ = intersections[0][2];
	float maxX = minX, maxY = minY, maxZ = minZ;
	for (int i = 0; i < intersections.size(); i++) {
		OpenMesh::Vec3f intersection = intersections[i];

		minX = min(minX, intersection[0]);
		minY = min(minY, intersection[1]);
		minZ = min(minZ, intersection[2]);
		
		maxX = max(maxX, intersection[0]);
		maxY = max(maxY, intersection[1]);
		maxZ = max(maxZ, intersection[2]);
	}
	return OpenMesh::Vec3f((minX + maxX) / 2.0, (minY + maxY) / 2.0, (minZ + maxZ) / 2.0);
}

OpenMesh::Vec3f SQMNode::polyhedronPointSumCenterCenter() {
	OpenMesh::Vec3f result(0, 0, 0);

	for (int i = 0; i < intersections.size(); i++) {
		result += intersections[i];
	}
	result /= intersections.size();

	return result;
}

OpenMesh::Vec3f SQMNode::translatePointToSphereFromCenter(OpenMesh::Vec3f point, OpenMesh::Vec3f center) {
	OpenMesh::Vec3f direction = (point - center).normalize();
	float t = 0;
	if (raySphereIntersection(center, direction, position, nodeRadius, t)) {
		return center + (direction * t);
	} else {
		return point;
	}
}

#pragma endregion

#pragma region BNP Subdivision

void SQMNode::subdividePolyhedra(SQMNode* parentBranchNode, int count) {
	//vector of next BNPs
	vector<SQMNode*> branchingNodes;
	for (int i = 0; i < nodes.size(); i++) {
		branchingNodes.push_back(getDescendantBranchNode(nodes[i]));
	}
	//prepare subdivide table of how many needed
	vector<VHandleCount> verticeCountTable;
	fillVerticeCountTable(verticeCountTable, branchingNodes);
	//take care of parent intersection aka addd to the end
	if (parentBranchNode) {
		verticeCountTable.push_back(VHandleCount(intersectionVHandles.back(), count, true));
	}
	//take first from table if needed subdivide
	//if one vertice needs more connection and other are fine subdivide them anyway but dont subdivide parent vertice
	//sort by edge length and test if needs more pick the longest that needs or longest that is not adjacent to parent
	for (int i = 0; i < verticeCountTable.size(); i++) {
		VHandleCount vhandleCount = verticeCountTable[i];
		if (vhandleCount.needs && vhandleCount.missingPoints > 0) {
			while (vhandleCount.missingPoints > 0) {
				//create sorted list of edges
				deque<EdgeLength> edgeLengthList;
				fillEdgeLengthList(edgeLengthList, vhandleCount.vhandle);
				sort(edgeLengthList.begin(), edgeLengthList.end(), EdgeLengthCompare);
				//pick one
				MyMesh::EdgeHandle eh = pickEdgeToSplit(edgeLengthList, verticeCountTable, vhandleCount.vhandle);
				//split one
				splitEdgeInHalf(eh);
				vhandleCount.missingPoints--;
				//TODO change the other one also
			}
		}
	}
	//take care of the rest
	for (int i = 0; i < branchingNodes.size(); i++)	{
		SQMNode* branchingNode = branchingNodes[i];
		//if we needed some they have been added if we had more points than descendant he needs to add them
		int missingPoints = (verticeCountTable[i].missingPoints >= 0) ? 0 : -verticeCountTable[i].missingPoints;
		if (branchingNode != NULL) {
			branchingNode->subdividePolyhedra(this, missingPoints);
		}
	}
}

void SQMNode::fillVerticeCountTable(vector<VHandleCount>& verticeCountTable, vector<SQMNode*>& branchingNodes) {
	for (int i = 0; i < nodes.size(); i++) {
		SQMNode* descendant = nodes[i];
		//get node intersection
		MyMesh::VHandle vhandle = intersectionVHandles[i];
		//get descendant branching node
		SQMNode* descendantBranchNode = branchingNodes[i];
		if (descendantBranchNode != NULL) {			
			//get intersection vhandle its the last one from parent
			MyMesh::VHandle descendantVHandle = descendantBranchNode->getIntersectionVHandles()->back();
			//get number of needed vertices
			int vhandleCount = 0;
			for (MyMesh::VVIter vvit = polyhedron->vv_begin(vhandle); vvit != polyhedron->vv_end(vhandle); ++vvit) {
				vhandleCount++;
			}
			int descendantVHandleCount = 0;
			MyMesh *descendantPolyhedron = descendant->getPolyhedron();
			for (MyMesh::VVIter vvit = descendantPolyhedron->vv_begin(descendantVHandle); vvit != descendantPolyhedron->vv_end(descendantVHandle); ++vvit) {
				descendantVHandleCount++;
			}
			verticeCountTable.push_back(VHandleCount(vhandle, descendantVHandleCount - vhandleCount, true));
		} else {
			verticeCountTable.push_back(VHandleCount(vhandle, 0, false));
		}
	}
}

void SQMNode::fillEdgeLengthList(deque<EdgeLength>& edgeLengthList, MyMesh::VHandle vertice) {
	for (MyMesh::VertexOHalfedgeIter vohit = polyhedron->voh_begin(vertice); vohit != polyhedron->voh_end(vertice); ++vohit) {
		MyMesh::HalfedgeHandle heh = polyhedron->next_halfedge_handle(vohit.handle());
		MyMesh::EdgeHandle eh = polyhedron->edge_handle(heh);
		float length = polyhedron->calc_edge_length(eh);
		edgeLengthList.push_back(EdgeLength(heh, length));
	}
}

MyMesh::EdgeHandle SQMNode::pickEdgeToSplit(deque<EdgeLength>& edgeLengthList, vector<VHandleCount>& verticeCountTable, MyMesh::VHandle vertice) {
	//mark vertices that should be split
	vector<MyMesh::VHandle> shouldSplit;
	vector<VHandleCount*> filteredVHandleCounts;
	MyMesh::VHandle parentVHandle = intersectionVHandles.back();
	for (int i = 0; i < verticeCountTable.size(); i++) {
		VHandleCount vhandleCount = verticeCountTable[i];
		if (vhandleCount.needs && vhandleCount.missingPoints > 0 && vhandleCount.vhandle != vertice) {
			shouldSplit.push_back(vhandleCount.vhandle);
			filteredVHandleCounts.push_back(&verticeCountTable[i]);
		}
	}
	//try to split one of them
	if (shouldSplit.size() > 0) {
		for (deque<EdgeLength>::iterator it = edgeLengthList.begin(); it != edgeLengthList.end(); it++) {
			EdgeLength el = (*it);
			//get the other vertex handle
			MyMesh::VHandle vhandle = polyhedron->to_vertex_handle(el.hehandle);
			vector<MyMesh::VHandle>::iterator found = find(shouldSplit.begin(), shouldSplit.end(), vhandle);
			if (found != shouldSplit.end()) {
				int index = found - shouldSplit.begin();
				filteredVHandleCounts[index]->missingPoints--;
				return polyhedron->edge_handle(el.hehandle);
			}
		}
	}
	//if there is none split one vertice except parent 
	//if parent should be split it would alread been
	for (deque<EdgeLength>::iterator it = edgeLengthList.begin(); it != edgeLengthList.end(); it++) {
		EdgeLength el = (*it);
		//get the other vertex handle
		MyMesh::VHandle vhandle = polyhedron->to_vertex_handle(el.hehandle);
		if (vhandle != parentVHandle) {
			for (int i = 0; i < verticeCountTable.size(); i++) {
				if (vhandle == verticeCountTable[i].vhandle) {
					verticeCountTable[i].missingPoints--;
					break;
				}
			}
			return polyhedron->edge_handle(el.hehandle);
		}
	}
	//should never reach here
	EdgeLength el = edgeLengthList[0];
	return polyhedron->edge_handle(el.hehandle);
}

void SQMNode::splitEdgeInHalf(MyMesh::EdgeHandle eh) {
	MyMesh::HalfedgeHandle heh0 = polyhedron->halfedge_handle(eh, 0);
	MyMesh::HalfedgeHandle heh1 = polyhedron->halfedge_handle(eh, 1);
	MyMesh::FaceHandle fh0 = polyhedron->face_handle(heh0);
	MyMesh::FaceHandle fh1 = polyhedron->face_handle(heh1);
	MyMesh::Point p0 = polyhedron->point(polyhedron->to_vertex_handle(heh0));
	MyMesh::Point p1 = polyhedron->point(polyhedron->to_vertex_handle(heh1));

	MyMesh::Point x = 0.5*p0 + 0.5*p1;
	MyMesh::VertexHandle vh = polyhedron->add_vertex(x);

	//creates invalid faces :(
	polyhedron->split_edge(eh, vh);
	polyhedron->split(fh0, vh);
	polyhedron->split(fh1, vh);
}

#pragma endregion

#pragma region BNP Joining

void SQMNode::joinBNPs(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle> oneRing, OpenMesh::Vec3f directionVector) {
	if (this->isLeafNode()) {
		//finish mesh somehow
		finishLeafeNode(mesh, oneRing);
		return;
	}
	if (this->isBranchNode()) {
		addPolyhedronToMesh(mesh, parentBNPNode, oneRing, directionVector);
	} else {//connection node with one son
		extendMesh(mesh, parentBNPNode, oneRing, directionVector);
	}
}

void SQMNode::addPolyhedronToMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector) {
	//connect mesh to existing one
	vector<vector<MyMesh::VHandle> > oneRingsOfPolyhedron;
	addPolyhedronAndRememberVHandles(mesh, parentBNPNode, oneRing, oneRingsOfPolyhedron, directionVector);
	//for each son send him points on intersection perimeter
	for (int i = 0; i < nodes.size(); i++) {
		OpenMesh::Vec3f u = (nodes[i]->getPosition() - position).normalize();
		nodes[i]->joinBNPs(mesh, this, oneRingsOfPolyhedron[i], u);
	}
}
//!!!!!ORDERING OF INSERTED EDGES IS IMPORTANT!!!!!!!
void SQMNode::addPolyhedronAndRememberVHandles(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, vector<vector<MyMesh::VHandle> >& oneRingsOfPolyhedron, OpenMesh::Vec3f& directionVector) {
	//get non intersecting vhandles and remember one rings
	int vectorSize = intersectionVHandles.size();
	vector<vector<int> > intersectionOneRingIndexes(vectorSize);
	vector<MyMesh::VHandle> addedVHandles;
	for (MyMesh::VIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) {
		MyMesh::VHandle vhandle = v_it.handle();
		int position = getPositionInArray<MyMesh::VHandle>(vhandle, intersectionVHandles);
		if (position == -1) {
			MyMesh::Point P = polyhedron->point(vhandle);
			addedVHandles.push_back(mesh->add_vertex(P));
		} else {
			addedVHandles.push_back(vhandle);
			//collect one ring indexis
			for (MyMesh::VVIter vv_it = polyhedron->vv_begin(vhandle); vv_it != polyhedron->vv_end(vhandle); ++vv_it) {
				intersectionOneRingIndexes[position].push_back(vv_it.handle().idx());
			}
		}
	}
	//prepare one rings
	for (int i = 0; i < intersectionOneRingIndexes.size(); i++) {
		vector<MyMesh::VHandle> vhandles;
		for (int j = 0; j < intersectionOneRingIndexes[i].size(); j++) {
			int vertexIndex = intersectionOneRingIndexes[i][j];
			vhandles.push_back(addedVHandles[vertexIndex]);
		}
		oneRingsOfPolyhedron.push_back(vhandles);
	}
	//connect to the rest of the mesh
	/*if (parentBNPNode != NULL) { //keeped for future testing purposes
		vector<MyMesh::VHandle> oldOneRing = oneRingsOfPolyhedron.back();
		OpenMesh::Vec3f u = - directionVector;
		vector<MyMesh::VHandle> nor3;
		for (int i = 0; i < oldOneRing.size(); i++) {
			MyMesh::Point P = mesh->point(oldOneRing[i]);
			P = P + (10*u);
			nor3.push_back(mesh->add_vertex(P));
		}
		for (int i = 0; i < nor3.size(); i++) {
			int j = 0;
			if (i + 1 < nor3.size()) {
				j = i + 1;
			}
			MyMesh::VHandle vh1 = oldOneRing[i];
			MyMesh::VHandle vh2 = nor3[i];
			MyMesh::VHandle vh3 = nor3[j];
			MyMesh::VHandle vh4 = oldOneRing[j];

			mesh->add_face(oldOneRing[i], nor3[i], nor3[j], oldOneRing[j]);

			MyMesh::HalfedgeHandle he1 = mesh->find_halfedge(vh2, vh1);
			MyMesh::HalfedgeHandle he2 = mesh->find_halfedge(vh3, vh2);
			MyMesh::HalfedgeHandle he3 = mesh->find_halfedge(vh3, vh4);
			MyMesh::HalfedgeHandle he4 = mesh->find_halfedge(vh4, vh1);
			bool b1 = mesh->is_boundary(he1);
			bool b2 = mesh->is_boundary(he2);
			bool b3 = mesh->is_boundary(he3);
			bool b4 = mesh->is_boundary(he4);
		}
		vector<MyMesh::VHandle> nor;
		for (int i = 0; i < nor3.size(); i++) {
			MyMesh::Point P = mesh->point(nor3[i]);
			P = P + (10*u);
			nor.push_back(mesh->add_vertex(P));
		}
		for (int i = 0; i < nor.size(); i++) {
			int j = 0;
			if (i + 1 < nor.size()) {
				j = i + 1;
			}
			MyMesh::VHandle vh1 = nor3[i];
			MyMesh::VHandle vh2 = nor[i];
			MyMesh::VHandle vh3 = nor[j];
			MyMesh::VHandle vh4 = nor3[j];
			MyMesh::HalfedgeHandle he1 = mesh->find_halfedge(vh1, vh2);
			MyMesh::HalfedgeHandle he2 = mesh->find_halfedge(vh2, vh3);
			MyMesh::HalfedgeHandle he3 = mesh->find_halfedge(vh3, vh4);
			MyMesh::HalfedgeHandle he4 = mesh->find_halfedge(vh4, vh1);
			bool b4 = mesh->is_boundary(he4);

			mesh->add_face(nor3[i], nor[i], nor[j], nor3[j]);
			
			he1 = mesh->find_halfedge(vh1, vh2);
			he2 = mesh->find_halfedge(vh2, vh3);
			he3 = mesh->find_halfedge(vh3, vh4);
			he4 = mesh->find_halfedge(vh4, vh1);
			bool b1 = mesh->is_boundary(he1);
			bool b2 = mesh->is_boundary(he2);
			bool b3 = mesh->is_boundary(he3);
			b4 = mesh->is_boundary(he4);
		}


		vector<MyMesh::VHandle> nor2;
		for (int i = 0; i < oneRing.size(); i++) {
			MyMesh::Point P = mesh->point(oneRing[i]);
			P = P + (10*directionVector);
			nor2.push_back(mesh->add_vertex(P));
		}
		for (int i = 0; i < nor2.size(); i++) {
			int j = 0;
			if (i + 1 < nor2.size()) {
				j = i + 1;
			}
			mesh->add_face(oneRing[i], nor2[i], nor2[j], oneRing[j]);
		}

		//order newOneRingArray
		//flip one ring if it has different orientation
		bool shouldFlip = false;
		if (nor.size() > 1) {
			MyMesh::Point P0 = mesh->point(nor[0]);
			MyMesh::Point P1 = mesh->point(nor[1]);
			MyMesh::Point Q0 = mesh->point(nor2[0]);
			MyMesh::Point Q1 = mesh->point(nor2[1]);
			CVector3 d(directionVector.values_);
			CVector3 A0(P0.values_);
			CVector3 A1(P1.values_);
			CVector3 B0(Q0.values_);
			CVector3 B1(Q1.values_);
			//calculate base with cross product
			CVector3 u = Cross(A0, A1);
			CVector3 v = Cross(B0, B1);
			//project base onto direction vector
			float d1 = Dot(u, d);
			float d2 = Dot(v, d);
			//if both have same sign both have same direction 
			shouldFlip = (d1 >= 0 && d2 < 0) || (d1 < 0 && d2 >= 0);
		}
		if (shouldFlip) {
			vector<MyMesh::VHandle> flipped;
			for (vector<MyMesh::VHandle>::reverse_iterator rit = nor.rbegin(); rit != nor.rend(); rit++) {
				flipped.push_back(*rit);
			}
			nor = flipped;
		}
		//find closest point
		vector<MyMesh::VertexHandle> newOneRing;
		MyMesh::Point P = mesh->point(nor2[0]);
		float minDist = 0;
		int index = 0;
		for (int i = 0; i < nor.size(); i++) {
			MyMesh::Point Q = mesh->point(nor[i]);
			float dist = (P - Q).norm();
			if (i == 0 || dist < minDist) {
				minDist = dist;
				index = i;
			}
		}
		//reorder array
		for (int i = 0; i < nor.size(); i++) {
			if (index == nor.size()) {
				index = 0;
			}
			newOneRing.push_back(nor[index]);
			index++;
		}
		//create new faces for the points
		for (int i = 0; i < newOneRing.size(); i++) {
			int j = 0;
			if (i + 1 < newOneRing.size()) {
				j = i + 1;
			}
			MyMesh::VHandle vh1 = nor2[i];
			MyMesh::VHandle vh2 = newOneRing[i];
			MyMesh::VHandle vh3 = newOneRing[j];
			MyMesh::VHandle vh4 = nor2[j];
			MyMesh::HalfedgeHandle he1 = mesh->find_halfedge(vh1, vh2);
			MyMesh::HalfedgeHandle he2 = mesh->find_halfedge(vh2, vh3);
			MyMesh::HalfedgeHandle he3 = mesh->find_halfedge(vh3, vh4);
			MyMesh::HalfedgeHandle he4 = mesh->find_halfedge(vh4, vh1);
			bool b2 = mesh->is_boundary(he2);
			bool b4 = mesh->is_boundary(he4);

			mesh->add_face(nor2[i], newOneRing[i], newOneRing[j], nor2[j]);
		}
	}*/
	if (parentBNPNode != NULL) {
		//order newOneRingArray
		vector<MyMesh::VHandle> oldOneRing = oneRingsOfPolyhedron.back();
		//flip one ring if it has different orientation
		bool shouldFlip = false;
		if (oldOneRing.size() > 1) {
			MyMesh::Point P0 = mesh->point(oldOneRing[0]);
			MyMesh::Point P1 = mesh->point(oldOneRing[1]);
			MyMesh::Point Q0 = mesh->point(oneRing[0]);
			MyMesh::Point Q1 = mesh->point(oneRing[1]);
			CVector3 d(directionVector.values_);
			CVector3 A0(P0.values_);
			CVector3 A1(P1.values_);
			CVector3 B0(Q0.values_);
			CVector3 B1(Q1.values_);
			//calculate base with cross product
			CVector3 u = Cross(A0, A1);
			CVector3 v = Cross(B0, B1);
			//project base onto direction vector
			float d1 = Dot(u, d);
			float d2 = Dot(v, d);
			//if both have same sign both have same direction 
			shouldFlip = (d1 >= 0 && d2 < 0) || (d1 < 0 && d2 >= 0);
		}
		if (shouldFlip) {
			vector<MyMesh::VHandle> flipped;
			flipVector(oldOneRing, flipped);
			oldOneRing = flipped;
		}
		//find closest point
		vector<MyMesh::VertexHandle> newOneRing;
		MyMesh::Point P = mesh->point(oneRing[0]);
		float minDist = 0;
		int index = 0;
		for (int i = 0; i < oldOneRing.size(); i++) {
			MyMesh::Point Q = mesh->point(oldOneRing[i]);
			float dist = (P - Q).norm();
			if (i == 0 || dist < minDist) {
				minDist = dist;
				index = i;
			}
		}
		//reorder array
		for (int i = 0; i < oldOneRing.size(); i++) {
			if (index == oldOneRing.size()) {
				index = 0;
			}
			newOneRing.push_back(oldOneRing[index]);
			index++;
		}
		//create new faces for the points
		for (int i = 0; i < newOneRing.size(); i++) {
			int j = 0;
			if (i + 1 < newOneRing.size()) {
				j = i + 1;
			}
			mesh->add_face(oneRing[i], newOneRing[i], newOneRing[j], oneRing[j]);
		}
	}
}

void SQMNode::extendMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector) {
	//create new points by translating parent points in the direction of the vector
	float d = -(position[0]*directionVector[0] + position[1]*directionVector[1] + position[2]*directionVector[2]);
	vector<MyMesh::Point> points;
	for (int i = 0; i < oneRing.size(); i++) {
		MyMesh::Point P = mesh->point(oneRing[i]);
		//OpenMesh::Vec3f u = (position - parentBNPNode->getPosition()).norm() * directionVector;
		float dist = fabsf(directionVector[0]*P[0] + directionVector[1]*P[1] + directionVector[2]*P[2] + d);
		OpenMesh::Vec3f u = directionVector * dist;
		MyMesh::Point Q = P + u;
		points.push_back(Q);
	}
	//map new points to node sphere
	for (int i = 0; i < points.size(); i++) {
		MyMesh::Point P = points[i];
		OpenMesh::Vec3f u(P[0], P[1], P[2]);
		u = (u - position).normalize();
		OpenMesh::Vec3f v = nodeRadius*u + position;
		MyMesh::Point Q(v[0], v[1], v[2]);
		points[i] = Q;
	}
	//insert new points into mesh
	vector<MyMesh::VertexHandle> newOneRing;
	for (int i = 0; i < points.size(); i++) {
		newOneRing.push_back(mesh->add_vertex(points[i]));
	}
	//create new faces for the points
	vector<MyMesh::FaceHandle> temp;
	for (int i = 0; i < newOneRing.size(); i++) {
		int j = 0;
		if (i + 1 < newOneRing.size()) {
			j = i + 1;
		}
		temp.push_back(mesh->add_face(oneRing[i], newOneRing[i], newOneRing[j], oneRing[j]));
	}
	//remember inseted points
	meshIntersectionVHandles = newOneRing;
	//continue joining BNPs
	nodes[0]->joinBNPs(mesh, this, newOneRing, directionVector);
}

void SQMNode::finishLeafeNode(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing) {
	//TODO finish as desired
	MyMesh::Point P(position[0], position[1], position[2]);
	MyMesh::VHandle vhandle = mesh->add_vertex(P);		
	for (int i = 0; i < oneRing.size(); i++) {	
		int j = 0;
		if (i + 1 < oneRing.size()) {
			j = i + 1;
		}
		mesh->add_face(oneRing[i], vhandle, oneRing[j]);
	}
}

#pragma endregion

#pragma region Utility

int SQMNode::getPointPositionInArrayOrAdd(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray) {
	for (int i = 0; i < vectorArray.size(); i++) {
		OpenMesh::Vec3f u = vectorArray[i];
		if (OpenMeshVec3fEqual(v, u))
			return i;
	}
	vectorArray.push_back(v);
	return vectorArray.size() - 1;
}

int SQMNode::getPointPositionInArray(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray) {
	for (int i = 0; i < vectorArray.size(); i++) {
		OpenMesh::Vec3f u = vectorArray[i];
		if (OpenMeshVec3fEqual(v, u))
			return i;
	}

	return -1;
}

SQMNode* SQMNode::getDescendantBranchNode(SQMNode* node) {
	if (node->isBranchNode()) {
		return node;
	}
	if (node->isLeafNode()) {
		return NULL;
	}

	return getDescendantBranchNode((*node->getDescendants())[0]);
}

#pragma endregion

#pragma region Utility Functions

template <typename T> int getPositionInArray(T& v, vector<T>& vectorArray) {
	for (int i = 0; i < vectorArray.size(); i++) {
		T u = vectorArray[i];
		if (equals<T>(u, v))
			return i;
	}

	return -1;
}

template <typename T> bool equals(T& a, T& b) {
	return (a == b);
}

template <> bool equals<MyMesh::VHandle>(MyMesh::VHandle& a, MyMesh::VHandle& b) {
	return a.idx() == b.idx();
}

template <typename T> void flipVector(vector<T>& toFlip, vector<T>& flipped) {
	for (vector<T>::reverse_iterator rit = toFlip.rbegin(); rit != toFlip.rend(); rit++) {
		flipped.push_back(*rit);
	}
}

bool lesser(MyMesh::FaceHandle& a, MyMesh::FaceHandle& b) {
	return a.idx() < b.idx();
}

bool validTriFace(vector<MyMesh::VHandle>& faceVHandles) {
	if (faceVHandles.size() == 3) {
		if (faceVHandles[0].idx() == faceVHandles[1].idx() ||
			faceVHandles[0].idx() == faceVHandles[2].idx() ||
			faceVHandles[1].idx() == faceVHandles[2].idx()) {
				return false;
		}
		return true;
	}

	return false;
}

bool rayTriangleIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f V0, OpenMesh::Vec3f V1, OpenMesh::Vec3f V2, float &t_param) {
	//point on triagle T(u, v) = (1 - u - v)*V0 + u*V1 + v*V2;
	//ray: O + d
	//ray triangle intersection: O + d = (1 - u - v)*V0 + u*V1 + v*V2 -> 3 equations solved by Cramer rule
	//triangle edges
	OpenMesh::Vec3f edge1 = V1 - V0;
	OpenMesh::Vec3f edge2 = V2 - V0;
	//calculation of determinant used to calculate u parameter
	OpenMesh::Vec3f pvec = cross(ray_direction, edge2);
	//if determinant is near zero ray lines in plane of triangle
	float det = dot(edge1, pvec);
	if (det > -FLOAT_ZERO && det < FLOAT_ZERO)
		return false;
	float inv_det = 1.0 / det;
	//distance from V0 to ray origin
	OpenMesh::Vec3f tvec = ray_origin - V0;
	//calculate u parameter and test bounds
	float u = dot(tvec, pvec) * inv_det;
	if (u < 0 || u > 1.0)
		return false;
	//prepare to test v parameter
	OpenMesh::Vec3f qvec = cross(tvec, edge1);
	//calculate v parameter and test bounds
	float v = dot(ray_direction, qvec) * inv_det;
	if (v < 0.0 || (u + v) > 1.0)
		return false;
	//calculate t ray intersects triangle
	float t = dot(edge2, qvec) * inv_det;
	//not in the direction of ray
	if (t < BIAS)
		return false;

	t_param = t;
	return true;
}

bool raySphereIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f sphere_center, float sphere_radius, float &t_param) {
	OpenMesh::Vec3f oo = ray_origin - sphere_center;
	// A = v^2
	float A = dot(ray_direction, ray_direction);
	// -B = v^T * (o_2 - o_1)
	float B = -2.0 * dot(ray_direction, oo);
	// C = (o_2 - o_1)^2 - r^2
	float C = dot(oo, oo) - sphere_radius * sphere_radius;
	// Discriminant
	float D = B * B - 4.0f * A * C;
	// No collision
	if (D < 0) 
		return false; 

	float sD = sqrtf(D);
	float t1 = 0.5 * (B + sD) / A;
	//if (t1 < Ray.Bias) t1 = Double.MaxValue;
	float t2 = 0.5 * (B - sD) / A;
	//if (t2 < Ray.Bias) t2 = Double.MaxValue;
	float t = (t1 > 0) ? t1 : t2;
	if (t < 0)
		return false;

	t_param = t;
	return true;
}

OpenMesh::Vec3i flipVec3i(OpenMesh::Vec3i& v) {
	return OpenMesh::Vec3i(v[2], v[1], v[0]);
}

#pragma endregion