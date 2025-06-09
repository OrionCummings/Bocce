#include "types.h"

void print_window_settings(WindowSettings settings) {
    printf("Window Settings:\n");
    printf("\twindow_width: '%d'\n", settings.window_width);
    printf("\twindow_height:' %d'\n", settings.window_height);
    printf("\tfullscreen: '%s'\n", (settings.fullscreen)? "true" : "false");
    printf("\twindow_title: '%s'\n", settings.window_title);
    printf("\tlog_level: '%d'\n", settings.log_level);
    printf("\ttarget_fps: '%d'\n", settings.target_fps);
    printf("\tconfig_flags: '%d'\n", settings.config_flags);
}

void print_client_settings(ClientSettings settings) {
    printf("Client Settings:\n");
    printf("\tversion: '%d.%d.%d'\n", settings.major_version, settings.minor_version, settings.patch_version);
    printf("\tserver_ip: '%s'\n", settings.server_ip);
    printf("\tserver_port: '%d'\n", settings.server_port);
}

void print_server_settings(ServerSettings settings) {
    printf("Server Settings:\n");
    printf("\tversion: '%d.%d.%d'\n", settings.major_version, settings.minor_version, settings.patch_version);
    printf("\tmax_players: '%d'\n", settings.max_players);
}

void print_application_settings(const ApplicationSettings settings) {
    print_window_settings(settings.window_settings);
    print_client_settings(settings.client_settings);
    print_server_settings(settings.server_settings);
}