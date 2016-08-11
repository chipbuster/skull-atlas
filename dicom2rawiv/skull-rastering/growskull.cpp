#include <rawivutil.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>

#include <unordered_set>
#include <queue>

using namespace std;
using namespace RawivUtil;
typedef RawivUtil::Point3<float> Point;
typedef RawivUtil::Point3<uint32_t> Coord;
typedef unordered_set<Coord> CoordSet;
typedef queue<Coord> CoordQ;

/* When trying to extract voxelized skulls, we run into the issue that 









 */

/* Initializes the voxel set we start searching from */
void initKnownVoxels(CoordSet &knownVoxels, RawivImage const &img, int threshold){
  // Need to do column-major iteration to take advantage of cache locality
  for(size_t k = 0; k < img.dim.z; k++){
    for(size_t j = 0; j < img.dim.y; j++){
      for(size_t i = 0; i < img.dim.x; i++){
        if (img(i,j,k) > threshold){ knownVoxels.insert(Coord(i,j,k)); }
      }
    }
  }
}

/* Checks to see if a coordinate is known in a CoordSet. Basically a fancy `elem`*/
inline bool isIn(Coord toTest,CoordSet const &known){
  if (known.find(toTest) == known.end()){return false;}
  else{return true;}
}

/* Implicitly assuming that dimensions run from 0 to boundaries - 1, generate
   the valid neighbors of the input coordinate */
CoordSet getNeighbors(Coord inp, Coord boundaries){
  CoordSet neighbors = CoordSet();
  constexpr unsigned int radius = 2; // How many voxels to each side do we define as a "neighbor"

  for(auto i = -2; i <= 2; i++){
    for(auto j = -2; j <= 2; j++){
      for(auto k = -2; k <= 2; k++){
        size_t x = inp.x + i;
        size_t y = inp.y + j;
        size_t z = inp.z + k;
        if(x >= 0 && x < boundaries.x &&
           y >= 0 && y < boundaries.y &&
           z >= 0 && z < boundaries.z &&
           (x != 0 || y != 0 || z != 0)){
          neighbors.insert(Coord(x,y,z));
        }
      }
    }
  }
  return neighbors;
}

/* Runs BFS (dur!) */
void runBFS(CoordSet &known, CoordQ &active, Coord maxDim, RawivImage const &img, int threshold){

  long ctr = 5000;

  //TODO: Figure out how to task this sucker
  while(!active.empty()){
    Coord current = active.front();
    active.pop();

    ctr--;
    if (ctr == 0){
    cout << "Active Voxel Count (program will finish when this reaches zero): " << active.size() << endl;
    ctr = 50000;
    }

    CoordSet currNeighbors = getNeighbors(current, maxDim);
    for (auto& elem : currNeighbors){
      if (isIn(elem, known)){
        continue; // This coordinate is already known. Skip it and move on.
      }

      //Coordinate is not known: check it for suitability.
      if(img(elem) < threshold){
        continue; //Element is below lower threshold: reject this voxel
      }

      //Coordinate is candidate. Add it to known set and active set.
      known.insert(elem);
      active.push(elem);

    }
  }

  // Active voxels are empty: no new known voxels. Exit the function.
}

// Used to sort coordinates in next function. Sorts into column-major access
struct coordSorter{
  bool operator()(Coord const &a, Coord const &b){

    if(a.z != b.z)       { return (a.z < b.z); }
    else if (a.y != b.y) { return (a.y < b.y); }
    else                 { return (a.x < b.x); }
  }
};

RawivImage pickBoneVoxels(RawivImage const &img, CoordSet coordsToPick){
  RawivImage output = RawivImage(img);
  constexpr float fillValue = -1000; //The fill value for zeroed voxels

  // Convert coordset to a vector and then sort for better cache locality.
  vector<Coord> coordList = vector<Coord>();
  for(auto const &elem : coordsToPick){
    coordList.push_back(elem);
  }

  coordSorter myCoordSorter;
  std::sort(coordList.begin(), coordList.end(), myCoordSorter);

  // Iterate through list, zeroing nonmatching voxels
  auto iter = coordList.begin();
  for(size_t k = 0; k < img.dim.z; k++){
    for(size_t j = 0; j < img.dim.y; j++){
      for(size_t i = 0; i < img.dim.x; i++){
        if (i == iter->x && j == iter->y && k == iter->z){
          iter++; //Skip this voxel and leave it at its original value
        }
        else{ output(i,j,k) = fillValue; }
      }
    }
  }

  return output;
}

int main(int argc, char* argv[]){

  if (argc == 2 && strcmp(argv[1], "--help") == 0){
    cout << "Skull Growth Program: " << endl;
    cout << "\t Grows a rasterized representation of a skull by first selecting" << endl;
    cout << "\t a set of known bone voxels (set with the seed_threshold) and then " << endl;
    cout << "\t declaring that a voxel that is adjacent to a known bone voxel" << endl;
    cout << "\t and above the bone threshold to be a known bone voxel. This continues" << endl;
    cout << "\t until no new bone voxels can be found." << endl << endl;

    cout << "See comments at the top of growskull.cpp for more details." << endl << endl;

    cout << "Usage: " << argv[0] << " input_rawiv output_rawiv seed_threshold bone_threshold" << endl;

    return 1;
  }
  if (argc != 5){
    cout << "Usage: " << argv[0] << " input_rawiv output_rawiv seed_threshold bone_threshold" << endl;
    cout << endl << "Run " << argv[0] << " --help for more information" << endl;
    return 1;
  }

  string inFilename = string(argv[1]);
  string outFilename = string(argv[2]);
  int initThreshold = atof(argv[3]);
  int lowThreshold = atof(argv[4]);
  RawivImage img = RawivImage(inFilename);
  CoordSet knownVoxels = CoordSet();
  CoordQ activeVoxels; //Used for voxels that have been discovered but not BFSed

  // Initialize the knownVoxel set and activeVoxel set
  initKnownVoxels(knownVoxels, img, initThreshold);
  for(const auto elem : knownVoxels){
    activeVoxels.push(elem);
  }

  runBFS(knownVoxels, activeVoxels, img.dim, img, lowThreshold);

  RawivImage output = pickBoneVoxels(img, knownVoxels);

  output.writeToFile(outFilename);

  return 0;

}
