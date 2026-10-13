#include "goat-source.h"
#include <util/platform.h>

struct goat_state {
    float duration;
    float elapsed;
};

static const char *goat_get_name(void *unused)
{
    UNUSED_PARAMETER(unused);
    return "GOAT.exe — тестовый источник";
}

static void *goat_create(obs_data_t *settings, obs_source_t *source)
{
    UNUSED_PARAMETER(source);
    struct goat_state *state = bzalloc(sizeof(*state));
    state->duration = (float)obs_data_get_double(settings, "duration");
    if (state->duration < 1.0f)
        state->duration = 5.0f;
    state->elapsed = 0.0f;
    return state;
}

static void goat_destroy(void *data)
{
    bfree(data);
}

static void goat_update(void *data, obs_data_t *settings)
{
    struct goat_state *state = data;
    if (!state)
        return;

    state->duration = (float)obs_data_get_double(settings, "duration");
    if (state->duration < 1.0f)
        state->duration = 5.0f;
}

static void goat_defaults(obs_data_t *settings)
{
    obs_data_set_default_double(settings, "duration", 5.0);
}

static obs_properties_t *goat_properties(void *data)
{
    UNUSED_PARAMETER(data);
    obs_properties_t *props = obs_properties_create();
    obs_properties_add_float_slider(props, "duration",
                                    "Длительность тестового события (сек.)",
                                    1.0, 30.0, 1.0);
    return props;
}

static uint32_t goat_width(void *data)
{
    UNUSED_PARAMETER(data);
    return 512;
}

static uint32_t goat_height(void *data)
{
    UNUSED_PARAMETER(data);
    return 512;
}

static void goat_tick(void *data, float seconds)
{
    struct goat_state *state = data;
    if (!state)
        return;
    state->elapsed += seconds;
}

/* Rendering and Twitch command handling are intentionally the next stage. */
static void goat_render(void *data, gs_effect_t *effect)
{
    UNUSED_PARAMETER(data);
    UNUSED_PARAMETER(effect);
}

static struct obs_source_info goat_source_info = {
    .id = "goat_exe_pet_source",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = goat_get_name,
    .create = goat_create,
    .destroy = goat_destroy,
    .update = goat_update,
    .get_defaults = goat_defaults,
    .get_properties = goat_properties,
    .get_width = goat_width,
    .get_height = goat_height,
    .video_tick = goat_tick,
    .video_render = goat_render,
};

void goat_source_register(void)
{
    obs_register_source(&goat_source_info);
}
