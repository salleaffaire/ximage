#ifndef _XIMAGE_UTILS_HPP_____
#define _XIMAGE_UTILS_HPP_____

#include <string>

#include "ximage_formats.h"

namespace ximage {

int XImageBytesPerPixel(int format) {
  switch (format) {
    case XIMAGE_FORMAT_GRAY:
    case XIMAGE_FORMAT_Y:
    case XIMAGE_FORMAT_U:
    case XIMAGE_FORMAT_V:
      return 1;
    case XIMAGE_FORMAT_YUYV:
    case XIMAGE_FORMAT_UV:
      return 2;
    case XIMAGE_FORMAT_YUV:
      return 3;
    case XIMAGE_FORMAT_RGBX:
    case XIMAGE_FORMAT_BGRX:
    case XIMAGE_FORMAT_XRGB:
    case XIMAGE_FORMAT_XBGR:
    case XIMAGE_FORMAT_RGBA:
    case XIMAGE_FORMAT_BGRA:
    case XIMAGE_FORMAT_ARGB:
    case XIMAGE_FORMAT_ABGR:
    case XIMAGE_FORMAT_YUVA:
      return 4;
    default:
      return 0;
  }
}

std::string XImageFormatToString(int format) {
  switch (format) {
    case XIMAGE_FORMAT_GRAY:
      return "GRAY";
    case XIMAGE_FORMAT_Y:
      return "Y";
    case XIMAGE_FORMAT_U:
      return "U";
    case XIMAGE_FORMAT_V:
      return "V";
    case XIMAGE_FORMAT_YUV:
      return "YUV";
    case XIMAGE_FORMAT_YUYV:
      return "YUYV";
    case XIMAGE_FORMAT_UV:
      return "UV";
    case XIMAGE_FORMAT_YUVA:
      return "YUVA";
    case XIMAGE_FORMAT_RGBX:
      return "RGBX";
    case XIMAGE_FORMAT_BGRX:
      return "BGRX";
    case XIMAGE_FORMAT_XRGB:
      return "XRGB";
    case XIMAGE_FORMAT_XBGR:
      return "XBGR";
    case XIMAGE_FORMAT_RGBA:
      return "RGBA";
    case XIMAGE_FORMAT_BGRA:
      return "BGRA";
    case XIMAGE_FORMAT_ARGB:
      return "ARGB";
    case XIMAGE_FORMAT_ABGR:
      return "ABGR";

    default:
      return "INVALID";
  }  // switch
}

}  // namespace ximage

#endif