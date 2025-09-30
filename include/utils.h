#ifndef MEMORYLEAKDETECTOR_UTILS_H
#define MEMORYLEAKDETECTOR_UTILS_H

#include <stdbool.h>

/**
 *
 * @param path
 * @return
 */
bool is_valid_filepath(const char *path);

bool is_valid_directory(const char *path);

#endif //MEMORYLEAKDETECTOR_UTILS_H