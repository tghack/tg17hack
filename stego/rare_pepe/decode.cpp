#include <iostream>
#include <png++/png.hpp>

int main(int argc, char *argv[]){

  if(argc < 2){
    std::cerr << "Usage: " << argv[0] << " <encoded input image> <output image>" << std::endl;
    return 1;
  }
  
  png::image<png::rgba_pixel> encoded(argv[1]);
  png::image<png::rgba_pixel> decoded(encoded.get_width(), encoded.get_height());

  for (unsigned int y = 0; y < encoded.get_height(); y++){
    for (unsigned int x = 0; x < encoded.get_width(); x++){
      png::rgba_pixel pixel = encoded[y][x];
      uint8_t red = pixel.red;
      if((red & 0x01) == 1){
        decoded[y][x] = png::rgba_pixel(255, 255, 255, 255);
      }
      else {
        decoded[y][x] = png::rgba_pixel(0, 0, 0, 255);
      }
    }
  }
  decoded.write(argv[2]);
  return 0;
}
