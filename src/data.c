#include <data.h>
  
Routine test_routine = {
  .target_sets = {
      {.activity = "Crunches", .reps = 15, .weight = 0, .cooldown = 30},
      {.activity = "Bicycles", .reps = 30, .weight = 0, .cooldown = 30}, 
      {.activity = "Leg Lifts", .reps = 25, .weight = 0, .cooldown = 30}, 
      {.activity = "Planks", .reps = 25, .weight = 0, .cooldown = 30}, 
      {.activity = "Pull Ups", .reps = 20, .weight = 0, .cooldown = 30}, 
      {.activity = "Push Ups", .reps = 20, .weight = 13, .cooldown = 30}, 
      {.activity = "TB Shoulder Press", .reps = 60, .weight = 0, .cooldown = 30},  
      {.activity = "TB Lat Raise", .reps = 15, .weight = 45, .cooldown = 30}, 
      {.activity = "TB Front Raise", .reps = 15, .weight = 10, .cooldown = 30}, 
      {.activity = "TB Upright Rows", .reps = 15, .weight = 8, .cooldown = 30}, 
      {.activity = "TB Alternating Curls", .reps = 15, .weight = 5, .cooldown = 30}, 
      {.activity = "Dips", .reps = 15, .weight = 8, .cooldown = 30},
      {.activity = "TB Kickbacks", .reps = 15, .weight = 8, .cooldown = 30}
  },
  .name = "Bodyweight",
  .number_of_sets = 13
};