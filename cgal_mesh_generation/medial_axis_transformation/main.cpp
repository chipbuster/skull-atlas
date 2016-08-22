#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/print_wavefront.h>

#include "voronoi_medial_axis.h"
#include "polyhedron_builder.h"

int main(int argc, char* argv[]) {

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
  Polyhedron medialAxis;
  {
    ::skull_atlas::VoronoiMedialAxis<K> medialAxisTransform;
    auto triangleList = medialAxisTransform.compute(polyhedron);
    ::skull_atlas::PolyhedronBuilder<K, HalfedgeDS> builder(triangleList);
    medialAxis.delegate(builder);
  }

  // Writes "medialAxis" to "outputMeshFilename".
  {
    ::std::ofstream fout(outputMeshFilename);
    fout << medialAxis;
  }

  return 0;
}
