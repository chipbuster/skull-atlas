#include <cstdio>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include "VoxelwiseComparator.h"

int main(int argc, char* argv[]) {

  using namespace ::std;
  using namespace ::CGAL;
  using namespace ::cvc::skull_atlas;
  typedef Exact_predicates_inexact_constructions_kernel K;

  if (argc != 5) {
    fprintf(stderr, "Usage: %s input_image1.inr input_mesh2.obj output_image.inr isovalue\n", argv[0]);
    return -1;
  }

  const char* input_image1_filename = argv[1];
  const char* input_mesh2_filename = argv[2];
  const char* output_image_filename = argv[3];
  float isovalue = atof(argv[4]);

  // Reads image1.
  Image_3 input_image1;
  if (!input_image1.read(input_image1_filename)) {
    fprintf(stderr, "CGAL fails to read image file \"%s\".\n", input_image1_filename);
    return -1;
  }

  // Reads mesh2.
  Polyhedron_3<K> input_mesh2;
  {
    ifstream fin(input_mesh2_filename);
    fin >> input_mesh2;
  }

  // Compares image1 and mesh2.
  VoxelwiseComparator comparator = VoxelwiseComparator();
  comparator.SetIsovalue(isovalue);
  GrayScaleImage3 output_image = comparator.compare(input_image1, input_mesh2);

  // Writes comparison result.
  int status = write(output_image, output_image_filename);

  return status;
}
