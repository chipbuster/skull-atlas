#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>

#include "igl/adjacency_list.h"
#include "igl/adjacency_matrix.h"
#include "igl/components.h"
#include "igl/cotmatrix.h"
#include "igl/dijkstra.h"
#include "igl/embree/ambient_occlusion.h"
#include "igl/embree/unproject_onto_mesh.h"
#include "igl/gaussian_curvature.h"
#include "igl/invert_diag.h"
#include "igl/jet.h"
#include "igl/massmatrix.h"
#include "igl/per_vertex_attribute_smoothing.h"
#include "igl/per_vertex_normals.h"
#include "igl/polygon_mesh_to_triangle_mesh.h"
#include "igl/principal_curvature.h"
#include "igl/ray_mesh_intersect.h"
#include "igl/readOFF.h"
#include "igl/unproject_onto_mesh.h"
#include "igl/viewer/Viewer.h"
#include "igl/writeOFF.h"

void getMeanCurvature(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
                      Eigen::VectorXd &VCs) {
  // Get the laplacian
  Eigen::SparseMatrix<double> L,M,Minv;
  //   from the cotangent matrix
  igl::cotmatrix(V,F, L);
  //   and the mass matrix
  igl::massmatrix(V,F, igl::MASSMATRIX_TYPE_VORONOI, M);
  igl::invert_diag(M, Minv);

  Eigen::MatrixXd HN = -Minv*(L * V);
  // Mean curviture is this value, up to a sign.
  Eigen::VectorXd VC = HN.rowwise().norm();

  // Smooth this out a bit.
  //igl::per_vertex_attribute_smoothing(VC,F, VCs);
  VCs = VC;
  Eigen::VectorXd VC_before = VCs;
  VCs = VCs.cwiseMin(0.1);


  // Normalize all the values.
  VCs /= VCs.maxCoeff();
  VCs = VCs.cwiseSqrt();
  Eigen::VectorXd ones = Eigen::VectorXd::Constant(VCs.rows(), 1, 1);
  VCs = ones - VCs;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Used for selecting points on the surface of a mesh. Will write the\n"
                    "corresponding vertices to output.off\n");
    fprintf(stderr, "usage: %s <input.off>\n", argv[0]);
    return -1;
  }

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  
  printf("Reading in mesh...\n");
  // Read in the input file.
  igl::readOFF(argv[1], V, F);
  // Need the adjacency list
  std::vector<std::vector<int> > VV;
  igl::adjacency_list(F, VV);
  // Also caluculate the curvature values.
  printf("Calucalting curvature...\n");
  std::vector<std::vector<double> > curveWeight;
  Eigen::VectorXd mean_curve;
  

  Eigen::MatrixXd PD1,PD2;
  Eigen::VectorXd PV1,PV2;
  igl::principal_curvature(V,F,PD1,PD2,PV1,PV2, 5);
  //mean_curve = 0.5 * (PV1 + PV2);
  mean_curve = PV1.cwiseProduct(PV1) + PV2.cwiseProduct(PV2);
  mean_curve = mean_curve.cwiseMin(0.1);
  mean_curve.cwiseSqrt();
  std::cout << mean_curve << std::endl;
  //getMeanCurvature(V,F, mean_curve);


  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(V, F);
  Eigen::MatrixXd this_col;
  igl::jet(mean_curve, true, this_col);
  viewer.data.set_colors(this_col);
  viewer.core.show_lines = false;
  viewer.launch();
}

