#pragma once
#include "MeshGraph.h"
#include "SkeletonNode.h"
#define _USE_MATH_DEFINES
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <math.h>
#include <queue>
#include "MyMesh.h"
#include "EdgeLength.h"
//#include <GL/glu.h>
#include <GL\glew.h>
#include "LIENeedEntry.h"

#pragma region Structs

struct VHandleCount {
	MyTriMesh::VHandle vhandle;
	int missingPoints;
	bool needs;

	VHandleCount(MyTriMesh::VHandle handle, int points, bool need) : vhandle(handle), missingPoints(points), needs(need) { };
};

#pragma endregion

class SQMNode {
	friend class boost::serialization::access;

	string id;
	SQMNode* parent;
	float nodeRadius;
	OpenMesh::Vec3f position;
	Quaternion axisAngle;
	vector<SQMNode*> nodes;
	vector<OpenMesh::Vec3f> intersections;
	MyTriMesh *polyhedron;
	vector<MyTriMesh::VertexHandle> polyhedronPoints;
	vector<MyTriMesh::VertexHandle> intersectionVHandles;
	vector<MyTriMesh::VertexHandle> meshIntersectionVHandles;
	vector<MyMesh::VertexHandle> meshVhandlesToRotate;
	
	vector<OpenMesh::Vec3f> normals2;
	vector<OpenMesh::Vec3f> centers2;
	vector<OpenMesh::Vec3i> triangles2;
public:
#pragma region Init
	SQMNode(void);
	SQMNode(SkeletonNode &node, SQMNode* newParent);
	~SQMNode(void);	
#pragma endregion

#pragma region Getters
	string getId();
	bool isBranchNode();
	bool isLeafNode();
	OpenMesh::Vec3f getPosition();
	vector<SQMNode*>* getNodes();
	SQMNode* getParent();
	MyTriMesh* getPolyhedron();
	vector<SQMNode*>* getDescendants();
	vector<MyTriMesh::VertexHandle>* getIntersectionVHandles();
	float getNodeRadius();
	Quaternion getAxisAngle();
	int getNumOfChilds();
	float getX();
	float getY();
	float getZ();
#pragma endregion

#pragma region Setters
	void setNodeRadius(float newNodeRadius);
	void setPosition(OpenMesh::Vec3f newPosition);
	void addDescendant(SQMNode* node);
	void rotatePosition(Quaternion q, CVector3 offset);
	void addDescendant(float x, float y, float z);
	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
#pragma endregion

#pragma region Export
	SkeletonNode* exportToSkeletonNode();
#pragma endregion

#pragma region Skeleton Straightening
	void straightenSkeleton(OpenMesh::Vec3f *lineVector);
#pragma endregion

#pragma region BNP Generation
	void calculateConvexHull();
	void createPolyhedra(vector<OpenMesh::Vec3i> triangles);
	bool checkPolyhedronOrientation(int index, OpenMesh::Vec3f center, OpenMesh::Vec3f normal, vector<OpenMesh::Vec3i>& triangles);
	OpenMesh::Vec3f translatedPointToSphereWithFaceNormals(OpenMesh::Vec3f p, OpenMesh::Vec3f n1, OpenMesh::Vec3f n2, OpenMesh::Vec3f center1, OpenMesh::Vec3f center2);
	vector<int> getNormalIndexis(vector<int> indexis, int index);
	void openMeshFromIdexedFace(vector<OpenMesh::Vec3f> vertices, vector<OpenMesh::Vec3i> faces);
	
