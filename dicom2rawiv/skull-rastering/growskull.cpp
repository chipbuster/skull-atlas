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

/* When trying to extract voxelized skulls, we run into the issue that we cannot
   do naive thresholding. This is because some bone structures are so thin that
   their CT signature is closer to that of muscle than bone. We can set the
   threshold lower, but this picks up on undesirable junk in the brain and throat.

   The solution is to use a lower threshold, but to only select voxels that are
   adjacent to known bone voxels. This way, tissue that is not attached to
   bone (e.g. brain/eye lens) is not included in the voxel representation, but
   tissue that has similar density and is attached to bone (e.g. orbital lamina)
   are slightly more attached.

   To do this, we assume a graph-representation: every voxel is assume to be
   a node in a graph and adjacent to its L-inf neighbors. We then start with the
   nodes that are known to be above the seed threshold, then BFS for all connected
   voxels that are above the second threshold. Continue until no new bone voxels
   can be added.

   Note 8/11/2016: there may be a subtle bug in this algorithm: some voxels of the
   orbital lamina of the ethmoid are not being added properly even though
   DICOM viewers indicate that they should be added.

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
  constexpr int radius = 2; // How many voxels to each side do we define as a "neighbor"

  for(int i = -radius; i <= radius; i++){
    for(int j = -radius; j <= radius; j++){
      for(int k = -radius; k <= radius; k++){
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
    ctr = 50000;
    cout << "AVC: " << active.size() << endl;
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
          output(i,j,k) = 2000;
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

    cout << "Usage: " << argv[0] << " input_rawiv output_rawiv seed_threshold bone_threshold" << endl << endl;
    cout << "(Output will be input filename with '_filtered' appended)" << endl;

    return 1;
  }

  if (argc != 5 && argc != 4){
    cout << "Usage: " << argv[0] << " input_rawiv output_rawiv seed_threshold bone_threshold" << endl;
    cout << endl << "Run " << argv[0] << " --help for more information" << endl;
    return 1;
  }

  string inFilename, outFilename;
  int initThreshold, lowThreshold;

  if (argc == 5){
    inFilename = string(argv[1]);
    outFilename = string(argv[2]);
    initThreshold = atoi(argv[3]);
    lowThreshold = atoi(argv[4]);
  }else{
    return -1; // Keep compiler from warning about uninit'd vals
  }

  if (initThreshold < lowThreshold){
    std::cerr << "[WARN]: Your seed threshold is lower than your boundary "
              << "threshold. Usually you're supposed to do it the other way"
              << "around." << endl;
  }

  RawivImage img = RawivImage(inFilename);
  CoordSet knownVoxels = CoordSet();
  CoordQ activeVoxels; //Used for voxels that have been discovered but not BFSed

  // Initialize the knownVoxel set and activeVoxel set
  initKnownVoxels(knownVoxels, img, initThreshold);
  for(const auto elem : knownVoxels){
    activeVoxels.push(elem);
  }

  cout << "Program will terminate when AVC (active voxel count) reaches zero." << endl;

  runBFS(knownVoxels, activeVoxels, img.dim, img, lowThreshold);

  RawivImage output = pickBoneVoxels(img, knownVoxels);

  output.writeToFile(outFilename);

  return 0;

}
