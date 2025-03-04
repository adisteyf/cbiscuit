#include "bsqt_utils.h"

char *
bsqtReadFile (const char* filename, size_t* outSize)
{
    FILE *file;
    char *buffer = NULL;
    size_t size = 0;
    size_t bytesRead;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("\033[31mBiscuit001\033[0m: Can't open the file: %s.");
        return NULL;
    }

    while (1) {
        buffer = realloc(buffer, size + 1024);
        if (buffer == NULL) {
            perror("\033[31mBiscuit002\033[0m: Can't allocate the memory.");
            fclose(file);
            return NULL;
        }

        bytesRead = fread(buffer + size, 1, 1024, file);
        if (bytesRead == 0) {
            break;
        }
        size += bytesRead;
    }

    fclose(file);

    *outSize = size;
    return buffer;
}
