#include "stdio.h"
#include "string.h"
#include <stdlib.h>

typedef enum { red = 0, green = 1, blue = 2 } Color;

typedef struct {
  int red_max;
  int green_max;
  int blue_max;
} ColorMax;

Color matchColor(const char *colorName) {
  if (strcmp(colorName, "red") == 0) {
    return red;
  }
  if (strcmp(colorName, "green") == 0) {
    return green;
  }
  if (strcmp(colorName, "blue") == 0) {
    return blue;
  }
  return -1;
}

void set_max_number_for_color(int number, char *color, ColorMax *color_map) {
  Color color_enum = matchColor(color);
  switch (color_enum) {
  case 0:
    if (number > color_map->red_max) {
      color_map->red_max = number;
    }
    break;
  case 1:
    if (number > color_map->green_max) {
      color_map->green_max = number;
    }

    break;
  case 2:
    if (number > color_map->blue_max) {
      color_map->blue_max = number;
    }
    break;
  }
}

// part2 get max from each row, then at the end multiply them together

int main() {
  char *filename = "puzzle.txt";
  FILE *fptr;
  fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("Error reading file %s", filename);
    return 1;
  }

  char buffer[2048];
  long unsigned int game_total = 0;

  while (fgets(buffer, sizeof(buffer), fptr)) {
    int game_id, valid_game;
    char *token, *inner_token;
    char *saveptr1, *saveptr2;
    ColorMax color_map = {0, 0, 0};

    // Extract game id number
    // Need to use the reentrant version here to remember the context
    token = strtok_r(buffer, ":", &saveptr1);
    if (token != NULL) {
      sscanf(token, "Game %d", &game_id);
    }

    // extract each set
    token = strtok_r(NULL, ";", &saveptr1);
    valid_game = 1;
    while (token != NULL && valid_game != -1) {
      while (*token == ' ') {
        // cleanup whitespace
        token++;
      }

      // Further split each set into number-color pairs
      inner_token = strtok_r(token, ",", &saveptr2);
      while (inner_token != NULL) {
        // Trim leading spaces
        while (*inner_token == ' ') {
          inner_token++;
        }
        // Split each number-color pair
        char color[6];
        int number;

        while (*inner_token == ' ') {
          inner_token++;
        }

        sscanf(inner_token, "%d %[^\n]", &number, color);
        set_max_number_for_color(number, color, &color_map);
        if (valid_game == -1) {
          break;
        }

        inner_token = strtok_r(NULL, ",", &saveptr2);
      }
      token = strtok_r(NULL, ";", &saveptr1);
    }
    int power = color_map.red_max * color_map.green_max * color_map.blue_max;
    game_total += power;
  }

  printf("Game total: %lu\n", game_total);

  return 0;
}
