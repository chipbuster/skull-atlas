#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_IMAGE_LEVEL_SURFACE_FUNCTION_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_IMAGE_LEVEL_SURFACE_FUNCTION_H

#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Image_3.h>

#include "cubic_interpolation.h"

namespace cvc {

namespace skull_atlas {

// The level set surface of an image.
//
// Is Model Of: ::CGAL::ImplicitFunction
template <class GT = ::CGAL::Exact_predicates_inexact_constructions_kernel>
class ImageLevelSurfaceFunction {
 public:
  typedef typename GT::FT FT;
  typedef typename GT::Point_3 Point;

  // Constructs a level set surface of "image" at "isovalue".
  ImageLevelSurfaceFunction(const ::CGAL::Image_3* image, double isovalue)
    : image_(image),
      isovalue_(isovalue),
      interpolation_(),
      min_value_(0.0) {

    for (size_t x0 = 0; x0 != image_->xdim(); ++x0) {
      for (size_t x1 = 0; x1 != image_->ydim(); ++x1) {
        for (size_t x2 = 0; x2 != image_->zdim(); ++x2) {
          min_value_ = ::std::min(
              static_cast<double>(image_->value(x0, x1, x2)), min_value_);
        }
      }
    }
  }

  // Returns the value of the image at the (x0, x1, x2) subtracted by iso_value.
  double operator()(long x0, long x1, long x2) const {
   using namespace ::std;

    double value = min_value_;

    if (x0 >= 0 && x0 < image_->xdim()
        && x1 >= 0 && x1 < image_->ydim()
        && x2 >= 0 && x2 < image_->zdim()) {

      size_t y0 = static_cast<size_t>(x0);
      size_t y1 = static_cast<size_t>(x1);
      size_t y2 = static_cast<size_t>(x2);

      value = static_cast<FT>(image_->value(y0, y1, y2));
    }

    return value - isovalue_;
  }

  double operator()(const long x[3]) const {
    return (*this)(x[0], x[1], x[2]);
  }

  FT operator()(Point p) const {
    using namespace ::CGAL;

    const double x[3] = {
        to_double(p.x()) / image_->vx(),
        to_double(p.y()) / image_->vy(),
        to_double(p.z()) / image_->vz()};
    return FT(interpolation_(*this, x));
  }

 private:
  const ::CGAL::Image_3* image_;
  double isovalue_;
  double min_value_;
  CubicInterpolation interpolation_;
};

}  // skull_atlas

}  // cvc

#endif

