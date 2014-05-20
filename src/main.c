#include <pebble.h>
#include <data.h>
  
Window *window;
Window *menu_window;

MenuLayer *menu_layer;

TextLayer *title_text, *reps_layer, *weight_layer, *count_layer;
InverterLayer *count_inv, *reps_inv, *weight_inv;
GBitmap *set_bg_bitmap;
BitmapLayer *set_bg_layer;

Set *sets;
Set saved_sets[25];
Workout workouts[3];

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
    sets[exercise].reps = sets[exercise].reps + 1;
    static char dreps[20];
    snprintf(dreps, sizeof(dreps), "%d", sets[exercise].reps);
    text_layer_set_text(reps_layer, dreps);
  }  else if (edit_mode == 2) {
    sets[exercise].weight = sets[exercise].weight + 1;
    static char dweight[20];
    snprintf(dweight, sizeof(dweight), "%d", sets[exercise].weight);
    text_layer_set_text(weight_layer, dweight);    
  } else {

    //do nothing if we're on the first exercise
    if (exercise < 1) return;
    exercise = exercise - 1;
    update_exercise(sets[exercise]);
  }
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (edit_mode == 1) {
    sets[exercise].reps = sets[exercise].reps - 1;
    static char dreps[20];
    snprintf(dreps, sizeof(dreps), "%d", sets[exercise].reps);
    text_layer_set_text(reps_layer, dreps);
  } else if (edit_mode == 2) {
    sets[exercise].weight = sets[exercise].weight - 1;
    static char dweight[20];
    snprintf(dweight, sizeof(dweight), "%d", sets[exercise].weight);
    text_layer_set_text(weight_layer, dweight);    
  } else { //go to next exercise
    //do nothing if we're on the last exercise
    if (exercise >= set_count - 1) return;
    exercise = exercise + 1;
    update_exercise(sets[exercise]);
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
  
  update_exercise(sets[0]);

}

void window_unload(Window *window)
{
  //exit edit mode
  edit_mode = 0;
  
   persist_write_int(COUNT_KEY, set_count); 
   persist_write_data(SETS_DATA_KEY, sets, sizeof(sets[0])*set_count);
   persist_write_int(LENGTH_KEY, sizeof(sets[0])*set_count);
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Just wrote %d sets of length %d starting with %d %s", set_count, sizeof(sets[0]) * set_count, sets[0].reps, sets[0].activity);

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
        menu_cell_basic_draw(ctx, cell_layer, "Workout 1", "Back / Bis / Tris", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Workout 2", "Legs, etc", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Workout 3", "Bodyweight bonus day", NULL);
        break;
    case 3:
        menu_cell_basic_draw(ctx, cell_layer, "Reset Data", "Clear saved workouts", NULL);
        break;
    }
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
 return 4;
}
 
/*void load_set(){
  if(persist_exists(LENGTH_KEY) && persist_exists(COUNT_KEY) && persist_exists(SET_DATA_KEY){
    int last_seen_sets = persist_read_int(COUNT_KEY);
    int last_seen_length = persist_read_int(LENGTH_KEY);
    persist_read_data(SETS_DATA_KEY, &saved_sets, last_seen_length);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last saw %d sets of length %d starting with %d %s.", last_seen_sets, last_seen_length, saved_sets[0].reps, saved_sets[0].activity);
  
   sets = saved_sets;
   set_count = last_seen_sets;
  
  } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "No sets found");   
      sets = default_sets;
  }

}*/

void load_workout(int i){
  sets = workouts[i].sets;
  set_count = workouts[i].number_of_sets;
  exercise = 0;
  window_stack_push(window, true);
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  //Get which row
  switch(cell_index->row){
    case 0:
    case 1:
    case 2:
      load_workout(cell_index->row);
      break;
    case 3:
      persist_delete(SETS_DATA_KEY);
      persist_delete(LENGTH_KEY);
      persist_delete(COUNT_KEY);
      sets = workout_1.sets;
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
  workouts[0] = workout_1;
  workouts[1] = workout_2;
  workouts[2] = workout_3;

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