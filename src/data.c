#include <data.h>
  
Workout workout_1 = {
  .sets = {
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
      {.activity = "Upright Row", .reps = 15, .weight = 25}, 
      {.activity = "Skullcrushers", .reps = 15, .weight = 16}, 
      {.activity = "Overhead Extensions", .reps = 15, .weight = 10}, 
      {.activity = "Tricep Pushdowns", .reps = 15, .weight = 20}
  },
  .name = "Abs / Chest / Tris",
  .number_of_sets = 16
};
Workout workout_2 = {
  .sets = {
      {.activity = "Squats", .reps = 15, .weight = 0},
      {.activity = "Lunges", .reps = 30, .weight = 0}, 
      {.activity = "Leg Extensions", .reps = 25, .weight = 0}, 
      {.activity = "Leg Curls", .reps = 25, .weight = 0}, 
      {.activity = "Calf Raises", .reps = 20, .weight = 0}, 
      {.activity = "Lat Pulldowns", .reps = 20, .weight = 13}, 
      {.activity = "Bent Over Rows", .reps = 60, .weight = 0},  
      {.activity = "Bent Over Laterals", .reps = 15, .weight = 45}, 
      {.activity = "Deadlifts", .reps = 15, .weight = 10}, 
      {.activity = "Alternating DB Curls", .reps = 15, .weight = 8}, 
      {.activity = "EZ Bar Curls", .reps = 15, .weight = 5}, 
      {.activity = "Concentration Curls", .reps = 15, .weight = 8}, 
    },
    .name = "Legs / Shoulders / Biceps",
    .number_of_sets = 12
};
Workout workout_3 = {
  .sets = {
      {.activity = "Crunches", .reps = 15, .weight = 0},
      {.activity = "Bicycles", .reps = 30, .weight = 0}, 
      {.activity = "Leg Lifts", .reps = 25, .weight = 0}, 
      {.activity = "Planks", .reps = 25, .weight = 0}, 
      {.activity = "Pull Ups", .reps = 20, .weight = 0}, 
      {.activity = "Push Ups", .reps = 20, .weight = 13}, 
      {.activity = "TB Shoulder Press", .reps = 60, .weight = 0},  
      {.activity = "TB Lat Raise", .reps = 15, .weight = 45}, 
      {.activity = "TB Front Raise", .reps = 15, .weight = 10}, 
      {.activity = "TB Upright Rows", .reps = 15, .weight = 8}, 
      {.activity = "TB Alternating Curls", .reps = 15, .weight = 5}, 
      {.activity = "Dips", .reps = 15, .weight = 8},
      {.activity = "TB Kickbacks", .reps = 15, .weight = 8}
  },
  .name = "Bodyweight",
  .number_of_sets = 13
};