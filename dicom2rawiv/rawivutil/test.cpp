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


  img.writeToFile("test_out.rawiv");
  cout << "test_out.rawiv should have the same hash as your input" << endl;
  return 0;
}
