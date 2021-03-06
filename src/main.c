#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_date_layer;
static TextLayer *s_time_layer;
static TextLayer *s_dota_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "Wednesday";
  static char buffer2[] = "00:00";
  static char buffer3[] = "Dota O'Clock";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("Wednesday"), "%a %d", tick_time);
    strftime(buffer2, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("Wednesday"), "%a %d", tick_time);
    strftime(buffer2, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this date on the TextLayer
  text_layer_set_text(s_date_layer, buffer);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer2);
 
  if(tick_time->tm_hour == 13 && tick_time->tm_min > 0 && tick_time->tm_min <= 10) {
    text_layer_set_background_color(s_dota_layer, GColorBlack);
  } else {
    text_layer_set_background_color(s_dota_layer, GColorWhite);
  }
  
  text_layer_set_text(s_dota_layer, buffer3);

}

static void main_window_load(Window *window) {
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 20, 144, 50));
  text_layer_set_background_color(s_date_layer, GColorWhite);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "Wed 20");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter); 
  
  // DOTA Layer
  s_dota_layer = text_layer_create(GRect(0, 105, 144, 65));
  text_layer_set_background_color(s_dota_layer, GColorBlack);
  text_layer_set_text_color(s_dota_layer, GColorWhite);
  text_layer_set_text(s_dota_layer, "Dota O'Clock");
  text_layer_set_font(s_dota_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_dota_layer, GTextAlignmentCenter); 

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dota_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_dota_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

