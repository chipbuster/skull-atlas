#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_CUBIC_INTERPOLATION_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_CUBIC_INTERPOLATION_H

#include <iostream>

namespace cvc {

namespace skull_atlas {

// Cubic interpolation.
class CubicInterpolation {
  // TODO: Generalize this class.

 public:

  template <typename Function>
  double operator()(const Function& function, const double x[3]) const {
     using namespace ::std;

    double sum = 0.0;

    double t[3];
    long initial_knot[3];

    for (int k = 0; k != 3; ++k) {
      t[k] = floor(x[k]);
      initial_knot[k] = lround(t[k]) - 1;
      t[k] = x[k] - t[k];
    }

    int segment_id[3];
    long current_knot[3];
    for (int i = 0; i != 4*4*4; ++i) {

      // Computes segment_id and current_knot.
      int j = i;
      for (int k = 0; k != 3; ++k) {
        segment_id[k] = j % 4;
        j /= 4;
        current_knot[k] = initial_knot[k] + segment_id[k];
      }

      // Adds to sum.
      sum += function(current_knot) * getBasis(t, segment_id);
    }

    return sum;
  }

 protected:
   double getBasis(const double t[3], const int segment_id[3]) const {
     double factor = 1.0;
     for (int k = 0; k != 3; ++k) {
       double sum = 0.0;
       for (int degree = 0; degree != 4; ++degree) {
         sum = sum * t[k] + coefficients[segment_id[k]][degree];
       }
       factor *= sum / 6;
     }
     return factor;
   }

  const double coefficients[4][4] = {
      {-1, 3, -3, 1},
      {3, -6, 0, 4},
      {-3, 3, 3, 1},
      {1, 0, 0, 0}};
};

}  // skull_atlas

}  // cvc

#endif

