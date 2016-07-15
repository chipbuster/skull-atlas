#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_IMAGE_LEVEL_SURFACE_TO_MESH_CONVERTER_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_IMAGE_LEVEL_SURFACE_TO_MESH_CONVERTER_H

#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Image_3.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>

#include "image_level_surface_function.h"

namespace cvc {

namespace skull_atlas {

// Converter from ::CGAL::Image_3 to ::CGAL::Polyhedron.
//
// The template parameter "GT" should be a geometric trait kernel.
template <class GT = ::CGAL::Exact_predicates_inexact_constructions_kernel>
class ImageLevelSurfaceToMeshConverter {
 public:

  // Converts "image" to a mesh, using the level surface at "iso_value".
  ::CGAL::Polyhedron_3<GT> convertToMesh(
      const ::CGAL::Image_3& image, typename GT::FT isovalue){

    using namespace ::std;
    using namespace ::CGAL;
    using namespace ::CGAL::Surface_mesher;

    typedef typename Surface_mesh_default_triangulation_3_generator<GT>::Type Tr;

    Implicit_surface_3<GT, ImageLevelSurfaceFunction<GT> > surface(
        ImageLevelSurfaceFunction<GT>(&image, isovalue),
        computeBoundingSphere(image));

    FT facet_angle_lower_bound_in_degree = 30;
    // TODO: Choose these parameters carefully.
    FT delaunay_radius_upper_bound = 5;
    FT delaunay_center_to_center_distance_upper_bound = 5;
    Surface_mesh_default_criteria_3<Tr> criteria(
        facet_angle_lower_bound_in_degree,
        delaunay_radius_upper_bound,
        delaunay_center_to_center_distance_upper_bound);

    Tr tr;
    Complex_2_in_triangulation_3<Tr> c2t3(tr);
    make_surface_mesh(c2t3, surface, criteria, Manifold_tag(), 1000);

    Polyhedron_3<GT> polyhedron;
    output_surface_facets_to_polyhedron(c2t3, polyhedron);

    return polyhedron;
  }

 private:

  typedef typename GT::FT FT;
  typedef typename GT::Point_3 Point;
  typedef typename GT::Sphere_3 Sphere;

  // Computes a bounding sphere of "image".
  Sphere computeBoundingSphere(const ::CGAL::Image_3& image) {

    using namespace ::std;
    using namespace ::CGAL;
    
    // The image must have a negative value at the center.
    Point center( -image.vx(), -image.vy(), -image.vz());

    Point vertex_on_sphere(
        (image.xdim() - 1) * image.vx(),
        (image.ydim() - 1) * image.vy(),
        (image.zdim() - 1) * image.vz());

    FT squared_radius = squared_distance(center, vertex_on_sphere);

    return Sphere(center, squared_radius);
  }
};

} // namespace skull_atlas

} // namespace cvc

#endif
