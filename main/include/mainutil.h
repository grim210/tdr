#ifndef RENDERER_MAINUTIL_H
#define RENDERER_MAINUTIL_H

#ifdef RENDERER_DEBUG
#include <iostream>
#endif

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace util {
std::string load_text_file(std::string path);
}

extern "C" {
const char* tdrmain_version(void);
}

#endif /* RENDERER_MAINUTIL_H */
