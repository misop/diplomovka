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
#include "Utility.h"
#include "SkinSkeleton.h"

#pragma region Structs & Enums

struct VHandleCount {
	MyTriMesh::VHandle vhandle;
	int missingPoints;
	bool needs;

	VHandleCount(MyTriMesh::VHandle handle, int points, bool need) : vhandle(handle), missingPoints(points), needs(need) { };
};

typedef enum {
	SQMNoSmoothing = 0,
	SQMQuaternionSmoothing,
	SQMAvaragingSmoothing,
	SQMValencyLaplacianSmoothing,
	SQMOneRingLaplacianSmoothing
} SQMSmoothingAlgorithm;

typedef enum {
	SQMNone = 0,
	SQMPoint,
	SQMCapsule,
	SQMCycle,
	SQMCycleLeaf,
	SQMFormerCapsule,
	SQMCreatedCapsule
} SQMNodeType;

#pragma endregion

class SQMNode {
	friend class boost::serialization::access;
	
	unsigned int id;
	string idStr;
	SQMNode* parent;
	SQMNode *cycleNode;
	vector<SQMNode*> cycleParents;
	float nodeRadius;
	float tessLevel;
	SQMNodeType sqmNodeType;
	OpenMesh::Vec3f position;
	OpenMesh::Vec3f originalPosition;
	OpenMesh::Vec3f centerOfMass;
	OpenMesh::Vec3f oldPosition;
	Quaternion axisAngle;
	glm::ivec2 skinningIDs;
	vector<SQMNode*> nodes;
	vector<OpenMesh::Vec3f> intersections;
	MyTriMesh *polyhedron;
	glm::mat4 transformationMatrix;
	glm::vec3 scalev;
	glm::vec3 rotatev;
	vector<MyTriMesh::VertexHandle> polyhedronPoints;
	vector<MyTriMesh::VertexHandle> intersectionVHandles;
	vector<MyTriMesh::VertexHandle> meshIntersectionVHandles;
	vector<MyMesh::VertexHandle> meshVhandlesToRotate;
	vector<glm::vec3> cyclePoints;
	vector<MyMesh::VertexHandle> cycleVHandles;

	vector<OpenMesh::Vec3f> normals2;
	vector<OpenMesh::Vec3f> centers2;
	vector<OpenMesh::Vec3i> triangles2;
public:
#pragma region Init
	SQMNode(void);
	SQMNode(SkeletonNode &node, SQMNode* newParent);
	SQMNode(SQMNode &node);
	SQMNode(SQMNode &node, bool shalow);
	~SQMNode(void);	
#pragma endregion

#pragma region Getters
	unsigned int getId();
	string getIdStr();
	bool isBranchNode();
	bool isConnectionNode();
	bool isLeafNode();
	bool isValidCycleNode();
	bool isCycleNode();
	OpenMesh::Vec3f getPosition();
	OpenMesh::Vec3f getOldPosition();
	OpenMesh::Vec3f getOriginalPosition();
	glm::vec3 getPosition_glm();
	SQMNodeType getSQMNodeType();
	vector<SQMNode*>* getNodes();
	SQMNode* getParent();
	SQMNode* getCycleNode();
	MyTriMesh* getPolyhedron();
	vector<SQMNode*>* getDescendants();
	vector<MyTriMesh::VertexHandle>* getIntersectionVHandles();
	vector<MyMesh::VertexHandle>* getCycleVHandles();
	vector<glm::vec3>* getCyclePoints();
	float getNodeRadius();
	float getTessLevel();
	Quaternion getAxisAngle();
	int getNumOfChilds();
	float getX();
	float getY();
	float getZ();
	glm::mat4 getTransformationMatrix();
	glm::vec3 getScalev();
	glm::vec3 getRotatev();
	float getScaleX();
	float getScaleY();
	float getScaleZ();
	float getRotateX();
	float getRotateY();
	float getRotateZ();
	bool getIsCapsule();
#pragma endregion

#pragma region Setters
	void setID(unsigned int newID);
	void setParent(SQMNode *node);
	void setCycleNode(SQMNode *node);
	void setNodeRadius(float newNodeRadius);
	void setTessLevel(float newTessLevel);
	void setPosition(OpenMesh::Vec3f newPosition);
	void setPosition(float x, float y, float z);
	void setSQMNodeType(SQMNodeType newType);
	void addDescendant(SQMNode* node);
	void rotatePosition(Quaternion q, CVector3 offset);
	void addDescendant(float x, float y, float z);
	void removeDescendant(SQMNode* node);
	void removeDescendants();
	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
	void setTransformationMatrix(glm::mat4 tm);
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void setRotateX(float value);
	void setRotateY(float value);
	void setRotateZ(float value);
	void updateTransformationMatrix();
	void addVHandleToRotate(MyMesh::VHandle vh);
	void setIsCapsule(bool isCapsule);
	void addIntersection(OpenMesh::Vec3f intersection);
#pragma endregion

#pragma region Cycles
	void createCycle(SQMNode *node);
	void rotateCycleOneRing();
	void projectOnPlaneRotateAndScale(MyMesh *mesh, glm::vec3 origin, glm::vec3 normal);
#pragma endregion

#pragma region Export
	SkeletonNode* exportToSkeletonNode();
	SkinSkeleton* exportToSkinSkeleton(SkinSkeleton *parentSkin);
#pragma endregion

#pragma region SQM Preprocessing
	void breakCycles();
	void createCapsules(int minSmallCircles = 5);
#pragma endregion

#pragma region Skeleton Straightening
	void straightenSkeleton(OpenMesh::Vec3f *lineVector);
	void straightenSkl();
	void straightenSkl(glm::vec4 odir, glm::mat4 M);
#pragma endregion

#pragma region BNP Generation
	void calculateConvexHull();
	void createPolyhedra(vector<OpenMesh::Vec3i> triangles);
	OpenMesh::Vec3f translatedPointToSphereWithFaceNormals(OpenMesh::Vec3f p, OpenMesh::Vec3f n1, OpenMesh::Vec3f n2, OpenMesh::Vec3f center1, OpenMesh::Vec3f center2);
	OpenMesh::Vec3f translatedPointToSphereWithCenterOfMass(OpenMesh::Vec3f p);
	vector<int> getNormalIndexis(vector<int> indexis, int index);
	void openMeshFromIdexedFace(vector<OpenMesh::Vec3f> vertices, vector<OpenMesh::Vec3i> faces);

