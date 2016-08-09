#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

struct Point_3 {
  Point_3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) { }
  Point_3(const Point_3 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }
  double x;
  double y;
  double z;

  bool operator <(const Point_3 &rhs) const {
    if (x != rhs.x) {
      return x < rhs.x;
    }
    if (y != rhs.y) {
      return y < rhs.y;
    }
    return z < rhs.z;
  }
};

class Triangulation {
 private:
  int start_idx = 0;
 public:
  Triangulation() {
    _vert_idx = start_idx; // Starts at 0 for an off file.
  }
  Triangulation(const std::vector<Point_3> &v,
                const std::vector<std::vector<int> >& f) {
    set_verts(v);
    set_facets(f);
  }

  void set_verts(const std::vector<Point_3> &v) {
    _vert_idx = start_idx;
    _verts_linear = v;
    for (int i = 0; i < v.size(); ++i) {
      _verts_map[v[i]] = _vert_idx++;
    }
  }

  void set_facets(const std::vector<std::vector<int> >& f) {
    _facets = f;
  }

  void insert_facet(const std::vector<Point_3> &pts) {
    // This had better be true.
    assert(pts.size() == 3);

    std::vector<int> facet;

    for (const Point_3 &p : pts) {
      add_vertex(p);
      facet.push_back(_verts_map[p]);
    }

    _facets.push_back(facet);
  }

  Point_3 v(int i) const {
    return _verts_linear[i - start_idx];
  }

  int add_vertex(const Point_3 &p) {
    if (_verts_map.find(p) == _verts_map.end()) {
      // Add to the linear vertices
      _verts_linear.push_back(p);
      // Also update the faces
      _verts_map[p] = _vert_idx++;
    }
    return _verts_map[p];
  }

  void write_off(const char* fn) const {
    FILE *ofs = fopen(fn, "w");
    // print the header
    fprintf(ofs, "OFF\n");
    fprintf(ofs, "%d %d 0\n", _verts_linear.size(), _facets.size());
    // print the vertices
    for (const Point_3 &p : _verts_linear) {
      fprintf(ofs, "%f %f %f\n", p.x, p.y, p.z);
    }
    // print the faces
    for (const std::vector<int> &f : _facets) {
      // Should only be three for this, but we'll make it work for anything.
      fprintf(ofs, "%d", f.size());
      for (int i = 0; i < f.size(); ++i) {
        fprintf(ofs, " %d", f[i]);
      }
      fprintf(ofs, "\n");
    }
    fclose(ofs);
  }
 private:
  int _vert_idx;   // 1-based index for faces

  std::map<Point_3, int> _verts_map;
  std::vector<Point_3> _verts_linear;
  std::vector<std::vector<int> > _facets;
};

void make_facet_triangular(const std::vector<int> &f,
                           Triangulation &triang,
                           std::vector<std::vector<int> > &facets) {
  // Get the centroid
  double total_x = 0;
  double total_y = 0;
  double total_z = 0;
  for (int i = 0; i < f.size(); ++i) {
    total_x += triang.v(f[i]).x;
    total_y += triang.v(f[i]).y;
    total_z += triang.v(f[i]).z;
  }
  total_x /= f.size();
  total_y /= f.size();
  total_z /= f.size();

  Point_3 mid(total_x, total_y, total_z);
  // Make sure you add it to the triangulation.
  int mid_idx = triang.add_vertex(mid);

  // Now, construct new faces that have all edges with a point in the middle.
  facets.clear();
  for (int i = 0; i < f.size(); ++i) {
    // triangle with midpoint and two points.
    facets.push_back({mid_idx, f[i], f[(i+1) % f.size()]});
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Converts an irregular polygonal mesh to a regular triangular mesh\n\n");
    fprintf(stderr, "usage: %s <input.off> <output.off>\n", argv[0]);
    return -1;
  }

  Triangulation triang;
  // Open the file
  FILE* inf = fopen(argv[1], "r");
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  // Ignore the OFF
  read = getline(&line, &len, inf);
  char name[128];
  int ret;
  if ( sscanf(line, "%s", name) != 1) {
    fprintf(stderr, "ERROR: Looking for header, got something else [%s].\n",
            line);
    return -2;
  }
  // Get the size of the mesh.
  read = getline(&line, &len, inf);
  int numVerts, numFaces;
  if ( sscanf(line, "%d %d %*d", &numVerts, &numFaces) != 2) {
    fprintf(stderr, "ERROR: Looking for number of vertices, faces, and edges, got something else [%s].\n", line);
    return -2;
  }

  // Successful, report.
  fprintf(stderr, "Reading mesh with %d verts and %d faces\n",
          numVerts, numFaces);

  std::vector<Point_3> verts;
  for (int i = 0; i < numVerts; ++i) {
    read = getline(&line, &len, inf);
    double x, y, z;
    if ( sscanf(line, "%lf %lf %lf", &x, &y, &z) != 3) {
      fprintf(stderr, "ERROR: Looking for three points, got something else [%s]\n",
              line);
      return -3;
    }
    verts.emplace_back(x, y, z);
  }

  // Set the triangles at this point.
  triang.set_verts(verts);

  const char* delims = " \t\n";
  char* pch;

  // Now, read in the faces.
  std::vector<std::vector<int> > faces;
  for (int i = 0; i < numFaces; ++i) {
    read = getline(&line, &len, inf);
    pch = strtok(line, delims);
    //printf("%s\n", line);

    int vn;
    if (sscanf(pch, "%d", &vn) != 1) {
      fprintf(stderr, "ERROR: looking for number of verts for facet, got something else [%s, %s]\n",
              line, pch);
      return -4;
    }

    std::vector<int> facet_big;
    for (int j = 0; j < vn; ++j) {
      int idx;
      pch = strtok(NULL, delims);
      if (sscanf(pch, "%d", &idx) != 1) {
        fprintf(stderr, "ERROR: looking for vertex index, got something else [%s, at %s]\n",
                line, pch);
        return -5;
      }
      facet_big.push_back(idx);
    }

    if (facet_big.size() == 3) {
      faces.push_back(facet_big);
    } else {
      std::vector<std::vector<int> > facet_all;
      make_facet_triangular(facet_big, triang, facet_all);
      for (const auto &f : facet_all) {
        faces.push_back(f);
      }
    }
  }

  printf("Size of faces is now %d\n", faces.size());
  triang.set_facets(faces);

  // Then, output the mesh.
  triang.write_off(argv[2]);
}
