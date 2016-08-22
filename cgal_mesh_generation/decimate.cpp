#include "igl/decimate.h"
#include "igl/readOFF.h"
#include "igl/writeOFF.h"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Used to decimate a mesh by a given percentage (faces)\n\n");
    fprintf(stderr, "usage: %s <input.off> <output.off> <decimate perc>\n",
            argv[0]);
    return -1;
  }

  Eigen::MatrixXd V,V2;
  Eigen::MatrixXi F,F2;

  printf("Reading in mesh...\n");
  // Read in the input file.
  igl::readOFF(argv[1], V, F);
  
  float dec_perc = atof(argv[3]);
  int new_faces = dec_perc * F.rows();
  printf("Decimating by %f to %d faces...\n", dec_perc, new_faces);
  // First things first: decimate the mesh
  Eigen::VectorXi J;
  igl::decimate(V,F, new_faces, V2,F2,J);
  printf("Decimated from V:%d,F:%d to V:%d,F:%d\n",
         V.rows(), F.rows(), V2.rows(), F2.rows());

  printf("Writing mesh...\n");
  igl::writeOFF(argv[2], V2, F2);
  printf("Finished!\n");
}
