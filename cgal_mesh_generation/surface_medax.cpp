#include <cmath>
#include <cstdio>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

#include <CGAL/number_utils.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Random.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_around_facet_circulator Facet_circulator;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

/**
 * Will output the azimuth and polar coordinates of this point.
 * Ignores the radius.
 *
 * polar will be [0, pi)
 * azimuth will be [0, 2pi]
 */
void coords(const Vector_3 &p, double &azimuth, double &polar) {
  // Ignore radius
  double radius = CGAL::to_double(
      CGAL::sqrt(p.x()*p.x() + p.y()*p.y() + p.z()*p.z()));
  polar = CGAL::to_double(acos(CGAL::to_double(p.z()) / radius));
  azimuth = CGAL::to_double(atan2(p.y(), p.x()));
}

int getCoordsDodecahedron(const Vector_3 &norm) {
  // Normalize this point.
  Vector_3 n_norm = norm;
  n_norm = n_norm / CGAL::sqrt(n_norm.squared_length());

  // Calculate the 12 vertices of the icosahedron.
  Vector_3 verts[12];
  verts[0]  = Vector_3( 2,  1, 0);
  verts[1]  = Vector_3( 2, -1, 0);
  verts[2]  = Vector_3(-2,  1, 0);
  verts[3]  = Vector_3(-2, -1, 0);

  verts[4]  = Vector_3( 1, 0,  2);
  verts[5]  = Vector_3( 1, 0, -2);
  verts[6]  = Vector_3(-1, 0,  2);
  verts[7]  = Vector_3(-1, 0, -2);

  verts[8 ] = Vector_3(0,  2,  1);
  verts[9 ] = Vector_3(0,  2, -1);
  verts[10] = Vector_3(0, -2,  1);
  verts[11] = Vector_3(0, -2, -1);
  
  // Calculate the closest point.
  int max_p = 0;
  for (int i = 0; i < 20; ++i) {
    double dot_prod = CGAL::to_double(n_norm * verts[i]);
    if (dot_prod > CGAL::to_double(n_norm * verts[max_p])) {
      max_p = i;
    }
  }

  return max_p;
}

int getCoordsIcosahedral(const Vector_3 &norm) {
  double h = (sqrt(5) - 1.0)/2.0;
  double h2 = pow(h, 2.0);

  // Normalize this point.
  Vector_3 n_norm = norm;
  n_norm = n_norm / CGAL::sqrt(n_norm.squared_length());

  // Calculate the 20 vertices of the dual dodecahedron.
  Vector_3 verts[20];
  verts[0]  = Vector_3(0,  (1.0 + h),  (1.0 - h2));
  verts[1]  = Vector_3(0,  (1.0 + h), -(1.0 - h2));
  verts[2]  = Vector_3(0, -(1.0 + h),  (1.0 - h2));
  verts[3]  = Vector_3(0, -(1.0 + h), -(1.0 - h2));

  verts[4]  = Vector_3( (1 + h),  (1 - h2), 0);
  verts[5]  = Vector_3( (1 + h), -(1 - h2), 0);
  verts[6]  = Vector_3(-(1 + h),  (1 - h2), 0);
  verts[7]  = Vector_3(-(1 + h), -(1 - h2), 0);

  verts[8]  = Vector_3( (1 - h2), 0,  (1 + h));
  verts[9]  = Vector_3( (1 - h2), 0, -(1 + h));
  verts[10] = Vector_3(-(1 - h2), 0,  (1 + h));
  verts[11] = Vector_3(-(1 - h2), 0, -(1 + h));

  verts[12] = Vector_3( 1,  1,  1);
  verts[13] = Vector_3( 1,  1, -1);
  verts[14] = Vector_3( 1, -1,  1);
  verts[15] = Vector_3( 1, -1, -1);
  verts[16] = Vector_3(-1,  1,  1);
  verts[17] = Vector_3(-1,  1, -1);
  verts[18] = Vector_3(-1, -1,  1);
  verts[19] = Vector_3(-1, -1, -1);

  // Calculate the closest point.
  int max_p = 0;
  for (int i = 0; i < 20; ++i) {
    double dot_prod = CGAL::to_double(n_norm * verts[i]);
    if (dot_prod > CGAL::to_double(n_norm * verts[max_p])) {
      max_p = i;
    }
  }

  return max_p;
}

/**
 * From a azimuth and polar coordinate, returns the int this belongs to.
 * The azimuth-polar grid is broken into az_split and pol_split segments.
 */
int getCoordsIdx(double az, double pol, int az_split, int pol_split) {
  int az_i = ((az + M_PI) / (2.0 * M_PI)) * az_split;
  az_i = std::min(az_i, az_split - 1);
  int pol_i = (pol / (1.0 * M_PI)) * pol_split;
  pol_i = std::min(pol_i, pol_split - 1);
  // Give the coordinates as on a grid.
  return az_i * pol_split + pol_i;
}

