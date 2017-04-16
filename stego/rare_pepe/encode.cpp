#include <iostream>
#include <cstdint>
#include <png++/png.hpp>

int main(int argc, char *argv[]){

  if(argc < 3){
    std::cerr << "Usage: " << argv[0] << " <input image> <secret image> <output image>" << std::endl
	      << "All images must be .png files!" << std::endl;
    return 1;
  }
  
  png::image<png::rgba_pixel> original(argv[1]);
  png::image<png::rgba_pixel> secret(argv[2]);
  png::image<png::rgba_pixel> encoded(original.get_width(), original.get_height());

  if(!(original.get_height() == secret.get_height()) or !(original.get_width() == secret.get_width())){
    std::cerr << "Error: dimensions of input image and secret image must match! "
	      << original.get_width() << "x" << original.get_height() << " (input image) vs. "
	      << secret.get_width() << "x" << secret.get_height() << " (secret)" << std::endl;
    return 1;
  }
  std::cout << "Processing " << argv[1] << "(" <<original.get_width() << "x" << original.get_height() << ") and "
	    << argv[2] << "(" << secret.get_width() << "x" << secret.get_height() << ") into " << argv[3] << std::endl;
  
  for (unsigned int y = 0; y < original.get_height(); y++){
    for (unsigned int x = 0; x < original.get_width(); x++){
      png::rgba_pixel pix_original = original[y][x];
      uint8_t pix_secret = secret[y][x].red;
      uint8_t red = pix_original.red;
      if(pix_secret > 127){
        red = red | 0x01;
      }
      else {
        red = red & 0xFE;
      }
      encoded[y][x] = png::rgba_pixel(red, pix_original.green, pix_original.blue, pix_original.alpha);
    }
  }
  encoded.write(argv[3]);
  return 0;
}
