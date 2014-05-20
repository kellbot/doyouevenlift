#define LENGTH_KEY 1
#define SETS_DATA_KEY 2
#define COUNT_KEY 3
  
typedef struct {
  const char *activity;
  unsigned int reps;
  unsigned int weight;
} Set;

typedef struct {
  char *name;
  unsigned int number_of_sets;
  Set sets[16];
} Workout;

extern Workout workout_1;
extern Workout workout_2;
extern Workout workout_3;