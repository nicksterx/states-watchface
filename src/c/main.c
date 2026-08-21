/*
 * States Watchface for Pebble Time
 * src/c/main.c  — v1.3
 *
 * Layout (144x168px shown; widths/heights come from the window bounds):
 *   [time]           42px  — Bitham 42 Bold
 *   [divider]         1px
 *   [title]                — Gothic 24/28 Bold
 *   [subtitle]             — Gothic 14/18 (optional, hideable)
 *   [fact]           rest  — Gothic 14/18/24 Bold (wraps)
 *
 * Settings (phone-side page, persisted on the watch):
 *   - Text size: Normal / Large / Extra Large (see TEXT_SIZES)
 *   - Show capital & year line
 *   - Background color, fact color
 * Shake/flick the watch to cycle to another fact for the current minute.
 */

#include <pebble.h>
#include "states_data.h"

// ---------------------------------------------------------------------------
// Settings
// ---------------------------------------------------------------------------
// AppMessage keys — must match appKeys in appinfo.json.
#define MSG_KEY_TEXT_SIZE     0
#define MSG_KEY_SHOW_SUBTITLE 1
#define MSG_KEY_BG_COLOR      2
#define MSG_KEY_FACT_COLOR    3

// Persistent storage keys.
#define PERSIST_KEY_TEXT_SIZE     1
#define PERSIST_KEY_SHOW_SUBTITLE 2
#define PERSIST_KEY_BG_COLOR      3
#define PERSIST_KEY_FACT_COLOR    4

#define DEFAULT_BG_COLOR   0x000000
#define DEFAULT_FACT_COLOR 0xFFFF00

enum {
  TEXT_SIZE_NORMAL = 0,
  TEXT_SIZE_LARGE  = 1,
  TEXT_SIZE_XLARGE = 2,
};

// Emery (Pebble Time 2) packs more pixels into a similar physical area, so
// the same fonts render smaller — default to Large there.
#if defined(PBL_PLATFORM_EMERY)
  #define TEXT_SIZE_DEFAULT TEXT_SIZE_LARGE
#else
  #define TEXT_SIZE_DEFAULT TEXT_SIZE_NORMAL
#endif

typedef struct {
  const char *title_font;
  int16_t     title_h;
  const char *subtitle_font;
  int16_t     subtitle_h;
  const char *fact_font;
} TextSizeSpec;

static const TextSizeSpec TEXT_SIZES[] = {
  [TEXT_SIZE_NORMAL] = { FONT_KEY_GOTHIC_24_BOLD, 24,
                         FONT_KEY_GOTHIC_14,      15,
                         FONT_KEY_GOTHIC_14_BOLD },
  [TEXT_SIZE_LARGE]  = { FONT_KEY_GOTHIC_28_BOLD, 30,
                         FONT_KEY_GOTHIC_18,      20,
                         FONT_KEY_GOTHIC_18_BOLD },
  [TEXT_SIZE_XLARGE] = { FONT_KEY_GOTHIC_28_BOLD, 30,
                         FONT_KEY_GOTHIC_18,      20,
                         FONT_KEY_GOTHIC_24_BOLD },
};

static int     s_text_size      = TEXT_SIZE_DEFAULT;
static bool    s_show_subtitle  = true;
static int32_t s_bg_color_hex   = DEFAULT_BG_COLOR;
static int32_t s_fact_color_hex = DEFAULT_FACT_COLOR;

static int prv_clamp_text_size(int size) {
  return (size >= TEXT_SIZE_NORMAL && size <= TEXT_SIZE_XLARGE)
             ? size
             : TEXT_SIZE_DEFAULT;
}

static bool prv_color_is_light(int32_t hex) {
  int r = (hex >> 16) & 0xFF;
  int g = (hex >> 8) & 0xFF;
  int b = hex & 0xFF;
  return (r * 299 + g * 587 + b * 114) / 1000 >= 128;
}

// ---------------------------------------------------------------------------
// Layers & state
// ---------------------------------------------------------------------------
static Window      *s_window;
static TextLayer   *s_time_layer;
static Layer       *s_divider_layer;
static TextLayer   *s_title_layer;
static TextLayer   *s_subtitle_layer;
static TextLayer   *s_fact_layer;

