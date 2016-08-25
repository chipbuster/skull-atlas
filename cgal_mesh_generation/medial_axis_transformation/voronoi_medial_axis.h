#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_H

#include "voronoi_medial_axis_helper.h"

namespace skull_atlas {

class VoronoiMedialAxis {

 public:

  template <typename K>
  ::CGAL::Polyhedron_3<K> compute(const ::CGAL::Polyhedron_3<K>& polyhedron) {
    VoronoiMedialAxisHelper<K> helper(polyhedron);
    return helper.compute();
  }

};


}  // namespace skull_atlas

#endif
