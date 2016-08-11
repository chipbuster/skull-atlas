#include "rawivutil.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstring>

//Need to do some serious shenangans to get byteswap order going...
#ifdef __APPLE__
#include <machine/endian.h>
#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#define __BIG_ENDIAN    BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __BYTE_ORDER    BYTE_ORDER
#define __HAVE_ENDIAN__
#endif

#ifdef __linux__
#include <endian.h>
#define __HAVE_ENDIAN__
#endif

#ifndef __HAVE_ENDIAN__
#error "Cannot figure out how to switch endianness on this platform"
#endif


using namespace std;
using namespace RawivUtil;

RawivImage::RawivImage(){} //Does nothing

RawivImage::RawivImage(std::string filename){
  this->readFromFile(filename);
}

void RawivImage::readFromFile(std::string filename){
  ifstream rawivFile;
  rawivFile.open(filename, std::ifstream::binary);

  //Character buffers are used to read the raw bytes from the ifstream
  char minmaxReadBuf[24], nVertDimReadBuf[20], origSpanReadBuf[24];

  //These buffers are used to store the converted output of the header
  float minmaxArray[6], origSpanArray[6];
  uint32_t nVertDimArray[5];

  // Used for reinterpret_cast of non-uint datatypes so that we can cast them
  uint32_t *castArray;

  // Read and interpret the min and max coordinates
  rawivFile.read(minmaxReadBuf, 24); //Read 6 floats
  castArray = reinterpret_cast<uint32_t*>(&minmaxReadBuf[0]);
  for (int j = 0; j < 6; j++){
    minmaxArray[j] = this->bytesToFloat(castArray[j]);
  }

  // Read and interpret the numVerts/numCells variables
  rawivFile.read(nVertDimReadBuf, 20); // Read five uint32_t
  castArray = reinterpret_cast<uint32_t*>(&nVertDimReadBuf[0]);
  for (int j = 0; j < 5; j++){
    nVertDimArray[j] = this->bytesToUint( castArray[j] );
  }

  // Read and interpret the span/origin/dim variables
  rawivFile.read(origSpanReadBuf, 24); //Read 6 floats
  castArray = reinterpret_cast<uint32_t*>(origSpanReadBuf);
  for (int j = 0; j < 6; j++){
    origSpanArray[j] = this->bytesToFloat(castArray[j]);
  }

  minCoords = Point3<float>(minmaxArray[0],minmaxArray[1],minmaxArray[2]);
  maxCoords = Point3<float>(minmaxArray[3],minmaxArray[4],minmaxArray[5]);
  numVerts  = nVertDimArray[0];
  numCells  = nVertDimArray[1];
  dim       = Point3<uint32_t>(nVertDimArray[2],nVertDimArray[3],nVertDimArray[4]);
  origin    = Point3<float>(origSpanArray[0],origSpanArray[1],origSpanArray[2]);
  span      = Point3<float>(origSpanArray[3],origSpanArray[4],origSpanArray[5]);
  this->inputFilename = filename;

  /* We have the header information. Now we need to load the data into this class.
     The numVerts variable tells us how many data elements we should have. We
     need to find out the length of the rest of the file to figure out whether
     we have char, short, or float data, and convert to float if needed. */

  // Gather some info about filesize and then read the raw data in
  long dataLength = getFileSize(filename) - RawivUtil::rawivHeaderSize;
  int dataElementSize = dataLength / numVerts;

  char* byteData = new char[dataLength];
  rawivFile.read(byteData, dataLength);

  this->data = vector<float>(numVerts);

  if (dataElementSize == 1){
    //Convert array elements from char to float
    for(uint32_t j = 0; j < numVerts; j++){
      data.at(j) = static_cast<float>(byteData[j]);
    }
  }
  else if (dataElementSize == 2){
    //Reinterpret as shorts, then fill data
    uint16_t* shortData = reinterpret_cast<uint16_t*>(byteData);
    for(uint32_t j = 0; j < numVerts; j++){
      shortData[j] = be16toh(shortData[j]);
      data.at(j) = static_cast<float>(shortData[j]);
    }
  }
  else{
    //We have float data. Reinterpret and assign to vector
    uint32_t* floatData = reinterpret_cast<uint32_t*>(byteData);
    for (uint32_t j = 0; j < numVerts; j++){
      data.at(j) = this->bytesToFloat(floatData[j]);
    }
  }

  delete[] byteData;
  rawivFile.close();
}

