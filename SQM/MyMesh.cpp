#include "stdafx.h"
#include "MyMesh.h"
#include "FloatArithmetic.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL\glew.h>

bool OpenMeshEqualVHandles(MyMesh::VHandle& h1, MyMesh::VHandle& h2) {
	return h1.idx() == h2.idx();
}

void drawArrowBetweenPointsWithColor(MyMesh::Point Q, MyMesh::Point P, float *c) {
	static GLUquadric* ball = NULL;
	if (ball == NULL)
		ball = gluNewQuadric();
	OpenMesh::Vec3f u = P - Q;
	u = u.normalize();

	glPushMatrix();
	glTranslatef(P[0], P[1], P[2]);
	for (int i = 1; i < 20; i++) {
		float t = 0.005*i;
		glTranslatef(-u[0]*t, -u[1]*t, -u[2]*t);
		gluSphere(ball, t, 5, 5);
	}
	glPopMatrix();


	glColor3f(c[0], c[1], c[2]);
	glBegin(GL_LINES);
	glVertex3fv(Q.values_);
	glVertex3fv(P.values_);
	glEnd();
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

void drawMeshHalfEdgesWithArrows(MyMesh* mesh) {
	float white[] = {1, 1, 1};
	drawMeshHalfEdgesWithArrowsAndColor(mesh, white);
}

void drawMeshHalfEdgesWithArrowsAndColor(MyMesh* mesh, float *c) {
	MyMesh::HalfedgeIter he_it = mesh->halfedges_begin();
	MyMesh::HalfedgeIter fheh_it = mesh->halfedges_end();

	for (; he_it != fheh_it; ++he_it) {
		MyMesh::HalfedgeHandle heh1 = he_it.handle();
		MyMesh::HalfedgeHandle heh2 = mesh->opposite_halfedge_handle(heh1);
		MyMesh::VertexHandle vh1 = mesh->to_vertex_handle(heh1);
		MyMesh::VertexHandle vh2 = mesh->to_vertex_handle(heh2);
		MyMesh::Point P = mesh->point(vh1);
		MyMesh::Point Q = mesh->point(vh2);

		if (!mesh->is_boundary(heh1)) {
			MyMesh::FaceHandle fh = mesh->face_handle(heh1);
			MyMesh::Point Centroid;
			mesh->calc_face_centroid(fh, Centroid);
			P = P + (Centroid - P).normalize()*0.3;
			Q = Q + (Centroid - Q).normalize()*0.3;
			/*MyMesh::Point C = P*0.5 + Q*0.5;
			MyMesh::Point n = mesh->calc_face_normal(fh);
			glColor3f(1, 0, 0);
			glBegin(GL_LINES);
			glVertex3fv(Centroid.values_);
			n = Centroid + (n*20);
			glVertex3fv(n.values_);
			glEnd();*/
			//centroids
			/*float red[] = {1, 0, 0};
			glColor3f(1, 1, 1);
			drawArrowBetweenPointsWithColor(C, Centroid, red);*/
		}
		//drawing
		//float white[] = {1, 1, 1};
		glColor3f(0, 1, 0);
		drawArrowBetweenPointsWithColor(Q, P, c);
	}
}

void drawMeshHalfEdgesWithArrowsAndColor(MyTriMesh* mesh, float *c) {
	MyTriMesh::HalfedgeIter he_it = mesh->halfedges_begin();
	MyTriMesh::HalfedgeIter fheh_it = mesh->halfedges_end();

	for (; he_it != fheh_it; ++he_it) {
		MyTriMesh::HalfedgeHandle heh1 = he_it.handle();
		MyTriMesh::HalfedgeHandle heh2 = mesh->opposite_halfedge_handle(heh1);
		MyTriMesh::VertexHandle vh1 = mesh->to_vertex_handle(heh1);
		MyTriMesh::VertexHandle vh2 = mesh->to_vertex_handle(heh2);
		MyTriMesh::Point P = mesh->point(vh1);
		MyTriMesh::Point Q = mesh->point(vh2);

		if (!mesh->is_boundary(heh1)) {
			MyTriMesh::FaceHandle fh = mesh->face_handle(heh1);
			MyTriMesh::Point Centroid;
			mesh->calc_face_centroid(fh, Centroid);
			P = P + (Centroid - P).normalize()*0.3;
			Q = Q + (Centroid - Q).normalize()*0.3;
		}
		glColor3f(0, 1, 0);
		drawArrowBetweenPointsWithColor(Q, P, c);
	}
}

#pragma region Mesh Converting

void convertTriMeshToArray(MyTriMesh *mesh, std::vector<float> &points, std::vector<int> &indices) {
	int length = (int)floor((float)points.size() / 3.0);
	points.reserve(length + (mesh->n_vertices() * 3));
	indices.reserve(indices.size() + (mesh->n_faces() * 3));

	for (MyTriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		MyTriMesh::Point P = mesh->point(v_it.handle());
		points.push_back(P[0]);
		points.push_back(P[1]);
		points.push_back(P[2]);
	}

	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		MyTriMesh::FaceVertexIter fv_it = mesh->fv_begin(f_it.handle());
		for ( ; fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			indices.push_back(fv_it.handle().idx() + length);
		}
	}
}