struct Tri {
  Tri(Point_3 a, Point_3 b, Point_3 c) :
      p1(a), p2(b), p3(c) {}
  Point_3 p1;
  Point_3 p2;
  Point_3 p3;

  bool operator< (const Tri &rhs) const {
    if (p1 != rhs.p1) {
      return p1 < rhs.p1;
    }
    if (p2 != rhs.p2) {
      return p2 < rhs.p2;
    }
    return p3 < rhs.p3;
  }
};

class OBJ {
 private:
  int max_three(const int &a, const int &b, const int &c) const {
    return std::max(a, std::max(b, c));
  }
 public:
  int flood_fill(std::vector<int> &facet_nums,
                  std::vector<int> &vertex_nums) const {
    vertex_nums.clear();
    vertex_nums.push_back(-1);  // an extra -1 at position 0 (it's 1-based)
    facet_nums.clear();
    // fill with -1
    for (int i = 0; i < _verts.size(); ++i) {
      vertex_nums.push_back(-1);
    }
    for (int i = 0; i < _facets.size(); ++i) {
      facet_nums.push_back(-1);
    }

    // Then, starting at number one, visit all facets until they all
    // have a number.
    int count = 0;
    facet_nums[0] = count;
    vertex_nums[_facets[0][0]] = count;
    vertex_nums[_facets[0][1]] = count;
    vertex_nums[_facets[0][2]] = count;
    bool finished = false;
    while(!finished) {
      bool dirty = true;
      while(dirty) {
        dirty = false;
        for (int i = 0; i < _facets.size(); ++i) {
          const auto &f = _facets[i];
          // If it's already been labeled, ignore it.
          if (facet_nums[i] != -1) {
            continue;
          }
          
          // Find the vertex number.
          int max_no = max_three(vertex_nums[f.at(0)],
                                 vertex_nums[f.at(1)],
                                 vertex_nums[f.at(2)]);
          // If we've already labeled one of the vertices:
          if (max_no != -1) {
            // Label the facet.
            facet_nums[i] = count;

            // Label all the vertices.
            for (int j = 0; j < 3; j++) {
              if (vertex_nums[f[j]] == -1) {
                dirty = true;
              }
              vertex_nums[f[j]] = max_no;
            }
          } // end check for max_no != -1
        } // end loop over each face
      } // end check for dirty.

      // If everything does not need to be set, continue until all facets
      // are set. Increment count and start again.
      int first_notset = -1;
      for (int i = 0; i < facet_nums.size(); ++i) {
        if (facet_nums[i] == -1) {
          first_notset = i;
          break;
        }
      }
      if (first_notset == -1) {
        finished = true;
      } else {
        count++;
        facet_nums[first_notset] = count;
        const auto &f = _facets[first_notset];
        vertex_nums[f[0]] = count;
        vertex_nums[f[1]] = count;
        vertex_nums[f[2]] = count;
      }
      // Go at the loop again with the new count!
    } // end while


    return count + 1;
  }

  int size() const {
    return _facets.size();
  }
  void insert_facet(const std::vector<Point_3> &pts) {
    std::vector<int> facet;
    for (const Point_3 &p : pts) {
      if (_verts.find(p) == _verts.end()) {
        // Needs to start at index 1
        int newIdx = _verts_linear.size() + 1;
        _verts_linear.push_back(p);
        _verts[p] = newIdx;
      }
      facet.push_back(_verts[p]);
    }
    _facets.push_back(facet);
  }
 

  void write_obj(const char* fn_pre) const {

    std::vector<int> v_ids;
    std::vector<int> f_ids;
    int num_comps = flood_fill(f_ids, v_ids);
    printf("  Number of components is: %d\n", num_comps);

    OBJ* sub_objs = new OBJ[num_comps];

    // Loop over each face.
    for (int i = 0; i < _facets.size(); ++i) {
      // Add a new facet to this object.
      std::vector<Point_3> facet;
      for (int j = 0; j < _facets[i].size(); ++j) {
        facet.push_back(_verts_linear[_facets[i][j] - 1]); // 1-based
      }

      sub_objs[f_ids[i]].insert_facet(facet);
    }
    
    // Then, write them all out to their respective objects.
    for (int i = 0; i < num_comps; ++i) {
      char* fn = new char[strlen(fn_pre) + 16];
      sprintf(fn, "%s_%d.obj", fn_pre, i);

      sub_objs[i]._write_obj_single(fn);

      delete[] fn;
    }

    delete[] sub_objs;
  }

 private:
  void _write_obj_single(const char* fn) const {

    FILE *ofs = fopen(fn, "w");
    if (!ofs) {
      perror("could not open file!");
      return;
    }

    // Needs to start at index 1
    int id_count = 1;
    for (int j = 0; j < _verts_linear.size(); ++j) {
      const Point_3 &p = _verts_linear[j];
      double x = CGAL::to_double(p.x());
      double y = CGAL::to_double(p.y());
      double z = CGAL::to_double(p.z());
      fprintf(ofs, "v %f %f %f\n", x,y,z);
    }
    for (int j = 0; j < _facets.size(); ++j) {
      const auto &f = _facets[j];
      fprintf(ofs, "f");
      for (int f_i : f) {
        //fprintf(ofs, " %d", new_id[f_i]);
        fprintf(ofs, " %d", f_i);
      }
      fprintf(ofs, "\n");
    }
    fclose(ofs);
  }


