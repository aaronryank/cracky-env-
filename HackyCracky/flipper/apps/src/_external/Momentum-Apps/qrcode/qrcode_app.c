#include <furi.h>

#include <dialogs/dialogs.h>
#include <gui/gui.h>
#include <storage/storage.h>

#include <lib/flipper_format/flipper_format.h>

// this file is generated by the build script
#include <qrcode_icons.h>
#include "qrcode.h"

#define TAG "qrcode"

#define QRCODE_FOLDER       STORAGE_APP_DATA_PATH_PREFIX
#define QRCODE_EXTENSION    ".qrcode"
#define QRCODE_FILETYPE     "QRCode"
#define QRCODE_FILE_VERSION 1

/** Valid modes are Numeric (0), Alpha-Numeric (1), and Binary (2) */
#define MAX_QRCODE_MODE 2

/**
 * Maximum version is 11 because the f0 screen is only 64 pixels high and
 * version 12 is 65x65. Version 11 is 61x61.
 */
#define MAX_QRCODE_VERSION 11

/** Valid ECC levels are Low (0), Medium (1), Quartile (2), and High (3) */
#define MAX_QRCODE_ECC 3

/** Maximum length by mode, ecc, and version */
static const uint16_t MAX_LENGTH[3][4][MAX_QRCODE_VERSION] = {
    {
        // Numeric
        {41, 77, 127, 187, 255, 322, 370, 461, 552, 652, 772}, // Low
        {34, 63, 101, 149, 202, 255, 293, 365, 432, 513, 604}, // Medium
        {27, 48, 77, 111, 144, 178, 207, 259, 312, 364, 427}, // Quartile
        {17, 34, 58, 82, 106, 139, 154, 202, 235, 288, 331}, // High
    },
    {
        // Alphanumeric
        {25, 47, 77, 114, 154, 195, 224, 279, 335, 395, 468}, // Low
        {20, 38, 61, 90, 122, 154, 178, 221, 262, 311, 366}, // Medium
        {16, 29, 47, 67, 87, 108, 125, 157, 189, 221, 259}, // Quartile
        {10, 20, 35, 50, 64, 84, 93, 122, 143, 174, 200}, // High
    },
    {
        // Binary
        {17, 32, 53, 78, 106, 134, 154, 192, 230, 271, 321}, // Low
        {14, 26, 42, 62, 84, 106, 122, 152, 180, 213, 251}, // Medium
        {11, 20, 32, 46, 60, 74, 86, 108, 130, 151, 177}, // Quartile
        {7, 14, 24, 34, 44, 58, 64, 84, 98, 119, 137}, // High
    },
};

/** Main app instance */
typedef struct {
    FuriMessageQueue* input_queue;
    Gui* gui;
    ViewPort* view_port;

    FuriMutex* mutex;
    FuriString* message;
    QRCode* qrcode;
    uint8_t min_mode;
    uint8_t max_mode;
    uint8_t min_version;
    uint8_t max_ecc_at_min_version;
    bool loading;
    bool too_long;
    bool show_stats;
    uint8_t selected_idx;
    bool edit;
    uint8_t set_mode;
    uint8_t set_version;
    uint8_t set_ecc;
} QRCodeApp;

/**
 * @param ecc ECC number
 * @returns a character corresponding to the ecc level
 */
static char get_ecc_char(uint8_t ecc) {
    switch(ecc) {
    case 0:
        return 'L';
    case 1:
        return 'M';
    case 2:
        return 'Q';
    case 3:
        return 'H';
    default:
        return '?';
    }
}

/**
 * @param ecc A character representing an ECC mode (L, M, Q, or H)
 * @returns the ecc level or 255 representing an unknown ECC mode
 */
static uint8_t get_ecc_value(char ecc) {
    switch(ecc) {
    case 'L':
    case 'l':
        return 0;
    case 'M':
    case 'm':
        return 1;
    case 'Q':
    case 'q':
        return 2;
    case 'H':
    case 'h':
        return 3;
    default:
        return 255;
    }
}

/**
 * @param mode qrcode mode
 * @returns a character corresponding to the mode
 */
