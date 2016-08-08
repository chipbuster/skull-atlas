#ifndef RAWIVUTIL_H
#define RAWIVUTIL_H

#include<tuple>
#include<vector>
#include<string>
#include<cstdint>

namespace RawivUtil{

  constexpr size_t rawivHeaderSize = 68;

  template <typename T> class Point3{
  public:
    T x;
    T y;
    T z;

    Point3(){} //Do nothing

    Point3(T x_in,T y_in,T z_in){
      x = x_in;
      y = y_in;
      z = z_in;
    }
  };

  class RawivImage{
  private:
    float bytesToFloat(uint32_t byte);
    uint32_t bytesToUint(uint32_t byte);
    long getFileSize(std::string filename);

  public:
    /* See the VolumeRover manual at https://svn.ices.utexas.edu/repos/cvc/trunk/VolumeRoverDoc/
       for details on what these variables mean. The code ordering is not very pretty, but
       it is the actual order they will appear in the file. */
    Point3<float> minCoords;
    Point3<float> maxCoords;
    uint32_t numVerts;
    uint32_t numCells;
    Point3<uint32_t> dim;
    Point3<float> origin;
    Point3<float> span;
    std::vector<float> data;
    std::string inputFilename;

    /* The data element of the class should always be in column-major order.
       Instead of converting it to row-major order so that we can use C-style
       indexing on it, we will expose it through the () operator, which will
       allow us to provide normal access to the data without having to worry
       about column/row-major translations */

    RawivImage();
    explicit RawivImage(std::string filename);

    void readFromFile(std::string filename);
    void writeToFile(std::string filename);

    //Coordinate access
    float& operator() (uint32_t i, uint32_t j, uint32_t k);
    const float& operator() (uint32_t i, uint32_t j, uint32_t k) const;
  };

}

#endif
