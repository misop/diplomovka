#include "stdafx.h"
#include "QhullController.h"
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <fstream>

#include <boost\chrono\chrono.hpp>
#include <CGAL\convex_hull_3.h>

#define qh_QHpointer 1

#include <libqhullcpp\QhullError.h>
#include <libqhullcpp\QhullQh.h>
#include <libqhullcpp\QhullFacet.h>
#include <libqhullcpp\QhullFacetList.h>
#include <libqhullcpp\QhullLinkedList.h>
#include <libqhullcpp\QhullVertex.h>
#include <libqhullcpp\Qhull.h>

using orgQhull::Qhull;
using orgQhull::QhullError;
using orgQhull::QhullFacet;
using orgQhull::QhullFacetList;
using orgQhull::QhullQh;
using orgQhull::RboxPoints;
using orgQhull::QhullVertex;
using orgQhull::QhullVertexSet;


QhullController::QhullController(void)
{

}


QhullController::~QhullController(void)
{
}

vector<vector<int> > QhullController::generateConvexHullFacets(vector<OpenMesh::Vec4f> vertices) {
	RboxPoints points;
	ofstream o("o.txt");
	for (int i = 0; i < vertices.size(); i++) {
		double point[3] = {vertices[i][0], vertices[i][1], vertices[i][2]};
		//points.append(3, point);

		orgQhull::QhullPoint qp(3, point);
		points.append(qp);
	}
	o << points;
	Qhull qhull;
	qhull.runQhull(points, "i Qt");

	return vector<vector<int> >();
}