static char get_mode_char(uint8_t mode) {
    switch(mode) {
    case 0:
        return 'N';
    case 1:
        return 'A';
    case 2:
        return 'B';
    case 3:
        return 'K';
    default:
        return '?';
    }
}

/**
 * @param mode A character representing a qrcode mode (N, A, or B)
 * @returns the mode or 255 representing an unknown mode
 */
static uint8_t get_mode_value(char mode) {
    switch(mode) {
    case 'N':
    case 'n':
        return 0;
    case 'A':
    case 'a':
        return 1;
    case 'B':
    case 'b':
        return 2;
    default:
        return 255;
    }
}

/**
 * Render
 * @param canvas The canvas to render to
 * @param ctx Context provided to the callback by view_port_draw_callback_set
 */
static void render_callback(Canvas* canvas, void* ctx) {
    furi_assert(canvas);
    furi_assert(ctx);

    QRCodeApp* instance = ctx;
    furi_check(furi_mutex_acquire(instance->mutex, FuriWaitForever) == FuriStatusOk);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    uint8_t font_height = canvas_current_font_height(canvas);
    uint8_t width = canvas_width(canvas);
    uint8_t height = canvas_height(canvas);
    if(instance->loading) {
        canvas_draw_str_aligned(
            canvas, width / 2, height / 2, AlignCenter, AlignCenter, "Loading...");
    } else if(instance->qrcode) {
        uint8_t size = instance->qrcode->size;
        uint8_t pixel_size = height / size;
        uint8_t top = (height - pixel_size * size) / 2;
        uint8_t left = ((instance->show_stats ? 65 : width) - pixel_size * size) / 2;
        for(uint8_t y = 0; y < size; y++) {
            for(uint8_t x = 0; x < size; x++) {
                if(qrcode_getModule(instance->qrcode, x, y)) {
                    if(pixel_size == 1) {
                        canvas_draw_dot(canvas, left + x * pixel_size, top + y * pixel_size);
                    } else {
                        canvas_draw_box(
                            canvas,
                            left + x * pixel_size,
                            top + y * pixel_size,
                            pixel_size,
                            pixel_size);
                    }
                }
            }
        }

        if(instance->show_stats) {
            top = 10;
            left = 66;

            FuriString* str = furi_string_alloc();

            if(!instance->edit || instance->selected_idx == 0) {
                furi_string_printf(str, "Mod: %c", get_mode_char(instance->set_mode));
                canvas_draw_str(canvas, left + 5, font_height + top, furi_string_get_cstr(str));
                if(instance->selected_idx == 0) {
                    canvas_draw_triangle(
                        canvas,
                        left,
                        top + font_height / 2,
                        font_height - 4,
                        4,
                        CanvasDirectionLeftToRight);
                }
                if(instance->edit) {
                    uint8_t arrow_left = left + 5 + canvas_string_width(canvas, "Mod: B") / 2;
                    canvas_draw_triangle(
                        canvas, arrow_left, top, font_height - 4, 4, CanvasDirectionBottomToTop);
                    canvas_draw_triangle(
                        canvas,
                        arrow_left,
                        top + font_height + 1,
                        font_height - 4,
                        4,
                        CanvasDirectionTopToBottom);
                }
            }

            if(!instance->edit || instance->selected_idx == 1) {
                furi_string_printf(str, "Ver: %i", instance->set_version);
                canvas_draw_str(
                    canvas, left + 5, 2 * font_height + top + 2, furi_string_get_cstr(str));
                if(instance->selected_idx == 1) {
                    canvas_draw_triangle(
                        canvas,
                        left,
                        3 * font_height / 2 + top + 2,
                        font_height - 4,
                        4,
                        CanvasDirectionLeftToRight);
                }
                if(instance->edit) {
                    uint8_t arrow_left = left + 5 + canvas_string_width(canvas, "Ver: 8") / 2;
                    canvas_draw_triangle(
                        canvas,
                        arrow_left,
                        font_height + top + 2,
                        font_height - 4,
                        4,
                        CanvasDirectionBottomToTop);
                    canvas_draw_triangle(
                        canvas,
                        arrow_left,
                        2 * font_height + top + 3,
                        font_height - 4,
                        4,
                        CanvasDirectionTopToBottom);
                }
            }

            if(!instance->edit || instance->selected_idx == 2) {
                furi_string_printf(str, "ECC: %c", get_ecc_char(instance->set_ecc));
                canvas_draw_str(
                    canvas, left + 5, 3 * font_height + top + 4, furi_string_get_cstr(str));
                if(instance->selected_idx == 2) {
                    canvas_draw_triangle(
                        canvas,
                        left,
                        5 * font_height / 2 + top + 4,
                        font_height - 4,
                        4,
                        CanvasDirectionLeftToRight);
                }
                if(instance->edit) {
                    uint8_t arrow_left = left + 5 + canvas_string_width(canvas, "ECC: H") / 2;
                    canvas_draw_triangle(
                        canvas,
                        arrow_left,
                        2 * font_height + top + 4,
                        font_height - 4,
                        4,
                        CanvasDirectionBottomToTop);
                    canvas_draw_triangle(
                        canvas,
                        arrow_left,
                        3 * font_height + top + 5,
                        font_height - 4,
                        4,
                        CanvasDirectionTopToBottom);
                }
            }

            furi_string_free(str);
        }
    } else {
        uint8_t margin = (height - font_height * 2) / 3;
        canvas_draw_str_aligned(
            canvas, width / 2, margin, AlignCenter, AlignTop, "Could not load qrcode.");
        if(instance->too_long) {
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, width / 2, margin * 2 + font_height, "Message is too long.");
        }
    }

    furi_mutex_release(instance->mutex);
}

