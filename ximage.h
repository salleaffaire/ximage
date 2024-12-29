#ifndef _XIMAGE_H_____
#define _XIMAGE_H_____

#include <cstring>
#include <iomanip>
#include <iostream>

#include "ximage_formats.h"

#define MAX_NUMBER_OF_PLANES 8

// Forward declaration of the template
template <typename T>
class XImage;

// Define the operator<< template
template <typename T>
std::ostream &operator<<(std::ostream &os, const XImage<T> &x);

template <typename T>
class XImage {
 public:
  // Rule of 5 -
  // https://stackoverflow.com/questions/3106110/what-is-move-semantics

  XImage() : mWidth(0), mHeight(0), mStride(0), mNumberOfPlanes(1) {
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = nullptr;
    }
    std::cout << "XImage() : " << (void *)mpData << std::endl;
  }

  XImage(int width, int height)
      : mWidth(width), mHeight(height), mStride(width), mNumberOfPlanes(1) {
    std::cout << "XImage(int, int) : " << (void *)mpData << std::endl;

    mpData[0] = new T[width * height * sizeof(T)];
    for (int plane = 1; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = nullptr;
    }
  }

  XImage(const XImage<T> &that) {
    std::cout << "XImage(const XImage<T> &) : " << (void *)mpData << std::endl;
    mWidth = that.mWidth;
    mHeight = that.mHeight;
    mStride = that.mStride;

    mNumberOfPlanes = that.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = new T[mStride * mHeight];
      std::memcpy(mpData[plane], that.mpData[plane],
                  mStride * mHeight * sizeof(T));
    }
  }

  XImage(const XImage<T> &&x) {
    std::cout << "XImage(const XImage<T> &&) : " << (void *)mpData << std::endl;
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;

    mNumberOfPlanes = x.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = x.mpData[plane];
      x.mpData[plane] = nullptr;
    }
  }

  XImage operator=(const XImage<T> x) {
    std::cout << "XImage operator=(const XImage<T>) : " << (void *)mpData
              << std::endl;
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;

    mNumberOfPlanes = x.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      std::swap(mpData[plane], x.mpData[plane]);
    }

    return *this;
  }

  // Destructor
  virtual ~XImage() {
    std::cout << "~XImage() : " << (void *)mpData << std::endl;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      if (mpData[plane]) {
        delete[] mpData[plane];
      }
    }
  }

  // Getters
  int getWidth() const { return mWidth; }
  int getHeight() const { return mHeight; }

  // Return a reference to pixel (x,y) in plane
  T &operator()(int x, int y, int plane = 0) {
    return mpData[plane][y * mStride + x];
  }

  // Setters
  void setWidth(int width) { mWidth = width; }
  void setHeight(int height) { mHeight = height; }

 private:
  int mWidth;
  int mHeight;
  int mStride;

  int mNumberOfPlanes;
  T *mpData[MAX_NUMBER_OF_PLANES];

  friend std::ostream &operator<< <>(std::ostream &os, const XImage<T> &x);
};

// overloading the << operator
template <typename T>
std::ostream &operator<<(std::ostream &os, const XImage<T> &x) {
  os << "XImage<" << typeid(T).name() << ">(" << x.mWidth << ", " << x.mHeight
     << ")" << std::endl;

  for (int plane = 0; plane < x.mNumberOfPlanes; ++plane) {
    os << "Plane " << plane << std::endl;
    // If the plane is less than 6 by 6 then print the data
    if (x.mWidth <= 6 && x.mHeight <= 6) {
      for (int i = 0; i < x.mHeight; ++i) {
        for (int j = 0; j < x.mWidth; ++j) {
          os << std::setw(4) << (int)x.mpData[plane][i * x.mStride + j] << " ";
        }
        os << std::endl;
      }
    }
  }

  return os;
}

#endif