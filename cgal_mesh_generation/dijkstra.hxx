#include <map>
#include <set>
#include <utility>
#include <vector>

#include <Eigen/Core>

namespace from_igl {

  using namespace igl;

// Dijstra's algorithm for shortest paths, with multiple targets.
// Adapted from http://rosettacode.org/wiki/Dijkstra%27s_algorithm .
//
// Inputs:
//   source           index of source vertex
//   targets          target vector set
//   w                weight of edge corresponding to VV[i][j]
//   VV               #V list of lists of incident vertices (adjacency list), e.g.
//                    as returned by igl::adjacency_list
//
// Output:
//   min_distance     #V by 1 list of the minimum distances from source to all vertices
//   previous         #V by 1 list of the previous visited vertices (for each vertex) - used for backtracking
//
int dijkstra_compute_paths(const int &source,
                           const std::set<int> &targets,
                           const std::vector<std::vector<int> >& VV,
                           const std::vector<std::vector<double> > &w,
                           Eigen::VectorXd &min_distance,
                           Eigen::VectorXi &previous)
{
  int numV = VV.size();
  min_distance.setConstant(numV, 1, std::numeric_limits<double>::infinity());
  min_distance[source] = 0;
  previous.setConstant(numV, 1, -1);
  std::set<std::pair<double, int> > vertex_queue;
  vertex_queue.insert(std::make_pair(min_distance[source], source));

  while (!vertex_queue.empty())
  {
    double dist = vertex_queue.begin()->first;
    int u = vertex_queue.begin()->second;
    vertex_queue.erase(vertex_queue.begin());

    if (targets.find(u)!= targets.end())
      return u;

    // Visit each edge exiting u
    const std::vector<int> &neighbors = VV[u];
    for (int i = 0; i < neighbors.size(); ++i)
    {
      int v = neighbors[i];
      double distance_through_u = dist + w[u][i];

      if (distance_through_u < min_distance[v]) {
        vertex_queue.erase(std::make_pair(min_distance[v], v));

        min_distance[v] = distance_through_u;
        previous[v] = u;
        vertex_queue.insert(std::make_pair(min_distance[v], v));
      }

    }
  }
  //we should never get here
  return -1;
}
} //from_igl
