#include "utils/utils.hpp"

void  checkFile(const char* fileName, const char* extension)
{
  struct stat status;

  if (stat(fileName, &status) != 0) {
    std::cerr << "Failed to stat file: " << fileName << std::endl;
    std::exit(EXIT_FAILURE);   
  }
  if (status.st_mode & S_IFDIR) {
    std::cerr << fileName << ": is a directory" << std::endl;
    std::exit(EXIT_FAILURE);   
  }
  std::string name(fileName);
  std::string ext(extension);
  if (name.size() < ext.size() ||
    name.compare(name.size() - ext.size(), ext.size(), ext) != 0) {
    std::cerr << "filename must have " << ext << " extention" << std::endl;
    std::exit(EXIT_FAILURE);   
  }
}

void  Utils::checkArgv(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Argument number error" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  checkFile(argv[1], ".bmp");
}