void convertMeshToArray(MyMesh *mesh, std::vector<float> &points, std::vector<int> &indices) {
	int length = (int)floor((float)points.size() / 3.0);
	points.reserve(length + (mesh->n_vertices() * 3));
	indices.reserve(indices.size() + (mesh->n_faces() * 4));

	for (MyTriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		MyTriMesh::Point P = mesh->point(v_it.handle());
		points.push_back(P[0]);
		points.push_back(P[1]);
		points.push_back(P[2]);
	}

	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		MyTriMesh::FaceVertexIter fv_it = mesh->fv_begin(f_it.handle());
		std::vector<int> vertices;
		for ( ; fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vertices.push_back(fv_it.handle().idx() + length);
		}
		addToConvertedMeshArray(vertices, indices);
	}
}

void convertMeshToArray(MyMesh *mesh, std::vector<float> &points, std::vector<float> &normals, std::vector<int> &indices) {
	int length = (int)floor((float)points.size() / 3.0);
	points.reserve(length + (mesh->n_vertices() * 3));
	normals.reserve(length + (mesh->n_vertices() * 3));
	indices.reserve(indices.size() + (mesh->n_faces() * 4));
	
	mesh->request_vertex_normals();
	mesh->request_face_normals();
	mesh->update_normals();

	for (MyTriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		MyTriMesh::Point P = mesh->point(v_it.handle());
		points.push_back(P[0]);
		points.push_back(P[1]);
		points.push_back(P[2]);
		OpenMesh::Vec3f n = mesh->normal(v_it.handle());
		normals.push_back(n[0]);
		normals.push_back(n[1]);
		normals.push_back(n[2]);
	}

	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		MyTriMesh::FaceVertexIter fv_it = mesh->fv_begin(f_it.handle());
		std::vector<int> vertices;
		for ( ; fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vertices.push_back(fv_it.handle().idx() + length);
		}
		addToConvertedMeshArray(vertices, indices);
	}
}

void addToConvertedMeshArray(std::vector<int> &vertices, std::vector<int> &indices) {
	if (vertices.size() >= 3) {
		indices.push_back(vertices[0]);
		indices.push_back(vertices[1]);
		indices.push_back(vertices[2]);
	}
	if (vertices.size() == 4) {		
		indices.push_back(vertices[2]);
		indices.push_back(vertices[3]);
		indices.push_back(vertices[0]);
	}
}

void convertMeshToArray(MyMesh *mesh, std::vector<float> &points, std::vector<int> &triIndices, std::vector<int> &quadIndices) {
	int length = (int)floor((float)points.size() / 3.0);
	points.reserve(length + (mesh->n_vertices() * 3));
	triIndices.reserve(triIndices.size() + (mesh->n_faces() * 3));
	quadIndices.reserve(quadIndices.size() + (mesh->n_faces() * 4));

	for (MyTriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		MyTriMesh::Point P = mesh->point(v_it.handle());
		points.push_back(P[0]);
		points.push_back(P[1]);
		points.push_back(P[2]);
	}

	mesh->release_face_normals();
	mesh->release_vertex_normals();

	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		MyTriMesh::FaceVertexIter fv_it = mesh->fv_begin(f_it.handle());
		std::vector<int> vertices;
		for ( ; fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vertices.push_back(fv_it.handle().idx() + length);
		}
		addToConvertedMeshArray(vertices, triIndices, quadIndices);
	}
}

void convertMeshToArray(MyMesh *mesh, std::vector<float> &points, std::vector<float> &vertex_normals, std::vector<int> &triIndices, std::vector<int> &quadIndices) {
	int length = (int)floor((float)points.size() / 3.0);
	points.reserve(length + (mesh->n_vertices() * 3));
	vertex_normals.reserve(length + (mesh->n_vertices() * 3));
	triIndices.reserve(triIndices.size() + (mesh->n_faces() * 3));
	quadIndices.reserve(quadIndices.size() + (mesh->n_faces() * 4));

	mesh->request_vertex_normals();
	mesh->request_face_normals();
	mesh->update_normals();

	for (MyTriMesh::VertexIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		MyTriMesh::Point P = mesh->point(v_it.handle());
		points.push_back(P[0]);
		points.push_back(P[1]);
		points.push_back(P[2]);
		OpenMesh::Vec3f n = mesh->normal(v_it.handle());
		vertex_normals.push_back(n[0]);
		vertex_normals.push_back(n[1]);
		vertex_normals.push_back(n[2]);
	}

	mesh->release_face_normals();
	mesh->release_vertex_normals();

	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		MyTriMesh::FaceVertexIter fv_it = mesh->fv_begin(f_it.handle());
		std::vector<int> vertices;
		for ( ; fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vertices.push_back(fv_it.handle().idx() + length);
		}
		addToConvertedMeshArray(vertices, triIndices, quadIndices);
	}
}

