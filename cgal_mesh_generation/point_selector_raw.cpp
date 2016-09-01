#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Core>

#include "igl/AABB.h"
#include "igl/adjacency_list.h"
#include "igl/adjacency_matrix.h"
#include "igl/ambient_occlusion.h"
#include "igl/components.h"
#include "igl/cotmatrix.h"
#include "igl/decimate.h"
#include "igl/dijkstra.h"
#include "igl/embree/ambient_occlusion.h"
#include "igl/embree/EmbreeIntersector.h"
#include "igl/embree/unproject_onto_mesh.h"
#include "igl/gaussian_curvature.h"
#include "igl/invert_diag.h"
#include "igl/jet.h"
#include "igl/per_vertex_attribute_smoothing.h"
#include "igl/per_vertex_normals.h"
#include "igl/point_mesh_squared_distance.h"
#include "igl/polygon_mesh_to_triangle_mesh.h"
#include "igl/principal_curvature.h"
#include "igl/ray_mesh_intersect.h"
#include "igl/readOFF.h"
#include "igl/unproject_onto_mesh.h"
#include "igl/viewer/Viewer.h"
#include "igl/writeOFF.h"

#include "GLFW/glfw3.h"

#include "dijkstra.hxx"

const double mOCC_MAX_RAY_DIST = 10.0;
const int mOCC_NUM_RAYS = 20;
const double mDECIMATE_PERC = 0.6;

void getMeanCurvature(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
                      Eigen::VectorXd &VCs) {
  Eigen::MatrixXd PD1,PD2;
  Eigen::VectorXd PV1,PV2;
  //igl::principal_curvature(V,F,PD1,PD2,PV1,PV2, 5);
  igl::principal_curvature(V,F,PD1,PD2,PV1,PV2, 8);
  //mean_curve = 0.5 * (PV1 + PV2);
  VCs = PV1.cwiseProduct(PV1) + PV2.cwiseProduct(PV2);
  VCs = VCs.cwiseMin(0.1);
  VCs /= VCs.maxCoeff();

  Eigen::VectorXd ones = Eigen::VectorXd::Constant(VCs.rows(), 1, 1);
  VCs = ones - VCs;


  /*
  // Get the laplacian
  Eigen::SparseMatrix<double> L,M,Minv;
  //   from the cotangent matrix
  igl::cotmatrix(V,F, L);
  //   and the mass matrix
  igl::massmatrix(V,F, igl::MASSMATRIX_TYPE_VORONOI, M);
  igl::invert_diag(M, Minv);

  Eigen::MatrixXd HN = -Minv*(L * V);
  // Mean curviture is this value.
  Eigen::VectorXd VC = HN.rowwise().norm();

  // Smooth this out a bit.
  igl::per_vertex_attribute_smoothing(VC,F, VCs);
  // Cutoff at 1 (to avoid overpowering by high curvature)
  VCs = VCs.cwiseMin(1.0);

  // Normalize
  VCs /= VCs.maxCoeff();
  // Invert so high curvature has low weight.
  Eigen::VectorXd ones = Eigen::VectorXd::Constant(VCs.rows(), 1, 1);
  // Sqrt before switching
  VCs = VCs.cwiseSqrt();
  VCs = ones - VCs;
  //std::cout << VCs;
  */
  
}

void printHelp() {
  printf("Press 'U' to undo last point\n");
  printf("Press 'C' to show colors from curvature\n");
  printf("Press 'D' to show colors from ambient occlusion\n");
}

