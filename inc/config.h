#ifndef __CONFIG_H
#define __CONFIG_H

#include "toml.h"
#include "safe_lib.h"
#include "debug.h"
#include "types.h"
#include "error_codes.h"
#include "networking.h"
#include "cwalk.h"
#include "file_system.h"

// Config handling
ErrorCode parse_config(ApplicationSettings*, Server*, Client*);
ErrorCode parse_config_window(ApplicationSettings*, TomlTable*);
ErrorCode parse_config_client(ClientSettings*, TomlTable*);
ErrorCode parse_config_server(ServerSettings*, TomlTable*);

// TOML handling
void print_table(const TomlTable* table);
void print_value(const TomlValue* value);
void print_array(const TomlArray* array);
void print_keyval(const TomlKeyValue* keyval);

// Helper functions
bool is_known_table(const TomlKeyValue*, const char*);
bool is_known_key(const TomlKeyValue*, const char*);
bool is_type(const TomlKeyValue, TomlType);
ErrorCode get_config_file_path(char* project_dir_path, char* config_file_path);

#endif