#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_HELPER_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_HELPER_H

#include <vector>

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Side_of_triangle_mesh.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_data_structure_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>

namespace skull_atlas {

template <typename C>
::std::vector<typename C::value_type> convertCirculatorToList(C start) {
  C i = start;
  ::std::vector<typename C::value_type> result;
  do {
    result.push_back(*i);
    ++i;
  } while (i != start);
  return result;
}

template <typename K>
class VoronoiMedialAxisHelper {

 private:
  template <typename Key, typename T> using map = ::std::map<Key, T>;
  template <typename T> using vector = ::std::vector<T>;

  typedef ::CGAL::Delaunay_triangulation_3<K> Triangulation;
  typedef typename Triangulation::Edge Edge;

  typedef ::CGAL::Polyhedron_3<K> Polyhedron;
  typedef typename K::Point_3 Point;
  typedef typename K::Vector_3 Vector;

 public:
  VoronoiMedialAxisHelper(const ::CGAL::Polyhedron_3<K>& polyhedron)
      : classifier_(polyhedron),
        triangulation_() {
    for (auto vertex = polyhedron.vertices_begin();
        vertex != polyhedron.vertices_end();
        ++vertex) {
      triangulation_.insert(vertex->point());
    }

    surfaceNormals_ = computeSurfaceNormals(polyhedron);
  }

  ::CGAL::Polyhedron_3<K> compute() {

    vector<vector<size_t>> polygons;

    // Iterates over each edge.
    for (auto edge = triangulation_.finite_edges_begin();
        edge != triangulation_.finite_edges_end();
        ++edge) {

      if (!isSatisfyingAngleCondition(*edge) /*|| !isSatisfyingRatioCondition(edge)*/) {
        continue;
      }

      const vector<size_t> voronoiFacet = getVoronoiFacet(*edge);

      if (isOnBoundedSide(voronoiFacet)) {
        polygons.push_back(voronoiFacet);
      }
    }

    ::CGAL::Polyhedron_3<K> result;
    ::CGAL::Polygon_mesh_processing::orient_polygon_soup(
        voronoiVertices_, polygons);
    ::CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(
        voronoiVertices_, polygons, result);
    ::CGAL::Polygon_mesh_processing::triangulate_faces(result);

    return result;
  }

 protected:

  vector<size_t> getVoronoiFacet (Edge edge) {
    vector<size_t> result;
    auto startCell = triangulation_.incident_cells(edge);
    auto cell = startCell;
    do {
      Point vertex = cell->circumcenter();
      if (vertexIds_.count(vertex) == 0) {
        vertexIds_[vertex] = voronoiVertices_.size();
        voronoiVertices_.push_back(vertex);
      }
      result.push_back(vertexIds_[vertex]);
      ++cell;
    } while (cell != startCell);
    return result;
  }

  bool isOnBoundedSide(const vector<size_t>& vertexIds) {
    for (size_t vertexId : vertexIds) {
      if (classifier_(voronoiVertices_[vertexId]) != ::CGAL::ON_BOUNDED_SIDE) {
        return false;
      }
    }
    return true;
  }

  Vector computeSurfaceNormal(
      const typename Polyhedron::Vertex& vertex) {
    Point p = vertex.point();

    Vector result(0,0,0);
    for (auto halfedge : convertCirculatorToList(vertex.vertex_begin())) {
      Point a = halfedge.next()->vertex()->point();
      Point b = halfedge.prev()->vertex()->point();
      result = result + ::CGAL::cross_product(a-p, b-p);
    }
    return result;
  }

  map<Point, Vector> computeSurfaceNormals(Polyhedron polyhedron) {
    map<Point, Vector> result;
    for (auto vertex = polyhedron.vertices_begin();
        vertex != polyhedron.vertices_end();
        ++vertex) {
      result[vertex->point()] = computeSurfaceNormal(*vertex);
    }
    return result;
  }

  bool isSatisfyingAngleCondition(const Edge& edge) {
    const auto cell = edge.first;
    const int i = edge.second;
    const int j = edge.third;
    Point p = cell->vertex(i)->point();
    Point q = cell->vertex(j)->point();

    return surfaceNormals_[p] * surfaceNormals_[q] < 0;

//    double theta = M_PI * 0.125;
//    auto product = a * b;
//    return product * product > (a*a) * (b*b) * cos(theta) * cos(theta);
  }

 private:

  Triangulation triangulation_;

  ::CGAL::Side_of_triangle_mesh<Polyhedron, K, ::CGAL::Default> classifier_;

  map<Point, Vector> surfaceNormals_;

  vector<Point> voronoiVertices_;

  map<Point, size_t> vertexIds_;

};


}  // namespace skull_atlas

#endif
