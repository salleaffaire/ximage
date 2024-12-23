#ifndef _XIMAGE_H_____
#define _XIMAGE_H_____

#define MAX_NUMBER_OF_PLANES 8

template <typedef T>
class XImage {
 public:
  XIMage() : mWidth(0), mHeight(0), mStride(0), mNumberOfPlanes(1) {
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = null;
    }
  }

  XIMage(int width, int height)
      : mWidth(width), mHeight(height), mStride(width), mNumberOfPlanes(1) {
    mpData[0] = new T[width * height * sizeof(T)];
    for (int plane = 1; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = nullptr;
    }
  }

  XImage(const XImage<T> &that) {
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;

    mNumberOfPlanes = x.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = new T[mStride * mHeight];
      std::memcpy(mpData[plane], that.mpData[plane],
                  mStride * mHeight * sizeof(T));
    }
  }

  XImage(const XImage<T> &&x) {
    mWidth = x.mWidth;
    mHeight = x.mHeight;
    mStride = x.mStride;

    mNumberOfPlanes = x.mNumberOfPlanes;
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      mpData[plane] = x.mpData[plane];
      x.mpData[plane] = nullptr;
    }
  }

  XImage operator=(const XImage<T> x) {}

  virtual ~XImage() {
    for (int plane = 0; plane < mNumberOfPlanes; ++plane) {
      if (mpData[plane]) {
        delete[] mpData[plane];
      }
    }
  }

 private:
  int mWidth;
  int mHeight;
  int mStride;

  int mNumberOfPlanes;
  T *mpData[MAX_NUMBER_OF_PLANES];
};

#endif