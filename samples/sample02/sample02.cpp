#include <iostream>
#include <tdrmain.h>
#include <rendererutil.h>

#define BUFF_LEN        (256)

int main(int argc, char* argv[])
{
    std::cout << "Main: " << tdrmain_version() << std::endl;
    std::cout << "Renderer: " << tdrrenderer_version() << std::endl;

    std::string json = load_text_file("./cube.json");
    if (json.length() < 1) {
        return EXIT_FAILURE;
    }

    std::cout << "JSON:" << std::endl << json << std::endl;

    jsmn_parser parser;
    jsmn_init(&parser);
    int count = jsmn_parse(&parser, json.c_str(), json.length(),
        nullptr, 0);

    std::vector<jsmntok_t> tokens;
    tokens.resize(count);
    std::cout << "Resizing to " << count << " JSON tokens." << std::endl;

    jsmn_init(&parser);
    int result = jsmn_parse(&parser, json.c_str(), json.length(), tokens.data(),
        tokens.size());
    std::cout << result << " JSON tokens successfully parsed." << std::endl;

    char buff[BUFF_LEN];
    for (size_t i = 0; i < tokens.size(); i++) {
        int len;
        memset(buff, 0, BUFF_LEN);
        std::cout << "Object [" << i << "] is ";
        len = tokens[i].end - tokens[i].start + 1;
        std::string temp;
        float val;

        switch (tokens[i].type) {
        case JSMN_OBJECT:
            std::cout << "an object.  Skipping." << std::endl;
            break;
        case JSMN_ARRAY:
            std::cout << "an array." << std::endl;
            break;
        case JSMN_STRING:
            snprintf(buff, len, json.c_str() + tokens[i].start);
            std::cout << "a string: " << buff << std::endl;
            break;
        case JSMN_PRIMITIVE:
            snprintf(buff, len, json.c_str() + tokens[i].start);
            temp = buff;
            val = std::atof(temp.c_str());
            std::cout << "a primitive: " << val << std::endl;
            break;
        default:
            std::cout << "UNDEFINED.  ABORTING!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
