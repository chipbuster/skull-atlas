/*
Copyright 2011. All rights reserved.
Institute of Measurement and Control Systems
Karlsruhe Institute of Technology, Germany

This file is part of libicp.
Authors: Andreas Geiger

libicp is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or any later version.

libicp is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
libicp; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA 
*/

// Demo program showing how libicp can be used

#include <iostream>

#include "icpPointToPlane.h"
#include "icpPointToPoint.h"
#include "io_OFF.h"

using namespace std;

int main (int argc, char** argv) {

  if (argc < 3) {
    cout << "usage: " << argv[0] << " <model1.off> <model2.off> <outlier> [model2trans.off]" << endl;
    return -1;
  }

  vector<vector<double> > V1, V2;
  vector<vector<int> > F1, F2;
  readOff(argv[1], V1, F1);
  readOff(argv[2], V2, F2);

  double outliers = atof(argv[3]);

  // define a 3 dim problem with 10000 model points
  // and 10000 template points:
  int32_t dim = 3;

  // allocate model and template memory
  double* M = (double*)calloc(3*V1.size(),sizeof(double));
  double* T = (double*)calloc(3*V2.size(),sizeof(double));

  // set model and template points
  for (int i = 0; i < V1.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      M[i*3 + j] = V1[i][j];
    }
  }
  for (int i = 0; i < V2.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      T[i*3 + j] = V2[i][j];
    }
  }
  cout << M[0] << " " << T[0] << endl;
  // start with identity as initial transformation
  // in practice you might want to use some kind of prediction here
  Matrix R = Matrix::eye(3);
  Matrix t(3,1);

  // run point-to-plane ICP (-1 = no outlier threshold)
  cout << endl << "Running ICP (point-to-plane, outliers " 
       << outliers << ")" << endl;
  IcpPointToPlane icp(M,V1.size(),dim);
  //IcpPointToPoint icp(M,V1.size(),dim);
  icp.fit(T,V2.size(), R,t, outliers);

  // results
  cout << endl << "Transformation results:" << endl;
  cout << "R:" << endl << R << endl << endl;
  cout << "t:" << endl << t << endl << endl;
  cout << "R*R': " << endl << R * ~R << endl << endl;

  if (argc == 5) {
    Matrix T_mat(V2.size(), 3, T);
    //Matrix M_mat(V1.size(), 3, M);

    /*
    for (int i = 0; i < V2.size(); ++i) {
      for (int j = 0; j < 3; ++j) {
        T_mat.val[i][j] -= t.val[j][0];
      }
    }
    */
    //M_mat = M_mat * R;
    T_mat = T_mat * ~R;
    //for (int i = 0; i < V1.size(); ++i) {
    for (int i = 0; i < V2.size(); ++i) {
      for (int j = 0; j < 3; ++j) {
        //V1[i][j] = M_mat.val[i][j] + t.val[j][0];
        //V2[i][j] = T_mat.val[j][i] + t.val[j][0];
        
        //V2[i][j] = T_mat.val[j][i] + t.val[j][0];
        
        V2[i][j] = T_mat.val[i][j] + t.val[j][0];
        //V2[i][j] = T_mat.val[i][j];
      }
    }
  
    //writeOff(argv[4], V1, F1);
    writeOff(argv[4], V2, F2);
    cout << "Written to " << argv[4] << endl;
  }

  // free memory
  free(M);
  free(T);

  // success
  return 0;
}

