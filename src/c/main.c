/*
 * States Watchface for Pebble Time
 * src/c/main.c  — v1.1 with state shape bitmaps
 *
 * Layout (144x168px):
 *   [time]           42px  — Bitham 42 Bold, white
 *   [divider]         1px
 *   [title]          24px  — Gothic 24 Bold, white
 *   [subtitle]       16px  — Gothic 14, gray
 *   [fact]           ~15px — Gothic 14 Bold, yellow (single line)
 */

#include <pebble.h>
#include "states_data.h"

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
// Display update
// ---------------------------------------------------------------------------
static void prv_update_display(struct tm *tick_time) {
  int minute = tick_time->tm_min;
  int yday   = tick_time->tm_yday;

  // Time
  clock_copy_time_string(s_time_buf, sizeof(s_time_buf));
  text_layer_set_text(s_time_layer, s_time_buf);

  // Minute label removed

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
// Window load
// ---------------------------------------------------------------------------
static void prv_window_load(Window *window) {
  Layer  *root   = window_get_root_layer(window);
  GRect   bounds = layer_get_bounds(root);
  int16_t w      = bounds.size.w;  // 144
  int16_t y      = 2;

  window_set_background_color(window, GColorBlack);

  // ---- Time ----
  s_time_layer = text_layer_create(GRect(0, y, w, 42));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_time_layer));
  y += 44;

  // ---- Divider ----
  s_divider_layer = layer_create(GRect(8, y, w - 16, 1));
  layer_set_update_proc(s_divider_layer, prv_divider_draw);
  layer_add_child(root, s_divider_layer);
  y += 4;

  // ---- Title ----
  s_title_layer = text_layer_create(GRect(2, y, w - 4, 24));
  text_layer_set_background_color(s_title_layer, GColorClear);
  text_layer_set_text_color(s_title_layer, GColorWhite);
  text_layer_set_font(s_title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_title_layer));
  y += 24;

  // ---- Subtitle ----
  s_subtitle_layer = text_layer_create(GRect(2, y, w - 4, 14));
  text_layer_set_background_color(s_subtitle_layer, GColorClear);
  text_layer_set_text_color(s_subtitle_layer, GColorLightGray);
  text_layer_set_font(s_subtitle_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_subtitle_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_subtitle_layer));
  y += 15;

  // ---- Fact (remaining space, wraps) ----
  s_fact_layer = text_layer_create(GRect(2, y, w - 4, bounds.size.h - y - 1));
  text_layer_set_background_color(s_fact_layer, GColorClear);
  text_layer_set_text_color(s_fact_layer, GColorYellow);
  text_layer_set_font(s_fact_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_fact_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_fact_layer, GTextOverflowModeWordWrap);
  layer_add_child(root, text_layer_get_layer(s_fact_layer));

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
}

// ---------------------------------------------------------------------------
// Init / deinit
// ---------------------------------------------------------------------------
static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load   = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
  return 0;
}
