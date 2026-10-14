
#include <obs-module.h>
#include <graphics/image-file.h>
#include <util/platform.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

struct goat_state {
    gs_image_file_t image;
    uint32_t width;
    uint32_t height;
};

static const char *goat_get_name(void *unused)
{
    UNUSED_PARAMETER(unused);
    return "GOAT.exe";
}

static void *goat_create(obs_data_t *settings, obs_source_t *source)
{
    UNUSED_PARAMETER(settings);
    UNUSED_PARAMETER(source);

    struct goat_state *state = bzalloc(sizeof(*state));

    char *path = obs_module_file("goat_idle.png");
    if (!path)
        return state;

    gs_image_file_init(&state->image, path);
    bfree(path);

    if (!state->image.loaded) {
        blog(LOG_ERROR, "[GOAT.exe] Не удалось загрузить goat_idle.png");
        return state;
    }

    state->width = state->image.cx;
    state->height = state->image.cy;

    obs_enter_graphics();
    gs_image_file_init_texture(&state->image);
    obs_leave_graphics();

    blog(LOG_INFO, "[GOAT.exe] Изображение загружено: %ux%u",
         state->width, state->height);

    return state;
}

static void goat_destroy(void *data)
{
    struct goat_state *state = data;
    if (!state)
        return;

    obs_enter_graphics();
    gs_image_file_free(&state->image);
    obs_leave_graphics();

    bfree(state);
}

static uint32_t goat_width(void *data)
{
    struct goat_state *state = data;
    return state && state->width ? state->width : 512;
}

static uint32_t goat_height(void *data)
{
    struct goat_state *state = data;
    return state && state->height ? state->height : 512;
}

static void goat_render(void *data, gs_effect_t *effect)
{
    struct goat_state *state = data;

    if (!state || !state->image.loaded || !state->image.texture)
        return;

    gs_eparam_t *image =
        gs_effect_get_param_by_name(effect, "image");

    if (!image)
        return;

    gs_effect_set_texture(image, state->image.texture);

    while (gs_effect_loop(effect, "Draw")) {
        gs_draw_sprite(state->image.texture, 0,
                       state->width, state->height);
    }
}

static struct obs_source_info goat_source_info = {
    .id = "goat_exe_pet_source",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = goat_get_name,
    .create = goat_create,
    .destroy = goat_destroy,
    .get_width = goat_width,
    .get_height = goat_height,
    .video_render = goat_render,
};

void goat_source_register(void)
{
    obs_register_source(&goat_source_info);
}

bool obs_module_load(void)
{
    goat_source_register();
    blog(LOG_INFO, "[GOAT.exe] Модуль загружен");
    return true;
}
