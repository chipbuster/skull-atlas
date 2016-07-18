#include <cstdio>
#include <map>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Image_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;

int idx(Point_3 p,
        std::vector<Point_3> &V,
        std::map<Point_3, int> &V_idx) {
  // Hasn't been inserted yet. 0 is an invalid index.
  if (V_idx[p] == 0) {
    V.push_back(p);
    V_idx[p] = V.size();
  }
  return V_idx[p];
}

int main(int argc, char** argv) {

  if (argc < 4) {
    fprintf(stderr, "Usage: %s input_image.inr output_mesh.obj isovalue\n", argv[0]);
    return -1;
  }

  const char* input_image_filename = argv[1];
  const char* output_mesh_filename = argv[2];
  double isovalue = atof(argv[3]);

  CGAL::Image_3 image;
  if (!image.read(input_image_filename)) {
    fprintf(stderr, "CGAL failed to read image file \"%s\".\n", input_image_filename);
    return -1;
  }

  fprintf(stderr, "Creating minecraft image...\n");
  std::map<Point_3, int> V_idx;
  std::vector<Point_3> V;
  std::vector<std::vector<int> > F;
  double vx = image.vx();
  double vy = image.vy();
  double vz = image.vz();

  for (int i = 1; i < image.xdim(); ++i) {
    for (int j = 1; j < image.ydim(); ++j) {
      for (int k = 1; k < image.zdim(); ++k) {
        // If the value is *greater* than the isovalue, add it and all its neighbors
        if (image.value(i, j, k) > isovalue) {
          // All 8 corners
          std::vector<int> idc;
          idc.push_back(idx(Point_3(i*vx, j*vy, k*vz), V, V_idx));
          idc.push_back(idx(Point_3(i*vx, j*vy, (k-1)*vz), V, V_idx));
          idc.push_back(idx(Point_3(i*vx, (j-1)*vy, (k-1)*vz), V, V_idx));
          idc.push_back(idx(Point_3(i*vx, (j-1)*vy, k*vz), V, V_idx));
          idc.push_back(idx(Point_3((i-1)*vx, j*vy, k*vz), V, V_idx));
          idc.push_back(idx(Point_3((i-1)*vx, j*vy, (k-1)*vz), V, V_idx));
          idc.push_back(idx(Point_3((i-1)*vx, (j-1)*vy, (k-1)*vz), V, V_idx));
          idc.push_back(idx(Point_3((i-1)*vx, (j-1)*vy, k*vz), V, V_idx));
          // All 6 faces, 2 triangles for each face.
          F.push_back({idc[0], idc[1], idc[2]}); // left
          F.push_back({idc[0], idc[2], idc[3]}); // left
          F.push_back({idc[4], idc[5], idc[6]}); // right
          F.push_back({idc[4], idc[6], idc[7]}); // right
          F.push_back({idc[0], idc[1], idc[5]}); // front
          F.push_back({idc[0], idc[5], idc[4]}); // front
          F.push_back({idc[2], idc[3], idc[7]}); // back
          F.push_back({idc[2], idc[7], idc[6]}); // back
          F.push_back({idc[0], idc[3], idc[7]}); // top
          F.push_back({idc[0], idc[7], idc[4]}); // top
          F.push_back({idc[1], idc[2], idc[6]}); // bottom
          F.push_back({idc[1], idc[6], idc[5]}); // bottom
        }
      }
    }
  }
  printf("Removing unnecessary vertices and faces...\n");
  printf(" - sorting faces...\n");
  // Sort the faces. If there's a duplicate face, remove the row.
  std::sort(F.begin(), F.end(), 
            [](const std::vector<int> &a, const std::vector<int> &b) {
            std::vector<int> a2 = a, b2 = b;
            sort(a2.begin(), a2.end());
            sort(b2.begin(), b2.end());
            if (a2[0] != b2[0]) return a2[0] < b2[0];
            if (a2[1] != b2[1]) return a2[1] < b2[1];
            if (a2[2] != b2[2]) return a2[2] < b2[2];
            });
  // Remove duplicate faces. Keep track of unique ones.
  std::vector<bool> unique(F.size(), true);
  std::vector<bool> named_V(V.size(), false);

  printf(" - checking for duplicates...\n");
  int removed = 0;
  for (int i = 1; i < F.size(); ++i) {
    std::vector<int> f1 = F[i], f2 = F[i-1];
    sort(f1.begin(), f1.end()); sort(f2.begin(), f2.end());

    if ( (f1[0] == f2[0]) &&
        (f1[1] == f2[1]) &&
        (f1[2] == f2[2]) ) {
      // Can remove both faces that are duplicated
      unique[i] = unique[i-1] = false;
      removed += 2;
    }
  }
  // See which vertices are referenced.
  for (int i = 0; i < F.size(); ++i) {
    if (unique[i]) {
      named_V[F[i][0] - 1] = true;
      named_V[F[i][1] - 1] = true;
      named_V[F[i][2] - 1] = true;
    }
  }

  printf(" - changing vertex indices...\n");
  // Get the new indices (ignoring unreferenced vertices)
  std::vector<int> new_v_idx(V.size() + 1, -1);
  int count = 0;
  for (int i = 0; i < V.size(); ++i) {
    if (named_V[i]) {
      // Faces are 1-based indices.
      new_v_idx[i + 1] = ++count;
    }
  }
  // Change the values for F to the new vertex indices.
  for (int i = 0; i < F.size(); ++i) {
    F[i][0] = new_v_idx[F[i][0]];
    F[i][1] = new_v_idx[F[i][1]];
    F[i][2] = new_v_idx[F[i][2]];
  }
  printf("Number of duplicates removed: %df, %dv\n", removed, V.size() - count);
  printf("Finished! Writing output\n");

  FILE* of = fopen(argv[4], "w");
  for (int i = 0; i < V.size(); ++i) {
    if (named_V[i]) {
      fprintf(of, "v %lf %lf %lf\n",
              V[i].x(), V[i].y(), V[i].z());
    }
  }
  for (int i = 0; i < F.size(); ++i) {
    if (unique[i]) {
      fprintf(of, "f %d %d %d\n",
              F[i][0], F[i][1], F[i][2]);
    }
  }
  fclose(of);
}