bool file_exists(const char* fn) {
  FILE* ifs = fopen(fn, "r");
  if (ifs) {
    fclose(ifs);
    return true;
  }
  return false;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Used for selecting points on the surface of a mesh. Will write the\n"
                    "corresponding vertices to output.off\n");
    fprintf(stderr, "usage: %s <input.off> <output.off> [decimate perc]\n",
            argv[0]);
    return -1;
  }

  Eigen::MatrixXd V,V2;
  Eigen::MatrixXi F,F2;
  igl::embree::EmbreeIntersector ei;

  printf("Reading in mesh...\n");
  // Read in the input file.
  igl::readOFF(argv[1], V, F);
  
  if (argc == 4) {
    float dec_perc = atof(argv[3]);
    int new_faces = dec_perc * F.rows();
    printf("Decimating by %f to %d faces...\n", dec_perc, new_faces);
    // First things first: decimate the mesh
    Eigen::VectorXi J;
    igl::decimate(V,F, new_faces, V2,F2,J);
    printf("Decimated from V:%d,F:%d to V:%d,F:%d\n",
           V.rows(), F.rows(), V2.rows(), F2.rows());
    V = V2; F = F2;
  }

  // Colors are all white to start.
  Eigen::MatrixXd VC = Eigen::MatrixXd::Constant(V.rows(),3,1);
  // Initialize this thing so we can get the vertices.
  ei.init(V.cast<float>(), F);

  // Ambient occlusion
  Eigen::VectorXd AO;
  Eigen::MatrixXd N;
  igl::per_vertex_normals(V,F,N);
  // Compute ambient occlusion factor using embree
  printf("Building AABB tree...\n");
  igl::AABB<Eigen::MatrixXd, 3> aabb;
  aabb.init(V, F);
  const auto & shoot_ray = [&aabb,&V,&F](
      const Eigen::Vector3f& _s,
      const Eigen::Vector3f& dir)->bool
  {
    Eigen::Vector3f s = _s+1e-4*dir;
    igl::Hit hit;
    if (aabb.intersect_ray(V,F, s.cast<double>().eval(),dir.cast<double>().eval(), hit)) {
      return hit.t < mOCC_MAX_RAY_DIST;
    } 
    return false;
  };

  printf("Now setting ambient occlusion vertices to improve contrast (patience)...\n");
  //igl::embree::ambient_occlusion(V,F,V,N,300,AO);
  igl::ambient_occlusion(shoot_ray, V,N, mOCC_NUM_RAYS,AO);
  AO = 1.0 - AO.array();
  for (int i = 0; i < V.rows(); ++i) {
    VC.row(i) *= AO(i);
  }
  
  printf("Constructing adjacency list...\n");
  // Need the adjacency list
  std::vector<std::vector<int> > VV;
  igl::adjacency_list(F, VV);

  std::vector<std::vector<double> > darkWeight;
  darkWeight.resize(VV.size());
  //std::vector<std::vector<double> > curveWeight;
  //curveWeight.resize(VV.size());
  for (int i = 0; i < VV.size(); ++i) {
    for (int j = 0; j < VV[i].size(); ++j) {
      // Just use the value of the ambient occlusion.
      darkWeight[i].push_back(AO(VV[i][j]));
      //curveWeight[i].push_back(1.0 - AO(VV[i][j]));
    }
  }

  // Also caluculate the curvature values.
  printf("Calculating mean curvature...\n");
  Eigen::VectorXd mean_curve;
  getMeanCurvature(V,F, mean_curve);
  std::vector<std::vector<double> > curveWeight;
  curveWeight.resize(VV.size());
  double maxCurve = 0;
  double minCurve = 0;
  for (int i = 0; i < VV.size(); ++i) {
    for (int j = 0; j < VV[i].size(); ++j) {
      // Difference in gausssian curvature
      //double diff = std::abs(VCs(i) - VCs(j));
      double diff = mean_curve(VV[i][j]);
      curveWeight[i].push_back(diff);
      maxCurve = std::max(diff, maxCurve);
      minCurve = std::min(diff, minCurve);
    }
  }

  // Create the colors for mean curviture to use
  Eigen::MatrixXd curve_colors;
  igl::jet(mean_curve, true, curve_colors);
  

  std::vector<int> all_points;  // will contain all selected points.
  
  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(V, F);
  viewer.data.set_colors(curve_colors);
  viewer.core.show_lines = false;

  if (file_exists(argv[2])) {
    printf("File exists! Adding points from previous run!\n");
    Eigen::MatrixXd sp;
    Eigen::MatrixXd spf;
    igl::readOFF(argv[2], sp, spf);

    // Create indices.
    Eigen::VectorXi ids(V.rows());
    for (int i = 0; i < V.rows(); ++i) {
      ids(i) = i;
    }

    Eigen::VectorXd D;
    Eigen::VectorXi I;
    Eigen::MatrixXd CP;
    igl::point_mesh_squared_distance(sp, V,ids, D,I,CP);

    // Add them to the viewer.
    for (int i = 0; i < sp.rows(); ++i) {
      all_points.push_back(I(i));
      VC.row(I(i)) << 1,0,0;

      std::stringstream label;
      label << all_points.size();
      viewer.data.add_label(sp.row(i), label.str());
    }
  }

  viewer.callback_key_down = [&](igl::viewer::Viewer& viewer, unsigned char key, int modifier)->bool {
    switch(key) {
      case 'U':
      {
        printf("Undoing last point...\n");
        // Undo the last choice
        int last = all_points.back();
        printf("Undoing last choice... %d\n", last);
        all_points.pop_back();
        VC.row(last) << 1,1,1;
        VC.row(last) *= AO(last);
        
        // Set the colors back to what they were.
        viewer.data.set_colors(VC);
        viewer.data.labels_positions.conservativeResize(all_points.size(), 3);
        viewer.data.labels_strings.pop_back();
        break;
      }
      case 'C':
        printf("Setting curvature colors...\n");
        viewer.data.set_colors(curve_colors);
        break;
      case 'D':
        printf("Setting colors to highlight darkness...\n");
        viewer.data.set_colors(VC);
        break;
      case 'W':
      {
        printf("Writing output to %s\n", argv[2]);
        // Write to output file
        FILE* ofs = fopen(argv[2], "w");
        fprintf(ofs, "OFF\n");
        fprintf(ofs, "%d 0 0\n", all_points.size());
        for (int idx : all_points) {
          fprintf(ofs, "%f %f %f\n", V(idx, 0), V(idx, 1), V(idx, 2));
        }
        fclose(ofs);
        // You can quit now.
        printf("Finished! Please close the program.\n");
        break;
      }
      default:
        // Question mark ('?')
        if (key == '/' && modifier == GLFW_MOD_SHIFT) {
          printHelp();
        }
    }

    // Make sure this stays the same.
    viewer.core.lighting_factor = 
        std::min(std::max(viewer.core.lighting_factor,0.f),1.f);
    return false;
  };
  viewer.callback_mouse_down = 
      [&](igl::viewer::Viewer &viewer, int, int)->bool
      {
        // fid will be the face ID. Need to get the vertex ID
        int fid, vid;
        // Cast a ray in the view direction starting from the mouse position
        double x = viewer.current_mouse_x;
        double y = viewer.core.viewport(3) - viewer.current_mouse_y;
        if(igl::embree::unproject_onto_mesh(Eigen::Vector2f(x,y), F, 
                                            viewer.core.view * viewer.core.model,
                                            viewer.core.proj,
                                            viewer.core.viewport,
                                            ei,
                                            fid, vid)) {
          // If it already exists, ignore it.
          for (int i : all_points) {
            if (vid == i) return false;
          }

          all_points.push_back(vid);
          VC.row(vid) << 1,0,0;
          viewer.data.set_colors(VC);
          std::stringstream label;
          label << all_points.size();
          viewer.data.add_label(V.row(vid), label.str());

          return true;
        }
        return false;
      };

  printHelp();
  viewer.launch();
}

