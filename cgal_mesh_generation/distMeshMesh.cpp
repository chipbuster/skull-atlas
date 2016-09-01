#include "igl/point_mesh_squared_distance.h"
#include "igl/readOFF.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Used to get the distance between two meshes (non-symmetric)\n");
    fprintf(stderr, "usage: %s <input1.off> <input2.off> [outlier_dist]\n",
            argv[0]);
    return -1;
  }

  Eigen::MatrixXd V1,V2;
  Eigen::MatrixXi F1,F2;

  // Read in the input file.
  igl::readOFF(argv[1], V1, F1);
  igl::readOFF(argv[2], V2, F2);
  

  Eigen::VectorXd sqrD;
  Eigen::VectorXd I;
  Eigen::MatrixXd C;
  igl::point_mesh_squared_distance(V2, V1,F1, sqrD, I,C);

  sqrD.cwiseSqrt();
  double sum = 0;
  if (argc == 4) {
    double count = 0;
    double out_dist = atof(argv[3]);
    for (int i = 0; i < sqrD.rows(); ++i) {
      if (sqrD(i) < out_dist) {
        sum += sqrD(i);
        count ++;
      }
    }
    // Divide by the number of points within the range.
    sum /= count;
  } else {
    sum = sqrD.sum();
    sum /= V2.rows();
  }
  printf("%lf\n", sum);
}
