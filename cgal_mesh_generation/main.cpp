#include <cstdio>
#include <fstream>

#include <CGAL/IO/print_wavefront.h>

#include "image_level_surface_to_mesh_converter.h"

int main(int argc, char* argv[]) {
  using namespace ::std;
  using namespace ::CGAL;
  using namespace ::cvc::skull_atlas;
  typedef Polyhedron_3<Exact_predicates_inexact_constructions_kernel> Polyhedron;

  if (argc != 4) {
    fprintf(stderr, "Usage: ./inr2obj input_image.inr output_mesh.obj iso_value\n");
    return -1;
  }

  const char* input_image_filename = argv[1];
  const char* output_mesh_filename = argv[2];
  double iso_value = atof(argv[3]);

  Image_3 image;
  if (!image.read(input_image_filename)) {
    fprintf(stderr, "CGAL fails to read image file \"%s\".\n", input_image_filename);
    return -1;
  }

  Polyhedron mesh = ImageLevelSurfaceToMeshConverter<>().convertToMesh(image, iso_value);
  
  {
    ofstream fout(output_mesh_filename);
    print_polyhedron_wavefront(fout, mesh);
  }

  return 0;
}

