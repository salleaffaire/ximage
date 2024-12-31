#ifndef _XIMAGE_H_____
#define _XIMAGE_H_____

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>

#include "ximage_formats.h"
#include "ximage_utils.h"

#define MAX_NUMBER_OF_PLANES 8

template <typename T>
class XPlane {
 public:
  XPlane() {
    mWidth = 0;
    mHeight = 0;
    mStride = 0;
    mpData = nullptr;
    mFormat = XIMAGE_FORMAT_INVALID;
  }

  XPlane(int width, int height, int format)
      : mWidth(width), mHeight(height), mStride(width) {
    mFormat = format;
    mpData = std::make_unique<T>(width * height *
                                 ximage::XImageBytesPerPixel(format));
  }

  XPlane(const XPlane<T> &that)
      : mWidth(that.mWidth), mHeight(that.mHeight), mStride(that.mStride) {
    mFormat = that.mFormat;
    mpData = std::make_unique<T>(mStride * mHeight *
                                 ximage::XImageBytesPerPixel(mFormat));
    std::memcpy(
        mpData.get(), that.mpData.get(),
        mStride * mHeight * sizeof(T) * ximage::XImageBytesPerPixel(mFormat));
  }

  XPlane(const XPlane<T> &&x) noexcept
      : mWidth(x.mWidth), mHeight(x.mHeight), mStride(x.mStride) {
    mFormat = x.mFormat;
    mpData = std::move(x.mpData);
  }

  XPlane operator=(const XPlane<T> x) {
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;
    mFormat = x.mFormat;
    std::swap(mpData, x.mpData);
    return *this;
  }

 private:
  int mWidth;   // In bytes
  int mHeight;  // In bytes
  int mStride;  // In bytes
  std::unique_ptr<T> mpData;
  int mFormat;
};

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

  // Null image
  // has 0 planes and all the pointers are nullptr
  // and the format is invalid
  XImage() : mWidth(0), mHeight(0), mStride(0), mNumberOfPlanes(0) {
    std::cout << "XImage() : " << (void *)this << std::endl;
    for (int plane = 0; plane < MAX_NUMBER_OF_PLANES; ++plane) {
      mFormat[plane] = XIMAGE_FORMAT_INVALID;
      mpData[plane] = nullptr;
    }
  }

  XImage(int width, int height)
      : mWidth(width), mHeight(height), mStride(width), mNumberOfPlanes(1) {
    std::cout << "XImage(int, int) : " << (void *)this << std::endl;
    mFormat[0] = XIMAGE_FORMAT_GRAY;
    mpData[0] = std::make_unique<T>(width * height *
                                    ximage::XImageBytesPerPixel(mFormat[0]));

    for (int plane = 1; plane < MAX_NUMBER_OF_PLANES; ++plane) {
      mpData[plane] = nullptr;
      mFormat[plane] = XIMAGE_FORMAT_INVALID;
    }
  }

  XImage(int width, int height, int numberOfPlanes, int format[])
      : mWidth(width),
        mHeight(height),
        mStride(width),
        mNumberOfPlanes(numberOfPlanes) {
    std::cout << "XImage(int, int, int, int[]) : " << (void *)this << std::endl;

    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mFormat[plane] = format[plane];
      mpData[plane] = std::make_unique<T>(
          width * height * ximage::XImageBytesPerPixel(mFormat[plane]));
    }

    for (int plane = numberOfPlanes; plane < MAX_NUMBER_OF_PLANES; ++plane) {
      mpData[plane] = nullptr;
      mFormat[plane] = XIMAGE_FORMAT_INVALID;
    }
  }

  XImage(const XImage<T> &that)
      : mWidth(that.mWidth),
        mHeight(that.mHeight),
        mStride(that.mStride),
        mNumberOfPlanes(that.mNumberOfPlanes) {
    std::cout << "XImage(const XImage<T> &) : " << (void *)this << std::endl;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mFormat[plane] = that.mFormat[plane];
      mpData[plane] = std::make_unique<T>(
          mStride * mHeight * ximage::XImageBytesPerPixel(mFormat[plane]));
      std::memcpy(mpData[plane].get(), that.mpData[plane].get(),
                  mStride * mHeight * sizeof(T) *
                      ximage::XImageBytesPerPixel(mFormat[plane]));
    }
  }

  XImage(const XImage<T> &&x) noexcept
      : mWidth(x.mWidth),
        mHeight(x.mHeight),
        mStride(x.mStride),
        mNumberOfPlanes(x.mNumberOfPlanes) {
    std::cout << "XImage(const XImage<T> &&) : " << (void *)this << std::endl;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mFormat[plane] = x.mFormat[plane];
      mpData[plane] = std::move(x.mpData[plane]);
    }
  }

  XImage operator=(const XImage<T> x) {
    std::cout << "XImage operator=(const XImage<T>) : " << (void *)this
              << std::endl;
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;
    mNumberOfPlanes = x.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mFormat[plane] = x.mFormat[plane];
      std::swap(mpData[plane], x.mpData[plane]);
    }
    return *this;
  }

  // Destructor
  virtual ~XImage() {
    std::cout << "~XImage() : " << (void *)this << std::endl;
  }

  // Getters
  int getWidth() const { return mWidth; }
  int getHeight() const { return mHeight; }

  // Return a reference to pixel (x,y) in plane
  T &operator()(int x, int y, int plane = 0) {
    return mpData[plane].get()[y * mStride + x];
  }

  // Setters
  void setWidth(int width) { mWidth = width; }
  void setHeight(int height) { mHeight = height; }

 private:
  int mWidth;   // In pixels
  int mHeight;  // In pixels
  int mStride;  // In pixels

  int mNumberOfPlanes;
  std::unique_ptr<T> mpData[MAX_NUMBER_OF_PLANES];
  int mFormat[MAX_NUMBER_OF_PLANES];

  friend std::ostream &operator<< <>(std::ostream &os, const XImage<T> &x);
};

// overloading the << operator
template <typename T>
std::ostream &operator<<(std::ostream &os, const XImage<T> &x) {
  os << "XImage<" << typeid(T).name() << ">(" << x.mWidth << ", " << x.mHeight
     << ")" << std::endl;
  os << "Number of planes: " << x.mNumberOfPlanes << std::endl;
  for (int plane = 0; plane < x.mNumberOfPlanes; ++plane) {
    os << " | -- Plane " << plane << std::endl;
    os << " |    | -- Format: "
       << ximage::XImageFormatToString(x.mFormat[plane]) << std::endl;
    os << " |    | -- Bytes per plane: "
       << x.mStride * x.mHeight * ximage::XImageBytesPerPixel(x.mFormat[plane])
       << std::endl;
  }

  // for (int plane = 0; plane < x.mNumberOfPlanes; ++plane) {
  //   os << "Plane " << plane << std::endl;
  //   // If the plane is less than 6 by 6 then print the data
  //   if (x.mWidth <= 6 && x.mHeight <= 6) {
  //     for (int i = 0; i < x.mHeight; ++i) {
  //       for (int j = 0; j < x.mWidth; ++j) {
  //         os << std::setw(4) << (int)x.mpData[plane].get()[i * x.mStride + j]
  //            << " ";
  //       }
  //       os << std::endl;
  //     }
  //   }
  // }

  return os;
}

#endif