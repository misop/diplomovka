#include "stdafx.h"
#include "MyMesh.h"
#include "FloatArithmetic.h"
#include <GL/gl.h>
#include <GL/glu.h>

bool OpenMeshEqualVHandles(MyMesh::VHandle& h1, MyMesh::VHandle& h2) {
	return h1.idx() == h2.idx();
}

void drawMeshHalfEdges(MyMesh* mesh) {
	MyMesh::HalfedgeIter he_it = mesh->halfedges_begin();
	MyMesh::HalfedgeIter fheh_it = mesh->halfedges_end();
	
	glBegin(GL_LINES);
	for (; he_it != fheh_it; ++he_it) {
		MyMesh::HalfedgeHandle oposite = mesh->opposite_halfedge_handle(he_it.handle());
		MyMesh::VertexHandle vh1 = mesh->to_vertex_handle(he_it.handle());
		MyMesh::VertexHandle vh2 = mesh->to_vertex_handle(oposite);
		MyMesh::Point P = mesh->point(vh1);
		MyMesh::Point Q = mesh->point(vh2);
		glColor4f(1, 1, 1, 0);
		glVertex3fv(Q.values_);
		glColor4f(1, 1, 1, 1);
		glVertex3fv(P.values_);
	}
	glEnd();
}

void drawMeshEdges(MyMesh* mesh) {
	MyMesh::EdgeIter e_it = mesh->edges_begin();
	MyMesh::EdgeIter feh_it = mesh->edges_end();
	
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (; e_it != feh_it; ++e_it) {
		MyMesh::EdgeHandle ehandle = e_it.handle();
		MyMesh::HalfedgeHandle heh1 = mesh->halfedge_handle(ehandle, 0);
		MyMesh::HalfedgeHandle heh2 = mesh->halfedge_handle(ehandle, 1);
		MyMesh::VertexHandle vh1 = mesh->to_vertex_handle(heh1);
		MyMesh::VertexHandle vh2 = mesh->to_vertex_handle(heh2);
		MyMesh::Point P = mesh->point(vh1);
		MyMesh::Point Q = mesh->point(vh2);
		glVertex3fv(Q.values_);
		glVertex3fv(P.values_);
	}
	glEnd();
}