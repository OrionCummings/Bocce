#include "file_system.h"

ErrorCode get_project_dir(char* full_path) {

    // Get the CWD
    char cwd[FILENAME_MAX];
    char* buffer = getcwd(cwd, sizeof(cwd));

    // Verify that the CWD is usable
    struct cwk_segment segment;
    if (!cwk_path_get_first_segment(buffer, &segment)) {
        B_ERROR("Config path doesn't have any segments");
        return EC_CONFIG_BAD_PATH;
    }

    int found_folder = 0;
    const char* target_segment_name = "Bocce";
    char current_segment_name[segment.size + 1];

    // While there are still more segments to process
    do {
        // Clear the contents of the current_segment_name
        memset_s(current_segment_name, segment.size + 1, 0);

        // Copy the current segment name (with null terminator!)
        strncpy_s(current_segment_name, FILENAME_MAX, segment.begin, segment.size);

        // Check if it's the folder we want
        strcmp_s(current_segment_name, strlen(target_segment_name), target_segment_name, &found_folder);

        // If it is, then break
        if (!found_folder) {
            break;
        }

    } while (cwk_path_get_next_segment(&segment));

    // If we are here without finding the folder, then something went wrong!
    if (found_folder) {
        B_ERROR("Failed to self-orient");
        return EC_CONFIG_SELF_ORIENT_FAILURE;
    }

    // TODO: Improve by clearing the contents of the segment.path BEYOND the part we care about!

    // Create a buffer to place the full path into
    size_t current_segment_name_len = strlen(current_segment_name);
    size_t full_path_len = strlen(segment.path);

    // Calculate the number of characters in the full buffer
    size_t n = (size_t)(segment.end - segment.path) / sizeof(*segment.path);

    // Copy from the segment struct into the full path
    memcpy_s(full_path, full_path_len, segment.path, n);

    return EC_OK;
}