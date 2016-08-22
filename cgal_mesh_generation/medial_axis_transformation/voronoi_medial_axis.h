#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_VORONOI_MEDIAL_AXIS_H

#include <vector>

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Side_of_triangle_mesh.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_data_structure_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>

#include "triangulation_edge_comparator.h"

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
class VoronoiMedialAxis {

 private:
  template <typename Key, typename T> using map = ::std::map<Key, T>;
  template <typename T> using vector = ::std::vector<T>;

  typedef ::CGAL::Delaunay_triangulation_3<K> Triangulation;
  typedef typename Triangulation::Edge Edge;
  typedef typename Triangulation::Cell_handle Cell;

  typedef ::CGAL::Polyhedron_3<K> Polyhedron;

  typedef ::CGAL::Side_of_triangle_mesh<Polyhedron, K, ::CGAL::Default> Side;
  typedef typename K::Point_3 Point;
  typedef typename K::Vector_3 Vector;
  typedef typename K::Triangle_3 Triangle;

 public:
  vector<Triangle> compute(::CGAL::Polyhedron_3<K> polyhedron) {

    vector<Triangle> result;

    Side side_of_polyhedron(polyhedron);

    // Creates the delaunay triangulation of the vertex set of "polyhedron".
    Triangulation triangulation;
    {
      for (auto surfaceVertex = polyhedron.vertices_begin();
          surfaceVertex != polyhedron.vertices_end();
          ++surfaceVertex) {
        auto vertex = triangulation.insert(surfaceVertex->point());
      }
    }

    map<Point, Vector> surfaceNormals = computeSurfaceNormals(polyhedron);

    ::std::set<Edge, TriangulationEdgeComparator<Triangulation>> visitedEdges;

    // Iterates over each edge.
    for (auto edge = triangulation.finite_edges_begin();
        edge != triangulation.finite_edges_end();
        ++edge) {

      // Checks that "edge" has not been visited. If so, mark "edge" as
      // visited. Otherwise, skip "edge".
      if (visitedEdges.count(*edge) == 0) {
        visitedEdges.insert(*edge);
      } else {
        continue;
      }

      if (!isSatisfyingAngleCondition(*edge, surfaceNormals) /*|| !isSatisfyingRatioCondition(edge)*/) {
        continue;
      }

      const vector<Point> voronoiFacet = getVoronoiFacet(triangulation, *edge);

      if (!isOnBoundedSide(voronoiFacet, side_of_polyhedron)) {
        continue;
      }

      for (const Triangle& triangle : triangulateVoronoiFacet(voronoiFacet)) {
        result.push_back(triangle);
      }
    }

    return result;
  }

 protected:

  vector<Point> getVoronoiFacet (
      const Triangulation& triangulation,
      Edge edge) const {
    vector<Point> result;
    auto startCell = triangulation.incident_cells(edge);
    auto cell = startCell;
    do {
      result.push_back(cell->circumcenter());
      ++cell;
    } while (cell != startCell);
    return result;
  }

  Point computeCentroid(const vector<Point>& points) const {
    Point origin = points[0];
    Vector sum(0,0,0);
    for (const Point& point : points) {
      sum = sum + (point - origin);
    }
    return origin + (sum / points.size());
  }

  vector<Triangle> triangulateVoronoiFacet(const vector<Point>& points) const {
    vector<Triangle> result;
    Point centroid = computeCentroid(points);
    int n = points.size();
    for (int i = 0; i != n; ++i) {
      Triangle triangle(
        centroid,
        points[i],
        points[(i+1) % n]
      );
      result.push_back(triangle);
    }
    return result;
  }

  bool isOnBoundedSide(const vector<Point>& points, const Side& side) const {
    for (const Point& point : points) {
      if (side(point) != ::CGAL::ON_BOUNDED_SIDE) {
        return false;
      }
    }
    return true;
  }

  Vector computeSurfaceNormal(
      const typename Polyhedron::Vertex& vertex) const {
    Point p = vertex.point();

    Vector result(0,0,0);
    for (auto halfedge : convertCirculatorToList(vertex.vertex_begin())) {
      Point a = halfedge.next()->vertex()->point();
      Point b = halfedge.prev()->vertex()->point();
      result = result + ::CGAL::cross_product(a-p, b-p);
    }
    return result;
  }

  map<Point, Vector> computeSurfaceNormals(Polyhedron polyhedron) const {
    map<Point, Vector> result;
    for (auto vertex = polyhedron.vertices_begin();
        vertex != polyhedron.vertices_end();
        ++vertex) {
      result[vertex->point()] = computeSurfaceNormal(*vertex);
    }
    return result;
  }

  bool isSatisfyingAngleCondition(
      const Edge& edge,
      map<Point, Vector>& surfaceNormals) const {
    const Cell cell = edge.first;
    const int i = edge.second;
    const int j = edge.third;
    Point p = cell->vertex(i)->point();
    Point q = cell->vertex(j)->point();

    return surfaceNormals[p] * surfaceNormals[q] < 0;

//    double theta = M_PI * 0.125;
//    auto product = a * b;
//    return product * product > (a*a) * (b*b) * cos(theta) * cos(theta);
  }

//  bool isSatisfyingRatioCondition(const Edge& edge) const {
//    using namespace ::CGAL;
//
//    const Cell cell = edge.first;
//    const int i = edge.second;
//    const int j = edge.third;
//    auto q = cell->vertex(i);
//    auto p = cell->vertex(j);
//
//    for (auto halfedge : convertCirculatorToList(p->info().surfaceVertex.vertex_begin())) {
//      Point a = halfedge.next()->vertex()->point();
//      Point b = halfedge.prev()->vertex()->point();
//      Point c = p->point();
//
//      Point center = circumcenter(a, b, c);
//
//      double rho = 8;
//
//      if (squared_distance(p->point(), q->point()) > rho * rho * squared_distance(c, center)) {
//        return false;
//      }
//    }
//    return true;
//  }
};


}  // namespace skull_atlas

#endif