static char s_time_buf[8];
static char s_fact_buf[80];

// Shake/flick advances this to show another fact for the current minute.
static int s_fact_offset = 0;

// ---------------------------------------------------------------------------
// Divider
// ---------------------------------------------------------------------------
static void prv_divider_draw(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_stroke_color(ctx, GColorDarkGray);
  graphics_draw_line(ctx, GPoint(0, 0), GPoint(b.size.w, 0));
}

// ---------------------------------------------------------------------------
// Layout — positions layers and picks fonts for the current settings.
// Safe to call again when a setting changes.
// ---------------------------------------------------------------------------
static void prv_apply_layout(void) {
  if (!s_window || !s_time_layer) {
    return;
  }

  Layer  *root   = window_get_root_layer(s_window);
  GRect   bounds = layer_get_bounds(root);
  int16_t w      = bounds.size.w;
  int16_t y      = 2;

  const TextSizeSpec *spec = &TEXT_SIZES[prv_clamp_text_size(s_text_size)];

  // ---- Time ----
  layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0, y, w, 42));
  y += 44;

  // ---- Divider ----
  layer_set_frame(s_divider_layer, GRect(8, y, w - 16, 1));
  y += 4;

  // ---- Title ----
  text_layer_set_font(s_title_layer, fonts_get_system_font(spec->title_font));
  layer_set_frame(text_layer_get_layer(s_title_layer),
                  GRect(2, y, w - 4, spec->title_h));
  y += spec->title_h;

  // ---- Subtitle (optional) ----
  layer_set_hidden(text_layer_get_layer(s_subtitle_layer), !s_show_subtitle);
  if (s_show_subtitle) {
    text_layer_set_font(s_subtitle_layer,
                        fonts_get_system_font(spec->subtitle_font));
    layer_set_frame(text_layer_get_layer(s_subtitle_layer),
                    GRect(2, y, w - 4, spec->subtitle_h - 1));
    y += spec->subtitle_h;
  }

  // ---- Fact (remaining space, wraps) ----
  text_layer_set_font(s_fact_layer, fonts_get_system_font(spec->fact_font));
  layer_set_frame(text_layer_get_layer(s_fact_layer),
                  GRect(2, y, w - 4, bounds.size.h - y - 1));
}

// ---------------------------------------------------------------------------
// Colors — applies background/fact colors; time/title/subtitle flip between
// light-on-dark and dark-on-light based on the background's luminance.
// ---------------------------------------------------------------------------
static void prv_apply_colors(void) {
  if (!s_window || !s_time_layer) {
    return;
  }

  bool   light_bg = prv_color_is_light(s_bg_color_hex);
  GColor primary  = light_bg ? GColorBlack : GColorWhite;
  GColor muted    = light_bg ? GColorDarkGray : GColorLightGray;

  window_set_background_color(s_window, GColorFromHEX(s_bg_color_hex));
  text_layer_set_text_color(s_time_layer, primary);
  text_layer_set_text_color(s_title_layer, primary);
  text_layer_set_text_color(s_subtitle_layer, muted);
  text_layer_set_text_color(s_fact_layer, GColorFromHEX(s_fact_color_hex));
  layer_mark_dirty(s_divider_layer);
}

// ---------------------------------------------------------------------------
// Display update
// ---------------------------------------------------------------------------
static void prv_update_display(struct tm *tick_time) {
  int minute = tick_time->tm_min;
  int yday   = tick_time->tm_yday;

  // Time
  clock_copy_time_string(s_time_buf, sizeof(s_time_buf));
  text_layer_set_text(s_time_layer, s_time_buf);

  // Text content
  const MinuteEntry *entry = &MINUTE_DATA[minute];
  text_layer_set_text(s_title_layer,    entry->title);
  text_layer_set_text(s_subtitle_layer, entry->subtitle);

  uint8_t fact_index = (yday + s_fact_offset) % entry->fact_count;
  snprintf(s_fact_buf, sizeof(s_fact_buf), "%s", entry->facts[fact_index]);
  text_layer_set_text(s_fact_layer, s_fact_buf);
}

// ---------------------------------------------------------------------------
// Tick handler
// ---------------------------------------------------------------------------
static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  prv_update_display(tick_time);
}