	void createPolyhedraFromCenter(vector<OpenMesh::Vec3i> triangles);
	OpenMesh::Vec3f polyhedronBoundingBoxCenter();
	OpenMesh::Vec3f polyhedronPointSumCenterCenter();
	OpenMesh::Vec3f translatePointToSphereFromCenter(OpenMesh::Vec3f point, OpenMesh::Vec3f center);
#pragma endregion

#pragma region BNP Subdivision
	void subdividePolyhedra(SQMNode* parentBranchNode, int count, SQMSmoothingAlgorithm algorithm);
	void fillLIEMap(int parentNeed, std::map<int, LIENeedEntry>& lieMap, std::vector<SQMNode*>& branchingNodes);
	void splitLIEs(std::map<int, LIENeedEntry>& lieMap, SQMSmoothingAlgorithm algorithm);
	void splitLIE(LIE lie, std::map<int, LIENeedEntry>& lieMap, int entryIndex, int lieIndex, SQMSmoothingAlgorithm algorithm);
	LIE splitLIEEdge(LIE lie);
	MyTriMesh::EHandle splitEdgeInHalfAndReturnNewEdge(MyTriMesh::EdgeHandle eh);
	
#pragma endregion

#pragma region Smoothing

#pragma region Quaternion Smoothing
	void smoothLIE(LIE lie);
	void smoothLIEs(map<int, LIENeedEntry> lieMap);
#pragma endregion

#pragma region Laplacian Smoothing
	void laplacianSMoothing(SQMSmoothingAlgorithm algorithm);
	void mesh2graph(MeshGraph& meshGraph, SQMSmoothingAlgorithm algorithm);
	void mesh2graphValencyWeighted(MeshGraph& meshGraph);
	void mesh2graphOneRingWeighted(MeshGraph& meshGraph);
	void recalculateSmoothedVertices(MeshGraph& meshGraph);
#pragma endregion

#pragma region Avaraging Smoothing
	void smoothLIEByAvaraging(LIE lie);
#pragma endregion

#pragma endregion

#pragma region BNP Joining
	void joinBNPs(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle> oneRing, OpenMesh::Vec3f directionVector);
	void addPolyhedronToMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void addPolyhedronAndRememberVHandles(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, vector<vector<MyMesh::VHandle> >& oneRingsOfPolyhedron, OpenMesh::Vec3f& directionVector);
	void extendMesh(MyMesh* mesh, SQMNode* parentBNPNode, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector);
	void finishLeafeNode(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing);
	void finishLeafeCycleNode(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f directionVector);
#pragma endregion

#pragma region Final Vertex Placement
	void rotateBack(MyMesh *mesh);
	void rotateWithSkeleton(MyMesh *mesh, SkinSkeleton *skeleton);
#pragma endregion

#pragma region Skinning Matrix Setup
	void setupSkinningMatrixIDs(SkinSkeleton *skeleton);
#pragma endregion

#pragma region BNP Tesselation
	void getMeshTessLevel(std::vector<float> &tessLevels);
	void getMeshTessDataf(std::vector<float> &tessLevels, std::vector<float> &nodePositions, std::vector<float> &data);
	void getMeshTessDatai(std::vector<float> &tessLevels, std::vector<float> &nodePositions, std::vector<int> &skinMatrices, std::vector<int> &data);
	void calculateOneRingRadiusAndMap(std::vector<float> &oneRingRadius, std::map<int, std::vector<int> > &intersectionMap);
	void fillRadiusTable(float *table, int width);
	void fillCentersTable(float *table);
#pragma endregion

#pragma region SQM Special Cases

#pragma region Single Node
	void createScaledIcosahderon(MyMesh* mesh);
#pragma endregion

#pragma region Worm
	void wormCreate(MyMesh *mesh, int vertices = 5);
	void wormStraighten(OpenMesh::Vec3f lineVector);
	void wormStep(MyMesh *mesh, vector<MyMesh::VHandle> &oneRing, OpenMesh::Vec3f lineVector);
	void wormFinalVertexPlacement(MyMesh *mesh);
#pragma endregion

#pragma endregion

#pragma region Utility
	void rotateSelfAndDescendants(Quaternion q, CVector3 offset);
	SQMNode* getDescendantBranchNode(SQMNode* node);
	SQMNode* getAncestorBranchNode(SQMNode* node);
	MyMesh::VHandle getSecondVHandle(MyMesh::EdgeHandle edgeHandle, MyMesh::VHandle vhandle);
	MyTriMesh::HalfedgeHandle startLink(MyTriMesh::VertexHandle vh);
	MyTriMesh::HalfedgeHandle nextLink(MyTriMesh::HalfedgeHandle heh);
	MyTriMesh::HalfedgeHandle prevLink(MyTriMesh::HalfedgeHandle heh);
	MyTriMesh::VHandle oppositeVHandle(MyTriMesh::HalfedgeHandle heh);
	void translateAndScalePointsToSphere(MyMesh* mesh, vector<MyMesh::VertexHandle>& oneRing, OpenMesh::Vec3f& directionVector, vector<MyMesh::Point>& points);
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

#pragma region Template Functions
template <typename T> int getPositionInArray(T& v, std::vector<T>& vectorArray);
template <typename T> bool equals(T& a, T& b);
template <typename T> void flipVector(std::vector<T>& toFlip, std::vector<T>& flipped);
#pragma endregion

#pragma region Position In Array
int getPointPositionInArrayOrAdd(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
int getPointPositionInArray(OpenMesh::Vec3f& v, vector<OpenMesh::Vec3f>& vectorArray);
int getPointPositionInArray(OpenMesh::Vec2i& v, vector<OpenMesh::Vec2i>& vectorArray);
#pragma endregion

#pragma region Mesh Functions
bool lesser(MyMesh::FaceHandle& a, MyMesh::FaceHandle& b);
bool validTriFace(vector<MyMesh::VHandle>& faceVHandles);
OpenMesh::Vec3i flipVec3i(OpenMesh::Vec3i& v);
bool sameOneRingOrientation(MyMesh* mesh, vector<MyMesh::VHandle>& oneRing, vector<MyMesh::VHandle>& oneRing2, OpenMesh::Vec3f& center1, OpenMesh::Vec3f& center2, OpenMesh::Vec3f& direction);
bool equalOrientation(MyMesh::Point P0, MyMesh::Point P1, MyMesh::Point Q0, MyMesh::Point Q1, OpenMesh::Vec3f& direction);
MyMesh::Point preparePointForOrientationCheck(MyMesh::Point P, OpenMesh::Vec3f& center, OpenMesh::Vec3f& direction);
int inLIEs(std::vector<LIE>& LIEs, MyTriMesh::VHandle vh1, MyTriMesh::VHandle vh2);
#pragma endregion

#pragma region SQMNode Functions
int verticeDifferenceFatherSon(SQMNode* father, SQMNode* son, MyTriMesh::VHandle vhandle);
SQMNode* lastBranchNodeInChain(SQMNode* node);
#pragma endregion

#pragma region Helper Functions
glm::vec3 projectPointOntoPlane(glm::vec3 point, glm::vec3 origin, glm::vec3 normal);
OpenMesh::Vec3f projectPointOntoPlane(OpenMesh::Vec3f point, OpenMesh::Vec3f origin, OpenMesh::Vec3f normal);
bool isObtuseTriangle(OpenMesh::Vec3f v1, OpenMesh::Vec3f v2, OpenMesh::Vec3f v3);
glm::mat4 matrixRotationBetweenVectors(glm::vec4 A, glm::vec4 B, glm::vec4 C);
#pragma endregion

#pragma endregion

