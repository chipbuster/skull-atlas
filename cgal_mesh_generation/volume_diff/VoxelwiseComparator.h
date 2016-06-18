#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_VOXELWISECOMPARATOR_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_VOXELWISECOMPARATOR_H

#include <CGAL/Image_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Side_of_triangle_mesh.h>

#include "GrayScaleImage3.h"

namespace cvc {

namespace skull_atlas {

class VoxelwiseComparator {
 public:

  float GetIsovalue() const;

  void SetIsovalue(float isovalue);

  template <class K>  GrayScaleImage3 compare(
      const ::CGAL::Image_3& image,
      const ::CGAL::Polyhedron_3<K>& polyhedron);

 private:
  float isovalue_ = 1.0;
  float true_positive = 1.0;
  float false_negative = 0.75;
  float false_positive = 0.25;
  float true_negative = 0.0;
};

float VoxelwiseComparator::GetIsovalue() const{
  return isovalue_;
}

void VoxelwiseComparator::SetIsovalue(float isovalue) {
  isovalue_ = isovalue;
}

template <class K> GrayScaleImage3 VoxelwiseComparator::compare(
      const ::CGAL::Image_3& image,
      const ::CGAL::Polyhedron_3<K>& polyhedron) {
  using namespace ::CGAL;

  typedef typename K::Point_3 Point;
  typedef Side_of_triangle_mesh<Polyhedron_3<K>, K, Default> Side;

  Side side(polyhedron);

  GrayScaleImage3 result(
      image.xdim(), image.ydim(), image.zdim(),
      image.vx(), image.vy(), image.vz());

  for (int i = 0; i < image.xdim(); ++i) {
    for (int j = 0; j < image.ydim(); ++j) {
      for (int k = 0; k < image.zdim(); ++k) {

        Point point(i * image.vx(), j * image.vy(), k * image.vz() );
        bool is_inside_mesh = (side(point) != ON_UNBOUNDED_SIDE);

        if (image.value(i, j, k) >= isovalue_) {
          if (is_inside_mesh) {
            result(i,j,k) = true_positive;
          } else {
            result(i,j,k) = false_negative;
          }
        } else {
          if (is_inside_mesh) {
            result(i,j,k) = false_positive;
          } else {
            result(i,j,k) = true_negative;
          }
        }
      }
    }
  }

  return result;
}

}  // namespace skull_atlas

}  // namespace cvc

#endif
