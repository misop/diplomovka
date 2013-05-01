#pragma once
#include "MeshGraph.h"
#include <vector>
#include <boost\numeric\ublas\matrix.hpp>
#include <jama_qr.h>
#include "SkeletonNode.h"
#include <fstream>

void contractMeshGraphCPUCotangent(MeshGraph * pMesh);

void computeLaplacian(MeshGraph * pMesh);

void log(TNT::Array2D< double > matrix, ostream *os);
void logB(TNT::Array2D< bool > matrix, ostream *os);