void addToConvertedMeshArray(std::vector<int> &vertices, std::vector<int> &triIndices, std::vector<int> &quadIndices) {
	if (vertices.size() == 3) {
		triIndices.push_back(vertices[0]);
		triIndices.push_back(vertices[1]);
		triIndices.push_back(vertices[2]);
	}
	if (vertices.size() == 4) {		
		quadIndices.push_back(vertices[0]);
		quadIndices.push_back(vertices[1]);
		quadIndices.push_back(vertices[2]);
		quadIndices.push_back(vertices[3]);
	}
}

void calculateTriMeshNormals(MyTriMesh *mesh, std::vector<float> &points) {
	for (MyTriMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		std::vector<MyTriMesh::VHandle> vhandles;
		for (MyTriMesh::FVIter fv_it = mesh->fv_begin(f_it.handle()); fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vhandles.push_back(fv_it.handle());
		}
		MyTriMesh::Point P0 = mesh->point(vhandles[0]);
		MyTriMesh::Point P1 = mesh->point(vhandles[1]);
		MyTriMesh::Point P2 = mesh->point(vhandles[2]);
		OpenMesh::Vec3f u = P1 - P0;
		OpenMesh::Vec3f v = P2 - P0;
		OpenMesh::Vec3f normal = cross(u, v).normalize();
		MyTriMesh::Point center = (P0 + P1 + P2) / 3.0;
		OpenMesh::Vec3f dest = center + (normal * 50);
		points.push_back(center[0]);
		points.push_back(center[1]);
		points.push_back(center[2]);

		points.push_back(dest[0]);
		points.push_back(dest[1]);
		points.push_back(dest[2]);
	}
}

void calculateMeshNormals(MyMesh *mesh, std::vector<float> &points) {
	for (MyMesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
		std::vector<MyTriMesh::VHandle> vhandles;
		for (MyMesh::FVIter fv_it = mesh->fv_begin(f_it.handle()); fv_it != mesh->fv_end(f_it.handle()); ++fv_it) {
			vhandles.push_back(fv_it.handle());
		}
		MyMesh::Point P0 = mesh->point(vhandles[0]);
		MyMesh::Point P1 = mesh->point(vhandles[1]);
		MyMesh::Point P2 = mesh->point(vhandles[2]);
		OpenMesh::Vec3f u = P1 - P0;
		OpenMesh::Vec3f v = P2 - P0;
		OpenMesh::Vec3f normal = cross(u, v).normalize();
		MyMesh::Point center;
		if (vhandles.size() == 3) {
			center = (P0 + P1 + P2) / 3.0;
		} else {
			MyMesh::Point P3 = mesh->point(vhandles[3]);
			center = (P0 + P1 + P2 + P3) / 4.0;
		}
		OpenMesh::Vec3f dest = center + (normal * 50);
		points.push_back(center[0]);
		points.push_back(center[1]);
		points.push_back(center[2]);

		points.push_back(dest[0]);
		points.push_back(dest[1]);
		points.push_back(dest[2]);
	}
}

int calculateMaxValency(MyMesh *mesh) {
	int max = 0;

	for (MyMesh::VIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
		int count = 0;
		for (MyMesh::VVIter vv_it = mesh->vv_begin(v_it.handle()); vv_it != mesh->vv_end(v_it.handle()); ++vv_it) {
			count++;
		}
		if (count > max) {
			max = count;
		}
	}

	return max;
}

float calculateOneRingRadius(MyTriMesh *mesh, MyTriMesh::VHandle vh) {
	MyTriMesh::VHandle first_vh(-1);
	MyTriMesh::VHandle prev_vh(-1);
	float radius = 0;

	for (MyTriMesh::VVIter vv_it = mesh->vv_begin(vh); vv_it != mesh->vv_end(vh); ++vv_it) {
		if (first_vh.idx() == -1) {
			first_vh = vv_it.handle();
			prev_vh = vv_it.handle();
		}
		//calculate length
		MyTriMesh::Point P = mesh->point(prev_vh);
		MyTriMesh::Point Q = mesh->point(vv_it.handle());
		radius += (P - Q).norm();

		prev_vh = vv_it.handle();
	}
	MyTriMesh::Point P = mesh->point(prev_vh);
	MyTriMesh::Point Q = mesh->point(first_vh);
	radius += (P - Q).norm();
	//we calculated circumference (2*PI*r) so now we need to divide by 2*PI
	radius /= (2*M_PI);

	return radius;
}

#pragma endregion

#pragma region Mesh Writing

void writeTriMesh(MyTriMesh* mesh) {
	static int number = 0;
	number++;
	std::stringstream ss;//create a stringstream
	ss << number;
	std::string file = ss.str() + ".obj";
	writeTriMesh(mesh, file);
}

void writeTriMesh(MyTriMesh* mesh, std::string fileName) {
	OpenMesh::IO::Options wopt;
	if (!OpenMesh::IO::write_mesh(*mesh, fileName)) {
	}
}

void writeMesh(MyMesh* mesh, std::string fileName) {
	OpenMesh::IO::Options wopt;
	if (!OpenMesh::IO::write_mesh(*mesh, fileName)) {
	}
}

#pragma endregion