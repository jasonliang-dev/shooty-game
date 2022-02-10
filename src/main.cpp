#define _CRT_SECURE_NO_WARNINGS
#define LUA_LIB

#include "app.h"

#define CUTE_TILED_IMPLEMENTATION
#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#include "deps/cute_tiled.h"

#define MAKE_LIB
#include "deps/lua/onelua.c"

#define MINIAUDIO_IMPLEMENTATION
#include "deps/miniaudio.h"

#define SOKOL_IMPL
#define SOKOL_WIN32_FORCE_MAIN
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_time.h"

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "deps/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "deps/stb_truetype.h"

sapp_desc sokol_main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    return {
        .init_cb = app_init,
        .frame_cb = app_frame,
        .cleanup_cb = app_cleanup,
        .event_cb = app_event,
        .width = 1366,
        .height = 768,
        .sample_count = 4,
        .window_title = "This is a title",
        .icon = {.sokol_default = true},
        .win32_console_attach = true,
    };
}
