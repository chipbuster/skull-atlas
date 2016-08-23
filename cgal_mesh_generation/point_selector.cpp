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

enum SelectionType { CURVATURE, DARKNESS };

// Split a string into words.
std::vector<std::string> split(const std::string &input) {
  using namespace std;
  istringstream iss(input);
  vector<string> tokens{istream_iterator<string>{iss},
                        istream_iterator<string>{}};
  return tokens;
}

std::string prettyprint(const std::string &input, int width, int trailing_spaces) {
  using namespace std;
  vector<string> words = split(input);
  
  string ret;
  int cur_width = 0;
  for (const string &s : words) {
    if (cur_width + s.size() > width) {
      ret += "\n";
      for (int i = 0; i < trailing_spaces; ++i) {
        ret += " ";
      }
      cur_width = 0;
    }
    ret += s + " ";
    cur_width += s.size() + 1;
  }

  return ret;
}

void buildChoiceSide(std::vector<std::string> &prompts,
                     std::vector<SelectionType> &types,
                     std::string side) {
  using namespace std;
  prompts.push_back(string("Insert points along the squamous suture [patient ")
                    + side + "]");
  types.push_back(DARKNESS);
  types.push_back(CURVATURE);
  prompts.push_back(string("Insert points from the squamous x lambdoid sutures "
                           "to the tip of the mastoid process [patient ")
                    + side + "]");
  types.push_back(CURVATURE);
  prompts.push_back(string("Insert points from the tip of the mastoid process "
                           "along the zygomatic bone to the top of the skull "
                           "[patient ")
                    + side + "]");
}

void buildChoiceTop(std::vector<std::string> &prompts,
                    std::vector<SelectionType> &types) {
  using namespace std;

  prompts.push_back("Insert points along the coronal suture, "
                    "starting at the sphenoid bone on one side, "
                    "and ending at the sphenoid bone on the other side.");
  types.push_back(DARKNESS);
  prompts.push_back("Insert points along the sagittal suture.");
  types.push_back(DARKNESS);
  prompts.push_back("Insert points along the lambdoid suture");
  types.push_back(DARKNESS);
}

void buildChoiceFront(std::vector<std::string> &prompts,
                      std::vector<SelectionType> &types) {
  prompts.push_back("Insert points along the chin, starting at the jaw attachment "
                    "point on one side, ending on the other side");
  types.push_back(CURVATURE);
  prompts.push_back("Insert points along the outside of the left eye");
  types.push_back(CURVATURE);
  prompts.push_back("Insert points along the outside of the right eye");
  types.push_back(CURVATURE);
  prompts.push_back("Insert points along the outside of the nose");
  types.push_back(CURVATURE);
}

void buildChoices(std::vector<std::string> &prompts,
                  std::vector<SelectionType> &types) {
  buildChoiceTop(prompts, types);
  buildChoiceSide(prompts, types, "left");
  buildChoiceSide(prompts, types, "right");
  buildChoiceFront(prompts, types);
}

void printChoice(int ptnum, const std::vector<std::string> &prompts) {
  if (ptnum >= prompts.size()) {
    printf("Press 'w' to write points to an output file\n");
  } else {
    std::string msg = prettyprint(prompts[ptnum], 74, 8);
    printf("%2d/%2d -- %s\n", ptnum + 1, prompts.size(), msg.c_str());
  }
}

// Select all points between a and b, given mesh V,F with edge weights defined by w.
// Will output indices to new_points.
void selectPointsBetween(int a, int b,
                         const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
                         const std::vector<std::vector<int> > &VV,
                         const std::vector<std::vector<double> > &w,
                         std::vector<int> &new_points) {
  // Call the Dijkstra's code.
  Eigen::VectorXd min_distance;
  Eigen::VectorXi previous;
  std::set<int> targets;
  targets.insert(b);
  int vertex_found = 
      from_igl::dijkstra_compute_paths(a, targets, VV, w, min_distance,previous);
      //igl::dijkstra_compute_paths(a, targets, VV, min_distance,previous);
  if (vertex_found == -1) {
    // ERROR
    printf("ERROR: No path found!!\n");
  } else {
    igl::dijkstra_get_shortest_path_to(vertex_found, previous, new_points);
  }
}

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

