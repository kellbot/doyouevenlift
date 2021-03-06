typedef struct Set {
  unsigned char exercise_key;
  unsigned char reps;
  unsigned char weight;
}  __attribute__((__packed__)) Set;

typedef struct Routine {
  char *name;
  int number_of_sets;
  int number_of_exercises;
  Set target_sets[50];
  char *exercise_names[];
} __attribute__((__packed__)) Routine;

extern Routine sample_routine;