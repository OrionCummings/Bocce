#include "config.h"

// TODO: Make this function accept a path (from cwalk!)
// TODO: Make this function/subfunctions work properly with small integer types! As is, integers are used in places where small integer types would be more accurate (make invalid types unrepresentable!)
ErrorCode parse_config(ApplicationSettings* application_settings, Server* server, Client* client) {

    char project_dir_path[FILENAME_MAX];
    char config_file_path[FILENAME_MAX];
    memset(project_dir_path, 0, FILENAME_MAX * sizeof(*project_dir_path));
    memset(config_file_path, 0, FILENAME_MAX * sizeof(*config_file_path));

    ErrorCode ec_get_file_path_from_application_mode
        = get_config_file_path(&(project_dir_path[0]), &(config_file_path[0]));
    if (ec_get_file_path_from_application_mode) {
        B_ERROR("Failed to get config file path");
        return ec_get_file_path_from_application_mode;
    }

    B_INFO("Found config file '%s'", config_file_path);

    // Load the config.toml
    TomlTable* table = toml_load_filename(config_file_path);

    // If the config file loading fails, then send an error message and then early return
    if (toml_err()->code != TOML_OK) {
        B_ERROR("Failed to load toml file: %d: %s", toml_err()->code, toml_err()->message);
        toml_err_clear();
        return EC_CONFIG_TOML_PARSE_FAILURE;
    }

    // Flags to track the existence of these tables
    bool has_window_table = false;
    bool has_server_table = false;
    bool has_client_table = false;

    // Create a table iterator and loop through each element (sub-tables)
    TomlTableIter it = toml_table_iter_new(table);
    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        TomlTable* top_level_table = keyval->value->value.table;

        // If the current iterator head is a table with a known/expected name, parse it
        if (is_known_table(keyval, "window")) {
            has_window_table = true;
            ErrorCode ec_parse_config_window = parse_config_window(application_settings, top_level_table);
            if (ec_parse_config_window) { B_ERROR("Failed to parse window config"); }
        } else if (is_known_table(keyval, "client")) {
            has_client_table = true;
            ErrorCode ec_parse_config_client = parse_config_client(&client->settings, top_level_table);
            if (ec_parse_config_client) { B_ERROR("Failed to parse client config"); }
        } else if (is_known_table(keyval, "server")) {
            has_server_table = true;
            ErrorCode ec_parse_config_server = parse_config_server(&server->settings, top_level_table);
            if (ec_parse_config_server) { B_ERROR("Failed to parse server config"); }
        } else {

            // If we find an unexpected table, throw a warning
            char* unexpected_table = keyval->key->str;
            B_WARNING("Unexpected TOML table '%s' found in config.toml", unexpected_table);
        }

        toml_table_iter_next(&it);
    }

    // Free the parsed table
    toml_table_free(table);

    // Figure out if this is a client, server, or dual application
    if (has_window_table && has_client_table && !has_server_table) {
        application_settings->application_mode = AM_CLIENT;
    } else if (!has_window_table && !has_client_table && has_server_table) {
        application_settings->application_mode = AM_SERVER;
    } else if (has_window_table && has_client_table && has_server_table) {
        application_settings->application_mode = AM_DUAL;
    } else {
        application_settings->application_mode = AM_UNKNOWN;
    }

    return 0;
}

ErrorCode parse_config_window(ApplicationSettings* settings, TomlTable* table) {

    const char* table_name = "window";
    TomlTableIter it = toml_table_iter_new(table);

    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (is_known_key(keyval, "width")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->window_settings.dim.window_width = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "height")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->window_settings.dim.window_height = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'height' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "fullscreen")) {
            if (is_type(*keyval, TOML_BOOLEAN)){
                settings->window_settings.fullscreen = keyval->value->value.boolean;
            } else {
                B_WARNING("Key-value-pair 'fullscreen' in table '%s' does not have value type 'boolean'", table_name);
            }
        } else if (is_known_key(keyval, "window_title")) {
            if (is_type(*keyval, TOML_STRING)){
                memset(settings->window_settings.window_title, 0, 256);
                memcpy_s(settings->window_settings.window_title, 256, keyval->value->value.string->str, strlen(keyval->value->value.string->str));
            } else {
                B_WARNING("Key-value-pair 'window_title' in table '%s' does not have value type 'string'", table_name);
            }
        } else {

            // NOTE: Avoid putting complex expressions in the debug macros!
            const char* unexpected_field = keyval->key->str;
            B_WARNING("Unexpected field '%s' in table '%s'", unexpected_field, table_name);
        }

        toml_table_iter_next(&it);
    }

    return 0;
}

ErrorCode parse_config_client(ClientSettings* settings, TomlTable* table) {

    const char* table_name = "client";
    TomlTableIter it = toml_table_iter_new(table);

    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (is_known_key(keyval, "major_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->major_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "minor_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->minor_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'height' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "patch_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->patch_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'path_version' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "server_ip")) {
            if (is_type(*keyval, TOML_STRING)){
                memset(settings->server_ip, 0, 16);
                memcpy_s(settings->server_ip, 16, keyval->value->value.string->str, strlen(keyval->value->value.string->str));
            } else {
                B_WARNING("Key-value-pair 'server_ip' in table '%s' does not have value type 'string'", table_name);
            }
        } else if (is_known_key(keyval, "server_port")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->server_port = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'server_port' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "id")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->id = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'id' in table '%s' does not have value type 'integer'", table_name);
            }
        } else {

            // NOTE: Avoid putting complex expressions in the debug macros!
            const char* unexpected_field = keyval->key->str;
            B_WARNING("Unexpected field '%s' in table '%s'", unexpected_field, table_name);
        }

        toml_table_iter_next(&it);
    }

    return 0;
}

