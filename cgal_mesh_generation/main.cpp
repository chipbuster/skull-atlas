#include <cstdio>
#include <cstdlib>  // for atof
#include <fstream>

#include <CGAL/IO/print_wavefront.h>
#include <CGAL/Random.h>

#include "image_level_surface_to_mesh_converter.h"

int main(int argc, char* argv[]) {

  using namespace ::std;
  using namespace ::CGAL;
  using namespace ::cvc::skull_atlas;
  typedef Polyhedron_3<Exact_predicates_inexact_constructions_kernel> Polyhedron;

  if (argc < 4 || argc > 5) {
    fprintf(stderr, "This program meshes an INR file at the requested isocontour and resolution.\n");
    fprintf(stderr, "Usage: %s input_image.inr output_mesh.obj isovalue [mesh resolution = 1.0]\n", argv[0]);
    return -1;
  }

  Random::State state;
  unsigned int seed = 0;
  Random random(seed);
  random.save_state(state);
  default_random.restore_state(state);

  const char* input_image_filename = argv[1];
  const char* output_mesh_filename = argv[2];
  double isovalue = atof(argv[3]);

  Image_3 image;
  if (!image.read(input_image_filename)) {
    fprintf(stderr, "CGAL fails to read image file \"%s\".\n", input_image_filename);
    return -1;
  }

  Polyhedron mesh;
  auto converter = ImageLevelSurfaceToMeshConverter<>();

  if (argc == 4){ mesh = converter.convertToMesh(image,isovalue);}
  else if (argc == 5){
    double lod = atof(argv[4]);
    mesh = converter.convertToMesh(image,isovalue, lod);
  }

  {
    ofstream fout(output_mesh_filename);
    print_polyhedron_wavefront(fout, mesh);
  }

  return 0;
}

