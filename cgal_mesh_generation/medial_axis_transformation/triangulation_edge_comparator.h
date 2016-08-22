#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_TRIANGULATION_EDGE_COMPARATOR_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_TRIANGULATION_EDGE_COMPARATOR_H

#include <algorithm>
#include <vector>

namespace skull_atlas {

template <typename Triangulation>
class TriangulationEdgeComparator {
 private:
  template <typename T> using vector = ::std::vector<T>;
  using Edge = typename Triangulation::Edge;
  using Point = typename Triangulation::Point;

 public:
  bool operator()(const Edge& x, const Edge& y) const {
    return convertToPoints(x) < convertToPoints(y);
  }

 protected:
  vector<Point> convertToPoints(const Edge& edge) const {
    const auto cell = edge.first;
    const int i = edge.second;
    const int j = edge.third;

    vector<Point> result = {
        cell->vertex(i)->point(),
        cell->vertex(j)->point()
    };
    ::std::sort(result.begin(), result.end());

    return result;
  }
};

}  // namespace skull_atlas

#endif