ErrorCode parse_config_server(ServerSettings* settings, TomlTable* table) {

    const char* table_name = "server";
    TomlTableIter it = toml_table_iter_new(table);

    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (is_known_key(keyval, "major_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->major_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "minor_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->minor_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'height' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "patch_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->patch_version = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'path_version' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "max_players")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->max_players = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'max_players' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "port")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->port = (uint16_t)keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'port' in table '%s' does not have value type 'integer'", table_name);
            }
        } else {

            // NOTE: Avoid putting complex expressions in the debug macros!
            const char* unexpected_field = keyval->key->str;
            B_WARNING("Unexpected field '%s' in table '%s'", unexpected_field, table_name);
        }

        toml_table_iter_next(&it);
    }

    return 0;
}

void print_array(const TomlArray* array) {
    printf("[");
    for (size_t i = 0; i < array->len; i++) {
        if (i > 0) {
            printf(", ");
        }
        print_value(array->elements[i]);
    }
    printf("]");
}

void print_value(const TomlValue* value) {
    switch (value->type) {
    case TOML_TABLE:
        print_table(value->value.table);
        break;
    case TOML_ARRAY:
        print_array(value->value.array);
        break;
    case TOML_STRING:
        printf("\"%s\"", value->value.string->str);
        break;
    case TOML_INTEGER:
        printf("%ld", value->value.integer);
        break;
    case TOML_FLOAT:
        printf("%f", value->value.float_);
        break;
    case TOML_DATETIME:
        printf("(datetime)");
        break;
    case TOML_BOOLEAN:
        printf("%s", value->value.boolean ? "true" : "false");
        break;
    }
}

void print_keyval(const TomlKeyValue* keyval) {
    printf("\"%s\": ", keyval->key->str);
    print_value(keyval->value);
}

void print_table(const TomlTable* table) {
    TomlTableIter it = toml_table_iter_new((TomlTable*)table);

    printf("{");
    size_t i = 0;
    while (toml_table_iter_has_next(&it)) {
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (i > 0) { printf(", "); }
        print_keyval(keyval);

        toml_table_iter_next(&it);
        i++;
    }
    printf("}");
}

bool is_known_table(const TomlKeyValue* keyval, const char* table_name) {
    return (keyval->value->type == TOML_TABLE) && (strcmp(keyval->key->str, table_name) == 0);
}

bool is_known_key(const TomlKeyValue* keyval, const char* key) {
    return strcmp(keyval->key->str, key) == 0;
}

bool is_type(const TomlKeyValue keyval, TomlType type) {
    return keyval.value->type == type;
}

ErrorCode get_config_file_path(char* project_dir_path, char* config_file_path) {

    const char* config_folder_name = "config";
    const char* client_config = "client_config.toml";
    const char* server_config = "server_config.toml";
    const char* dual_config = "dual_config.toml";

    char client_config_file_path[FILENAME_MAX] = { 0 };
    char server_config_file_path[FILENAME_MAX] = { 0 };
    char dual_config_file_path[FILENAME_MAX] = { 0 };

    // Get the project directory
    ErrorCode ec_get_project_dir = get_project_dir(project_dir_path);
    if (ec_get_project_dir) {
        B_ERROR("Failed to find project dir");
        return ec_get_project_dir;
    }

    // Create the config folder by appending "config" onto the project folder
    char config_folder_path[FILENAME_MAX] = { 0 };
    cwk_path_join(project_dir_path, config_folder_name, config_folder_path, FILENAME_MAX * sizeof(*config_folder_path));

    // Create the config file paths from the config base path
    cwk_path_join(config_folder_path, client_config, client_config_file_path, FILENAME_MAX * sizeof(*client_config_file_path));
    cwk_path_join(config_folder_path, server_config, server_config_file_path, FILENAME_MAX * sizeof(*server_config_file_path));
    cwk_path_join(config_folder_path, dual_config, dual_config_file_path, FILENAME_MAX * sizeof(*dual_config_file_path));

    // TODO: Don't use fopen as it fails if the user doesn't have permission!

    // Check if the client config exists
    FILE* file_client = fopen(client_config_file_path, "r");
    if (file_client != NULL) {
        memcpy_s(config_file_path, FILENAME_MAX * sizeof(*config_file_path), client_config_file_path, FILENAME_MAX * sizeof(*client_config_file_path));

        // Close the opened files
        fclose(file_client);

        return EC_OK;
    }

    // Check if the server config exists
    FILE* file_server = fopen(server_config_file_path, "r");
    if (file_server != NULL) {
        memcpy_s(config_file_path, FILENAME_MAX * sizeof(*config_file_path), server_config_file_path, FILENAME_MAX * sizeof(*server_config_file_path));

        // Close the opened files
        fclose(file_server);

        return EC_OK;
    }

    // Check if the dual config exists
    FILE* file_dual = fopen(dual_config_file_path, "r");
    if (file_dual != NULL) {
        memcpy_s(config_file_path, FILENAME_MAX * sizeof(*config_file_path), dual_config_file_path, FILENAME_MAX * sizeof(*dual_config_file_path));

        // Close the opened files
        fclose(file_dual);

        return EC_OK;
    }

    return EC_CONFIG_NO_FILE_FOUND;
}