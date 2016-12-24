#include <iostream>
#include <tdrmain.h>
#include <rendererutil.h>

#define BUFF_LEN        (256)

std::vector<float> parse_array(const char* json_str, size_t len);

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
        std::vector<float> values;
        std::string temp;
        float val;

        switch (tokens[i].type) {
        case JSMN_OBJECT:
            std::cout << "an object.  Skipping." << std::endl;
            break;
        case JSMN_ARRAY:
            values = parse_array(json.c_str() + tokens[i].start,
                len);
            std::cout << "an array of length " << len << "." << std::endl;
            break;
        case JSMN_STRING:
            snprintf(buff, len, json.c_str() + tokens[i].start);
            std::cout << "a string: " << buff << std::endl;
            break;
        case JSMN_PRIMITIVE:
            std::cout << "a primitive.  Skipping." << std::endl;
            /*
            snprintf(buff, len, json.c_str() + tokens[i].start);
            temp = buff;
            val = std::atof(temp.c_str());
            std::cout << "a primitive: " << val << std::endl;
            */
            break;
        default:
            std::cout << "UNDEFINED.  ABORTING!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

std::vector<float> parse_array(const char* json_str, size_t len)
{
    int tok_count;
    char buff[16];
    char str[len + 1];
    std::vector<float> ret;
    std::vector<jsmntok_t> tokens;

    memset(buff, 0, 16);
    memset(str, 0, len + 1);
    snprintf(str, len, "%s", json_str);

    jsmn_parser parser;
    jsmn_init(&parser);
    tok_count = jsmn_parse(&parser, str, len, nullptr, 0);
    tokens.resize(tok_count);

    jsmn_init(&parser);
    if (tok_count != jsmn_parse(&parser, str, len,
        tokens.data(), tokens.size())) {
        std::cerr << "Failed to parse array.  Token size mismatch.";
        std::cerr << std::endl;
        return ret;
    }

    if (tokens[0].type != JSMN_ARRAY) {
        std::cerr << "Failed to parse array.  Array not found.";
        std::cerr << std::endl;
        return ret;
    }

    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].type != JSMN_PRIMITIVE) {
            std::cerr << "Failed to parse array.  Expecting primitive.";
            std::cerr << std::endl;
            return ret;
        }

        memset(buff, 0, 16);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);
        float temp = std::atof(buff);
        ret.push_back(temp);
    }

    //std::cout << "ARRAY:" << std::endl << buff << std::endl;

    return ret;
}
