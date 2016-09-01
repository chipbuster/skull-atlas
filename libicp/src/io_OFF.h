#ifndef IO_OFF_H
#define IO_OFF_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * Reads the given OFF file from the user, returns vectors with the data
 * Will return true iff successful, false otherwise.
 */
bool readOff(char* fn, std::vector<std::vector<double> > &V, std::vector<std::vector<int> > &F) {
  using namespace std;
  ifstream inf(fn);
  if (!inf) {
    perror("File opening error");
    return false;
  }

  string line;
  // First, get the header
  getline(inf, line);
  // Then, get the number of vertices and faces
  getline(inf, line);
  istringstream iss_num(line);
  int numV, numF, numE;
  iss_num >> numV >> numF >> numE;

  for (int i = 0; i < numV; ++i) {
    getline(inf, line);
    double x,y,z;
    istringstream iss(line);

    // Read the x,y,z coords to this point.
    vector<double> pts;
    iss >> x >> y >> z;
    pts.push_back(x);
    pts.push_back(y);
    pts.push_back(z);

    V.push_back(pts);
  }

  for (int i = 0; i < numF; ++i) {
    getline(inf, line);
    vector<int> facet;

    // Read as many vertices as you can.
    istringstream iss(line);
    int next;
    // ignore the first number
    int numf;
    iss >> numf;
    for (int j = 0; j < numf; ++j) {
      iss >> next;
      facet.push_back(next);
    }

    F.push_back(facet);
  }

  // Success
  return true;
}

bool writeOff(char *fn, const std::vector<std::vector<double> > &V,
                        const std::vector<std::vector<int> > &F) {
  using namespace std;
  ofstream ofs(fn);
  ofs << "OFF\n";
  ofs << V.size() << " " << F.size() << " 0\n";
  for (int i = 0; i <V.size(); ++i) {
    ofs << V[i][0];
    for (int j = 1; j < V[i].size(); ++j) {
      ofs << " " << V[i][j];
    }
    ofs << "\n";
  }
  // write faces
  for (int i = 0; i < F.size(); ++i) {
    ofs << F[i].size();
    for (int j = 0; j < F[i].size(); ++j) {
      ofs << " " << F[i][j];
    }
    ofs << "\n";
  }

  ofs.close();
}

#endif
