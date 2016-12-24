#ifndef TDRMAIN_H
#define TDRMAIN_H

#include <string>

#include "camera.h"
#include "directdrawtexture.h"
#include "jsmn.h"
#include "renderobject.h"
#include "tdrmesh.h"
#include "texture.h"

std::string load_text_file(std::string path);
const char* tdrmain_version(void);

#endif
