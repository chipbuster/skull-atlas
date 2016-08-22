#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_POLYHEDRON_BUILDER_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_POLYHEDRON_BUILDER_H

#include <vector>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace skull_atlas {

template <typename K, typename HalfedgeDataStructure>
class PolyhedronBuilder : public ::CGAL::Modifier_base<HalfedgeDataStructure> {

 private:
  template <typename Key, typename T> using map = ::std::map<Key, T>;
  template <typename T> using vector = ::std::vector<T>;
  using Point = typename K::Point_3;
  using Triangle = typename K::Triangle_3;
  using IncrementalBuilder =
      ::CGAL::Polyhedron_incremental_builder_3<HalfedgeDataStructure>;

 public:
  PolyhedronBuilder(const vector<Triangle>& triangleList)
      : triangleList_(triangleList) {}

  void operator()(HalfedgeDataStructure& halfedgeDataStructure) {

    IncrementalBuilder builder(halfedgeDataStructure, true);

    builder.begin_surface(0,0,0);

    int vertexCount = 0;
    for (const Triangle& triangle : triangleList_) {

      // Add vertices.
      for (int i = 0; i != 3; ++i) {
        builder.add_vertex(triangle[i]);
      }

      // Add facet.
      builder.begin_facet();
      for (int i = 0; i != 3; ++i) {
        builder.add_vertex_to_facet(vertexCount + i);
      }
      builder.end_facet();

      vertexCount += 3;
    }

    builder.end_surface();
  }

 private:
  map<Point, int> pointIds_;
  const vector<Triangle>& triangleList_;
};

}  // namespace skull_atlas

#endif
