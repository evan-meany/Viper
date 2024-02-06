// This is here to ensure that the STB_IMAGE_IMPLEMENTATION
// macro is defined. In premake5.lua the files sections ensures
// that this cpp file is compiled at build time. Note that somethings
// similar was done with glm.

#include "vppch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
