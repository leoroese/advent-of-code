#include "stdio.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *key;
  char value;
} item;

item items[] = {
    {"one", '1'}, {"two", '2'},   {"three", '3'}, {"four", '4'}, {"five", '5'},
    {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'},
};
size_t num_items = sizeof(items) / sizeof(item);

// Linear search shouldn't matter given that items is 10 elements
item *linear_search(const char *key) {
  for (size_t i = 0; i < num_items; i++) {
    if (strcmp(items[i].key, key) == 0) {
      return &items[i];
    }
  }
  return NULL;
}

// Check if the current word is a number or represents a number
char *is_number_string(char *current_word) {
  int word_len = strlen(current_word);
  int is_digit_string =
      word_len == 1 && (*current_word >= '0') && (*current_word <= '9');
  if (is_digit_string == 1) {
    return current_word;
  }

  int is_possible_word_digit_string = word_len >= 3 && word_len <= 5;
  if (is_possible_word_digit_string == 1) {
    item *word_item = linear_search(current_word);
    if (word_item != NULL) {
      // static to avoid returning pointer to local variable
      static char value_str[2] = {0};
      value_str[0] = word_item->value;
      return value_str;
    }
  }

  return NULL;
}

// Recursive function to find digit strings
void build_and_find_digits(const char *iterator, char num_string[3]) {
  if (*iterator == '\n' && num_string[0] != '\0') {
    return;
  }

  char current_word[256];
  const char *original_iterator = iterator;
  char *cw_ptr = current_word; // Separate iterator for current_word

  while (*iterator != '\n' && *iterator != '\0') {
    *cw_ptr++ = *iterator;
    *cw_ptr = '\0'; // Ensure null-termination
    char *digit = is_number_string(current_word);
    if (digit != NULL) {
      if (num_string[0] == '\0') {
        num_string[0] = *digit;
      } else {
        num_string[1] = *digit;
      }
      // if a word string then recurse in case another number at end of word
      if (strlen(current_word) > 1) {
        build_and_find_digits(iterator + strlen(cw_ptr), num_string);
        return;
      }
      break;
    }
    iterator++;
  }
  build_and_find_digits(original_iterator + 1, num_string);
}

int main() {
  char *filename = "puzzle-input.txt";
  FILE *fptr;
  fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("Error reading file %s", filename);
    return 1;
  }
  unsigned int total = 0;
  char buffer[256];
  char num_string[3]; // 2 plus null terminator

  while (fgets(buffer, sizeof(buffer), fptr)) {
    const char *iterator = buffer;
    memset(num_string, '\0', sizeof(num_string));
    build_and_find_digits(iterator, num_string);
    if (num_string[1] == '\0') {
      // check if only single digit
      // Guaranteed to always have at least 1 number?
      num_string[1] = num_string[0];
    }
    total += atoi(num_string);
  }

  fclose(fptr);
  printf("total is: %d\n", total);
  return total;
}
