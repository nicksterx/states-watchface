/*
 * States Watchface for Pebble Time
 * src/c/main.c  — v1.2 with configurable text size
 *
 * Layout (144x168px shown; widths/heights come from the window bounds):
 *   [time]           42px  — Bitham 42 Bold, white
 *   [divider]         1px
 *   [title]                — Gothic 24/28 Bold, white
 *   [subtitle]             — Gothic 14/18, gray
 *   [fact]           rest  — Gothic 14/18/24 Bold, yellow (wraps)
 *
 * Text size (Normal/Large/Extra Large) is set from the phone-side settings
 * page and persisted on the watch; see TEXT_SIZES for the font table.
 */

#include <pebble.h>
#include "states_data.h"

// ---------------------------------------------------------------------------
// Settings
// ---------------------------------------------------------------------------
// AppMessage key — must match "TextSize" in appinfo.json appKeys.
#define MSG_KEY_TEXT_SIZE 0
// Persistent storage key.
#define PERSIST_KEY_TEXT_SIZE 1

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

static int s_text_size = TEXT_SIZE_DEFAULT;

static int prv_clamp_text_size(int size) {
  return (size >= TEXT_SIZE_NORMAL && size <= TEXT_SIZE_XLARGE)
             ? size
             : TEXT_SIZE_DEFAULT;
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

// ---------------------------------------------------------------------------
// Divider
// ---------------------------------------------------------------------------
static void prv_divider_draw(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_stroke_color(ctx, GColorDarkGray);
  graphics_draw_line(ctx, GPoint(0, 0), GPoint(b.size.w, 0));
}

// ---------------------------------------------------------------------------
// Layout — positions layers and picks fonts for the current text size.
// Safe to call again when the setting changes.
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

  // ---- Subtitle ----
  text_layer_set_font(s_subtitle_layer, fonts_get_system_font(spec->subtitle_font));
  layer_set_frame(text_layer_get_layer(s_subtitle_layer),
                  GRect(2, y, w - 4, spec->subtitle_h - 1));
  y += spec->subtitle_h;

  // ---- Fact (remaining space, wraps) ----
  text_layer_set_font(s_fact_layer, fonts_get_system_font(spec->fact_font));
  layer_set_frame(text_layer_get_layer(s_fact_layer),
                  GRect(2, y, w - 4, bounds.size.h - y - 1));
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

  uint8_t fact_index = yday % entry->fact_count;
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
// AppMessage — settings from the phone
// ---------------------------------------------------------------------------
static void prv_inbox_received(DictionaryIterator *iter, void *context) {
  Tuple *text_size_tuple = dict_find(iter, MSG_KEY_TEXT_SIZE);
  if (text_size_tuple) {
    s_text_size = prv_clamp_text_size(text_size_tuple->value->int32);
    persist_write_int(PERSIST_KEY_TEXT_SIZE, s_text_size);
    prv_apply_layout();
  }
}

// ---------------------------------------------------------------------------
// Window load
// ---------------------------------------------------------------------------
static void prv_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);

  window_set_background_color(window, GColorBlack);

  // ---- Time ----
  s_time_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
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
  text_layer_set_text_color(s_title_layer, GColorWhite);
  text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_title_layer));

  // ---- Subtitle ----
  s_subtitle_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_subtitle_layer, GColorClear);
  text_layer_set_text_color(s_subtitle_layer, GColorLightGray);
  text_layer_set_text_alignment(s_subtitle_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_subtitle_layer));

  // ---- Fact ----
  s_fact_layer = text_layer_create(GRectZero);
  text_layer_set_background_color(s_fact_layer, GColorClear);
  text_layer_set_text_color(s_fact_layer, GColorYellow);
  text_layer_set_text_alignment(s_fact_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_fact_layer, GTextOverflowModeWordWrap);
  layer_add_child(root, text_layer_get_layer(s_fact_layer));

  // ---- Frames & fonts for the current text size ----
  prv_apply_layout();

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
static void prv_init(void) {
  s_text_size = persist_exists(PERSIST_KEY_TEXT_SIZE)
                    ? prv_clamp_text_size(persist_read_int(PERSIST_KEY_TEXT_SIZE))
                    : TEXT_SIZE_DEFAULT;

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);

  app_message_register_inbox_received(prv_inbox_received);
  app_message_open(64, 16);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
  return 0;
}
