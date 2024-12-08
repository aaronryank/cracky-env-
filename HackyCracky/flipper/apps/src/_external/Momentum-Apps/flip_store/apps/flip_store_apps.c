#include <apps/flip_store_apps.h>

FlipStoreAppInfo* flip_catalog = NULL;

uint32_t app_selected_index = 0;
bool flip_store_sent_request = false;
bool flip_store_success = false;
bool flip_store_saved_data = false;
bool flip_store_saved_success = false;
uint32_t flip_store_category_index = 0;

FlipStoreAppInfo* flip_catalog_alloc() {
    FlipStoreAppInfo* app_catalog =
        (FlipStoreAppInfo*)malloc(MAX_APP_COUNT * sizeof(FlipStoreAppInfo));
    if(!app_catalog) {
        FURI_LOG_E(TAG, "Failed to allocate memory for flip_catalog.");
        return NULL;
    }
    for(int i = 0; i < MAX_APP_COUNT; i++) {
        app_catalog[i].app_name = (char*)malloc(MAX_APP_NAME_LENGTH * sizeof(char));
        if(!app_catalog[i].app_name) {
            FURI_LOG_E(TAG, "Failed to allocate memory for app_name.");
            return NULL;
        }
        app_catalog[i].app_id = (char*)malloc(MAX_APP_NAME_LENGTH * sizeof(char));
        if(!app_catalog[i].app_id) {
            FURI_LOG_E(TAG, "Failed to allocate memory for app_id.");
            return NULL;
        }
        app_catalog[i].app_build_id = (char*)malloc(MAX_ID_LENGTH * sizeof(char));
        if(!app_catalog[i].app_build_id) {
            FURI_LOG_E(TAG, "Failed to allocate memory for app_build_id.");
            return NULL;
        }
    }
    return app_catalog;
}
void flip_catalog_free() {
    if(!flip_catalog) {
        return;
    }
    for(int i = 0; i < MAX_APP_COUNT; i++) {
        if(flip_catalog[i].app_name) {
            free(flip_catalog[i].app_name);
        }
        if(flip_catalog[i].app_id) {
            free(flip_catalog[i].app_id);
        }
        if(flip_catalog[i].app_build_id) {
            free(flip_catalog[i].app_build_id);
        }
    }
}

