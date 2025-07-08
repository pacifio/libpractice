#include <stdio.h>
#include <stdlib.h>

void add(int ***dest, int *rc, int item[], int nItems, int **row_sizes) {
  int c = *rc;

  *dest = realloc(*dest, sizeof(int *) * (c + 1));
  *row_sizes = realloc(*row_sizes, sizeof(int) * (c + 1));

  int *new_row = malloc(sizeof(int) * nItems);
  for (int i = 0; i < nItems; i++) {
    new_row[i] = item[i];
  }

  (*dest)[c] = new_row;
  (*row_sizes)[c] = nItems;

  *rc = c + 1;
}

void update_row(int ***dest, int *rc, int **row_sizes, int index, int item[], int nItems) {
  int *new_row = malloc(sizeof(int) * nItems);
  for (int i = 0; i < nItems; i++) {
    new_row[i] = item[i];
  }

  (*dest)[index] = new_row;
  (*row_sizes)[index] = nItems;
}

void get_row(int **dest, int *row_sizes, int rc, int index) {
  for (int i = 0; i < rc; i++) {
    if (i == index) {
      printf("\t[");
      for (int j = 0; j < row_sizes[i]; j++) {
        printf("%d", dest[i][j]);
        if (j < row_sizes[i] - 1) printf(", ");
      }
      printf("]\n");
    }
  }
}

void delete_row(int ***dest, int *rc, int **row_sizes, int index) {
  free((*dest)[index]);

  for (int i = index; i < (*rc) - 1; i++) {
    (*dest)[i] = (*dest)[i + 1];
    (*row_sizes)[i] = (*row_sizes)[i + 1];
  }

  *rc = *rc - 1;
  *row_sizes = realloc(*row_sizes, sizeof(int) * (*rc));
  *dest = realloc(*dest, sizeof(int *) * (*rc));
}

void out(int **matrix, int *row_sizes, int rc) {
  printf("[\n");
  for (int i = 0; i < rc; i++) {
    printf("\t[");
    for (int j = 0; j < row_sizes[i]; j++) {
      printf("%d", matrix[i][j]);
      if (j < row_sizes[i] - 1) printf(", ");
    }
    printf("]\n");
  }
  printf("]\n");
}

int main(int argc, char *argv[]) {
  int rc = 0;
  int **matrix = NULL;
  int *row_sizes = NULL;

  int n1[] = {20, 2, 3, 4, 10};
  int n2[] = {4, 5};
  int n3[] = {6, 7, 8};

  add(&matrix, &rc, n1, 5, &row_sizes);
  add(&matrix, &rc, n2, 2, &row_sizes);
  add(&matrix, &rc, n3, 3, &row_sizes);

  int n4[] = {5, 5};
  update_row(&matrix, &rc, &row_sizes, 1, n4, 2);
  delete_row(&matrix, &rc, &row_sizes, 1);

  out(matrix, row_sizes, rc);

  for (int i = 0; i < rc; i++) {
    free(matrix[i]);
  }

  free(matrix);
  free(row_sizes);

  return 0;
}
