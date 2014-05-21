#include <pebble.h>
#include <data.h>
#define ROUTINE_KEY 9
  
  
Window *window;
Window *menu_window;

MenuLayer *menu_layer;

TextLayer *title_text, *reps_layer, *weight_layer, *count_layer;
InverterLayer *count_inv, *reps_inv, *weight_inv;
GBitmap *set_bg_bitmap;
BitmapLayer *set_bg_layer;

Routine *current_routine;
Routine stored_routine;

unsigned int exercise = 0;
unsigned int set_count = 16;
//flag to see if we're editing
unsigned int edit_mode = 0;

void update_exercise(Set current_set){
    text_layer_set_text(title_text, current_set.activity);
    static char nreps[20];
    snprintf(nreps, sizeof(nreps), "%d", current_set.reps);
    text_layer_set_text(reps_layer, nreps);
    static char nweight[20];
    snprintf(nweight, sizeof(nweight), "%d", current_set.weight);
    text_layer_set_text(weight_layer,  nweight);
    static char nexercise[20];
    snprintf(nexercise, sizeof(nexercise), "Set %d of %d", exercise +1, set_count);  
    text_layer_set_text(count_layer, nexercise);

}

void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (edit_mode == 1) {
    current_routine->target_sets[exercise].reps = current_routine->target_sets[exercise].reps + 1;
    static char dreps[20];
    snprintf(dreps, sizeof(dreps), "%d", current_routine->target_sets[exercise].reps);
    text_layer_set_text(reps_layer, dreps);
  }  else if (edit_mode == 2) {
    current_routine->target_sets[exercise].weight = current_routine->target_sets[exercise].weight + 1;
    static char dweight[20];
    snprintf(dweight, sizeof(dweight), "%d", current_routine->target_sets[exercise].weight);
    text_layer_set_text(weight_layer, dweight);    
  } else {

    //do nothing if we're on the first exercise
    if (exercise < 1) return;
    exercise = exercise - 1;
    update_exercise(current_routine->target_sets[exercise]);
  }
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (edit_mode == 1) {
    current_routine->target_sets[exercise].reps = current_routine->target_sets[exercise].reps - 1;
    static char dreps[20];
    snprintf(dreps, sizeof(dreps), "%d", current_routine->target_sets[exercise].reps);
    text_layer_set_text(reps_layer, dreps);
  } else if (edit_mode == 2) {
    current_routine->target_sets[exercise].weight = current_routine->target_sets[exercise].weight - 1;
    static char dweight[20];
    snprintf(dweight, sizeof(dweight), "%d", current_routine->target_sets[exercise].weight);
    text_layer_set_text(weight_layer, dweight);    
  } else { //go to next exercise
    //do nothing if we're on the last exercise
    if (exercise >= current_routine->number_of_sets -1) return;
    exercise = exercise + 1;
    update_exercise(current_routine->target_sets[exercise]);
  }
}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  //enter edit mode
  if (edit_mode == 0) {
    edit_mode = 1;
    //draw an inversion layer over the reps number
    layer_add_child(window_get_root_layer(window), (Layer*) reps_inv);

  } else if (edit_mode == 1) { //if we're editing reps, move to weight
    edit_mode = 2;
    layer_remove_from_parent( (Layer*) reps_inv);
    layer_add_child(window_get_root_layer(window), (Layer*) weight_inv);
  }  else { //edxit edit mote
    edit_mode = 0;
    layer_remove_from_parent( (Layer*) weight_inv);
  }
}

void select_long_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(menu_window, true);
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_handler, NULL);
  
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

  //exercise count
  count_layer = text_layer_create(GRect(0, 129, 144, 20));
  text_layer_set_background_color(count_layer, GColorClear);
  text_layer_set_text_color(count_layer, GColorBlack);
  text_layer_set_text_alignment(count_layer, GTextAlignmentCenter);
  text_layer_set_font(count_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(window), (Layer*) count_layer);
  
  count_inv = inverter_layer_create(GRect(0, 133, 144, 20));
  layer_add_child(window_get_root_layer(window), (Layer*) count_inv);
  
  //edit layer invs
  reps_inv = inverter_layer_create(GRect(12, 76, 54, 34));
  weight_inv = inverter_layer_create(GRect(78, 76, 54, 34));
  
  update_exercise(current_routine->target_sets[0]);

}

void window_unload(Window *window)
{
  //exit edit mode
  edit_mode = 0;
  
  //save the current routine.
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Writing routine %d bytes long.", 512);
  int wrote = persist_write_data(ROUTINE_KEY, current_routine, 512);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrote %d bytes.", wrote);

  
  //free up the ram
  inverter_layer_destroy(reps_inv);
  inverter_layer_destroy(weight_inv);
  inverter_layer_destroy(count_inv);
  gbitmap_destroy(set_bg_bitmap);
  bitmap_layer_destroy(set_bg_layer);
  text_layer_destroy(weight_layer);
  text_layer_destroy(reps_layer);
  text_layer_destroy(title_text);

}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  //Which row is it?
    switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "Start Workout", current_routine->name, NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Sync Saved Data", "TODO saved workouts", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Reset Data", "Clear saved workouts", NULL);
        break;
    }
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
 return 3;
}
 
//fetches the current routine. Looks in persistent memory, then queries the phone if it can't find it
void get_active_routine(){
  //check in persistent memory
  if(persist_exists(ROUTINE_KEY)){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Found a routine of size %d.",  persist_get_size(ROUTINE_KEY));
    persist_read_data(ROUTINE_KEY, &stored_routine, persist_get_size(ROUTINE_KEY));
    current_routine = &stored_routine;
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Using default routine.");
    //TODO get app message, for now a default
    current_routine = &test_routine;
  }
}

void load_workout(){
 // get_active_routine();
  window_stack_push(window, true);
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  //Get which row
  switch(cell_index->row){
    case 0:
      load_workout();
      break;
    case 1:
      //TODO
      break;
    case 2:
      persist_delete(ROUTINE_KEY);
      break;
  }

}

void menu_load(Window *menu_window){
      //Create it - 12 is approx height of the top bar
    menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    //Let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer, menu_window);
 
    //Give it its callbacks
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_click_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
 
    //Add to Window
    layer_add_child(window_get_root_layer(menu_window), menu_layer_get_layer(menu_layer));
}
void menu_unload(Window *menu_window){
  menu_layer_destroy(menu_layer);
}


void init()
{
  
  //Window initialization
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  menu_window = window_create();
  window_set_window_handlers(menu_window, (WindowHandlers){
    .load = menu_load,
    .unload = menu_unload
  });
  
  window_set_click_config_provider(window, click_config_provider);
  
  get_active_routine();
  
  window_stack_push(menu_window, true);
}

void deinit()
{
  window_destroy(window);
  window_destroy(menu_window);
}
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
}