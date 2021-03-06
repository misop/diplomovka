/*
 *  SphereDelaunay.cpp
 *  gtm
 *
 *  Created by J. Andreas Bærentzen on 04/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include <vector>
#include <list>
#include <map>

#include <cmath>
#include <cstdlib>

//#include <GEL/CGLA/Vec3d.h>
#include "SphereDelaunay.h"

using namespace std;
//using namespace CGLA;
using namespace OpenMesh;
namespace
{
	
	const Vec3f rand_unit_vector()
	{
		Vec3f p;
		do
		{	
			p=2.0*Vec3f(rand()/double(RAND_MAX),rand()/double(RAND_MAX),rand()/double(RAND_MAX))-Vec3f(1, 1, 1);
		}
		while(sqr_length(p)>1.0);
		return p.normalized();
	}
	
	bool comp_points_sphere(const Vec3d& p0, const Vec3d& p1)
	{
		double d0 = dot(Vec3d(1,0,0), p0);
		double d1 = dot(Vec3d(1,0,0), p1);
		return d0 < d1;
	}
	
	struct SphereTriangle
	{
		Vec3i corners;
		Vec3f normal;
		double alpha;
		
		SphereTriangle(int a, int b, int c, const vector<Vec3f>& vertices):
		corners(a,b,c),
		normal(cross(vertices[b]-vertices[a],vertices[c]-vertices[a]).normalized()),
		alpha(dot(normal,vertices[a]))
		{
		}
		
		bool in_circle(const Vec3f& n)
		{
			double a = dot(n,normal);
			return (a>(alpha+1e-6));
		}
	};
	
	struct EdgeKey
	{
		size_t v0, v1, minv, maxv;
		
		EdgeKey(size_t _v0, size_t _v1): v0(_v0), v1(_v1), minv(min(_v0,_v1)), maxv(max(_v0,_v1))
		{
		}
	};
	
	
	bool operator<(const EdgeKey& e0, const EdgeKey& e1)
	{
		if(e0.minv<e1.minv) return true;
		if(e0.minv>e1.minv) return false;
		return e0.maxv<e1.maxv ;
	}
	
	
	//typedef hash_map<EdgeKey,int, EdgeHashFun> EdgeMap;
	typedef map<EdgeKey,int> EdgeMap;
	typedef EdgeMap::iterator EdgeMapIter;
	
	class EdgeDB
		{
			
			EdgeMap edge_map;
		public:
			
			void insert(int a, int b)
			{
				pair<EdgeMapIter, bool> ret = 
				edge_map.insert(EdgeMap::value_type(EdgeKey(a,b),1));
				if(ret.second == false) ++ret.first->second;
			}
			
			EdgeMapIter begin() {return edge_map.begin();}
			EdgeMapIter end() {return edge_map.end();}			
		};
	
}

// subroutine triangulate
// input : vertex list
// output : triangle list
void Delaunay_on_sphere(const vector<Vec3f>& vertices_tmp,
						vector<Vec3i>& triangles_out)
{
	vector<Vec3f> vertices(vertices_tmp.size());
	for(int i=0;i<vertices.size(); ++i)
		vertices[i] = vertices_tmp[i].normalized();
	list<SphereTriangle> triangles;				// Internally, we use a list of triangles to be able
												// to add and delete triangles from the list.

	// Now create the initial two triangles. This is a degenerate situation with two triangles
	// having the same three vertices. As soon as more points are inserted, the situation is 
	// rectified.d
	SphereTriangle initial_tri0 = SphereTriangle(0,1,2,vertices);
	triangles.push_back(initial_tri0);
	SphereTriangle initial_tri1 = SphereTriangle(2,1,0,vertices);
	triangles.push_back(initial_tri1);
	
	// We might need to visit the same point several times, so we use a vector of vertex indices
	// rather than directly iterating over the vertices. In this way, we can reinject a vertex index
	// twice.
	vector<int> vertex_indices;
	for(int i=3;i<vertices.size();++i)
		vertex_indices.push_back(i);
	int orig_v_size = vertices.size();
	
	// for each sample point in the vertex list
	for(int i=0;i<vertex_indices.size();++i)
	{
		//assert(i<2*orig_v_size); // kill, if the list of vertex indices is twice the original size
		int v_idx = vertex_indices[i];
		Vec3f p = vertices[v_idx];
		// initialize the edge buffer
		EdgeDB edge_map;
		
		bool did_work = false;
		// for each triangle currently in the triangle list
		list<SphereTriangle>::iterator tri_iter = triangles.begin();
		while(tri_iter != triangles.end())
		{
			
			// If the point lies in the triangle circumcircle then
			if(tri_iter->in_circle(p))
			{
				Vec3i corners = tri_iter->corners;
				// add the three triangle edges to the edge buffer
				edge_map.insert(corners[0],corners[1]);
				edge_map.insert(corners[1],corners[2]);
				edge_map.insert(corners[2],corners[0]);
				
				// remove the triangle from the triangle list
				tri_iter = triangles.erase(tri_iter);
				assert(triangles.size() >= 1);
				did_work = true;
			}
			else
				++tri_iter;
		}
		
		if(did_work) // I.e. if we did find at least one circumcircle containing the point
		{
			// Ignore all doubly specified edges from the edge buffer
			// this leaves the edges of the enclosing polygon only
			// add to the triangle list all triangles formed between the point 
			// and the edges of the enclosing polygon
			for(EdgeMapIter itr = edge_map.begin(); 
				itr != edge_map.end(); ++itr)
				if(itr->second == 1)
					triangles.push_front(SphereTriangle(itr->first.v0, itr->first.v1, v_idx, vertices));
		}
		else
		{
			// In case no triangle seems to contain the point, we add a small random vector to its position
			// and add the point to the end of the list. In the final triangulation we will use the original
			// position so the modified point is just to avoid degeneracies.
			vertices[v_idx] = (vertices[v_idx] + 0.001 * rand_unit_vector()).normalized();
			vertices[v_idx] = vertices[v_idx];
			vertex_indices.push_back(v_idx);
		}
	}
	
	// Produce the triangle vector in output format.
	for(list<SphereTriangle>::iterator tri_iter = triangles.begin(); tri_iter!=triangles.end(); ++tri_iter)
		triangles_out.push_back(tri_iter->corners);
}
	
