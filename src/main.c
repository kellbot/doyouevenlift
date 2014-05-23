#include <pebble.h>
#include <data.h>
#define PERSIST_LIMIT 256
#define ACTIVITY_STRINGS_KEY 1
#define ROUTINE_NAME_KEY 2
#define ROUTINE_DATA_KEY 3

Window *exercise_window, *menu_window;
TextLayer *exercise_name_text, *reps_text,  *weight_text, *exercise_count_text;
MenuLayer *menu_layer;
InverterLayer *count_inv, *reps_inv, *weight_inv;
GBitmap *exercise_bg_bitmap;
BitmapLayer *exercise_bg_layer;

int edit_mode = 0;
Routine *current_routine;
char saved_name[20];
Routine saved_routine;
int current_exercise = 0;

void exercise_window_load(){
  //exercise name
  exercise_name_text = text_layer_create(GRect(0, 0, 144, 60));
  text_layer_set_background_color(exercise_name_text, GColorClear);
  text_layer_set_text_color(exercise_name_text, GColorBlack);
  text_layer_set_text_alignment(exercise_name_text, GTextAlignmentCenter);
  text_layer_set_font(exercise_name_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(exercise_window), (Layer*) exercise_name_text);

   //labels for sets and reps
  exercise_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SET_LABEL_WHITE);
  exercise_bg_layer = bitmap_layer_create(GRect(0, 67, 144, 66));
  bitmap_layer_set_bitmap(exercise_bg_layer, exercise_bg_bitmap);
  layer_add_child(window_get_root_layer(exercise_window), bitmap_layer_get_layer(exercise_bg_layer));

  
  //reps count
  reps_text = text_layer_create(GRect(12,65, 54, 54));
  text_layer_set_background_color(reps_text, GColorClear);
  text_layer_set_text_color(reps_text, GColorBlack);
  text_layer_set_text_alignment(reps_text, GTextAlignmentCenter);
  text_layer_set_font(reps_text, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(exercise_window), (Layer*) reps_text);

  //weight
  weight_text = text_layer_create(GRect(78,65, 54, 54));
  text_layer_set_background_color(weight_text, GColorClear);
  text_layer_set_text_color(weight_text, GColorBlack);
  text_layer_set_text_alignment(weight_text, GTextAlignmentCenter);
  text_layer_set_font(weight_text, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(exercise_window), (Layer*) weight_text);

  //exercise count
  exercise_count_text = text_layer_create(GRect(0, 129, 144, 20));
  text_layer_set_background_color(exercise_count_text, GColorClear);
  text_layer_set_text_color(exercise_count_text, GColorBlack);
  text_layer_set_text_alignment(exercise_count_text, GTextAlignmentCenter);
  text_layer_set_font(exercise_count_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(exercise_window), (Layer*) exercise_count_text);
  
  count_inv = inverter_layer_create(GRect(0, 133, 144, 20));
  layer_add_child(window_get_root_layer(exercise_window), (Layer*) count_inv);
  
  //edit layer invs
  reps_inv = inverter_layer_create(GRect(12, 76, 54, 34));
  weight_inv = inverter_layer_create(GRect(78, 76, 54, 34));
  

}

//writes the currently loaded routine into persistent storage, returns b
void save_current_routine(){
  //first, concatenate all the activity strings into one string
  char activity_strings[PERSIST_LIMIT];
  for (unsigned int i = 0; i < PERSIST_LIMIT - 2; i++){
    activity_strings[i] = 0;
  }
  int len = 0;
  for(int i=0; i < current_routine->number_of_exercises; i++){
    char *stw = current_routine->exercise_names[i];
    
    //break if it's too long
    if (len + strlen(stw) > PERSIST_LIMIT - 2) break;
    
    memcpy(&activity_strings[len], stw, strlen(stw));
    memcpy(&activity_strings[len + strlen(stw)], ",", 1);
    len = len + strlen(stw) + 1;
  }
  
  persist_write_string(ACTIVITY_STRINGS_KEY, activity_strings);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrote %d bytes of strings.", wrote);
  
  //save the name of the routine
  persist_write_string(ROUTINE_NAME_KEY, current_routine->name);
  
  //save the routine itself
  int rwrote = persist_write_data(ROUTINE_DATA_KEY, &current_routine->target_sets, sizeof(current_routine->target_sets));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrote %d of %d bytes of routine", rwrote,  sizeof(current_routine->target_sets));
  
}

void exercise_window_unload(){
  //exit edit mode
  edit_mode = 0;
  
  save_current_routine();
  
  //free up all the ram
  text_layer_destroy(exercise_name_text);
  text_layer_destroy(reps_text);
  text_layer_destroy(weight_text);
  text_layer_destroy(exercise_count_text);
  inverter_layer_destroy(count_inv);
  inverter_layer_destroy(reps_inv);
  inverter_layer_destroy(weight_inv);
  bitmap_layer_destroy(exercise_bg_layer);
  
}

