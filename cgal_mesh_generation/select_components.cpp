#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

#include "igl/components.h"
#include "igl/jet.h"
#include "igl/polygon_mesh_to_triangle_mesh.h"
#include "igl/readOFF.h"
#include "igl/viewer/Viewer.h"
#include "igl/writeOFF.h"

void selectValidVerts(const Eigen::MatrixXd &Vi, const Eigen::MatrixXi &Fi,
                      const Eigen::VectorXi &Ci, int comp_num,
                      Eigen::MatrixXd &Vo, Eigen::MatrixXi &Fo) {
  printf("Extracting component number %d\n", comp_num);

  std::map<int, int> old_to_new;
  int n_i = 0;
  for (int i = 0; i < Vi.rows(); ++i) {
    if (Ci(i) == comp_num) {
      old_to_new[i] = n_i;
      n_i++;
    } else {
      // not valid
      old_to_new[i] = -1;
    }
  }
  printf("  Found %d/%d valid vertices\n", n_i, Vi.rows());

  // Resize the vertices to be the number we need.
  Vo.resize(n_i, 3);
  int v_idx = 0;
  for (int i = 0; i < Vi.rows(); ++i) {
    if (old_to_new[i] != -1) {
      Vo.row(v_idx++) = Vi.row(i);
    }
  }
  // Same thing for the faces.
  // Do this twice so we know how many we need.
  std::vector<int> f_idxs;
  for (int i = 0; i < Fi.rows(); ++i) {
    bool contained = true;
    for (int j = 0; j < Fi.row(i).size(); ++j) {
      if (old_to_new[Fi(i, j)] == -1) {
        contained = false;
        break;
      }
    }
    if (contained) {
      f_idxs.push_back(i);
    }
  }
  fprintf(stderr,"  Found %d/%d valid faces\n", f_idxs.size(), Fi.rows());
  Fo.resize(f_idxs.size(), 3);
  for (int i = 0; i < f_idxs.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      Fo(i, j) = old_to_new[Fi(f_idxs[i], j)];
    }
  }
  printf("  Done! has size %dx%d\n", Fo.rows(), Fo.cols());
}

void extractValidVerts(const Eigen::MatrixXd &Vi, const Eigen::MatrixXi &Fi,
                       const Eigen::VectorXi &Ci, const std::vector<bool> &valid,
                       Eigen::MatrixXd &Vo, Eigen::MatrixXi &Fo, Eigen::VectorXi &Co) {
  std::map<int, int> old_to_new;
  int n_i = 0;
  for (int i = 0; i < Vi.rows(); ++i) {
    if (valid[Ci(i)]) {
      old_to_new[i] = n_i;
      n_i++;
    } else {
      // not valid
      old_to_new[i] = -1;
    }
  }
  printf("  Found %d/%d valid vertices\n", n_i, Vi.rows());

  // Resize the vertices to be the number we need.
  Vo.resize(n_i, 3);
  Co.resize(n_i);
  int v_idx = 0;
  for (int i = 0; i < Vi.rows(); ++i) {
    if (old_to_new[i] != -1) {
      Co(v_idx) = Ci(i);
      Vo.row(v_idx++) = Vi.row(i);
    }
  }
  // Same thing for the faces.
  // Do this twice so we know how many we need.
  std::vector<int> f_idxs;
  for (int i = 0; i < Fi.rows(); ++i) {
    bool contained = true;
    for (int j = 0; j < Fi.row(i).size(); ++j) {
      if (old_to_new[Fi(i, j)] == -1) {
        contained = false;
        break;
      }
    }
    if (contained) {
      f_idxs.push_back(i);
    }
  }
  printf("  Found %d/%d valid faces\n", f_idxs.size(), Fi.rows());
  Fo.resize(f_idxs.size(), 3);
  for (int i = 0; i < f_idxs.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      Fo(i, j) = old_to_new[Fi(f_idxs[i], j)];
    }
  }
  printf("  Done! has size %dx%d\n", Fo.rows(), Fo.cols());
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Used for selecting connected components and saving them to a file.\n");
    fprintf(stderr, "usage: %s <input.off> <output.off>\n", argv[0]);
    return -1;
  }

  Eigen::MatrixXd Vi, Vo;
  Eigen::MatrixXi Fi, Fo;
  Eigen::VectorXi Comps;

  // Read in the input file.
  igl::readOFF(argv[1], Vi, Fi);
  igl::components(Fi, Comps);
  // Get the number of components
  int max_comp = Comps.maxCoeff();
  std::vector<bool> valid_comps(max_comp + 1);

  int component_num = 0;
  igl::viewer::Viewer viewer;
  viewer.callback_init = [&](igl::viewer::Viewer& viewer) {
    viewer.callback_key_down = [&](igl::viewer::Viewer& viewer, unsigned char key, int modifier) {
      // Can increase or decrease number of components.
      if (key == 'C' || key == 'V') {
        if (key == 'C') {
          printf("Increasing component with %c\n", key);
          component_num++;
          if (component_num > max_comp) {
            component_num = 0;
          }
        } else {
          printf("Decreasing component with %c\n", key);
          component_num--;
          component_num = std::max(component_num, 0);
        }
        selectValidVerts(Vi,Fi,Comps, component_num, Vo,Fo);
        viewer.data.clear();
        viewer.data.set_mesh(Vo, Fo);
        return true;

      } else if (key == ' ' || key == 'X') {
        if (key == ' ') {
          valid_comps[component_num] = true;
        } else {
          valid_comps[component_num] = false;
        }
        
        Eigen::VectorXi Co;
        Eigen::MatrixXd cols;
        extractValidVerts(Vi,Fi,Comps, valid_comps, Vo,Fo,Co);
        igl::jet(Co, true, cols);
        viewer.data.clear();
        viewer.data.set_mesh(Vo, Fo);
        viewer.data.set_colors(cols);
        return true;
      } else if (key == 'w' || key == 'W') {
        int n_comps = 0;
        for (int i = 0; i < valid_comps.size(); ++i) {
          if (valid_comps[i]) ++n_comps;
        }
        printf("Writing output with %d components to %s\n", n_comps, argv[2]);
        igl::writeOFF(argv[2], Vo,Fo);
      }
      return false;
    };
    viewer.screen->performLayout();
    return false;
  };

  Eigen::MatrixXd cols;
  igl::jet(Comps, true, cols);
  viewer.data.set_mesh(Vi, Fi);
  viewer.data.set_colors(cols);
  printf("Press 'C' or 'V' to (respectively) increase or decrease the components id.\n");
  printf("Press ' ' to add the current component to the total components\n");
  printf("Press 'x' to remove the current component from the total components\n");
  printf("Press 'w' to write the resulting mesh to the output file\n");
  viewer.launch();
}

