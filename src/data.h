typedef struct {
  char *activity;
  unsigned int reps;
  unsigned int weight;
  unsigned int cooldown;
} Set;

typedef struct {
  char *start_time;
  unsigned int number_of_sets;
  Set completed_sets[20];
} Activity;

typedef struct Routine {
  char *name;
  unsigned int number_of_sets;
  Set target_sets[50];
} __attribute__((__packed__)) Routine;

extern Routine test_routine;