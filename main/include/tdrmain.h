#ifndef TDRMAIN_H
#define TDRMAIN_H

#include <string>

#include "config.h"

#include "camera.h"
#include "directdrawtexture.h"
#include "jsmn.h"
#include "renderobject.h"
#include "tdrmesh.h"
#include "texture.h"
#include "timer.h"
#include "tinyxml2.h"

std::string load_text_file(std::string path);
void tdrmain_version(char* buff, size_t len);

#endif
