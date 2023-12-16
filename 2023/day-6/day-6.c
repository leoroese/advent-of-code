
#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

// need to go further in each race
// starting speed of 0
// speed increases by 1 mm per millisecond
// the amount of time is the amount of time held + distance traveling
// so if its 7 seconds total for the race, and I hold down 2 milliseconds, it
// travels for 5 milliseconds at 2 millimeters per second
// figure out the number of ways you can win, then multiply them all together
// for each race
//
FILE *get_file();
int get_next_number(char **buffer);
int is_solution(int time_held_down, int total_time, int distance_to_beat);

int main() {
  FILE *file = get_file();

  char time_buffer[512];
  char distance_buffer[512];
  int race = 0;
  int size_of_array = 100;
  int *solutions = (int *)calloc(size_of_array, sizeof(int));

  while (fgets(time_buffer, sizeof(time_buffer), file)) {
    fgets(distance_buffer, sizeof(distance_buffer), file);

    char *time_ptr = time_buffer;
    char *distance_ptr = distance_buffer;

    // Gets number of characters before first occurent in both strings
    time_ptr += strcspn(time_ptr, "0123456789");
    distance_ptr += strcspn(distance_ptr, "0123456789");

    while (*time_ptr != '\0' && *distance_ptr != '\0') {
      int total_time = get_next_number(&time_ptr);
      int distance = get_next_number(&distance_ptr);
      if (total_time == -1 && distance == -1) {
        // No more numbers
        break;
      }

      for (int i = 0; i <= total_time; i++) {
        if (is_solution(i, total_time, distance) == 1) {
          solutions[race] = solutions[race] + 1;
        }
      }

      race++;
      if (race > size_of_array) {
        size_of_array *= 2;
        solutions = (int *)reallocf(solutions, size_of_array);
      }
    }
  }

  long int answer = 1;

  for (int i = 0; i < race; i++) {
    printf("Solutions for race %d is %d\n", i, solutions[i]);
    if (solutions[i] > 0) {
      answer *= solutions[i];
    }
  }

  // when calculating total be wary of 0

  // we want to read the first 2 lines individually
  free(solutions);

  if (answer == 1) {
    printf("Answer: 0\n");
  } else {

    printf("Answer: %li\n", answer);
  }

  return 0;
}

int is_solution(int time_held_down, int total_time, int distance_to_beat) {
  int distance_traveled = time_held_down * (total_time - time_held_down);
  if (distance_traveled > distance_to_beat) {
    return 1;
  }
  return 0;
}

int get_next_number(char **buffer_ptr) {
  char *buffer = *buffer_ptr;
  while (*buffer != '\0' && !isdigit((unsigned char)*buffer)) {
    buffer++;
  }
  if (*buffer == '\0') {
    return -1; // No more numbers
  }
  int number, chars_read;
  sscanf(buffer, "%d%n", &number, &chars_read);
  buffer += chars_read;
  *buffer_ptr = buffer;
  return number;
}

FILE *get_file() {
  FILE *ptr = fopen(
      "/Users/leoroese/dev/learning/advent-of-code/2023/day-6/puzzle.txt", "r");
  if (ptr == NULL) {
    perror("Error opening file");
  }
  return ptr;
}