// Utility function to parse JSON incrementally from a file
bool flip_store_process_app_list() {
    // initialize the flip_catalog
    flip_catalog = flip_catalog_alloc();
    if(!flip_catalog) {
        FURI_LOG_E(TAG, "Failed to allocate memory for flip_catalog.");
        return false;
    }

    Storage* _storage = NULL;
    File* _file = NULL;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    bool in_string = false;
    bool is_escaped = false;
    bool reading_key = false;
    bool reading_value = false;
    bool inside_app_object = false;
    bool found_name = false;
    bool found_id = false;
    bool found_build_id = false;
    char current_key[MAX_KEY_LENGTH] = {0};
    size_t key_index = 0;
    char current_value[MAX_VALUE_LENGTH] = {0};
    size_t value_index = 0;
    int app_count = 0;
    enum ObjectState object_state = OBJECT_EXPECT_KEY; // Initialize object_state

    // Initialize parser state
    enum {
        STATE_SEARCH_APPS_KEY,
        STATE_SEARCH_ARRAY_START,
        STATE_READ_ARRAY_ELEMENTS,
        STATE_DONE
    } state = STATE_SEARCH_APPS_KEY;

    // Open storage and file
    _storage = furi_record_open(RECORD_STORAGE);
    if(!_storage) {
        FURI_LOG_E(TAG, "Failed to open storage.");
        return false;
    }

    _file = storage_file_alloc(_storage);
    if(!_file) {
        FURI_LOG_E(TAG, "Failed to allocate file.");
        furi_record_close(RECORD_STORAGE);
        return false;
    }

    if(!storage_file_open(_file, fhttp.file_path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FURI_LOG_E(TAG, "Failed to open JSON file for reading.");
        storage_file_free(_file);
        furi_record_close(RECORD_STORAGE);
        return false;
    }

    while((bytes_read = storage_file_read(_file, buffer, BUFFER_SIZE)) > 0 &&
          state != STATE_DONE) {
        for(size_t i = 0; i < bytes_read; ++i) {
            char c = buffer[i];

            if(is_escaped) {
                is_escaped = false;
                if(reading_key) {
                    if(key_index < MAX_KEY_LENGTH - 1) {
                        current_key[key_index++] = c;
                    }
                } else if(reading_value) {
                    if(value_index < MAX_VALUE_LENGTH - 1) {
                        current_value[value_index++] = c;
                    }
                }
                continue;
            }

            if(c == '\\') {
                is_escaped = true;
                continue;
            }

            if(c == '\"') {
                in_string = !in_string;

                if(in_string) {
                    // Start of a string
                    if(!reading_key && !reading_value) {
                        if(state == STATE_SEARCH_APPS_KEY) {
                            reading_key = true;
                            key_index = 0;
                            current_key[0] = '\0';
                        } else if(inside_app_object) {
                            if(object_state == OBJECT_EXPECT_KEY) {
                                reading_key = true;
                                key_index = 0;
                                current_key[0] = '\0';
                            } else if(object_state == OBJECT_EXPECT_VALUE) {
                                reading_value = true;
                                value_index = 0;
                                current_value[0] = '\0';
                            }
                        }
                    }
                } else {
                    // End of a string
                    if(reading_key) {
                        reading_key = false;
                        current_key[key_index] = '\0';

                        if(state == STATE_SEARCH_APPS_KEY && strcmp(current_key, "apps") == 0) {
                            state = STATE_SEARCH_ARRAY_START;
                        } else if(inside_app_object) {
                            object_state = OBJECT_EXPECT_COLON;
                        }
                    } else if(reading_value) {
                        reading_value = false;
                        current_value[value_index] = '\0';

                        if(inside_app_object) {
                            if(strcmp(current_key, "name") == 0) {
                                snprintf(
                                    flip_catalog[app_count].app_name,
                                    MAX_APP_NAME_LENGTH,
                                    "%.31s",
                                    current_value);
                                found_name = true;
                            } else if(strcmp(current_key, "id") == 0) {
                                snprintf(
                                    flip_catalog[app_count].app_id,
                                    MAX_ID_LENGTH,
                                    "%.31s",
                                    current_value);
                                found_id = true;
                            } else if(strcmp(current_key, "build_id") == 0) {
                                snprintf(
                                    flip_catalog[app_count].app_build_id,
                                    MAX_ID_LENGTH,
                                    "%.31s",
                                    current_value);
                                found_build_id = true;
                            }

                            // After processing value, expect comma or end
                            object_state = OBJECT_EXPECT_COMMA_OR_END;

                            // Check if both name and id are found
                            if(found_name && found_id && found_build_id) {
                                app_count++;
                                if(app_count >= MAX_APP_COUNT) {
                                    FURI_LOG_I(TAG, "Reached maximum app count.");
                                    state = STATE_DONE;
                                    break;
                                }

                                // Reset for next app
                                found_name = false;
                                found_id = false;
                                found_build_id = false;
                            }
                        }
                    }
                }
                continue;
            }

            if(in_string) {
                if(reading_key) {
                    if(key_index < MAX_KEY_LENGTH - 1) {
                        current_key[key_index++] = c;
                    }
                } else if(reading_value) {
                    if(value_index < MAX_VALUE_LENGTH - 1) {
                        current_value[value_index++] = c;
                    }
                }
                continue;
            }

            // Not inside a string

            if(state == STATE_SEARCH_ARRAY_START) {
                if(c == '[') {
                    state = STATE_READ_ARRAY_ELEMENTS;
                }
                continue;
            }

            if(state == STATE_READ_ARRAY_ELEMENTS) {
                if(c == '{') {
                    inside_app_object = true;
                    found_name = false;
                    found_id = false;
                    found_build_id = false;
                    object_state = OBJECT_EXPECT_KEY;
                } else if(c == '}') {
                    inside_app_object = false;
                    object_state = OBJECT_EXPECT_KEY;
                } else if(c == ']') {
                    state = STATE_DONE;
                    break;
                } else if(c == ':') {
                    if(inside_app_object && object_state == OBJECT_EXPECT_COLON) {
                        object_state = OBJECT_EXPECT_VALUE;
                    }
                } else if(c == ',') {
                    if(inside_app_object && object_state == OBJECT_EXPECT_COMMA_OR_END) {
                        object_state = OBJECT_EXPECT_KEY;
                    }
                    // Else, separator between objects or values
                }
                // Ignore other characters like whitespace, etc.
                continue;
            }
        }
    }

    // Clean up
    storage_file_close(_file);
    storage_file_free(_file);
    furi_record_close(RECORD_STORAGE);

    return app_count > 0;
}

bool flip_store_get_fap_file(
    char* build_id,
    uint8_t target,
    uint16_t api_major,
    uint16_t api_minor) {
    char url[128];
    fhttp.save_received_data = false;
    fhttp.is_bytes_request = true;
    snprintf(
        url,
        sizeof(url),
        "https://catalog.flipperzero.one/api/v0/application/version/%s/build/compatible?target=f%d&api=%d.%d",
        build_id,
        target,
        api_major,
        api_minor);
    char* headers = jsmn("Content-Type", "application/octet-stream");
    bool sent_request = flipper_http_get_request_bytes(url, headers);
    free(headers);
    return sent_request;
}

void flip_store_request_error(Canvas* canvas) {
    if(fhttp.last_response != NULL) {
        if(strstr(fhttp.last_response, "[ERROR] Not connected to Wifi. Failed to reconnect.") !=
           NULL) {
            canvas_clear(canvas);
            canvas_draw_str(canvas, 0, 10, "[ERROR] Not connected to Wifi.");
            canvas_draw_str(canvas, 0, 50, "Update your WiFi settings.");
            canvas_draw_str(canvas, 0, 60, "Press BACK to return.");
        } else if(strstr(fhttp.last_response, "[ERROR] Failed to connect to Wifi.") != NULL) {
            canvas_clear(canvas);
            canvas_draw_str(canvas, 0, 10, "[ERROR] Not connected to Wifi.");
            canvas_draw_str(canvas, 0, 50, "Update your WiFi settings.");
            canvas_draw_str(canvas, 0, 60, "Press BACK to return.");
        } else {
            FURI_LOG_E(TAG, "Received an error: %s", fhttp.last_response);
            canvas_draw_str(canvas, 0, 42, "Unusual error...");
            canvas_draw_str(canvas, 0, 50, "Update your WiFi settings.");
            canvas_draw_str(canvas, 0, 60, "Press BACK to return.");
        }
    } else {
        canvas_clear(canvas);
        canvas_draw_str(canvas, 0, 10, "[ERROR] Unknown error.");
        canvas_draw_str(canvas, 0, 50, "Update your WiFi settings.");
        canvas_draw_str(canvas, 0, 60, "Press BACK to return.");
    }
}
// function to handle the entire installation process "asynchronously"
bool flip_store_install_app(Canvas* canvas, char* category) {
    // create /apps/FlipStore directory if it doesn't exist
    char directory_path[128];
    snprintf(directory_path, sizeof(directory_path), STORAGE_EXT_PATH_PREFIX "/apps/%s", category);

    // Create the directory
    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_common_mkdir(storage, directory_path);

    // Adjusted to access flip_catalog as an array of structures
    char installing_text[64];
    snprintf(
        installing_text,
        sizeof(installing_text),
        "Installing %s",
        flip_catalog[app_selected_index].app_name);
    snprintf(
        fhttp.file_path,
        sizeof(fhttp.file_path),
        STORAGE_EXT_PATH_PREFIX "/apps/%s/%s.fap",
        category,
        flip_catalog[app_selected_index].app_id);
    canvas_draw_str(canvas, 0, 10, installing_text);
    canvas_draw_str(canvas, 0, 20, "Sending request..");
    uint8_t target = furi_hal_version_get_hw_target();
    uint16_t api_major, api_minor;
    furi_hal_info_get_api_version(&api_major, &api_minor);
    if(fhttp.state != INACTIVE &&
       flip_store_get_fap_file(
           flip_catalog[app_selected_index].app_build_id, target, api_major, api_minor)) {
        canvas_draw_str(canvas, 0, 30, "Request sent.");
        fhttp.state = RECEIVING;
        // furi_timer_start(fhttp.get_timeout_timer, TIMEOUT_DURATION_TICKS);
        canvas_draw_str(canvas, 0, 40, "Receiving...");
    } else {
        FURI_LOG_E(TAG, "Failed to send the request");
        flip_store_success = false;
        return false;
    }
    while(fhttp.state == RECEIVING && furi_timer_is_running(fhttp.get_timeout_timer) > 0) {
        // Wait for the feed to be received
        furi_delay_ms(10);
    }
    // furi_timer_stop(fhttp.get_timeout_timer);
    if(fhttp.state == ISSUE) {
        flip_store_request_error(canvas);
        flip_store_success = false;
        return false;
    }
    flip_store_success = true;
    return true;
}

// process the app list and return view
int32_t flip_store_handle_app_list(
    FlipStoreApp* app,
    int32_t success_view,
    char* category,
    Submenu** submenu) {
    // reset the flip_catalog
    flip_catalog_free();

    if(!app) {
        FURI_LOG_E(TAG, "FlipStoreApp is NULL");
        return FlipStoreViewPopup;
    }
    char url[128];
    snprintf(
        fhttp.file_path,
        sizeof(fhttp.file_path),
        STORAGE_EXT_PATH_PREFIX "/apps_data/flip_store/apps.txt");

    fhttp.save_received_data = true;
    fhttp.is_bytes_request = false;
    snprintf(
        url, sizeof(url), "https://www.flipsocial.net/api/flipper/apps/%s/extended/", category);
    char* headers = jsmn("Content-Type", "application/json");
    // async call to the app list with timer
    if(fhttp.state != INACTIVE && flipper_http_get_request_with_headers(url, headers)) {
        furi_timer_start(fhttp.get_timeout_timer, TIMEOUT_DURATION_TICKS);
        free(headers);
        fhttp.state = RECEIVING;
    } else {
        FURI_LOG_E(TAG, "Failed to send the request");
        free(headers);
        fhttp.state = ISSUE;
        return FlipStoreViewPopup;
    }
    while(fhttp.state == RECEIVING && furi_timer_is_running(fhttp.get_timeout_timer) > 0) {
        // Wait for the feed to be received
        furi_delay_ms(100);
    }
    furi_timer_stop(fhttp.get_timeout_timer);
    if(fhttp.state == ISSUE) {
        FURI_LOG_E(TAG, "Failed to receive data");
        if(fhttp.last_response == NULL) {
            if(fhttp.last_response != NULL) {
                if(strstr(
                       fhttp.last_response,
                       "[ERROR] Not connected to Wifi. Failed to reconnect.") != NULL) {
                    popup_set_text(
                        app->popup,
                        "[ERROR] WiFi Disconnected.\n\n\nUpdate your WiFi settings.\nPress BACK to return.",
                        0,
                        10,
                        AlignLeft,
                        AlignTop);
                } else if(strstr(fhttp.last_response, "[ERROR] Failed to connect to Wifi.") != NULL) {
                    popup_set_text(
                        app->popup,
                        "[ERROR] WiFi Disconnected.\n\n\nUpdate your WiFi settings.\nPress BACK to return.",
                        0,
                        10,
                        AlignLeft,
                        AlignTop);
                } else {
                    popup_set_text(app->popup, fhttp.last_response, 0, 10, AlignLeft, AlignTop);
                }
            } else {
                popup_set_text(
                    app->popup,
                    "[ERROR] Unknown Error.\n\n\nUpdate your WiFi settings.\nPress BACK to return.",
                    0,
                    10,
                    AlignLeft,
                    AlignTop);
            }
            return FlipStoreViewPopup;
        } else {
            popup_set_text(app->popup, "Failed to received data.", 0, 10, AlignLeft, AlignTop);
            return FlipStoreViewPopup;
        }
    } else {
        // process the app list
        if(flip_store_process_app_list()) {
            submenu_reset(*submenu);
            // add each app name to submenu
            for(int i = 0; i < MAX_APP_COUNT; i++) {
                if(strlen(flip_catalog[i].app_name) > 0) {
                    submenu_add_item(
                        *submenu,
                        flip_catalog[i].app_name,
                        FlipStoreSubmenuIndexStartAppList + i,
                        callback_submenu_choices,
                        app);
                }
            }
            return success_view;
        } else {
            FURI_LOG_E(TAG, "Failed to process the app list");
            popup_set_text(
                app->popup, "Failed to process the app list", 0, 10, AlignLeft, AlignTop);
            return FlipStoreViewPopup;
        }
    }
}
