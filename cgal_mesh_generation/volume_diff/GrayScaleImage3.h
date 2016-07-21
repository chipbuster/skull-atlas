#ifndef SKULL_ATLAS_CGAL_MESH_GENERATION_GRAYSCALEIMAGE3_H
#define SKULL_ATLAS_CGAL_MESH_GENERATION_GRAYSCALEIMAGE3_H

#include <deque>
#include <iostream>

#include <CGAL/ImageIO.h>

namespace cvc {

namespace skull_atlas {

class GrayScaleImage3 {

 public:
  GrayScaleImage3();

  GrayScaleImage3(size_t xdim, size_t ydim, size_t zdim);

  GrayScaleImage3(
      size_t xdim, size_t ydim, size_t zdim, float vx, float vy, float vz);

  // Returns the value of the voxel at (x,y,z).
  float& operator()(size_t x, size_t y, size_t z);

  // Returns the value of the voxel at (x,y,z).
  float operator()(size_t x, size_t y, size_t z) const;

  // Returns the number of voxels along the x direction.
  size_t xdim() const;

  // Returns the number of voxels along the y direction.
  size_t ydim() const;

  // Returns the number of voxels along the z direction.
  size_t zdim() const;

  // Returns the spacing in the x direction.
  float vx() const;

  // Returns the spacing in the y direction.
  float vy() const;

  // Returns the spacing in the z direction.
  float vz() const;

  const ::std::deque<float>& data() const;

 private:
  size_t xdim_, ydim_, zdim_;
  float vx_, vy_, vz_;
  ::std::deque<float> data_;
};

int write(const GrayScaleImage3& image, const char* filename);

GrayScaleImage3::GrayScaleImage3()
    : GrayScaleImage3(1, 1, 1, 1.0, 1.0, 1.0) {}

GrayScaleImage3::GrayScaleImage3(
    size_t xdim, size_t ydim, size_t zdim)
    : GrayScaleImage3(xdim, ydim, zdim, 1.0, 1.0, 1.0) {}

GrayScaleImage3::GrayScaleImage3(
    size_t xdim, size_t ydim, size_t zdim, float vx, float vy, float vz)
    : xdim_(xdim),
      ydim_(ydim),
      zdim_(zdim),
      vx_(vx),
      vy_(vy),
      vz_(vz),
      data_(xdim*ydim*zdim, 0.0) {}

float& GrayScaleImage3::operator()(size_t x, size_t y, size_t z) {
  return data_[ (z * ydim_ + y) * xdim_ + x];
}

float GrayScaleImage3::operator()(size_t x, size_t y, size_t z) const {
  return data_[ (z * ydim_ + y) * xdim_ + x];
}

size_t GrayScaleImage3::xdim() const {
  return xdim_;
}

size_t GrayScaleImage3::ydim() const {
  return ydim_;
}

size_t GrayScaleImage3::zdim() const {
  return zdim_;
}

float GrayScaleImage3::vx() const {
  return vx_;
}

float GrayScaleImage3::vy() const {
  return vy_;
}

float GrayScaleImage3::vz() const {
  return vz_;
}

const ::std::deque<float>& GrayScaleImage3::data() const {
  return data_;
}

int write(const GrayScaleImage3& image, const char* filename) {

  using namespace ::std;

  size_t xdim = image.xdim();
  size_t ydim = image.ydim();
  size_t zdim = image.zdim();
  float vx = image.vx();
  float vy = image.vy();
  float vz = image.vz();

  _image* im = _createImage(
      xdim, ydim, zdim, 1,
      vx, vy, vz,
      4, WK_FLOAT, SGN_SIGNED);
  if (im == nullptr) {
    cerr << "Failed to create image." << endl;
    return ImageIO_UNKNOWN_TYPE;
  }

  float* data = (float*) im->data;
  for (float x : image.data()) {
    *data = x;
    ++data;
  }

  return _writeImage(im, filename);

}

}  // namespace skull_atlas

}  // namespace cvc

#endif