  // Contains all the vertices
  std::map<Point_3, int> _verts;
  std::vector<Point_3> _verts_linear;
  // All the facets--which can be two or more points.
  std::vector<std::vector<int> > _facets;
};

int main(int argc, char* argv[]) {

  if (argc < 4) {
    fprintf(stderr, "This program takes an input surface (.off format) and outputs\n"
            "a directory with many different objects (.obj format), usable\n"
            "by the code from John Edward's GVD-3D.\n\n"
            "usage: %s input_mesh.off output_dir num_splits\n", argv[0]);
    return -1;
  }

  CGAL::Random::State state;
  unsigned int seed = 0;
  CGAL::Random random(seed);
  random.save_state(state);
  CGAL::default_random.restore_state(state);

  const char* input_mesh_fn = argv[1];
  std::string output_dir = argv[2];

  // Read the mesh in.
  Polyhedron mesh;
  std::ifstream is(input_mesh_fn);
  if (!is) {
    fprintf(stderr, "Could not open file [%s]\n", input_mesh_fn);
    perror("Could not open input file");
    return -1;
  }
  if (!(is >> mesh)) {
    fprintf(stderr, "Error: CGAL could not read image file [%s]\n", input_mesh_fn);
    return -2;
  }
  printf("Finished reading mesh in, now getting objects\n");

  int num_splits = atoi(argv[3]);
  // Loop over each facet, getting its normal and recording that id.
  //
  // First, find the id for each facet. Also store the label(s) for each
  // of the points.
  std::map<Tri, int> facet_ids;
  std::map<Point_3, std::vector<int> > point_ids;
  for (Polyhedron::Facet_iterator f = mesh.facets_begin();
       f != mesh.facets_end();
       ++f) {
    // Get the normal of the facet
    Facet_circulator circ = f->facet_begin();
    const Point_3 &p1 = circ->vertex()->point(); circ++;
    const Point_3 &p2 = circ->vertex()->point(); circ++;
    const Point_3 &p3 = circ->vertex()->point(); circ++;

    Vector_3 v = CGAL::normal(p1, p2, p3);
    double a, p;
    coords(v, a,p);

    //int obj_idx = getCoordsIdx(a,p, num_splits,num_splits);
    int obj_idx = getCoordsIcosahedral(v);
    //int obj_idx = getCoordsDodecahedron(v);
    Tri t(p1, p2, p3);
    facet_ids[t] = obj_idx;
    point_ids[p1].push_back(obj_idx);
    point_ids[p2].push_back(obj_idx);
    point_ids[p3].push_back(obj_idx);
  }

  int num_islands = 0;
  // Look for island facets, and remove them.
  for (const auto& f : facet_ids) {
    std::vector<int> all_ids;
    const auto& p1 = f.first.p1;
    const auto& p2 = f.first.p2;
    const auto& p3 = f.first.p3;

    all_ids.insert(all_ids.end(), point_ids[p1].begin(), point_ids[p1].end());
    all_ids.insert(all_ids.end(), point_ids[p2].begin(), point_ids[p2].end());
    all_ids.insert(all_ids.end(), point_ids[p3].begin(), point_ids[p3].end());

    // Count up each of the types.
    std::map<int, int> counts;
    for (int i : all_ids) {
      counts[i]++;
    }
    // Get the max
    int max_id = all_ids[0];
    for (const auto &i : counts) {
      if (i.second > counts[max_id]) {
        max_id = i.first;
      }
    }

    int cur_id = f.second;
    // If only three indices belong to this facet, it's an island. Remove it.
    if (counts[cur_id] <= 3) {
      //fprintf(stderr, "Changing from %d to %d, because max %d counts %d\n",
      //        facet_ids.at(f.first), max_id, counts[max_id], all_ids.size());
      num_islands++;
      facet_ids.at(f.first) = max_id;
    }
  }
  printf("Removed %d islands\n", num_islands);


  std::map<int, OBJ> objs;
  for (const auto &f : facet_ids) {
    int obj_idx = f.second;
    std::vector<Point_3> facet = {f.first.p1, f.first.p2, f.first.p3};
    objs[obj_idx].insert_facet(facet);
  }
  printf("Finished calculating indices, now writing %ld files...\n", objs.size());

  for (const auto& obj : objs) {
    std::stringstream ss;
    ss << output_dir << "/split_";
    ss << obj.first;
    fprintf(stderr, "[%d facet size=%d] Writing to [%s_*]\n",
            obj.first, obj.second.size(), ss.str().c_str());
    obj.second.write_obj(ss.str().c_str());
  }
  printf("Finished!\n");
}