void RawivImage::writeToFile(std::string filename){
  //By default, will only write float-type arrays. It's 2016; 32 bits per
  //value is pretty cheap. Can be extended if needed.
  char header[RawivUtil::rawivHeaderSize];

  // Used for endian-swaps and storing ints
  uint32_t* header32 = reinterpret_cast<uint32_t*>(header);
  float* headerFloat = reinterpret_cast<float*>(header); //Used for storing floats

  headerFloat[0]  = minCoords.x;
  headerFloat[1]  = minCoords.y;
  headerFloat[2]  = minCoords.z;
  headerFloat[3]  = maxCoords.x;
  headerFloat[4]  = maxCoords.y;
  headerFloat[5]  = maxCoords.z;
  header32[6]     = numVerts;
  header32[7]     = numCells;
  header32[8]     = dim.x;
  header32[9]     = dim.y;
  header32[10]    = dim.z;
  headerFloat[11] = origin.x;
  headerFloat[12] = origin.y;
  headerFloat[13] = origin.z;
  headerFloat[14] = span.x;
  headerFloat[15] = span.y;
  headerFloat[16] = span.z;

  // Convert the buffer to big-endian 32-bit
  for (int i = 0; i < 17; i++){
    header32[i] = htobe32(header32[i]);
  }

  /* The header array now has the correct raw-byte representation to write to
     a rawiv file. Open an ofstream with the chosen filename and write the
     header to file, then write the data buffer to file. */

  ofstream rawivOut;
  rawivOut.open(filename, std::ofstream::binary);

  rawivOut.write(header, RawivUtil::rawivHeaderSize);

  // Now write the data to the file
  vector<float> newVectorData = vector<float>(this->data);

  uint32_t* uintData = reinterpret_cast<uint32_t*>(newVectorData.data());

  for (size_t j = 0; j < data.size(); j++){
    uintData[j] = htobe32(uintData[j]);
  }

  char* byteData = reinterpret_cast<char*>(uintData);
  rawivOut.write(byteData, 4 * data.size());

  rawivOut.close();
}

// The accessor-mutator block
float& RawivImage::operator() (uint32_t i, uint32_t j, uint32_t k){
  // Access data in column-major order (because rawiv)
  uint32_t index = i + dim.x * ( j + dim.y * k );
  return this->data.at(index);
}
const float& RawivImage::operator() (uint32_t i, uint32_t j, uint32_t k) const{
  // Access data in column-major order (because rawiv)
  uint32_t index = i + dim.x * ( j + dim.y * k );
  return this->data.at(index);
}
// For Coords
float& RawivImage::operator() (Point3<uint32_t> inp){
  return (*this)(inp.x,inp.y,inp.z);
}
const float& RawivImage::operator() (Point3<uint32_t> inp) const{
  return (*this)(inp.x,inp.y,inp.z);
}

uint32_t RawivImage::bytesToUint(uint32_t byte){
  uint32_t tmp = be32toh(byte);
  return tmp;
}

float RawivImage::bytesToFloat(uint32_t byte){
  /* Reinterpret-casting this breaks strict type punning. Instead, use
     the mempcy trick to convert types. */

  static_assert(sizeof(float)==sizeof(uint32_t), "Float is not 32 bytes! Are you sure this system is IEEE 754-compliant?");
  uint32_t tmp = be32toh(byte);
  float f;
  memcpy(&f, &tmp, sizeof(float));

  return f;
}
long RawivImage::getFileSize(std::string filename){
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}
