#include <pebble.h>

Window *window;
TextLayer *title_text, *reps_layer, *weight_layer;
GBitmap *set_bg_bitmap;
BitmapLayer *set_bg_layer;

char buffer[] = "00:00";
typedef struct {
  const char *activity;
  unsigned int reps;
  unsigned int weight;
} Set;
Set sets[] = {
  {.activity = "Crunches", .reps = 15, .weight = 0},
  {.activity = "Side Oblique", .reps = 30, .weight = 0}, 
  {.activity = "Vertical Crunches", .reps = 25, .weight = 0}, 
  {.activity = "Bicycles", .reps = 25, .weight = 0}, 
  {.activity = "Leg Lifts", .reps = 20, .weight = 0}, 
  {.activity = "Torso Twists", .reps = 20, .weight = 13}, 
  {.activity = "Planks", .reps = 60, .weight = 0},  
  {.activity = "Bench Press", .reps = 15, .weight = 45}, 
  {.activity = "Dumbbell Incline Press", .reps = 15, .weight = 10}, 
  {.activity = "Dumbbell Fly", .reps = 15, .weight = 8}, 
  {.activity = "Lateral Raise", .reps = 15, .weight = 5}, 
  {.activity = "Front Raise", .reps = 15, .weight = 8}, 
  {.activity = "Uprighth Row", .reps = 15, .weight = 25}, 
  {.activity = "Skullcrushers", .reps = 15, .weight = 16}, 
  {.activity = "Overhead Extensions", .reps = 15, .weight = 10}, 
  {.activity = "Tricep Pushdowns", .reps = 15, .weight = 20}
};

unsigned int exercise = 0;

static char* itoc(int i)
{
  static char newc[20];
  snprintf(newc, 20, "%d", i);
  return newc;
}

void update_exercise(Set current_set){
    text_layer_set_text(title_text, current_set.activity);
    text_layer_set_text(reps_layer, itoc(current_set.reps));
    text_layer_set_text(weight_layer, itoc(current_set.weight));

}

void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  //do nothing if we're on the first exercise
  if (exercise < 1) return;
  exercise = exercise - 1;
  update_exercise(sets[exercise]);
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  unsigned int string_count = sizeof(sets) / sizeof(sets[0]);
  
  //do nothing if we're on the last exercise
  if (exercise >= string_count - 1) return;
  exercise = exercise + 1;
  update_exercise(sets[exercise]);
}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
 text_layer_set_text(title_text, "That doesn't do anything.");
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void window_load(Window *window)
{
  //activity name
  title_text = text_layer_create(GRect(0, 0, 144, 60));
  text_layer_set_background_color(title_text, GColorClear);
  text_layer_set_text_color(title_text, GColorBlack);
  text_layer_set_text_alignment(title_text, GTextAlignmentCenter);
  text_layer_set_font(title_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) title_text);

   //labels for sets and reps
  set_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SET_LABEL_WHITE);
 
  set_bg_layer = bitmap_layer_create(GRect(0, 67, 144, 66));
  bitmap_layer_set_bitmap(set_bg_layer, set_bg_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(set_bg_layer));

  
  //reps count
  reps_layer = text_layer_create(GRect(12,65, 54, 54));
  text_layer_set_background_color(reps_layer, GColorClear);
  text_layer_set_text_color(reps_layer, GColorBlack);
  text_layer_set_text_alignment(reps_layer, GTextAlignmentCenter);
  text_layer_set_font(reps_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) reps_layer);

  //weight
  weight_layer = text_layer_create(GRect(78,65, 54, 54));
  text_layer_set_background_color(weight_layer, GColorClear);
  text_layer_set_text_color(weight_layer, GColorBlack);
  text_layer_set_text_alignment(weight_layer, GTextAlignmentCenter);
  text_layer_set_font(weight_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) weight_layer);

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
  gbitmap_destroy(set_bg_bitmap);
  bitmap_layer_destroy(set_bg_layer);
  text_layer_destroy(weight_layer);
  text_layer_destroy(reps_layer);
  text_layer_destroy(title_text);
  window_destroy(window);
}
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
}