void update_exercise(Set *current_set){
    //correlate the exercise key in the current set to the string name in the current routine
    text_layer_set_text(exercise_name_text, current_routine->exercise_names[current_set->exercise_key]);

  static char nreps[20];
    snprintf(nreps, sizeof(nreps), "%d", current_set->reps);
    text_layer_set_text(reps_text, nreps);

  static char nweight[20];
    snprintf(nweight, sizeof(nweight), "%d", current_set->weight);
    text_layer_set_text(weight_text,  nweight);
  
    static char nexercise[20];
    snprintf(nexercise, sizeof(nexercise), "Set %d of %d", current_exercise +1, current_routine->number_of_sets);  
    text_layer_set_text(exercise_count_text, nexercise);

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
        menu_cell_basic_draw(ctx, cell_layer, "Load Sample Data", "From source", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Clear Data", "Clear saved workouts", NULL);
        break;
    }
}

//loads a sample routine for testing
void load_sample_data(){
  current_routine = &sample_routine;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded sample routine");
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row){
    case 0:
      window_stack_push(exercise_window, true);
      update_exercise(&current_routine->target_sets[0]);

      break;
    case 1: 
      load_sample_data();
      menu_layer_reload_data(menu_layer);
      break;
    case 2:
      break;
  }

}

//tries to load data from persistent memory, defaults to sample data if none found
void load_saved_data(){
  //check that we have all necessary persist keys
  if (persist_exists(ACTIVITY_STRINGS_KEY) && persist_exists(ROUTINE_NAME_KEY) && persist_exists(ROUTINE_DATA_KEY)){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Save data found...");

    //load routine data
    int rread = persist_read_data(ROUTINE_DATA_KEY, &saved_routine, persist_get_size(ROUTINE_DATA_KEY));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Read %d byte routine", rread);
    
    
    //load routine name
    int nread = persist_read_string(ROUTINE_NAME_KEY, saved_name, persist_get_size(ROUTINE_NAME_KEY));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Read %d byte name %s", nread, saved_name);
    
    saved_routine.name = saved_name;  
    

    
    current_routine = &saved_routine;
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "No save found. Loading sample data.");
    load_sample_data();
  }
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
 return 3;
}

void menu_load(){
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

void menu_unload(){
  menu_layer_destroy(menu_layer);
}

void select_click_handler(){
  //enter edit mode
  if (edit_mode == 0) {
    edit_mode = 1;
    //draw an inversion layer over the reps number
    layer_add_child(window_get_root_layer(exercise_window), (Layer*) reps_inv);

  } else if (edit_mode == 1) { //if we're editing reps, move to weight
    edit_mode = 2;
    layer_remove_from_parent( (Layer*) reps_inv);
    layer_add_child(window_get_root_layer(exercise_window), (Layer*) weight_inv);
  }  else { //edxit edit mote
    edit_mode = 0;
    layer_remove_from_parent( (Layer*) weight_inv);
  }
}

void edit_reps(int edit_amt){
  current_routine->target_sets[current_exercise].reps =   current_routine->target_sets[current_exercise].reps + edit_amt;
   static char dreps[20];
   snprintf(dreps, sizeof(dreps), "%d", current_routine->target_sets[current_exercise].reps);
   text_layer_set_text(reps_text, dreps);

}

void edit_weight(int edit_amt){
  current_routine->target_sets[current_exercise].weight =   current_routine->target_sets[current_exercise].weight + edit_amt;
   static char dweight[20];
   snprintf(dweight, sizeof(dweight), "%d", current_routine->target_sets[current_exercise].weight);
   text_layer_set_text(weight_text, dweight);

}

void down_click_handler(ClickRecognizerRef recognizer, void *context){
  if (edit_mode == 1){
    edit_reps(-1);
  } else if (edit_mode == 2) {
    edit_weight(-1);
  } else {
    //check bounds
    if (current_exercise >= current_routine->number_of_sets -1) return;  
    //increment exercise index
    current_exercise++;
    //update screen
    update_exercise(&current_routine->target_sets[current_exercise]);    
  }
}

void up_click_handler(ClickRecognizerRef recognizer, void *context){
  if (edit_mode == 1){
    edit_reps(1);
  } else if (edit_mode == 2) {
    edit_weight(1);
  } else {
  
    //check bounds
    if (current_exercise  < 1) return;  
    //increment exercise index
    current_exercise--;
    //update screen
    update_exercise(&current_routine->target_sets[current_exercise]); 
  }
}
void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  
}

void handle_init(void) {
  //initialize exercise and menu windows
  exercise_window = window_create();
  window_set_window_handlers(exercise_window, (WindowHandlers) {
    .load = exercise_window_load,
    .unload = exercise_window_unload,
  });
  menu_window = window_create();
  window_set_window_handlers(menu_window, (WindowHandlers) {
    .load = menu_load,
    .unload = menu_unload,
  });

  window_set_click_config_provider(exercise_window, click_config_provider);

  load_saved_data();
  window_stack_push(menu_window, true);
}

void handle_deinit(void) {
  window_destroy(exercise_window);
  window_destroy(menu_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
