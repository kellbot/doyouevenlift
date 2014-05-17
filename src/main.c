#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *sets_layer;
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

void update_exercise(Set current_set){
    text_layer_set_text(text_layer, current_set.activity);
    char reps_str[50];
    snprintf(reps_str, 50 , "%d", 10);
    text_layer_set_text(sets_layer, reps_str);
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
  //activity name
  text_layer = text_layer_create(GRect(0, 0, 132, 84));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);

  sets_layer = text_layer_create(GRect(0,84, 132, 168));
  text_layer_set_background_color(sets_layer, GColorClear);
  text_layer_set_text_color(sets_layer, GColorBlack);
  text_layer_set_text_alignment(sets_layer, GTextAlignmentLeft);
  text_layer_set_font(sets_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) sets_layer);
  
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
  text_layer_destroy(sets_layer);
  text_layer_destroy(text_layer);
  window_destroy(window);
}
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
}