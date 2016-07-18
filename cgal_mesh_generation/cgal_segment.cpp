#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/property_map.h>

#include <igl/copyleft/cgal/polyhedron_to_mesh.h>
#include <igl/jet.h>
#include <igl/viewer/Viewer.h>

#include <iostream>
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    printf("usage: cgal_segment <input.off>\n");
    return -2;
  }
  // create and read Polyhedron
  Polyhedron mesh;
  std::ifstream input(argv[1]);
  if ( !input || !(input >> mesh) || mesh.empty() ) {
    std::cerr << "Not a valid off file." << std::endl;
    return EXIT_FAILURE;
  }

  // create a property-map for SDF values
  typedef std::map<Polyhedron::Facet_const_handle, double> Facet_double_map;
  Facet_double_map internal_sdf_map;
  boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map);
  printf("Finishied sdf_property_map\n");

  // compute SDF values using default parameters for number of rays, and cone angle
  CGAL::sdf_values(mesh, sdf_property_map);
  printf("Finished generating sdf values\n");
  // Get the mesh in an igl-friendly manner
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::copyleft::cgal::polyhedron_to_mesh(mesh, V, F);
  Eigen::VectorXd cols(F.rows());
  // Extract color values from map
  int i = 0;
  for (typename Polyhedron::Facet_const_iterator f = mesh.facets_begin();
       f != mesh.facets_end(); ++f) {
    cols(i++) = internal_sdf_map.find(f)->second;
  }
  // Convert from vector to RGB colors
  Eigen::MatrixXd C;
  igl::jet(cols, true, C);
  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(V, F);
  viewer.data.set_colors(C);
  //viewer.launch();

  // create a property-map for segment-ids
  typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
  Facet_int_map internal_segment_map;
  boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);

  size_t num_clusters = 4;
  double smoothing_lambda = 0.3;
  bool hard_clusters = false;

  // Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
  // This feature is relevant for segmenting the mesh several times with different parameters.
  std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map);
  printf("Number of segments: %d\n", number_of_segments);

  i = 0;
  for(Polyhedron::Facet_const_iterator facet_it = mesh.facets_begin();
      facet_it != mesh.facets_end(); ++facet_it) {
    // ids are between [0, number_of_segments -1]
    //std::cout << segment_property_map[facet_it] << " ";
    cols(i++) = segment_property_map[facet_it];
  }
  //std::cout << std::endl;
  igl::jet(cols, true, C);
  viewer.data.set_colors(C);
  printf("Finished!\n");

  viewer.callback_init = [&](igl::viewer::Viewer& viewer) {
    viewer.ngui->addWindow(Eigen::Vector2i(220,10),"Segment Options");
    viewer.ngui->addVariable("# Segs", num_clusters);
    viewer.ngui->addVariable("lambda", smoothing_lambda);
    viewer.ngui->addVariable("hard clusters", hard_clusters);
    viewer.ngui->addButton("Redraw",[&]() {
           printf("Redrawing for %d segs and %lf lambda...",
                  num_clusters, smoothing_lambda);
           size_t number_of_segments = CGAL::segmentation_from_sdf_values(
               mesh, sdf_property_map, segment_property_map,
               num_clusters, smoothing_lambda, hard_clusters);
           printf("Number of segments: %d...", number_of_segments);

           i = 0;
           for(Polyhedron::Facet_const_iterator facet_it = mesh.facets_begin();
               facet_it != mesh.facets_end(); ++facet_it) {
             // ids are between [0, number_of_segments -1]
             //std::cout << segment_property_map[facet_it] << " ";
             cols(i++) = segment_property_map[facet_it];
           }
           //std::cout << std::endl;
           igl::jet(cols, true, C);
           viewer.data.set_colors(C);
           printf("Finished!\n");
        });
      viewer.screen->performLayout();
      return false;
    };

  viewer.launch();
}
