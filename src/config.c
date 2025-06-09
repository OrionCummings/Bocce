#include "config.h"

// TODO: Make this function accept a path (from cwalk!)
// TODO: Make this function/subfunctions work properly with small integer types! As is, integers are used in places where small integer types would be more accurate (make invalid types unrepresentable!)
ErrorCode parse_config(ApplicationSettings* application_settings) {

    // Load the config.toml
    TomlTable* table = toml_load_filename("../../../config.toml");

    // If the config file loading fails, then send an error message and then early return
    if (toml_err()->code != TOML_OK) {
        B_ERROR("toml: %d: %s", toml_err()->code, toml_err()->message);
        toml_err_clear();
        return 1;
    }

    // Create a table iterator and loop through each element (sub-tables)
    TomlTableIter it = toml_table_iter_new(table);
    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        TomlTable* top_level_table = keyval->value->value.table;

        // If the current iterator head is a table with a known/expected name, parse it
        if (is_known_table(keyval, "window")) {
            ErrorCode ec_parse_config_window = parse_config_window(application_settings, top_level_table);
            if (ec_parse_config_window) { B_ERROR("Failed to parse window config"); }
        } else if (is_known_table(keyval, "client")) {
            ErrorCode ec_parse_config_client = parse_config_client(application_settings, top_level_table);
            if (ec_parse_config_client) { B_ERROR("Failed to parse client config"); }
        } else if (is_known_table(keyval, "server")) {
            ErrorCode ec_parse_config_server = parse_config_server(application_settings, top_level_table);
            if (ec_parse_config_server) { B_ERROR("Failed to parse server config"); }
        } else {

            // If we find an unexpected table, throw a warning
            char* unexpected_table = keyval->key->str;
            B_WARNING("Unexpected TOML table '%s' found in config.toml", unexpected_table);
        }

        toml_table_iter_next(&it);
    }

    toml_table_free(table);

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
                settings->window_settings.window_width = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "height")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->window_settings.window_height = keyval->value->value.integer;
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

ErrorCode parse_config_client(ApplicationSettings* settings, TomlTable* table) {

    const char* table_name = "client";
    TomlTableIter it = toml_table_iter_new(table);

    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (is_known_key(keyval, "major_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->client_settings.major_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "minor_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->client_settings.minor_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'height' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "patch_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->client_settings.patch_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'path_version' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "server_ip")) {
            if (is_type(*keyval, TOML_STRING)){
                memset(settings->client_settings.server_ip, 0, 16);
                memcpy_s(settings->client_settings.server_ip, 16, keyval->value->value.string->str, strlen(keyval->value->value.string->str));
            } else {
                B_WARNING("Key-value-pair 'server_ip' in table '%s' does not have value type 'string'", table_name);
            }
        } else if (is_known_key(keyval, "server_port")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->client_settings.server_port = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'server_port' in table '%s' does not have value type 'integer'", table_name);
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

ErrorCode parse_config_server(ApplicationSettings* settings, TomlTable* table) {

    const char* table_name = "server";
    TomlTableIter it = toml_table_iter_new(table);

    while (toml_table_iter_has_next(&it)) {

        // Get the key-value-pair at the current iterator head
        TomlKeyValue* keyval = toml_table_iter_get(&it);

        if (is_known_key(keyval, "major_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->server_settings.major_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'width' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "minor_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->server_settings.minor_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'height' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "patch_version")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->server_settings.patch_version = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'path_version' in table '%s' does not have value type 'integer'", table_name);
            }
        } else if (is_known_key(keyval, "max_players")) {
            if (is_type(*keyval, TOML_INTEGER)){
                settings->server_settings.max_players = keyval->value->value.integer;
            } else {
                B_WARNING("Key-value-pair 'server_ip' in table '%s' does not have value type 'string'", table_name);
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

bool is_type(TomlKeyValue keyval, TomlType type) {
    return keyval.value->type == type;
}

