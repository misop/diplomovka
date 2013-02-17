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
		MyMesh::HalfedgeHandle next = mesh->opposite_halfedge_handle(mesh->next_halfedge_handle(he_it.handle()));
		MyMesh::VertexHandle vh1 = mesh->to_vertex_handle(he_it.handle());
		MyMesh::VertexHandle vh2 = mesh->to_vertex_handle(oposite);
		MyMesh::VertexHandle vh3 = mesh->to_vertex_handle(next);
		MyMesh::Point P = mesh->point(vh1);
		MyMesh::Point Q = mesh->point(vh2);
		MyMesh::Point N = mesh->point(vh3);
		glColor4f(1, 1, 1, 0);
		glVertex3fv(Q.values_);
		glColor4f(1, 1, 1, 1);
		glVertex3fv(P.values_);
		glColor4f(1, 0, 0, 1);
		glVertex3fv(P.values_);
		glColor4f(1, 0, 0, 1);
		glVertex3fv(N.values_);
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

void drawMeshEdgesWithArrows(MyMesh* mesh) {
	MyMesh::EdgeIter e_it = mesh->edges_begin();
	MyMesh::EdgeIter feh_it = mesh->edges_end();

	for (; e_it != feh_it; ++e_it) {
		MyMesh::EdgeHandle ehandle = e_it.handle();
		MyMesh::HalfedgeHandle heh1 = mesh->halfedge_handle(ehandle, 0);
		MyMesh::HalfedgeHandle heh2 = mesh->halfedge_handle(ehandle, 1);
		MyMesh::VertexHandle vh1 = mesh->to_vertex_handle(heh1);
		MyMesh::VertexHandle vh2 = mesh->to_vertex_handle(heh2);
		MyMesh::Point P = mesh->point(vh1);
		MyMesh::Point Q = mesh->point(vh2);
		OpenMesh::Vec3f u = P - Q;
		OpenMesh::Vec3f v = Q - P;
		float height = u.norm();
		u = u.normalize();
		v = v.normalize();
		
		glColor3f(0, 0, 1);
		glPushMatrix();
		glTranslatef(P[0], P[1], P[2]);
		for (int i = 1; i < 20; i++) {
			float t = 0.005*i;
			glTranslatef(-u[0]*t, -u[1]*t, -u[2]*t);
			gluSphere(gluNewQuadric(), t, 10, 10);
		}
		glPopMatrix();
		glColor3f(0, 1, 0);
		glPushMatrix();
		glTranslatef(Q[0], Q[1], Q[2]);
		for (int i = 1; i < 20; i++) {
			float t = 0.005*i;
			glTranslatef(-v[0]*t, -v[1]*t, -v[2]*t);
			gluSphere(gluNewQuadric(), t, 10, 10);
		}
		glPopMatrix();
		/*
		//draw 1st arrow
		glPushMatrix();
		glTranslatef(P[0] - u[0]*height/5, P[1] - u[1]*height/5, P[2] - u[2]*height/5);

		float phi = atan(u[2]/u[0]);//atan2(u[0], u[2]);
		float theta = acos(u[1]);
		glRotatef(phi*180/M_PI - 90, 0, 0, 1);
		glRotatef(theta*180/M_PI, 1, 0, 0);
		
		glColor3f(0, 0, 1);
		//gluCylinder(gluNewQuadric(), 0.2, 0, height/5, 10, 10);
		glPopMatrix();
		//draw 2nd arrow
		glPushMatrix();
		glTranslatef(Q[0] - v[0]*height/5, Q[1] - v[1]*height/5, Q[2] - v[2]*height/5);

		phi = atan(v[1]/v[0]);//atan2(u[0], u[2]);
		theta = acos(v[2]);
		glRotatef(phi*180/M_PI - 90, 0, 0, 1);
		glRotatef(theta*180/M_PI, 1, 0, 0);
		
		glColor3f(0, 1, 0);
		gluCylinder(gluNewQuadric(), 0.2, 0, height/5, 10, 10);
		glPopMatrix();*/
		//drawing
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex3fv(Q.values_);
		glVertex3fv(P.values_);
		glEnd();
	}
}