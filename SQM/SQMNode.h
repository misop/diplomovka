#pragma once
#include "SkeletonNode.h"
#define _USE_MATH_DEFINES
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <math.h>
#include <queue>
#include "MyMesh.h"
#include "EdgeLength.h"
#include <GL/glu.h>

#pragma region Structs

struct VHandleCount {
	MyMesh::VHandle vhandle;
	int missingPoints;
	bool needs;

	VHandleCount(MyMesh::VHandle handle, int points, bool need) : vhandle(handle), missingPoints(points), needs(need) { };
};

#pragma endregion

class SQMNode {
	friend class boost::serialization::access;

	SQMNode* parent;
	float nodeRadius;
	OpenMesh::Vec3f position;
	CVector4 axisAngle;
	vector<SQMNode*> nodes;
	vector<OpenMesh::Vec3f> intersections;
	MyMesh *polyhedron;
	vector<MyMesh::VertexHandle> polyhedronPoints;
	vector<MyMesh::VertexHandle> intersectionVHandles;
	vector<MyMesh::VertexHandle> meshIntersectionVHandles;
public:
#pragma region Init
	SQMNode(void);
	SQMNode(SkeletonNode &node, SQMNode* newParent);
	~SQMNode(void);	
#pragma endregion

#pragma region Getters
	bool isBranchNode();
	bool isLeafNode();
	OpenMesh::Vec3f getPosition();
	vector<SQMNode*>* getNodes();
	MyMesh* getPolyhedron();
	vector<SQMNode*>* getDescendants();
	vector<MyMesh::VertexHandle>* getIntersectionVHandles();
	float getNodeRadius();
#pragma endregion

#pragma region Skeleton Straightening
	void straightenSkeleton(OpenMesh::Vec3f *lineVector);
	void straightenSkeleton(OpenMesh::Vec3f lineVector);
#pragma endregion

#pragma region BNP Generation
	void calculateConvexHull();
	void createPolyhedra(vector<OpenMesh::Vec3i> triangles);
	OpenMesh::Vec3f translatedPointToSphereWithFaceNormals(OpenMesh::Vec3f p, OpenMesh::Vec3f n1, OpenMesh::Vec3f n2, OpenMesh::Vec3f center1, OpenMesh::Vec3f center2);
	vector<int> getNormalIndexis(vector<int> indexis, int index);
	void openMeshFromIdexedFace(vector<OpenMesh::Vec3f> vertices, vector<OpenMesh::Vec3i> faces);
#pragma endregion

#pragma region BNP Subdivision
	void subdividePolyhedra(SQMNode* parentBranchNode, int count);
	void fillVerticeCountTable(vector<VHandleCount>& verticeCountTable, vector<SQMNode*>& branchingNodes);
	void fillEdgeLengthList(deque<EdgeLength>& edgeLengthList, MyMesh::VHandle vertice);
	MyMesh::EdgeHandle pickEdgeToSplit(deque<EdgeLength>& edgeLengthList, vector<VHandleCount>& verticeCountTable, MyMesh::VHandle vertice);
	void splitEdgeInHalf(MyMesh::EdgeHandle eh);
#pragma endregion

#pragma region BNP Joining
	void joinBNPs(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle> oneRing, OpenMesh::Vec3f directionVector);
	void addPolyhedronToMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void addPolyhedronAndRememberVHandles(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, vector<vector<MyMesh::VHandle> >& oneRingsOfPolyhedron, OpenMesh::Vec3f& directionVector);
	void extendMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void finishLeafeNode(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing);
#pragma endregion

#pragma region Utility
	int getPointPositionInArrayOrAdd(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
	int getPointPositionInArray(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
	SQMNode* getDescendantBranchNode(SQMNode* node);
	MyMesh::VHandle getSecondVHandle(MyMesh::EdgeHandle edgeHandle, MyMesh::VHandle vhandle);
#pragma endregion

#pragma region Drawing
	void draw();
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
bool lesser(MyMesh::FaceHandle& a, MyMesh::FaceHandle& b);
bool validTriFace(vector<MyMesh::VHandle>& faceVHandles);

#pragma endregion

