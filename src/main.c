#include <pebble.h>

Window *window;
TextLayer *text_layer;
char buffer[] = "00:00";
const char * string_list[] = {
        "foo",
        "bar",
        "baz"
};
unsigned int exercise = 0;

void update_exercise_name(const char *name){
    text_layer_set_text(text_layer, name);
}

void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  //do nothing if we're on the first exercise
  if (exercise < 1) return;
  exercise = exercise - 1;
  update_exercise_name(string_list[exercise]);
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  //do nothing if we're on the last exercise
  if (exercise >= sizeof(string_list)) return;
  exercise = exercise + 1;
  update_exercise_name(string_list[exercise]);
}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
 text_layer_set_text(text_layer, "That doesn't do anything.");
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void window_load(Window *window)
{
  text_layer = text_layer_create(GRect(0, 0, 132, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);

  text_layer_set_text(text_layer, "Press A Button");
}

void window_unload(Window *window)
{
  
}

void init()
{
  //Window initialization
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  window_set_click_config_provider(window, click_config_provider);
  
  window_stack_push(window, true);
}

void deinit()
{
  text_layer_destroy(text_layer);
  window_destroy(window);
}
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
}