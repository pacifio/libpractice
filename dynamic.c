#include <stdio.h>
#include <stdlib.h>

void add_number(int **dest, int *count, int item) {
  int c = *count;
  int *temp = realloc(*dest, sizeof(int) * (c + 1));
  temp[c] = item;

  *count = c + 1;
  *dest = temp;
}

void update_number(int *numbers, int *count, int index, int item) {
  if (index > *count) {
    printf("illegal\n");
    return;
  }

  numbers[index] = item;
}

int get_number(int *numbers, int *count, int index) {
  for (int i = 0; i < *count; i++) {
    if (i == index) {
      return numbers[i];
    }
  }

  return -1;
}

void delete_number(int **numbers, int *count, int index) {
  if (index < 0 || index >= *count) {
    printf("illegal\n");
    return;
  }

  for (int i = index; i < *count - 1; i++) {
    (*numbers)[i] = (*numbers)[i + 1];
  }
  *count = *count - 1;
  *numbers = realloc(*numbers, sizeof(int) * (*count));
}

int main(int argc, char *argv[]) {
  int *count = malloc(sizeof(int));

  *count = 0;

  int *numbers = malloc(sizeof(int));

  add_number(&numbers, count, 10);
  add_number(&numbers, count, 30);
  add_number(&numbers, count, 20);

  delete_number(&numbers, count, 1);

  int c = *count;
  int i = 0;
  for (; i < c; i++) {
    printf("%d\n", numbers[i]);
  }

  free(count);
  free(numbers);

  return 0;
}