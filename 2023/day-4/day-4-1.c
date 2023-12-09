#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some maths
// left is winning numbers split by | where right are your numbers
// essentially you want to power of 2 for every match you find. So first card =
// 2^0, 2nd match = 2^2 and so forth Psuedo code read every line split until:,
// trim the beginning white space. Then split for every number by " " until |,
// store these values in some sort of easily hashable searchable value, or a
// list then after | iterate through every number, and check if it is in the
// winnings, if so, that card = 2^number of matches -1 Add all the totals

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
      // check if my_nums is in winners and increment if so
      if (my_nums[i] == winners[j]) {
        matching_nums++;
        break;
      }
    }
  }

  if (matching_nums == 0) {
    printf("Returning 0\n");
    return 0;
  }

  long int total = (long int)pow(2, matching_nums - 1);

  printf("Matching nums: %d, card_total: %li\n", matching_nums, total);

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
  long int total_poits = 0;
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

    printf("Winners are: ");
    for (int i = 0; i < winner_count; i++) {
      printf("%d ", winners[i]);
    }
    printf("\n");

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

    printf("My nums are: ");
    for (int i = 0; i < my_nums_size; i++) {
      printf("%d ", my_nums[i]);
    }
    printf("\n");

    long int card_total =
        calculate_card_point(winners, my_nums, winner_count, my_nums_size);
    // compare
    total_poits += card_total;

    free(winners);
    free(my_nums);
  }

  printf("total points: %li\n", total_poits);

  fclose(file); // Moved outside the loop

  return 0;
}
FILE *get_file() {
  FILE *ptr = fopen(
      "/Users/leoroese/dev/learning/advent-of-code/2023/day-4/puzzle.txt", "r");
  return ptr;
}
