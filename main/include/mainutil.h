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

std::string load_text_file(std::string path);
const char* tdrmain_version(void);

#endif /* RENDERER_MAINUTIL_H */
