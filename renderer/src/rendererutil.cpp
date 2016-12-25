#include <tdrrenderer.h>

void tdrrenderer_version(char* buff, size_t len)
{
    std::memset(buff, 0, len);
    snprintf(buff, len, "%d.%d.%d", TDRRENDERER_VERSION_MAJOR,
        TDRRENDERER_VERSION_MINOR, TDRRENDERER_VERSION_PATCH);
}
