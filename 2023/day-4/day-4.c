#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *get_file();

int count_numbers(char *number_string, char delimiter) {
  unsigned int count = 0;
  int num, chars_read;

  while (*number_string != delimiter && *number_string != '\0') {
    if (sscanf(number_string, "%d%n", &num, &chars_read) == 1) {
      count++;
      number_string += chars_read; // Move past the number
    } else {
      number_string++; // Move to the next character
    }
  }

  return count;
}
int *get_section_numbers(char *nums, char delimiter, int count) {
  int *numbers = (int *)malloc(count * sizeof(int));
  if (numbers == NULL) {
    perror("Memory allocation failed");
    return NULL;
  }

  const char *current = nums;
  int num, chars_read, index = 0;
  while (*current != delimiter && *current != '\0') {
    if (sscanf(current, "%d%n", &num, &chars_read) == 1) {
      numbers[index++] = num;
      current += chars_read; // Move past the number
    } else {
      current++; // Move to the next character
    }
  }

  return numbers;
}

long int calculate_card_point(int *winners, int *my_nums, int winner_count,
                              int my_nums_size) {
  long int card_total = 0;
  int matching_nums = 0;

  for (int i = 0; i < my_nums_size; i++) {
    for (int j = 0; j < winner_count; j++) {
      if (my_nums[i] == winners[j]) {
        matching_nums++;
        break;
      }
    }
  }

  if (matching_nums == 0) {
    return 0;
  }

  long int total = (long int)pow(2, matching_nums - 1);

  return total;
}

int main() {
  FILE *file = get_file();
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  } else {
    printf("File opened successfully.\n");
  }

  char buffer[2048];
  long int total_points = 0;
  int winner_count = 0, my_nums_count = 0;

  while (fgets(buffer, sizeof(buffer), file)) {
    char *winners_start = strchr(buffer, ':');
    if (winners_start != NULL) {
      winners_start += 2; // skip ':' and space after
    }

    winner_count = count_numbers(winners_start, '|');
    int *winners = get_section_numbers(winners_start, '|', winner_count);
    if (winners == NULL) {
      perror("Error reading numbers");
      break;
    }

    char *my_nums_section = strchr(winners_start, '|');
    if (my_nums_section != NULL) {
      my_nums_section += 2;
    }
    int my_nums_size = count_numbers(my_nums_section, '\n');
    int *my_nums = get_section_numbers(my_nums_section, '\n', my_nums_size);

    if (my_nums == NULL) {
      perror("Error reading numbers");
      break;
    }

    long int card_total =
        calculate_card_point(winners, my_nums, winner_count, my_nums_size);
    total_points += card_total;

    free(winners);
    free(my_nums);
  }

  printf("total points: %li\n", total_points);

  fclose(file); // Moved outside the loop

  return 0;
}
FILE *get_file() {
  FILE *ptr = fopen(
      "/Users/leoroese/dev/learning/advent-of-code/2023/day-4/puzzle.txt", "r");
  return ptr;
}