	void createPolyhedraFromCenter(vector<OpenMesh::Vec3i> triangles);
	OpenMesh::Vec3f polyhedronBoundingBoxCenter();
	OpenMesh::Vec3f polyhedronPointSumCenterCenter();
	OpenMesh::Vec3f translatePointToSphereFromCenter(OpenMesh::Vec3f point, OpenMesh::Vec3f center);
#pragma endregion

#pragma region BNP Subdivision
	void subdividePolyhedra(SQMNode* parentBranchNode, int count);

#pragma region OLD
	void subdividePolyhedraOld(SQMNode* parentBranchNode, int count);
	void fillVerticeCountTable(vector<VHandleCount>& verticeCountTable, vector<SQMNode*>& branchingNodes);
	void fillEdgeLengthList(deque<EdgeLength>& edgeLengthList, MyTriMesh::VHandle vertice);
	MyTriMesh::EdgeHandle pickEdgeToSplit(deque<EdgeLength>& edgeLengthList, vector<VHandleCount>& verticeCountTable, MyTriMesh::VHandle vertice);
	void splitEdgeInHalf(MyTriMesh::EdgeHandle eh);
#pragma endregion

#pragma region NEW
	void fillLIEMap(int parentNeed, std::map<int, LIENeedEntry>& lieMap, std::vector<SQMNode*>& branchingNodes);
	void splitLIEs(std::map<int, LIENeedEntry>& lieMap);
	void splitLIE(LIE lie, std::map<int, LIENeedEntry>& lieMap, int entryIndex, int lieIndex);
	LIE splitLIEEdge(LIE lie);
	MyTriMesh::EHandle splitEdgeInHalfAndReturnNewEdge(MyTriMesh::EdgeHandle eh);
#pragma endregion

#pragma region Smoothing
	void smoothLIE(LIE lie);
	void smoothLIEs(map<int, LIENeedEntry> lieMap);
	void smoothMesh();
	void mesh2graph(MeshGraph& meshGraph);
	void laplacianSMoothing();
	void recalculateSmoothedVertices(MeshGraph& meshGraph);
#pragma endregion

#pragma endregion

#pragma region BNP Joining
	void joinBNPs(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle> oneRing, OpenMesh::Vec3f directionVector);
	void addPolyhedronToMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void addPolyhedronAndRememberVHandles(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, vector<vector<MyMesh::VHandle> >& oneRingsOfPolyhedron, OpenMesh::Vec3f& directionVector);
	void extendMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void finishLeafeNode(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing);
#pragma endregion

#pragma region Final Vertex Placement
	void rotateBack(MyMesh *mesh);
#pragma endregion

#pragma region Utility
	void rotateSelfAndDescendants(Quaternion q, CVector3 offset);
	int getPointPositionInArrayOrAdd(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
	int getPointPositionInArray(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
	SQMNode* getDescendantBranchNode(SQMNode* node);
	SQMNode* getAncestorBranchNode(SQMNode* node);
	MyMesh::VHandle getSecondVHandle(MyMesh::EdgeHandle edgeHandle, MyMesh::VHandle vhandle);
	MyTriMesh::HalfedgeHandle nextLink(MyTriMesh::HalfedgeHandle heh);
	MyTriMesh::HalfedgeHandle prevLink(MyTriMesh::HalfedgeHandle heh);
	MyTriMesh::VHandle oppositeVHandle(MyTriMesh::HalfedgeHandle heh);
#pragma endregion

#pragma region Drawing
	void draw();
	void draw(CVector3 lineColor, CVector3 nodeColor);
	void draw2();
#pragma endregion

protected:
#pragma region Serialization
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & boost::serialization::base_object<SkeletonNode>(*this);  //serialize base class
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SkeletonNode);
	}
#pragma endregion

};

#pragma region Utility Functions

template <typename T> int getPositionInArray(T& v, vector<T>& vectorArray);
template <typename T> bool equals(T& a, T& b);
template <typename T> void flipVector(vector<T>& toFlip, vector<T>& flipped);
bool lesser(MyMesh::FaceHandle& a, MyMesh::FaceHandle& b);
bool validTriFace(vector<MyMesh::VHandle>& faceVHandles);
bool rayTriangleIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f V0, OpenMesh::Vec3f V1, OpenMesh::Vec3f V2, float &t_param);
bool raySphereIntersection(OpenMesh::Vec3f ray_origin, OpenMesh::Vec3f ray_direction, OpenMesh::Vec3f sphere_center, float sphere_radius, float &t_param);
OpenMesh::Vec3i flipVec3i(OpenMesh::Vec3i& v);
bool sameOneRingOrientation(MyMesh* mesh, vector<MyMesh::VHandle>& oneRing, vector<MyMesh::VHandle>& oneRing2, OpenMesh::Vec3f& center1, OpenMesh::Vec3f& center2, OpenMesh::Vec3f& direction);
int verticeDifferenceFatherSon(SQMNode* father, SQMNode* son, MyTriMesh::VHandle vhandle);
int inLIEs(std::vector<LIE>& LIEs, MyTriMesh::VHandle vh1, MyTriMesh::VHandle vh2);
SQMNode* lastBranchNodeInChain(SQMNode* node);

#pragma endregion