/**
 * Handle input
 * @param input_event The received input event
 * @param ctx Context provided to the callback by view_port_input_callback_set
 */
static void input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(input_event);
    furi_assert(ctx);
    if(input_event->type == InputTypeShort) {
        QRCodeApp* instance = ctx;
        furi_message_queue_put(instance->input_queue, input_event, 0);
    }
}

/**
 * Determine if the given string is all numeric
 * @param str The string to test
 * @returns true if the string is all numeric
 */
static bool is_numeric(const char* str, uint16_t len) {
    furi_assert(str);
    while(len > 0) {
        char c = str[--len];
        if(c < '0' || c > '9') return false;
    }
    return true;
}

/**
 * Determine if the given string is alphanumeric
 * @param str The string to test
 * @returns true if the string is alphanumeric
 */
static bool is_alphanumeric(const char* str, uint16_t len) {
    furi_assert(str);
    while(len > 0) {
        char c = str[--len];
        if(c >= '0' && c <= '9') continue;
        if(c >= 'A' && c <= 'Z') continue;
        if(c == ' ' || c == '$' || c == '%' || c == '*' || c == '+' || c == '-' || c == '.' ||
           c == '/' || c == ':')
            continue;
        return false;
    }
    return true;
}

/**
 * Allocate a qrcode
 * @param version qrcode version
 * @returns an allocated QRCode
 */
static QRCode* qrcode_alloc(uint8_t version) {
    QRCode* qrcode = malloc(sizeof(QRCode));
    qrcode->modules = malloc(qrcode_getBufferSize(version));
    return qrcode;
}

/**
 * Free a QRCode
 * @param qrcode The QRCode to free
 */
static void qrcode_free(QRCode* qrcode) {
    furi_assert(qrcode);
    free(qrcode->modules);
    free(qrcode);
}

/**
 * Rebuild the qrcode. Assumes that instance->message is the message to encode,
 * that the mutex has been acquired, and the specified version/ecc will be
 * sufficiently large enough to encode the full message. It is also assumed
 * that the old qrcode will be free'd by the caller.
 * @param instance The qrcode app instance
 * @param mode The qrcode mode to use
 * @param version The qrcode version to use
 * @param ecc The qrcode ECC level to use
 * @returns true if the qrcode was successfully created
 */
static bool rebuild_qrcode(QRCodeApp* instance, uint8_t mode, uint8_t version, uint8_t ecc) {
    furi_assert(instance);
    furi_assert(instance->message);

    const char* cstr = furi_string_get_cstr(instance->message);
    uint16_t len = (uint16_t)furi_string_size(instance->message);
    instance->qrcode = qrcode_alloc(version);

    int8_t res = qrcode_initBytes(
        instance->qrcode,
        instance->qrcode->modules,
        (int8_t)mode,
        version,
        ecc,
        (uint8_t*)cstr,
        len);
    if(res != 0) {
        FURI_LOG_E(TAG, "Could not create qrcode");

        qrcode_free(instance->qrcode);
        instance->qrcode = NULL;

        return false;
    }
    return true;
}

