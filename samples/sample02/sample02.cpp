#include <iostream>
#include <tdrmain.h>
#include <tdrrenderer.h>

#define BUFF_LEN        (256)

std::vector<float> parse_array(const char* json_str, size_t len);

int main(int argc, char* argv[])
{
    std::string json = load_text_file("./cube.json");
    if (json.length() <= 0) {
        std::cerr << "Failed to load the JSON file." << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Dump the string, just as a sanity check */
    std::cout << "====================" << std::endl;
    std::cout << "JSON Output:" << std::endl << json << std::endl;
    std::cout << "====================" << std::endl;

    /* determine how many tokens are required to parse the entire file. */
    jsmn_parser parser;
    jsmn_init(&parser);
    int count = jsmn_parse(&parser, json.c_str(), json.length(),
        nullptr, 0);

    /* after the token count is found, creat a vector of tokens and resize
     * it to the necessary size to parse the entire file */
    std::vector<jsmntok_t> tokens;
    tokens.resize(count);

    /* initialize the same parser and parse it for realz now. */
    jsmn_init(&parser);
    int result = jsmn_parse(&parser, json.c_str(), json.length(), tokens.data(),
        tokens.size());

    /* Holds one element at a time.. So strings and primitives.  Since it's
     * statically allocated just once I'm not too concerned about memory
     * usage for the buffer. */
    char buff[BUFF_LEN];

    /* Since this test program is only expecting vertices and UV coordinates,
     * this 'type' character is simply set to whatever we're expecting, which
     * would be 'u' or 'v'.  And the two vectors are holding the floats. */
    char type = '\0';
    std::vector<float> vertices;
    std::vector<float> uvs;

    for (size_t i = 0; i < tokens.size(); i++) {
        int len = tokens[i].end - tokens[i].start + 1;

        /* simply used to do pointer arithmetic, and we're not doing
         * math in the function calls. */
        const char* tjson = nullptr;

        /* So the JSMN parser doesn't really hold state, so it's probably a
         * bit more work to parse more complicated data structures, but that's
         * not the case with our model data.  Since all data is eventually
         * going to end up in a glBuffer, the data is relatively simple. */
        switch (tokens[i].type) {
        case JSMN_ARRAY:
            tjson = json.c_str() + tokens[i].start;
            switch (type) {
            case 'u':
                uvs = parse_array(tjson, len);
                break;
            case 'v':
                vertices = parse_array(tjson, len);
                break;
            default:
                std::cerr << "Found invalid type in switch statement.";
                std::cerr << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case JSMN_STRING:
            memset(buff, 0, BUFF_LEN);
            tjson = json.c_str() + static_cast<size_t>(tokens[i].start);
            snprintf(buff, len, "%s", tjson);

            /* This could easily be expanded to expect more types in order
             * to tell the parser what sort of data it's reading.  We have
             * to build a state machine of sorts to be able to get valuable
             * context from the raw JSON data. */
            if (strncmp(buff, "vertices", strlen("vertices")) == 0) {
                type = 'v';
            } else if (strncmp(buff, "uvs", strlen("uvs")) == 0) {
                type = 'u';
            } else {
                type = '\0';
            }
            break;
        case JSMN_PRIMITIVE:
        case JSMN_OBJECT:
            /* Primitives are currently grabbed by parse_array() and the
             * object type would be useful for determining if the JSON
             * file adhered to strict requirements for model data.  So
             * both data types are definitely useful. */
            break;
        default:
            std::cout << "UNDEFINED.  ABORTING!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "Found " << result << " total JSON tokens." << std::endl;
    std::cout << "Found " << vertices.size() << " vertices." << std::endl;
    std::cout << "Found " << uvs.size() << " uvs." << std::endl;

    return 0;
}

std::vector<float> parse_array(const char* json_str, size_t len)
{
    int tok_count;
    char buff[BUFF_LEN];
    char str[len + 1];
    std::vector<float> ret;
    std::vector<jsmntok_t> tokens;

    memset(buff, 0, BUFF_LEN);
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

        memset(buff, 0, BUFF_LEN);
        snprintf(buff, tokens[i].end - tokens[i].start + 1, "%s",
            str + tokens[i].start);
        float temp = std::atof(buff);
        ret.push_back(temp);
    }

    return ret;
}