// ---------------------------------------------------------------------------
// Shake/flick — show another fact for the current minute
// ---------------------------------------------------------------------------
static void prv_tap_handler(AccelAxisType axis, int32_t direction) {
  s_fact_offset++;
  time_t now = time(NULL);
  prv_update_display(localtime(&now));
}

// ---------------------------------------------------------------------------
// AppMessage — settings from the phone
// ---------------------------------------------------------------------------
static void prv_inbox_received(DictionaryIterator *iter, void *context) {
  Tuple *t;

  if ((t = dict_find(iter, MSG_KEY_TEXT_SIZE))) {
    s_text_size = prv_clamp_text_size(t->value->int32);
    persist_write_int(PERSIST_KEY_TEXT_SIZE, s_text_size);
  }
  if ((t = dict_find(iter, MSG_KEY_SHOW_SUBTITLE))) {
    s_show_subtitle = (t->value->int32 != 0);
    persist_write_bool(PERSIST_KEY_SHOW_SUBTITLE, s_show_subtitle);
  }
  if ((t = dict_find(iter, MSG_KEY_BG_COLOR))) {
    s_bg_color_hex = t->value->int32;
    persist_write_int(PERSIST_KEY_BG_COLOR, s_bg_color_hex);
  }
  if ((t = dict_find(iter, MSG_KEY_FACT_COLOR))) {
    s_fact_color_hex = t->value->int32;
    persist_write_int(PERSIST_KEY_FACT_COLOR, s_fact_color_hex);
  }

  prv_apply_layout();
  prv_apply_colors();
}

// ---------------------------------------------------------------------------
// Window load
// ---------------------------------------------------------------------------
static void prv_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);

  // ---- Time ----
  s_time_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_time_layer));

  // ---- Divider ----
  s_divider_layer = layer_create(GRectZero);
  layer_set_update_proc(s_divider_layer, prv_divider_draw);
  layer_add_child(root, s_divider_layer);

  // ---- Title ----
  s_title_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_title_layer, GColorClear);
  text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_title_layer));

  // ---- Subtitle ----
  s_subtitle_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_subtitle_layer, GColorClear);
  text_layer_set_text_alignment(s_subtitle_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_subtitle_layer));

  // ---- Fact ----
  s_fact_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_fact_layer, GColorClear);
  text_layer_set_text_alignment(s_fact_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_fact_layer, GTextOverflowModeWordWrap);
  layer_add_child(root, text_layer_get_layer(s_fact_layer));

  // ---- Frames, fonts & colors for the current settings ----
  prv_apply_layout();
  prv_apply_colors();

  // ---- Initial render ----
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  prv_update_display(t);
}

// ---------------------------------------------------------------------------
// Window unload
// ---------------------------------------------------------------------------
static void prv_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  layer_destroy(s_divider_layer);
  text_layer_destroy(s_title_layer);
  text_layer_destroy(s_subtitle_layer);
  text_layer_destroy(s_fact_layer);
  s_time_layer = NULL;
}

// ---------------------------------------------------------------------------
// Init / deinit
// ---------------------------------------------------------------------------
static void prv_load_settings(void) {
  if (persist_exists(PERSIST_KEY_TEXT_SIZE)) {
    s_text_size = prv_clamp_text_size(persist_read_int(PERSIST_KEY_TEXT_SIZE));
  }
  if (persist_exists(PERSIST_KEY_SHOW_SUBTITLE)) {
    s_show_subtitle = persist_read_bool(PERSIST_KEY_SHOW_SUBTITLE);
  }
  if (persist_exists(PERSIST_KEY_BG_COLOR)) {
    s_bg_color_hex = persist_read_int(PERSIST_KEY_BG_COLOR);
  }
  if (persist_exists(PERSIST_KEY_FACT_COLOR)) {
    s_fact_color_hex = persist_read_int(PERSIST_KEY_FACT_COLOR);
  }
}

static void prv_init(void) {
  prv_load_settings();

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
  accel_tap_service_subscribe(prv_tap_handler);

  app_message_register_inbox_received(prv_inbox_received);
  app_message_open(64, 16);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
  app_message_deregister_callbacks();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
  return 0;
}