/**
 * Determine the minimum version and maximum ECC for a message of a given
 * length and mode.
 * @param len The length of the message
 * @param mode The mode of the encoded message
 * @param version Pointer to variable that will receive the minimum version
 * @param ecc Pointer to variable that will receive the maximum ECC
 * @returns false if the data is too long for the given mode, true otherwise.
 */
static bool find_min_version_max_ecc(uint16_t len, uint8_t mode, uint8_t* version, uint8_t* ecc) {
    // Figure out the smallest qrcode version that'll fit all of the data - we
    // prefer the smallest version to maximize the pixel size of each module to
    // improve reader performance. Here, version is the 0-based index. The
    // qrcode_initBytes function will want a 1-based version number, so we'll
    // add one later.
    *ecc = ECC_LOW;
    *version = 0;
    while(*version < MAX_QRCODE_VERSION && MAX_LENGTH[mode][*ecc][*version] < len) {
        (*version)++;
    }

    if(*version == MAX_QRCODE_VERSION) {
        return false;
    }

    // Figure out the maximum ECC we can use. I shouldn't need to bounds-check
    // ecc in this loop because I already know from the loop above that ECC_LOW
    // (0) works... don't forget to add one to that version number, since we're
    // using it as a 0-based number here, but qrcode_initBytes will want a
    // 1-based number...
    *ecc = ECC_HIGH;
    while(MAX_LENGTH[mode][*ecc][*version] < len) {
        (*ecc)--;
    }
    (*version)++;

    return true;
}

/**
 * Load a qrcode from a string
 * @param instance The qrcode app instance
 * @param str The message to encode as a qrcode
 * @param desired_mode User selected mode, 255 = unset
 * @param desired_version User selected version, 255 = unset
 * @param desired_ecc User selected ECC, 255 = unset
 * @returns true if the string was successfully loaded
 */