void printHelp(int ptnum) {
  printf("Press 'u' to undo previous selection\n");
  printf("Press '[' or ']' to adjust scene lighting\n");
  printf("Press 'c' to show colors for curvature\n");
  printf("Press 'd' to switch back to current selected points\n");
  printf("Press ' ' (space) to complete the current task\n");
  printf("Press '?' to repeat this message (and the requested user input).\n");
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
  

  // Prompts and types
  std::vector<std::string> prompts;
  std::vector<SelectionType> types;
  buildChoices(prompts, types);

  int point_num = 0;
  std::vector<int> all_points;  // will contain all points, once the entire mesh has been selected.

  std::vector<int> selected_points;
  std::vector<int> additional_points;

  igl::viewer::Viewer viewer;
  viewer.callback_key_down = [&](igl::viewer::Viewer& viewer, unsigned char key, int modifier)->bool {
    printf("key: '%c' modifier: '%d'\n", key, modifier);
    switch(key) {
      case ']':
        viewer.core.lighting_factor += 0.1;
        printf("Turning lighting up to %f\n", viewer.core.lighting_factor);
        break;
      case '[':
        viewer.core.lighting_factor -= 0.1;
        printf("Turning lighting down to %f\n", viewer.core.lighting_factor);
        break;
      case 'U':
      {
        printf("Undoing last point...\n");
        // Undo the last choice
        int last = selected_points.back();
        printf("Undoing last choice... %d\n", last);
        selected_points.pop_back();
        VC.row(last) << 1,1,1;
        VC.row(last) *= AO(last);
        // Also remove points from additional_points
        int this_p = -1;
        while ( this_p != last && additional_points.size() > 0 ) {
          // Get the last point.
          this_p = additional_points.back();
          // Set the color
          VC.row(this_p) << 1,1,1;
          VC.row(this_p) *= AO(this_p);
          // Remove the point.
          additional_points.pop_back();
        }
        
        // Need to add in any colors we might have removed
        for (int i : additional_points) {
          VC.row(i) << 1,0,0;
        }
        for (int i : selected_points) {
          VC.row(i) << 1,0,0;
        }

        // Set the colors back to what they were.
        viewer.data.set_colors(VC);
        printChoice(point_num, prompts);
        break;
      }
      case ' ':
      {
        printf("Finished with current curve...Moving on to next\n");
        // Add all the points to all_points
        all_points.insert(all_points.end(), 
                          additional_points.begin(),
                          additional_points.end());
        additional_points.clear();
        selected_points.clear();

        point_num++;
        printChoice(point_num, prompts);
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
          printHelp(selected_points.size());
          printChoice(point_num, prompts);
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

          selected_points.push_back(vid);
          VC.row(vid) << 1,0,0;

          // Update all the colors
          //Eigen::MatrixXd vcol;
          //igl::jet(VC, false, vcol);
          if (selected_points.size() > 1) {
            std::vector<int> path;
            if (types[point_num] == DARKNESS) {
              printf("Using light shading...\n");
              selectPointsBetween(selected_points[selected_points.size() - 2],
                                  selected_points[selected_points.size() - 1],
                                  V,F, VV, darkWeight,
                                  path);
            } else {
              printf("Using curvature...\n");
              selectPointsBetween(selected_points[selected_points.size() - 2],
                                  selected_points[selected_points.size() - 1],
                                  V,F, VV, curveWeight,
                                  path);
            }
            printf("Setting colors of %d...\n", path.size());
            for (int i = 0; i < path.size(); ++i) {
              VC.row(path[i]) << 1,0,0;
            }
            additional_points.insert(additional_points.end(), path.begin(), path.end());
          }
          
          viewer.data.set_colors(VC);


          return true;
        }
        return false;
      };

  printHelp(0);
  viewer.data.set_mesh(V, F);
  viewer.data.set_colors(curve_colors);
  viewer.core.show_lines = false;
  viewer.launch();
}

