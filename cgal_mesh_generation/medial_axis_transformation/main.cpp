#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/IO/print_wavefront.h>

#include "voronoi_medial_axis.h"
#include "polyhedron_builder.h"

int main(int argc, char* argv[]) {

  using ::std::vector;
  typedef ::CGAL::Exact_predicates_inexact_constructions_kernel K;
  typedef ::CGAL::Polyhedron_3<K> Polyhedron;
  typedef Polyhedron::HalfedgeDS HalfedgeDS;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s input_mesh.off output_mesh.off\n", argv[0]);
    return -1;
  }

  const char* inputMeshFilename = argv[1];
  const char* outputMeshFilename = argv[2];

  // Reads "polyhedron" from "inputMeshFilename".
  Polyhedron polyhedron;
  {
    ::std::ifstream fin(inputMeshFilename);
    fin >> polyhedron;
  }

  // Computes "medialAxis" from "polyhedron".
  ::skull_atlas::VoronoiMedialAxis medialAxisTransform;
  Polyhedron medialAxis = medialAxisTransform.compute(polyhedron);

  // Writes "medialAxis" to "outputMeshFilename".
  {
    ::std::ofstream fout(outputMeshFilename);
    fout << medialAxis;
  }

  return 0;
}