static bool qrcode_load_string(
    QRCodeApp* instance,
    FuriString* str,
    uint8_t desired_mode,
    uint8_t desired_version,
    uint8_t desired_ecc) {
    furi_assert(instance);
    furi_assert(str);

    furi_check(furi_mutex_acquire(instance->mutex, FuriWaitForever) == FuriStatusOk);
    if(instance->message) {
        furi_string_free(instance->message);
        instance->message = NULL;
    }
    if(instance->qrcode) {
        qrcode_free(instance->qrcode);
        instance->qrcode = NULL;
    }
    instance->too_long = false;
    instance->show_stats = false;
    instance->selected_idx = 0;
    instance->edit = false;

    bool result = false;
    do {
        const char* cstr = furi_string_get_cstr(str);
        uint16_t len = (uint16_t)furi_string_size(str);

        instance->message = furi_string_alloc_set(str);
        if(!instance->message) {
            FURI_LOG_E(TAG, "Could not allocate message");
            break;
        }

        // figure out the minimum qrcode "mode"
        int8_t min_mode = MODE_BYTE;
        if(is_numeric(cstr, len))
            min_mode = MODE_NUMERIC;
        else if(is_alphanumeric(cstr, len))
            min_mode = MODE_ALPHANUMERIC;

        // determine the maximum "mode"
        int8_t max_mode = MAX_QRCODE_MODE;
        uint8_t min_version = 0;
        uint8_t max_ecc_at_min_version = 0;
        while(max_mode >= min_mode &&
              !find_min_version_max_ecc(
                  len, (uint8_t)max_mode, &min_version, &max_ecc_at_min_version)) {
            max_mode--;
        }

        // if the max is less than the min, the message is too long
        if(max_mode < min_mode) {
            instance->too_long = true;
            break;
        }

        // pick a mode based on the min/max and desired mode
        if(desired_mode == 255 || desired_mode < (uint8_t)min_mode) {
            desired_mode = (uint8_t)min_mode;
        } else if(desired_mode > (uint8_t)max_mode) {
            desired_mode = (uint8_t)max_mode;
        }
        if(desired_mode != (uint8_t)max_mode) {
            // if the desired mode equals the max mode, then min_version and
            // max_ecc_at_min_version are already set appropriately by the max
            // mode loop above... otherwise, we need to calculate them... this
            // should always return true because we already know the desired
            // mode is appropriate for the data, but, just in case...
            if(!find_min_version_max_ecc(
                   len, desired_mode, &min_version, &max_ecc_at_min_version)) {
                instance->too_long = true;
                break;
            }
        }

        // ensure desired version and ecc are appropriate
        if(desired_version == 255 || desired_version < min_version) {
            desired_version = min_version;
        } else if(desired_version > MAX_QRCODE_VERSION) {
            desired_version = MAX_QRCODE_VERSION;
        }
        if(desired_version == min_version) {
            if(desired_ecc > max_ecc_at_min_version) {
                desired_ecc = max_ecc_at_min_version;
            }
        } else if(desired_ecc > MAX_QRCODE_ECC) {
            desired_ecc = MAX_QRCODE_ECC;
        }

        // Build the qrcode
        if(!rebuild_qrcode(instance, desired_mode, desired_version, desired_ecc)) {
            break;
        }

        instance->min_mode = (uint8_t)min_mode;
        instance->max_mode = (uint8_t)max_mode;
        instance->set_mode = desired_mode;
        instance->min_version = min_version;
        instance->set_version = desired_version;
        instance->max_ecc_at_min_version = max_ecc_at_min_version;
        instance->set_ecc = desired_ecc;
        result = true;
    } while(false);

    if(!result) {
        if(instance->message) {
            furi_string_free(instance->message);
            instance->message = NULL;
        }
        if(instance->qrcode) {
            qrcode_free(instance->qrcode);
            instance->qrcode = NULL;
        }
    }

    instance->loading = false;

    furi_mutex_release(instance->mutex);

    return result;
}

/**
 * Load a qrcode from a file
 * @param instance The qrcode app instance
 * @param file_path Path to the file to read
 * @returns true if the file was successfully loaded
 */
static bool qrcode_load_file(QRCodeApp* instance, const char* file_path) {
    furi_assert(instance);
    furi_assert(file_path);

    FuriString* temp_str = furi_string_alloc();
    bool result = false;

    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* file = flipper_format_file_alloc(storage);

    do {
        if(!flipper_format_file_open_existing(file, file_path)) break;

        uint32_t file_version = 0;
        if(!flipper_format_read_header(file, temp_str, &file_version)) break;
        if(furi_string_cmp_str(temp_str, QRCODE_FILETYPE) || file_version > QRCODE_FILE_VERSION) {
            FURI_LOG_E(TAG, "Incorrect file format or version");
            break;
        }

        uint32_t desired_mode = 255;
        uint32_t desired_version = 255;
        uint32_t desired_ecc = 255;
        if(file_version > 0) {
            if(flipper_format_key_exist(file, "QRMode")) {
                if(flipper_format_read_string(file, "QRMode", temp_str)) {
                    if(furi_string_size(temp_str) > 0) {
                        desired_mode = get_mode_value(furi_string_get_char(temp_str, 0));
                    }
                } else {
                    FURI_LOG_E(TAG, "Could not read QRMode");
                    desired_mode = 255;
                }
            }

            if(flipper_format_key_exist(file, "QRVersion")) {
                if(flipper_format_read_uint32(file, "QRVersion", &desired_version, 1)) {
                    if(desired_version > MAX_QRCODE_VERSION) {
                        FURI_LOG_E(TAG, "Invalid QRVersion");
                        desired_version = 255;
                    }
                } else {
                    FURI_LOG_E(TAG, "Could not read QRVersion");
                    desired_version = 255;
                }
            }

            if(flipper_format_key_exist(file, "QRECC")) {
                if(flipper_format_read_string(file, "QRECC", temp_str)) {
                    if(furi_string_size(temp_str) > 0) {
                        desired_ecc = get_ecc_value(furi_string_get_char(temp_str, 0));
                    }
                } else {
                    FURI_LOG_E(TAG, "Could not read QRECC");
                    desired_ecc = 255;
                }
            }
        }

        if(!flipper_format_read_string(file, "Message", temp_str)) {
            FURI_LOG_E(TAG, "Message is missing");
            break;
        }
        if(file_version > 0) {
            FuriString* msg_cont = furi_string_alloc();
            while(flipper_format_key_exist(file, "Message")) {
                if(!flipper_format_read_string(file, "Message", msg_cont)) {
                    FURI_LOG_E(TAG, "Could not read next Message");
                    break;
                }
                furi_string_push_back(temp_str, '\n');
                furi_string_cat(temp_str, msg_cont);
            }
            furi_string_free(msg_cont);
        }

        if(!qrcode_load_string(
               instance,
               temp_str,
               (uint8_t)desired_mode,
               (uint8_t)desired_version,
               (uint8_t)desired_ecc)) {
            break;
        }

        result = true;
    } while(false);

    furi_record_close(RECORD_STORAGE);
    flipper_format_free(file);
    furi_string_free(temp_str);

    return result;
}

