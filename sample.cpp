#include <iostream>

#include "ximage.h"

int main(int argc, char *argv[]) {
  std::cout << "Testing XImage ..." << std::endl;

  XImage<char> a;
  for (int y = 0; y < a.getHeight(); ++y) {
    for (int x = 0; x < a.getWidth(); ++x) {
      a(x, y) = x + y;
    }
  }
  std::cout << a << std::endl;

  XImage<char> b(4, 4);
  for (int y = 0; y < b.getHeight(); ++y) {
    for (int x = 0; x < b.getWidth(); ++x) {
      b(x, y) = x * y;
    }
  }
  std::cout << b << std::endl;

  XImage<char> c = b;
  std::cout << c << std::endl;

  return 0;
}