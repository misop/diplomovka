#pragma once
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>

struct MyTraits : public OpenMesh::DefaultTraits
{
  VertexAttributes(OpenMesh::Attributes::Status);
  FaceAttributes(OpenMesh::Attributes::Status);
  EdgeAttributes(OpenMesh::Attributes::Status);
};

typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits> MyMesh;

bool OpenMeshEqualVHandles(MyMesh::VHandle& h1, MyMesh::VHandle& h2);

void drawMeshHalfEdges(MyMesh* mesh);
void drawMeshEdges(MyMesh* mesh);
void drawMeshEdgesWithArrows(MyMesh* mesh);