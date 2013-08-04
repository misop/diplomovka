#include "stdafx.h"
#include "SQMNode.h"
#include <algorithm>
#include "SphereDelaunay.h"
#include "FloatArithmetic.h"
#include "Laplacian.h"

#define BIAS 0.1

#pragma region Init

SQMNode::SQMNode(void) {
	position = OpenMesh::Vec3f(0, 0, 0);
	nodeRadius = 10;
	id = "0";
	parent = NULL;
	polyhedron = NULL;
	tessLevel = 1;
}

SQMNode::SQMNode(SkeletonNode &node, SQMNode* newParent) : parent(newParent) {
	if (parent == NULL) {
		id = "0";
	} else {
		id = parent->getId();
		string s = to_string(parent->getNumOfChilds());
		id += "-" + s;
	}
	position = OpenMesh::Vec3f(node.point.x, node.point.y, node.point.z);
	//nodeRadius = (float)(rand()%100)/100*10 + 5;
	nodeRadius = 10;
	tessLevel = 3;
	for (int i = 0; i < node.nodes.size(); i++) {
		SQMNode *newNode = new SQMNode(*node.nodes[i], this);
		nodes.push_back(newNode);
	}
	polyhedron = NULL;
}


SQMNode::SQMNode(SQMNode &node) {
	parent = NULL;
	polyhedron = NULL;
	id = node.id;
	nodeRadius = node.getNodeRadius();
	tessLevel = node.getTessLevel();
	position = node.getPosition();
	axisAngle = node.getAxisAngle();
	vector<SQMNode *> *childs = node.getNodes();
	for (int i = 0; i < childs->size(); i++) {
		SQMNode *childRef = (*childs)[i];
		SQMNode *child = new SQMNode(*childRef);
		child->setParent(this);
		nodes.push_back(child);
	}
}

SQMNode::~SQMNode(void) {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	if (polyhedron != NULL) delete polyhedron;
}

#pragma endregion

#pragma region Getters

string SQMNode::getId() {
	return id;
}

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

glm::vec3 SQMNode::getPosition_glm() {
	return glm::vec3(position[0], position[1], position[2]);
}

vector<SQMNode*>* SQMNode::getNodes() {
	return &nodes;
}

MyTriMesh* SQMNode::getPolyhedron() {
	return polyhedron;
}

vector<SQMNode*>* SQMNode::getDescendants() {
	return &nodes;
}

vector<MyTriMesh::VertexHandle>* SQMNode::getIntersectionVHandles() {
	return &intersectionVHandles;
}

float SQMNode::getNodeRadius() {
	return nodeRadius;
}

float SQMNode::getTessLevel() {
	return tessLevel;
}

Quaternion SQMNode::getAxisAngle() {
	return axisAngle;
}

SQMNode* SQMNode::getParent() {
	return parent;
}

int SQMNode::getNumOfChilds() {
	return nodes.size();
}

float SQMNode::getX() {
	return position[0];
}

float SQMNode::getY() {
	return position[1];
}

float SQMNode::getZ() {
	return position[2];
}

#pragma endregion

#pragma region Setters

void SQMNode::setParent(SQMNode *node) {
	parent = node;
}

void SQMNode::setNodeRadius(float newNodeRadius) {
	nodeRadius = newNodeRadius;
}

void SQMNode::setTessLevel(float newTessLevel) {
	tessLevel = newTessLevel;
}

void SQMNode::setPosition(OpenMesh::Vec3f newPosition) {
	position = newPosition;
}

void SQMNode::setPosition(float x, float y, float z) {
	position = OpenMesh::Vec3f(x, y, z);
}

void SQMNode::addDescendant(SQMNode* node) {
	nodes.push_back(node);
}

void SQMNode::rotatePosition(Quaternion q, CVector3 offset) {
	CVector3 pos(position.values_);
	pos = pos - offset;
	pos = QuaternionRotateVector(q, pos);
	pos = pos + offset;
	position = OpenMesh::Vec3f(pos.x, pos.y, pos.z);
}

void SQMNode::addDescendant(float x, float y, float z) {
	SkeletonNode *skeletonNode = new SkeletonNode(x, y, z);
	SQMNode *node = new SQMNode(*skeletonNode, this);
	addDescendant(node);
	delete skeletonNode;
}

void SQMNode::setX(float newX) {
	position[0] = newX;
}

void SQMNode::setY(float newY) {
	position[1] = newY;
}