/**
 * Allocate the qrcode app
 * @returns a qrcode app instance
 */
static QRCodeApp* qrcode_app_alloc() {
    QRCodeApp* instance = malloc(sizeof(QRCodeApp));

    instance->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    instance->view_port = view_port_alloc();
    view_port_draw_callback_set(instance->view_port, render_callback, instance);
    view_port_input_callback_set(instance->view_port, input_callback, instance);

    instance->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(instance->gui, instance->view_port, GuiLayerFullscreen);

    instance->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    instance->message = NULL;
    instance->qrcode = NULL;
    instance->loading = true;
    instance->too_long = false;
    instance->show_stats = false;
    instance->selected_idx = 0;
    instance->edit = false;

    return instance;
}

/**
 * Free the qrcode app
 * @param qrcode_app The app to free
 */
static void qrcode_app_free(QRCodeApp* instance) {
    if(instance->message) furi_string_free(instance->message);
    if(instance->qrcode) qrcode_free(instance->qrcode);

    gui_remove_view_port(instance->gui, instance->view_port);
    furi_record_close(RECORD_GUI);

    view_port_free(instance->view_port);

    furi_message_queue_free(instance->input_queue);

    furi_mutex_free(instance->mutex);

    free(instance);
}

/** App entrypoint */
int32_t qrcode_app(void* p) {
    QRCodeApp* instance = qrcode_app_alloc();
    FuriString* file_path = furi_string_alloc();

    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_common_migrate(storage, EXT_PATH("qrcodes"), QRCODE_FOLDER);
    furi_record_close(RECORD_STORAGE);

    do {
        if(p && strlen(p)) {
            furi_string_set(file_path, (const char*)p);
        } else {
            furi_string_set(file_path, QRCODE_FOLDER);

            DialogsFileBrowserOptions browser_options;
            dialog_file_browser_set_basic_options(
                &browser_options, QRCODE_EXTENSION, &I_qrcode_10px);
            browser_options.hide_ext = true;
            browser_options.base_path = QRCODE_FOLDER;

            DialogsApp* dialogs = furi_record_open(RECORD_DIALOGS);
            bool res = dialog_file_browser_show(dialogs, file_path, file_path, &browser_options);

            furi_record_close(RECORD_DIALOGS);
            if(!res) {
                FURI_LOG_E(TAG, "No file selected");
                break;
            }
        }

        if(!qrcode_load_file(instance, furi_string_get_cstr(file_path))) {
            FURI_LOG_E(TAG, "Unable to load file");
        }

        InputEvent input;
        while(1) {
            if(furi_message_queue_get(instance->input_queue, &input, 100) == FuriStatusOk) {
                furi_check(furi_mutex_acquire(instance->mutex, FuriWaitForever) == FuriStatusOk);

                if(input.key == InputKeyBack) {
                    if(instance->message) {
                        furi_string_free(instance->message);
                        instance->message = NULL;
                    }
                    if(instance->qrcode) {
                        qrcode_free(instance->qrcode);
                        instance->qrcode = NULL;
                    }
                    instance->loading = true;
                    instance->edit = false;
                    furi_mutex_release(instance->mutex);
                    break;
                } else if(input.key == InputKeyRight) {
                    instance->show_stats = true;
                } else if(input.key == InputKeyLeft) {
                    instance->show_stats = false;
                } else if(instance->show_stats && !instance->loading && instance->qrcode) {
                    if(input.key == InputKeyUp) {
                        if(!instance->edit) {
                            instance->selected_idx = MAX(0, instance->selected_idx - 1);
                        } else {
                            if(instance->selected_idx == 0 &&
                               instance->set_mode < instance->max_mode) {
                                instance->set_mode++;
                            } else if(
                                instance->selected_idx == 1 &&
                                instance->set_version < MAX_QRCODE_VERSION) {
                                instance->set_version++;
                            } else if(instance->selected_idx == 2) {
                                uint8_t max_ecc = instance->set_version == instance->min_version ?
                                                      instance->max_ecc_at_min_version :
                                                      ECC_HIGH;
                                if(instance->set_ecc < max_ecc) {
                                    instance->set_ecc++;
                                }
                            }
                        }
                    } else if(input.key == InputKeyDown) {
                        if(!instance->edit) {
                            instance->selected_idx = MIN(2, instance->selected_idx + 1);
                        } else {
                            if(instance->selected_idx == 0 &&
                               instance->set_mode > instance->min_mode) {
                                instance->set_mode--;
                            } else if(
                                instance->selected_idx == 1 &&
                                instance->set_version > instance->min_version) {
                                instance->set_version--;
                                if(instance->set_version == instance->min_version) {
                                    instance->set_ecc =
                                        MIN(instance->set_ecc, instance->max_ecc_at_min_version);
                                }
                            } else if(instance->selected_idx == 2 && instance->set_ecc > 0) {
                                instance->set_ecc--;
                            }
                        }
                    } else if(input.key == InputKeyOk) {
                        if(instance->edit && (instance->set_mode != instance->qrcode->mode ||
                                              instance->set_version != instance->qrcode->version ||
                                              instance->set_ecc != instance->qrcode->ecc)) {
                            uint8_t orig_min_version = instance->min_version;
                            uint8_t orig_max_ecc_at_min_version = instance->max_ecc_at_min_version;
                            if(instance->set_mode != instance->qrcode->mode) {
                                uint16_t len = (uint16_t)furi_string_size(instance->message);
                                uint8_t min_version = 0;
                                uint8_t max_ecc_at_min_version = 0;
                                if(find_min_version_max_ecc(
                                       len,
                                       instance->set_mode,
                                       &min_version,
                                       &max_ecc_at_min_version)) {
                                    if(instance->set_version < min_version) {
                                        instance->set_version = min_version;
                                    }
                                    if(instance->set_version == min_version &&
                                       instance->set_ecc > max_ecc_at_min_version) {
                                        instance->set_ecc = max_ecc_at_min_version;
                                    }
                                    instance->min_version = min_version;
                                    instance->max_ecc_at_min_version = max_ecc_at_min_version;
                                } else {
                                    instance->set_mode = instance->qrcode->mode;
                                }
                            }

                            QRCode* qrcode = instance->qrcode;
                            instance->loading = true;

                            if(rebuild_qrcode(
                                   instance,
                                   instance->set_mode,
                                   instance->set_version,
                                   instance->set_ecc)) {
                                qrcode_free(qrcode);
                            } else {
                                FURI_LOG_E(TAG, "Could not rebuild qrcode");
                                instance->qrcode = qrcode;
                                instance->set_mode = qrcode->mode;
                                instance->set_version = qrcode->version;
                                instance->set_ecc = qrcode->ecc;
                                instance->min_version = orig_min_version;
                                instance->max_ecc_at_min_version = orig_max_ecc_at_min_version;
                            }

                            instance->loading = false;
                        }
                        instance->edit = !instance->edit;
                    }
                }

                furi_mutex_release(instance->mutex);
            }
            view_port_update(instance->view_port);
        }

        if(p && strlen(p)) {
            // if started with an arg, exit instead
            // of looping back to the browser
            break;
        }
    } while(true);

    furi_string_free(file_path);
    qrcode_app_free(instance);

    return 0;
}
