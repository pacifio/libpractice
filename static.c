#include <stdio.h>
#include <stdlib.h>

int general() {
  int numbers[30] = {};
  numbers[0] = 1;
  numbers[1] = 2;

  int i = 2;
  for (; i < 30; i++) {
    numbers[i] = i + 1;
  }

  for (int i = 0; i < 30; i++) {
    printf("%d\n", numbers[i]);
  }

  return 0;
}

int allocated() {
  int *numbers = malloc(sizeof(int) * 30);
  int i = 2;
  for (; i < 30; i++) {
    numbers[i] = i + 1;
  }

  for (int i = 0; i < 30; i++) {
    printf("%d\n", numbers[i]);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  general();
  printf("---------\n");
  allocated();

  return 0;
}