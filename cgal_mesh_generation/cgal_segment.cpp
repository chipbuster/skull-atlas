#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/property_map.h>
#include <CGAL/Simple_cartesian.h>

#include <igl/barycenter.h>
#include <igl/copyleft/cgal/polyhedron_to_mesh.h>
#include <igl/jet.h>
#include <igl/point_mesh_squared_distance.h>
#include <igl/readOFF.h>
#include <igl/viewer/Viewer.h>

#include <nanogui/nanogui.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel   Kernel;
typedef Kernel::Point_3                                       Point_3;
typedef Kernel::Triangle_3                                    Triangle_3;
typedef CGAL::Polyhedron_3<Kernel>                            Polyhedron;
//typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron>  Primitive;
typedef std::list<Triangle_3>::iterator                       Tri_Iterator;
typedef CGAL::AABB_triangle_primitive<Kernel, Tri_Iterator>   Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive>                  Traits;
typedef CGAL::AABB_tree<Traits>                               AABB_Tree;
typedef Polyhedron::Halfedge_around_facet_circulator          HF_circulator;
typedef std::map<Polyhedron::Facet_const_handle, double>      Facet_double_map;

void read_triangle_soup(
    const char* in_fn, std::list<Triangle_3> &triangles) {
  std::ifstream inf(in_fn);
  // Get the header
  std::string header;
  inf >> header;
  // Get the number of verts and faces;
  int numVerts, numFaces, numEdges;
  inf >> numVerts >> numFaces >> numEdges;
  printf("Reading in %d verts, %d faces, %d edges\n",
         numVerts, numFaces, numEdges);
  
  std::vector<Point_3> points;
  // Get the vertices
  for (int i = 0; i < numVerts; ++i) {
    double x,y,z;
    inf >> x >> y >> z;
    //points.emplace_back(x, y, z);
    points.push_back(Point_3(x, y, z));
  }
  printf("Finished reading verts, now reading faces\n");

  // Get the faces -- for now, only triangles
  for (int i = 0; i < numFaces; ++i) {
    int n;
    inf >> n;
    if (n != 3) {
      fprintf(stderr, "Error: input is not a triangle mesh (expected 3, got %d)\n",
              n);
      return;
    }
    int idx1, idx2, idx3;
    inf >> idx1 >> idx2 >> idx3;
    //triangles.emplace_back(points[idx1], points[idx2], points[idx3]);
    triangles.push_back(Triangle_3(points[idx1], points[idx2], points[idx3]));
  }
}

// Instead of sdf values, uses distance to medial axis to segment.
// Needs to be normalized to [0,1]
void construct_facet_map(
    Polyhedron &mesh,  /* Need this to not be const, but won't modify it */
    const char* inf,
    boost::associative_property_map<Facet_double_map> &medax_property_map) {
  // First, read in the OFF file.
  Eigen::MatrixXd m_V, p_V, p_B;
  Eigen::MatrixXi m_F, p_F;
  igl::readOFF(inf, m_V, m_F);

  // Then, generate the barycenters of each point in the polygon.
  igl::copyleft::cgal::polyhedron_to_mesh(mesh, p_V,p_F);
  p_B.resize(p_F.rows(), 3);

  int i = 0;
  for (auto f = mesh.facets_begin(); f != mesh.facets_end(); ++f, ++i) {
    // Calculate the barycenter
    double x = 0, y = 0, z = 0;
    int num_pts = 0;
    HF_circulator h = f->facet_begin();
    do {
      const Point_3 &p = h->vertex()->point();
      x += p.x();
      y += p.y();
      z += p.z();
      num_pts ++;
    } while (++h != f->facet_begin());
    
    x /= num_pts;
    y /= num_pts;
    z /= num_pts;

    p_B.row(i) << x,y,z;

  }
  // Get the closest point to all of these.
  Eigen::VectorXi I;
  Eigen::MatrixXd C;
  Eigen::VectorXd sqrD;

  igl::point_mesh_squared_distance(p_B,p_V,p_F, sqrD,I,C);
  double max_d = sqrD.maxCoeff();

  // Then remember the point
  i = 0;
  for (auto f = mesh.facets_begin(); f != mesh.facets_end(); ++f, ++i) {
    // add it to the map
    medax_property_map[f] = sqrD(i) / max_d;
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr,"usage: cgal_segment <input.off> [medax.off]\n");
    return -2;
  }

  // create and read Polyhedron
  Polyhedron mesh; std::ifstream input(argv[1]);
  if ( !input ) {
    std::cerr << "Input is bad from the beginning\n";
    return EXIT_FAILURE;
  }
  if ( !input || !(input >> mesh) || mesh.empty() ) {
    std::cerr << "Not a valid off file." << std::endl;
    return EXIT_FAILURE;
  }

  Facet_double_map internal_medax_map;
  boost::associative_property_map<Facet_double_map> medax_property_map(
      internal_medax_map);
  bool has_medax = false;
  if (argc == 3) {
    has_medax = true;
    // create and read medax
    construct_facet_map(mesh, argv[2], medax_property_map);
    fprintf(stderr, "Finished generating medax values\n");
  }

  // create a property-map for SDF values
  Facet_double_map internal_sdf_map;
  boost::associative_property_map<Facet_double_map> sdf_property_map(
      internal_sdf_map);
  // compute SDF values using default parameters for number of rays, and cone angle
  CGAL::sdf_values(mesh, sdf_property_map);

  fprintf(stderr,"Finished generating sdf values\n");
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
  std::size_t number_of_segments = 
      CGAL::segmentation_from_sdf_values(
          mesh, sdf_property_map, segment_property_map);
  fprintf(stderr,"Number of segments: %d\n", number_of_segments);

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
  fprintf(stderr,"Finished!\n");

  bool use_medax = false;

  viewer.callback_init = [&](igl::viewer::Viewer& viewer) {
    viewer.ngui->addWindow(Eigen::Vector2i(220,10),"Segment Options");
    viewer.ngui->addVariable("# Segs", num_clusters);
    viewer.ngui->addVariable("lambda", smoothing_lambda);
    viewer.ngui->addVariable("hard clusters", hard_clusters);
    viewer.ngui->addVariable("use medax", use_medax);
    viewer.ngui->addButton("Redraw",[&]() {
           fprintf(stderr,"Redrawing for %d segs and %lf lambda...",
                  num_clusters, smoothing_lambda);
           size_t number_of_segments = 0;
           if (use_medax || !has_medax) {
             number_of_segments = CGAL::segmentation_from_sdf_values(
                 mesh, medax_property_map, segment_property_map,
                 num_clusters, smoothing_lambda, hard_clusters);
           } else {
             number_of_segments = CGAL::segmentation_from_sdf_values(
                 mesh, sdf_property_map, segment_property_map,
                 num_clusters, smoothing_lambda, hard_clusters);
           }
           fprintf(stderr,"Number of segments: %d...", number_of_segments);

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
           fprintf(stderr,"Finished!\n");
        });
      viewer.screen->performLayout();
      return false;
    };

  viewer.launch();
}
