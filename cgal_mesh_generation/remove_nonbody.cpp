#include <cstdio>
#include <fstream>

#include <CGAL/ImageIO.h>
#include <CGAL/IO/print_wavefront.h>
#include <CGAL/Random.h>
#include <CGAL/Side_of_triangle_mesh.h>

#include <Eigen/Core>

#include <igl/copyleft/cgal/polyhedron_to_mesh.h>
#include <igl/jet.h>
#include <igl/viewer/Viewer.h>

#include "image_level_surface_to_mesh_converter.h"

double volume(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
              const Eigen::VectorXi &include) {

  double total_det = 0;

  for (int i = 0; i < include.rows(); ++i) {
    Eigen::Matrix3d p;
    p << V(F(include(i), 0), 0), V(F(include(i), 1), 0), V(F(include(i), 2), 0),
         V(F(include(i), 0), 1), V(F(include(i), 1), 1), V(F(include(i), 2), 1),
         V(F(include(i), 0), 2), V(F(include(i), 1), 2), V(F(include(i), 2), 2);

    total_det += p.determinant();
  }

  return std::abs(total_det / 6.0);
}

int main(int argc, char* argv[]) {

  using namespace ::std;
  using namespace ::CGAL;
  using namespace ::cvc::skull_atlas;
  typedef Exact_predicates_inexact_constructions_kernel Kernel;
  typedef Polyhedron_3<Kernel> Polyhedron;
  typedef Kernel::Point_3 Point;

  bool debug = false;

  if (argc < 4) {
    fprintf(stderr, "Usage: %s input_image.inr output_mesh.inr isovalue [debug]\n", argv[0]);
    return -1;
  }

  if (argc == 5) {
    debug = true;
  }
  
  CGAL::Random::State state;
  unsigned int seed = 0;
  CGAL::Random random(seed);
  random.save_state(state);
  default_random.restore_state(state);

  const char* input_image_filename = argv[1];
  const char* output_mesh_filename = argv[2];
  double isovalue = atof(argv[3]);
  fprintf(stderr, "Using an isovalue of %lf\n", isovalue);

  Image_3 image;
  if (!image.read(input_image_filename)) {
    fprintf(stderr, "CGAL failed to read image file \"%s\".\n", input_image_filename);
    return -1;
  }
  fprintf(stderr, "Finished reading image, now extracting surface...\n");

  Polyhedron mesh = ImageLevelSurfaceToMeshConverter<>().convertToMesh(image, isovalue);

  fprintf(stderr, "Created mesh. Now processing...\n");
  mesh.keep_largest_connected_components(1);

  CGAL::Side_of_triangle_mesh<Polyhedron, Kernel> inside(mesh);

  for (int i = 0; i < image.xdim(); ++i) {
    for (int j = 0; j < image.ydim(); ++j) {
      for (int k = 0; k < image.zdim(); ++k) {
        Point p(i * image.vx(), j * image.vy(), k * image.vz());
        CGAL::Bounded_side res = inside(p);

        // If it's outside, set the value to be zero.
        if (!(res == CGAL::ON_BOUNDED_SIDE || res == CGAL::ON_BOUNDARY)) {
          // Method for accessing data, found at the definition of static_evaluate
          // in CGAL/ImageIO.h.
          ((float*)image.data())[ (k * image.ydim() + j) * image.xdim() + i ] = 0.f;
        }
      }
    }
  }

  // If debug is turned on, display the mesh.
  if (debug) {
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    // Viewing the mesh after we're done.
    igl::copyleft::cgal::polyhedron_to_mesh(mesh, V,F);
    igl::viewer::Viewer viewer;
    viewer.data.clear();
    viewer.data.set_mesh(V, F);
    viewer.launch();
  }

  fprintf(stderr, "Finished. Now writing image to %s\n", argv[2]);
  // Call CGAL's _writeImage function.
  _writeImage(image.image(), argv[2]);

  return 0;
}