void SQMNode::setZ(float newZ) {
	position[2] = newZ;
}

#pragma endregion

#pragma region Export

SkeletonNode* SQMNode::exportToSkeletonNode() {
	SkeletonNode* node = new SkeletonNode(position[0], position[1], position[2]);
	for (int i = 0; i < nodes.size(); i++) {
		node->addChild(nodes[i]->exportToSkeletonNode());
	}
	return node;
}

#pragma endregion

#pragma region Skeleton Straightening

void SQMNode::straightenSkeleton(OpenMesh::Vec3f *lineVector) {
	axisAngle = Quaternion();
	oldPosition = position;
	if (lineVector != NULL && !parent->isBranchNode()) {//straighten self
		SQMNode *ancestor = getAncestorBranchNode(this);
		if (ancestor != NULL) {
			rotatePosition(QuaternionOpposite(ancestor->getAxisAngle()), CVector3(ancestor->getParent()->getPosition().values_));
		}
		//translate parent to 0,0,0
		OpenMesh::Vec3f newPosition = position - parent->getPosition();
		CVector3 oldPos(newPosition.values_);
		//roatate
		float len = newPosition.length();
		newPosition = len*(*lineVector);
		CVector3 newPos(newPosition.values_);
		Quaternion quaternion = SQMQuaternionBetweenVectors(newPos, oldPos);
		//translate back by parent
		newPosition = newPosition + parent->getPosition();
		//setup
		axisAngle = quaternion;
		position = newPosition;
	}
	if (this->isBranchNode()) {//if this is branch node recalculate new vectors and intersections
		for (int i = 0; i < nodes.size(); i++) {//specifical order parent intersection needs to be last in vector
			if (parent != NULL) {
				nodes[i]->rotateSelfAndDescendants(QuaternionOpposite(axisAngle), CVector3(parent->getPosition().values_));
			}
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

#pragma endregion

#pragma region BNP generation

void SQMNode::calculateConvexHull() {
	vector<OpenMesh::Vec3i> triangles;
	vector<OpenMesh::Vec3f> translatedIntersections(intersections.size());
	//in triangularization the points are normalized
	centerOfMass = OpenMesh::Vec3f(0, 0, 0);
	for (int i = 0; i < intersections.size(); i++) {
		translatedIntersections[i] = intersections[i] - position;
		centerOfMass += intersections[i];
	}
	centerOfMass /= intersections.size();
	Delaunay_on_sphere(translatedIntersections, triangles);
	//Delaunay_on_sphere(intersections, triangles);
	triangles2 = triangles;
	createPolyhedra(triangles);
}

void SQMNode::createPolyhedra(vector<OpenMesh::Vec3i> triangles) {
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

		normals.push_back(normal);		
		centers.push_back(center);
	}
	normals2 = normals;
	centers2 = centers;
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
	}
	//create OpenMesh mesh from indexed face
	openMeshFromIdexedFace(vertices, faces);
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
	polyhedron = new MyTriMesh();
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
	writeTriMesh(polyhedron);
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

void SQMNode::subdividePolyhedra(SQMNode* parentBranchNode, int count, SQMSmoothingAlgorithm algorithm) {
	vector<SQMNode*> branchingNodes;
	for (int i = 0; i < nodes.size(); i++) {
		branchingNodes.push_back(getDescendantBranchNode(nodes[i]));
	}
	map<int, LIENeedEntry> lieMap;
	fillLIEMap(count, lieMap, branchingNodes);
	if (algorithm == SQMQuaternionSmoothing) {
		smoothLIEs(lieMap);
	}
	splitLIEs(lieMap, algorithm);
	//take care of the rest
	for (int i = 0; i < branchingNodes.size(); i++)	{
		SQMNode* branchingNode = branchingNodes[i];
		if (branchingNode != NULL) {
			MyTriMesh::VHandle vhandle = intersectionVHandles[i];
			//if we needed some they have been added if we had more points than descendant he needs to add them
			int missingPoints = verticeDifferenceFatherSon(this, branchingNode, vhandle);
			missingPoints = (missingPoints < 0) ? -missingPoints : 0;
			branchingNode->subdividePolyhedra(this, missingPoints, algorithm);
		}
	}
}

void SQMNode::fillLIEMap(int parentNeed, std::map<int, LIENeedEntry>& lieMap, std::vector<SQMNode*>& branchingNodes) {
	int stop = nodes.size();
	if (parent != NULL)
		stop++;
	for (int i = 0; i < stop; i++) {
		//count of need to split edges
		MyTriMesh::VHandle vhandle = intersectionVHandles[i];
		int need = 0;
		if (i == nodes.size()) {//parent intersection
			if (parentNeed > 0) {
				need = parentNeed;
			} else {
				need = NO_SPLITTING;
			}
		} else {
			SQMNode* branchingNode = branchingNodes[i];
			need = verticeDifferenceFatherSon(this, branchingNode, vhandle);
			//if father has more vertices than its son there is no need to split
			need = max(0, need);
			//if son is not branch node he can be splited as much as needed
			if (branchingNode == NULL)
				need = UNLIMITED_SPLITTING;
		}

		//we could check for uncreated LIEs only but the list could be long and thus take more time that creating new one
		vector<LIE> verticeLIEs;
		//get first halfedge
		MyTriMesh::HHandle heh = polyhedron->voh_begin(vhandle).current_halfedge_handle();
		heh = polyhedron->next_halfedge_handle(heh);
		//get first opposing vhandle
		MyTriMesh::VHandle ovhandle = oppositeVHandle(heh);
		//check previous for first ocurence and remember first heh
		MyTriMesh::HHandle prevHeh = prevLink(heh);
		while (oppositeVHandle(prevHeh) == ovhandle) {
			heh = prevHeh;
			prevHeh = prevLink(heh);
		}
		//current half edge and opposite handle
		MyTriMesh::HHandle cheh = nextLink(heh);
		MyTriMesh::VHandle covh = ovhandle;
		vector<MyTriMesh::EdgeHandle> edges;
		edges.push_back(polyhedron->edge_handle(heh));
		//for quaternion
		MyTriMesh::VHandle firstLieVertex = polyhedron->to_vertex_handle(polyhedron->opposite_halfedge_handle(heh));
		MyTriMesh::VHandle secondLieVertex = polyhedron->to_vertex_handle(heh);
		MyTriMesh::VHandle lastLieVertex = polyhedron->to_vertex_handle(heh);
		MyTriMesh::HHandle firstLieHEdge = heh;
		MyTriMesh::HHandle lastLieHEdge = heh;
		CVector3 offset = CVector3(position.values_);
		//CVector3 offset = CVector3(centerOfMass.values_);

		bool good = true;
		while (good) {
			good = (heh != cheh);
			//collect all halfedges of LIE
			ovhandle = oppositeVHandle(cheh);
			if (ovhandle == covh) { //if new collect all edges incident with oposing vhandle else just pass them
				edges.push_back(polyhedron->edge_handle(cheh));
				if (firstLieVertex.idx() == -1) {
					firstLieVertex = polyhedron->to_vertex_handle(polyhedron->opposite_halfedge_handle(cheh));
					secondLieVertex = polyhedron->to_vertex_handle(cheh);
					firstLieHEdge = cheh;
				}
				lastLieVertex = polyhedron->to_vertex_handle(cheh);
				lastLieHEdge = cheh;

				cheh = nextLink(cheh);
			} else {//add LIE to map and table of LIEs
				LIE lie(vhandle, covh);
				lie.edges = edges;
				lie.vertice1 = i;
				lie.vertice2 = getPositionInArray(covh, intersectionVHandles);
				lie.firstHHandle = firstLieHEdge;
				lie.lastHHandle = lastLieHEdge;
				CVector3 start = CVector3(polyhedron->point(firstLieVertex).values_) - offset;
				CVector3 dest = CVector3(polyhedron->point(lastLieVertex).values_) - offset;
				CVector3 P = CVector3(polyhedron->point(secondLieVertex).values_) - offset;
				//CVector3 axis = Normalize(Cross(P - start, dest - start));
				//default axis between start and P because they cannot be colinear and fit the one-ring
				/*CVector3 axis;
				float angle = Dot(Normalize(start), Normalize(dest));
				if (equal(fabs(angle), 1)) {
					axis = Normalize(Cross(start, P));
				} else {
					axis = Normalize(Cross(start, dest));
				}*/
				CVector3 axis = Normalize(Cross(start, P));
				lie.quaternion = SQMQuaternionBetweenVectorsWithAxis(start, dest, axis);
				verticeLIEs.push_back(lie);

				//clearing
				firstLieVertex = MyTriMesh::VHandle(-1);
				edges.clear();
				covh = ovhandle;
			}
		}

		//crate map item
		LIENeedEntry entry(vhandle, need);
		entry.lies = verticeLIEs;
		lieMap.insert(pair<int, LIENeedEntry>(i, entry));
	}
}

void SQMNode::splitLIEs(std::map<int, LIENeedEntry>& lieMap, SQMSmoothingAlgorithm algorithm) {
	bool parentFirst = (parent != NULL);
	for (int i = 0; i < nodes.size(); i++) {
		//parent should go first
		if (parentFirst) {
			i = nodes.size();
		}
		//get entry
		LIENeedEntry entry = lieMap.at(i);
		//if  need <= 0 skip
		while (entry.need > 0) {
			//else get LIE with greatest need
			LIE bestLIE;
			int bestNeed;
			int lieIndex = -1;
			for (int j = 0; j < entry.lies.size(); j++) {
				LIE lie = entry.lies[j];
				int otherIndex = lie.otherVerticeIndex(i);
				int lieNeed = lieMap.at(otherIndex).need;

				if (lieNeed == NO_SPLITTING) continue;

				if ((lieIndex == -1) || (lieNeed != 0 && bestNeed < lieNeed) || (bestNeed == 0 && lieNeed == UNLIMITED_SPLITTING) || (bestNeed == lieNeed && lie.refined < bestLIE.refined)) {
					bestNeed = lieNeed;
					bestLIE = lie;
					lieIndex = j;
				}
			}
			//split edge and adjust LIEs
			splitLIE(bestLIE, lieMap, i, lieIndex, algorithm);
			entry = lieMap.at(i);
		}
		if (parentFirst) {
			entry.need = -2;
			lieMap.at(i) = entry;
			i = 0;
			parentFirst = false;
		}
	}
}

void SQMNode::splitLIE(LIE lie, std::map<int, LIENeedEntry>& lieMap, int entryIndex, int lieIndex, SQMSmoothingAlgorithm algorithm) {
	LIE newLie = splitLIEEdge(lie);
	if (algorithm == SQMQuaternionSmoothing) {
		smoothLIE(newLie);
	}
	//decrease need for both vertices
	LIENeedEntry entry1 = lieMap.at(entryIndex);
	LIENeedEntry entry2 = lieMap.at(lie.otherVerticeIndex(entryIndex));
	//we always split first 
	entry1.lies[lieIndex] = newLie;
	int otherIndex = getPositionInArray(newLie, entry2.lies);
	entry2.lies[otherIndex] = newLie;
	//only if they are bigger than one
	if (entry1.need > 0)
		entry1.need--;
	if (entry2.need > 0)
		entry2.need--;
	lieMap.at(lie.vertice1) = entry1;
	lieMap.at(lie.vertice2) = entry2;
	if (algorithm == SQMOneRingLaplacianSmoothing || algorithm == SQMValencyLaplacianSmoothing) {
		laplacianSMoothing(algorithm);
	}
}

LIE SQMNode::splitLIEEdge(LIE lie) {
	MyTriMesh::EHandle eh = lie.edges[0];
	MyTriMesh::EHandle newEh = splitEdgeInHalfAndReturnNewEdge(eh);
	lie.edges.insert(lie.edges.begin()+1, newEh);
	lie.refined = lie.refined + 1;
	return lie;
}

MyTriMesh::EHandle SQMNode::splitEdgeInHalfAndReturnNewEdge(MyTriMesh::EdgeHandle eh) {
	MyTriMesh::HalfedgeHandle heh0 = polyhedron->halfedge_handle(eh, 0);
	MyTriMesh::HalfedgeHandle heh1 = polyhedron->halfedge_handle(eh, 1);

	MyTriMesh::Point p0 = polyhedron->point(polyhedron->to_vertex_handle(heh0));
	MyTriMesh::Point p1 = polyhedron->point(polyhedron->to_vertex_handle(heh1));

	MyTriMesh::Point x = 0.5*p0 + 0.5*p1;
	MyTriMesh::VertexHandle vh = polyhedron->add_vertex(x);

	polyhedron->split(eh, vh);

	MyTriMesh::VHandle v2 = polyhedron->to_vertex_handle(heh1);
	for (MyTriMesh::VOHIter voh_it = polyhedron->voh_begin(vh); voh_it != polyhedron->voh_end(vh); ++voh_it) {
		MyTriMesh::HHandle heh = voh_it.current_halfedge_handle();
		if (polyhedron->to_vertex_handle(heh) == v2) {
			return polyhedron->edge_handle(heh);
		}
	}

	return polyhedron->InvalidEdgeHandle;
}

#pragma region Smoothing

void SQMNode::smoothLIE(LIE lie) {
	//angle and axis of rotation
	float div = lie.edges.size();
	float alfa = acos(lie.quaternion.s) * 2.0 / div;//lie.quaternion.s / div;
	float partAlfa = alfa;
	CVector3 axis = Normalize(CVector3(lie.quaternion.i, lie.quaternion.j, lie.quaternion.k));
	CVector3 offset = CVector3(position.values_);
	//CVector3 offset = CVector3(centerOfMass.values_);
	//get first vertice
	MyTriMesh::HHandle heh = lie.firstHHandle;
	CVector3 v = CVector3(polyhedron->point(polyhedron->to_vertex_handle(polyhedron->opposite_halfedge_handle(heh))).values_) - offset;
	while (heh != lie.lastHHandle) {
		Quaternion q = QuaternionFromAngleAxis(alfa, axis);
		MyTriMesh::VHandle vh = polyhedron->to_vertex_handle(heh);
		CVector3 u = QuaternionRotateVector(q, v);
		u = u + offset;
		MyTriMesh::Point P(u.x, u.y, u.z);
		polyhedron->set_point(vh, P);

		alfa += partAlfa;
		heh = nextLink(heh);
		vh = polyhedron->to_vertex_handle(heh);
	}
}

void SQMNode::smoothLIEs(map<int, LIENeedEntry> lieMap) {
	for (map<int, LIENeedEntry>::iterator it = lieMap.begin(); it != lieMap.end(); it++) {
		LIENeedEntry lieNeed = it->second;
		for (int i = 0; i < lieNeed.lies.size(); i++) {
			smoothLIE(lieNeed.lies[i]);
		}
	}
}

void SQMNode::laplacianSMoothing(SQMSmoothingAlgorithm algorithm) {
	//convert mesh
	MeshGraph meshGraph = MeshGraph();
	mesh2graph(meshGraph, algorithm);
	//smooth mesh
	computeLaplacian(&meshGraph);
	//translate vertices
	//replace vertices in mesh
	recalculateSmoothedVertices(meshGraph);
}

void SQMNode::mesh2graph(MeshGraph& meshGraph, SQMSmoothingAlgorithm algorithm) {
	if (algorithm == SQMOneRingLaplacianSmoothing) {
		mesh2graphOneRingWeighted(meshGraph);
	} else if (algorithm == SQMValencyLaplacianSmoothing) {
		mesh2graphValencyWeighted(meshGraph);
	}
}


void SQMNode::mesh2graphValencyWeighted(MeshGraph& meshGraph) {
	int n_vertices = polyhedron->n_vertices();
	int n_faces = polyhedron->n_faces();
	int n_edges = polyhedron->n_edges();

	meshGraph.numOfFaces = n_faces;
	meshGraph.numOfVertices = n_vertices;
	meshGraph.wL = 1;
	meshGraph.wH = new float[n_vertices];

	CVector3 *vertices = new CVector3[n_vertices];
	int idx = 0;
	//get the vertices
	for (MyTriMesh::VertexIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) 
	{
		MyTriMesh::VHandle vh = v_it.handle();
		MyTriMesh::Point P = polyhedron->point(vh);
		vertices[idx] = CVector3(polyhedron->point(vh).values_);
		//valency weighted
		int count = 0;
		for (MyTriMesh::VVIter vv_it = polyhedron->vv_begin(v_it.handle()); vv_it != polyhedron->vv_end(v_it.handle()); ++vv_it) {
			count++;
		}
		meshGraph.wH[idx] = count;
		idx++;
	}

	meshGraph.pVerts = vertices;
	meshGraph.E = TNT::Array2D<bool>(n_vertices, n_vertices, false);

	for (MyTriMesh::EdgeIter e_it = polyhedron->edges_begin(); e_it != polyhedron->edges_end(); ++e_it) {
		MyTriMesh::HalfedgeHandle heh0 = polyhedron->halfedge_handle(e_it.handle(), 0);
		MyTriMesh::HalfedgeHandle heh1 = polyhedron->halfedge_handle(e_it.handle(), 1);
		int i = polyhedron->to_vertex_handle(heh0).idx();
		int j = polyhedron->to_vertex_handle(heh1).idx();
		//register to map
		meshGraph.E[i][j] = true;
		meshGraph.E[j][i] = true;
	}
}

void SQMNode::mesh2graphOneRingWeighted(MeshGraph& meshGraph) {
	int n_vertices = polyhedron->n_vertices();
	int n_faces = polyhedron->n_faces();
	int n_edges = polyhedron->n_edges();

	meshGraph.numOfFaces = n_faces;
	meshGraph.numOfVertices = n_vertices;
	meshGraph.wL = 1;
	meshGraph.wH = new float[n_vertices];

	CVector3 *vertices = new CVector3[n_vertices];
	int idx = 0;
	//avarage face area
	float area = 0;
	for (MyTriMesh::FaceIter f_it = polyhedron->faces_begin(); f_it != polyhedron->faces_end(); ++f_it)	{
		MyTriMesh::FVIter fv_it = polyhedron->fv_begin(f_it.handle());
		MyTriMesh::Point A = polyhedron->point(fv_it.handle());
		++fv_it;
		MyTriMesh::Point B = polyhedron->point(fv_it.handle());
		++fv_it;
		MyTriMesh::Point C = polyhedron->point(fv_it.handle());

		OpenMesh::Vec3f u = B - A;
		OpenMesh::Vec3f v = C - A;
		OpenMesh::Vec3f w = cross(u, v);
		area += (w.norm() / 2.0);
	}
	area /= (float)n_faces;
	//formula from paper
	meshGraph.wL = sqrtf(area)/1000.0;
	//get the vertices
	for (MyTriMesh::VertexIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) 
	{
		MyTriMesh::VHandle vh = v_it.handle();
		MyTriMesh::Point P = polyhedron->point(vh);
		vertices[idx] = CVector3(polyhedron->point(vh).values_);
		//one ring area weighted 1.0 from paper
		meshGraph.wH[idx] = 1.0;
		idx++;
	}

	meshGraph.pVerts = vertices;
	meshGraph.E = TNT::Array2D<bool>(n_vertices, n_vertices, false);

	for (MyTriMesh::EdgeIter e_it = polyhedron->edges_begin(); e_it != polyhedron->edges_end(); ++e_it) {
		MyTriMesh::HalfedgeHandle heh0 = polyhedron->halfedge_handle(e_it.handle(), 0);
		MyTriMesh::HalfedgeHandle heh1 = polyhedron->halfedge_handle(e_it.handle(), 1);
		int i = polyhedron->to_vertex_handle(heh0).idx();
		int j = polyhedron->to_vertex_handle(heh1).idx();
		//register to map
		meshGraph.E[i][j] = true;
		meshGraph.E[j][i] = true;
	}
}

void SQMNode::recalculateSmoothedVertices(MeshGraph& meshGraph) {
	//update vertices with new position
	int index = 0;
	for (MyTriMesh::VertexIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) 
	{
		MyTriMesh::VHandle vh = v_it.handle();
		//setup point
		CVector3 vec = meshGraph.pVerts[index];
		MyTriMesh::Point P = MyTriMesh::Point(vec.x, vec.y, vec.z);//polyhedron->point(vh);
		polyhedron->set_point(vh, P);
		index++;
	}
	//translate vertices on the sphere
	//not alwys in the center!!!!
	//CVector3 offset(position.values_);
	/*CVector3 offset(centerOfMass.values_);
	for (MyTriMesh::VertexIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) 
	{
		MyTriMesh::VHandle vh = v_it.handle();
		CVector3 P(polyhedron->point(vh).values_);
		P = Normalize(P - offset);
		P = P * nodeRadius;
		P = P + offset;
		MyTriMesh::Point Q(P.x, P.y, P.z);
		polyhedron->set_point(vh, Q);
	}*/
}

#pragma endregion

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
	for (MyTriMesh::VIter v_it = polyhedron->vertices_begin(); v_it != polyhedron->vertices_end(); ++v_it) {
		MyTriMesh::VHandle vhandle = v_it.handle();
		int position = getPositionInArray<MyTriMesh::VHandle>(vhandle, intersectionVHandles);
		if (position == -1) {
			MyTriMesh::Point P = polyhedron->point(vhandle);
			MyMesh::VHandle vhandle = mesh->add_vertex(P);
			addedVHandles.push_back(vhandle);
			meshVhandlesToRotate.push_back(vhandle);
		} else {
			addedVHandles.push_back(vhandle);
			//collect one ring indexis
			for (MyTriMesh::VVIter vv_it = polyhedron->vv_begin(vhandle); vv_it != polyhedron->vv_end(vhandle); ++vv_it) {
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
	if (parentBNPNode != NULL) {
		//order newOneRingArray
		vector<MyMesh::VHandle> oldOneRing = oneRingsOfPolyhedron.back();
		//flip one ring if it has different orientation
		bool shouldFlip = sameOneRingOrientation(mesh, oldOneRing, oneRing, position, parentBNPNode->getPosition(), directionVector);
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
			//float dist = (P - Q).norm();
			OpenMesh::Vec3f vv = -directionVector.normalized();
			float dist = dot((P - Q).normalized(), vv);
			if (i == 0 || dist > minDist) {
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
		MyMesh::VHandle vhandle = mesh->add_vertex(points[i]);
		newOneRing.push_back(vhandle);
		meshVhandlesToRotate.push_back(vhandle);
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
	meshVhandlesToRotate.push_back(vhandle);
	for (int i = 0; i < oneRing.size(); i++) {	
		int j = 0;
		if (i + 1 < oneRing.size()) {
			j = i + 1;
		}
		mesh->add_face(oneRing[i], vhandle, oneRing[j]);
	}
}

#pragma endregion

#pragma region Final Vertex Placement

void SQMNode::rotateBack(MyMesh *mesh) {
	//start with sons
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->rotateBack(mesh);
	}
	//rotate
	if (parent != NULL) {
		OpenMesh::Vec3f parentPosition = parent->getPosition();
		CVector3 parentPos(parentPosition.values_);
		for (int i = 0; i < meshVhandlesToRotate.size(); i++) {
			MyMesh::VHandle vhandle = meshVhandlesToRotate[i];
			MyMesh::Point P = mesh->point(vhandle);
			CVector3 v(P.values_);
			//translate rotate translate
			v = v - parentPos;
			v = QuaternionRotateVector(axisAngle, v);
			v = v + parentPos;
			SQMNode* ancestor = lastBranchNodeInChain(this);
			if (parent->isBranchNode() && ancestor != NULL && ancestor->getParent() != NULL) {
				CVector3 offset(ancestor->getParent()->getPosition().values_);
				v = v - offset;
				v = QuaternionRotateVector(ancestor->getAxisAngle(), v);
				v = v + offset;
			}
			P[0] = v.x;
			P[1] = v.y;
			P[2] = v.z;
			mesh->set_point(vhandle, P);
		}
	}
	position = oldPosition;
}

#pragma endregion

#pragma region BNP Tesselation

void SQMNode::getMeshTessLevel(std::vector<float> &tessLevels) {
	//each node stores vertices for rotation that belong to him we just need to push tess level for every vertex
	for (int i = 0; i < meshVhandlesToRotate.size(); i++) {
		tessLevels.push_back(tessLevel);
	}
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->getMeshTessLevel(tessLevels);
	}
}

void SQMNode::getMeshTessData(std::vector<float> &tessLevels, std::vector<float> &nodePositions) {
	for (int i = 0; i < meshVhandlesToRotate.size(); i++) {
		tessLevels.push_back(tessLevel);
		nodePositions.push_back(position[0]);
		nodePositions.push_back(position[1]);
		nodePositions.push_back(position[2]);
		nodePositions.push_back(nodeRadius);
	}
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->getMeshTessData(tessLevels, nodePositions);
	}
}

#pragma endregion

#pragma region Utility

void SQMNode::rotateSelfAndDescendants(Quaternion q, CVector3 offset) {
	if (this->isBranchNode()) {
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->rotateSelfAndDescendants(q, offset);
		}
	}
	this->rotatePosition(q, offset);
}

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

SQMNode* SQMNode::getAncestorBranchNode(SQMNode* node) {
	if (node->isBranchNode()) {
		return node;
	}
	if (node->parent == NULL) {
		return NULL;
	}

	return getDescendantBranchNode(node->parent);
}


MyTriMesh::HalfedgeHandle SQMNode::nextLink(MyTriMesh::HalfedgeHandle heh) {
	//get on the side of the next triangle
	//switch to next triangle half edges
	//return halfedge on the link
	return polyhedron->next_halfedge_handle(polyhedron->opposite_halfedge_handle(polyhedron->next_halfedge_handle(heh)));
}

MyTriMesh::HalfedgeHandle SQMNode::prevLink(MyTriMesh::HalfedgeHandle heh) {
	//two times next to get to the side of previous triangle
	//switch to previous triangle half edges
	//two times next halfedge to get the halfedge on the link
	return polyhedron->next_halfedge_handle(polyhedron->next_halfedge_handle(polyhedron->opposite_halfedge_handle(polyhedron->next_halfedge_handle(polyhedron->next_halfedge_handle(heh)))));
}

MyTriMesh::VHandle SQMNode::oppositeVHandle(MyTriMesh::HalfedgeHandle heh) {
	//switch to opposite triangle
	//get the next halfedge
	//get the vertex it points to
	return polyhedron->to_vertex_handle(polyhedron->next_halfedge_handle(polyhedron->opposite_halfedge_handle(heh)));
}


#pragma endregion

#pragma region Utility Functions

#pragma region Template Functions

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

#pragma endregion

#pragma region Mesh Functions

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

OpenMesh::Vec3i flipVec3i(OpenMesh::Vec3i& v) {
	return OpenMesh::Vec3i(v[2], v[1], v[0]);
}

bool sameOneRingOrientation(MyMesh* mesh, vector<MyMesh::VHandle>& oneRing, vector<MyMesh::VHandle>& oneRing2, OpenMesh::Vec3f& center1, OpenMesh::Vec3f& center2, OpenMesh::Vec3f& direction) {
	if (oneRing.size() > 1 && oneRing2.size() > 1) {
		//offset the position so we got vectors from O(0,0,0)
		MyMesh::Point P0 = mesh->point(oneRing[0]) - center1;
		MyMesh::Point P1 = mesh->point(oneRing[1]) - center1;
		MyMesh::Point Q0 = mesh->point(oneRing2[0]) - center2;
		MyMesh::Point Q1 = mesh->point(oneRing2[1]) - center2;
		CVector3 d(direction.values_);
		CVector3 A0(P0.values_);
		//projecting vectors onto the plane given by directionVector
		//it is enough to substract the projection of A0 onto d sience that is the distance between A0 and the plane
		A0 = A0 - d*Dot(A0, d);
		CVector3 A1(P1.values_);
		A1 = A1 - d*Dot(A1, d);
		CVector3 B0(Q0.values_);
		B0 = B0 - d*Dot(B0, d);
		CVector3 B1(Q1.values_);
		B1 = B1 - d*Dot(B1, d);
		//calculate base with cross product
		CVector3 u = Cross(A0, A1);
		CVector3 v = Cross(B0, B1);
		//project base onto direction vector
		float d1 = Dot(u, d);
		float d2 = Dot(v, d);
		//if both have same sign both have same direction 
		return (d1 >= 0 && d2 < 0) || (d1 < 0 && d2 >= 0);
	}
	return false;
}

int inLIEs(std::vector<LIE>& LIEs, MyTriMesh::VHandle vh1, MyTriMesh::VHandle vh2) {
	for (int i = 0; i < LIEs.size(); i++) {
		LIE lie = LIEs[i];
		if (lie.containsVertices(vh1, vh2))
			return i;
	}

	return -1;
}

#pragma endregion

#pragma region SQMNode Functions

int verticeDifferenceFatherSon(SQMNode* father, SQMNode* son, MyTriMesh::VHandle vhandle) {
	if (son != NULL) {			
		//get intersection vhandle its the last one from parent
		MyTriMesh::VHandle descendantVHandle = son->getIntersectionVHandles()->back();
		//get number of needed vertices
		int vhandleCount = 0;
		MyTriMesh* polyhedron = father->getPolyhedron();
		for (MyTriMesh::VVIter vvit = polyhedron->vv_begin(vhandle); vvit != polyhedron->vv_end(vhandle); ++vvit) {
			vhandleCount++;
		}
		int descendantVHandleCount = 0;
		polyhedron = son->getPolyhedron();
		for (MyTriMesh::VVIter vvit = polyhedron->vv_begin(descendantVHandle); vvit != polyhedron->vv_end(descendantVHandle); ++vvit) {
			descendantVHandleCount++;
		}
		return descendantVHandleCount - vhandleCount;
	}

	return 0;
}

SQMNode* lastBranchNodeInChain(SQMNode* node) {
	//nowhere to go
	if (node == NULL) return NULL;
	//get last branch node in a chain of branching nodes
	if (node->isBranchNode()) {
		if (node->getParent() != NULL && node->getParent()->isBranchNode()) {//parent exists and is a branch node
			return lastBranchNodeInChain(node->getParent());//continue search for last one
		}
		//else this is the last branching node in a chain
		return node;
	}
	//continiu until you find branch node
	return lastBranchNodeInChain(node->getParent());
}

#pragma endregion

#pragma endregion