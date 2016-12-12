#ifndef RENDERER_UTIL_H
#define RENDERER_UTIL_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

namespace util {
std::string load_text_file(std::string path);
}

#endif /* RENDERER_UTIL_H */
