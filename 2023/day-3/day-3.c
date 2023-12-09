#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lessons
// Prepending isn't free, need to copy memory over
// 2d arrays are annoying
// pass by reference args to track
// Careful with new lines when reading files into 2d arrays

typedef struct {
  int rows;
  int columns;
} GridDimensions;

GridDimensions get_dimensions(FILE *fptr) {
  GridDimensions dimensions = {0, 0};
  char buffer[512];
  int max_columns = 0;

  while (fgets(buffer, sizeof(buffer), fptr)) {
    int length = strlen(buffer);

    // Remove newline and/or carriage return characters
    if (length > 0 &&
        (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')) {
      buffer[--length] = '\0';
    }

    if (length > max_columns) {
      max_columns = length;
    }
    dimensions.rows++;
  }

  dimensions.columns = max_columns + 1;
  printf("rows: %d, columns: %d\n", dimensions.rows, dimensions.columns);
  return dimensions;
}

void addAdjacentNumbers(int rows, int columns, char grid[rows][columns],
                        int seen[rows][columns], int row, int col,
                        unsigned long int *total) {
  if (row < 0 || row >= rows || col < 0 || col >= columns ||
      seen[row][col] == 1) {
    return;
  }

  // Check if the current position is a number
  if (grid[row][col] >= '0' && grid[row][col] <= '9') {
    int number = 0;
    int start = col;

    // Move to the leftmost digit of the number
    while (start >= 0 && grid[row][start] >= '0' && grid[row][start] <= '9') {
      start--;
    }
    start++; // Adjust to point to the first digit

    // Accumulate the number from left to right
    while (start < columns && grid[row][start] >= '0' &&
           grid[row][start] <= '9') {
      number = number * 10 + (grid[row][start] - '0');
      seen[row][start] = 1; // Mark the digit as seen
      start++;
    }

    *total += number;
  }
}

// all 8 directions
int dirs[8][2] = {
    {-1, -1}, // top left
    {-1, 0},  // top
    {-1, 1},  // top right
    {0, -1},  // left
    {0, 1},   // right
    {1, -1},  // bottom left
    {1, 0},   // bottom
    {1, 1}    // bottom right
};

void solve(int rows, int columns, char grid[rows][columns],
           int seen[rows][columns], int current_row, int current_column,
           unsigned long int *total) {
  if (current_row < 0 || current_row >= rows || current_column < 0 ||
      current_column >= columns || seen[current_row][current_column] == 1) {
    return;
  }

  int ascii_value = grid[current_row][current_column];
  int is_symbol = (ascii_value >= 33 && ascii_value <= 47 && ascii_value != 46);

  if (is_symbol) {
    // Check all 8 directions for numbers
    for (int i = 0; i < 8; i++) {
      int next_row = current_row + dirs[i][0];
      int next_col = current_column + dirs[i][1];

      addAdjacentNumbers(rows, columns, grid, seen, next_row, next_col, total);
    }
  }
}

int main() {
  char *filename = "puzzle.txt";
  FILE *fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("Error reading file %s", filename);
    return 1;
  }

  GridDimensions dims = get_dimensions(fptr);
  fseek(fptr, 0, SEEK_SET);

  // Dynamically allocate memory for grid and seen arrays
  char(*grid)[dims.columns] = malloc(dims.rows * sizeof(*grid));
  int(*seen)[dims.columns] = malloc(dims.rows * sizeof(*seen));

  if (!grid || !seen) {
    printf("Memory allocation failed\n");
    fclose(fptr);
    return 1;
  }

  // Read file into grid and initialize seen
  int row = 0;
  while (row < dims.rows && fgets(grid[row], dims.columns + 1, fptr)) {
    // Remove any trailing newline or carriage return characters
    grid[row][strcspn(grid[row], "\r\n")] = '\0';
    memset(seen[row], 0,
           dims.columns * sizeof(int)); // Initialize seen row to 0
    row++;
  }

  // print out the grid
  for (int i = 0; i < dims.rows; i++) {
    for (int j = 0; j < dims.columns; j++) {
      printf("%c", grid[i][j]);
    }
    printf("\n");
  }

  // Print seen array
  memset(seen, 0, dims.rows * dims.columns * sizeof(int));
  unsigned long int total = 0;
  for (int i = 0; i < dims.rows; i++) {
    for (int j = 0; j < dims.columns; j++) {
      solve(dims.rows, dims.columns, grid, seen, i, j, &total);
    }
  }

  printf("total is %lu\n", total);

  // Clean up
  free(grid);
  free(seen);
  fclose(fptr);

  return 0;
}
