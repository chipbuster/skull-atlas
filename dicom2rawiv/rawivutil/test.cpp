#include "rawivutil.h"
#include <iostream>
#include <string>

using namespace std;
using namespace RawivUtil;

int main(int argc, char** argv){
  RawivUtil::RawivImage img = RawivImage(argv[1]);

  /*
  cout << "(" << img.dim.x << "," << img.dim.y << "," << img.dim.z << ")" << endl
       << "(" << img.maxCoords.x << "," << img.maxCoords.y << "," << img.maxCoords.z << ")" << endl
       << "(" << img.minCoords.x << "," << img.minCoords.y << "," << img.minCoords.z  << ")" << endl
       << img.numCells << endl << img.numVerts << endl
  << "(" << img.origin.x << "," << img.origin.y << "," << img.origin.z << ")" << endl
       << "(" << img.span.x << "," << img.span.y << "," << img.span.z << ")" << endl;
  */


  for (size_t i = 0; i < 10; i++){
    for (size_t j = 0; j < 10; j++){
      for (size_t k = 0; k < 10; k++){
        cout << "At coord " << i << "," << j << "," << k << "," << " value " << img(i,j,k) << endl;
      }
    }
  }

  img.writeToFile("test_out.rawiv");
  cout << "test_out.rawiv should have the same hash as your input" << endl;
  return 0;
}
